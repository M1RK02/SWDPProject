#include "main.h"
#include "features.h"
#include "as7341.h"
#include "arm_math.h"
#include <math.h>

#define EPS 1e-8f

// --- FFT Configuration to match Python prototype ---
#define AUDIO_FFT_SIZE 4096
#define BIN_RESOLUTION (16000.0f / (float)AUDIO_FFT_SIZE)

// Scratchpad array for processing the full feature window
static float32_t audio_float_scratch[TOTAL_WINDOW_SAMPLES];

// --- ⚙️ PYTHON GENERATED MEAN VALUES FROM YOUR NEW feature_normalization.json ---
static const float model_normalization_means[41] = {
    7.7480731f,
    6.76601028f,
    0.443025351f,
    0.153907344f,
    0.177605703f,
    0.386570096f,
    0.928631723f,
    0.131050006f,
    0.0f,
    0.0736622661f,
    0.0958541557f,
    0.139115587f,
    0.147679314f,
    0.270542741f,
    0.292400032f,
    0.181066647f,
    0.223509654f,
    0.00144717796f,
    0.876989961f,
    0.121562943f,
    0.0f,
    0.121562943f,
    0.0f,
    0.0230864864f,
    0.000992636778f,
    0.0172957685f,
    0.0230862461f,
    0.135541379f,
    6.20670033f,
    0.311448872f,
    0.055741217f,
    75.272049f,
    1081.96887f,
    8.1453352f,
    0.511914313f,
    7.85781336f,
    0.411112189f,
    5.63944483f,
    0.0722501874f,
    2.66789675f,
    0.00472297054f
};

// --- ⚙️ PYTHON GENERATED STANDARD DEVIATION VALUES FROM YOUR NEW feature_normalization.json ---
static const float model_normalization_stds[41] = {
    3.04983139f,
    2.85226345f,
    0.319931597f,
    0.224971205f,
    0.238588512f,
    0.234595254f,
    0.472528845f,
    0.336924732f,
    1.0f,
    0.075584814f,
    0.0813960135f,
    0.119588569f,
    0.128144234f,
    0.194127649f,
    0.200732604f,
    0.15868853f,
    0.157251239f,
    0.0125977248f,
    0.313987881f,
    0.31429553f,
    1.0f,
    0.31429553f,
    1.0f,
    0.0215211809f,
    0.00244961074f,
    0.0164556224f,
    0.0215209108f,
    0.125140756f,
    3.02142406f,
    0.184148282f,
    0.0393982865f,
    208.085693f,
    397.168671f,
    1.65522683f,
    0.250218689f,
    1.5984894f,
    0.22157225f,
    1.94542348f,
    0.0923911929f,
    1.68154514f,
    0.0107196448f
};

void Extract_Spectro_Features_Window(uint16_t spectro_hist[WINDOW_PACKET_COUNT][SPECTRAL_CHANNELS], float* feature_out_vec) {
    float channel_sums[SPECTRAL_CHANNELS] = {0.0f};
    float total_visible_sum = 0.0f;

    for (int w = 0; w < WINDOW_PACKET_COUNT; w++) {
        for (int ch = 0; ch < SPECTRAL_CHANNELS; ch++) {
            channel_sums[ch] += (float)spectro_hist[w][ch];
        }
        for (int ch = 0; ch < 8; ch++) {
            total_visible_sum += (float)spectro_hist[w][ch];
        }
    }

    float clear_mean = channel_sums[8] / (float)WINDOW_PACKET_COUNT;
    float nir_mean = channel_sums[9] / (float)WINDOW_PACKET_COUNT;
    float visible_mean = total_visible_sum / (float)WINDOW_PACKET_COUNT;
    
    float channel_means[SPECTRAL_CHANNELS];
    for (int i = 0; i < SPECTRAL_CHANNELS; i++) {
        channel_means[i] = channel_sums[i] / (float)WINDOW_PACKET_COUNT;
    }

    float clear_sq_diff_sum = 0.0f;
    float visible_sq_diff_sum = 0.0f;
    uint32_t dark_frames_count = 0;
    uint32_t saturation_frames_count = 0;

    for (int w = 0; w < WINDOW_PACKET_COUNT; w++) {
        float current_clear = (float)spectro_hist[w][8];
        float current_visible_sum = 0.0f;
        uint8_t saturated = 0;

        for (int ch = 0; ch < SPECTRAL_CHANNELS; ch++) {
            if (spectro_hist[w][ch] >= 65000) saturated = 1;
            if (ch < 8) current_visible_sum += (float)spectro_hist[w][ch];
        }

        if (current_clear < 10.0f && current_visible_sum < 80.0f) dark_frames_count++;
        if (saturated) saturation_frames_count++;

        clear_sq_diff_sum += (current_clear - clear_mean) * (current_clear - clear_mean);
        visible_sq_diff_sum += (current_visible_sum - visible_mean) * (current_visible_sum - visible_mean);
    }

    float clear_std = sqrtf(clear_sq_diff_sum / (float)WINDOW_PACKET_COUNT);
    float visible_std = sqrtf(visible_sq_diff_sum / (float)WINDOW_PACKET_COUNT);

    feature_out_vec[0] = log1pf(visible_mean + clear_mean + nir_mean); 
    feature_out_vec[1] = log1pf(clear_mean);                          
    feature_out_vec[2] = nir_mean / (clear_mean + EPS);               
    feature_out_vec[3] = clear_std / (clear_mean + EPS);              
    feature_out_vec[4] = visible_std / (visible_mean + EPS);          
    feature_out_vec[5] = (channel_means[0] + channel_means[1]) / (channel_means[6] + channel_means[7] + EPS); 
    feature_out_vec[6] = (channel_means[3] + channel_means[4]) / (channel_means[6] + channel_means[7] + EPS); 
    feature_out_vec[7] = (float)dark_frames_count / (float)WINDOW_PACKET_COUNT;        
    feature_out_vec[8] = (float)saturation_frames_count / (float)WINDOW_PACKET_COUNT;  

    for (int i = 0; i < 8; i++) {
        feature_out_vec[9 + i] = channel_means[i] / (clear_mean + EPS);
    }
}

void Extract_Flicker_Features_Window(AS7341_Flicker_t* flicker_hist, uint32_t total_packets, float* feature_out_vec) {
    uint32_t count_off = 0, count_none = 0, count_100hz = 0, count_120hz = 0, count_unknown = 0;

    for (uint32_t i = 0; i < total_packets; i++) {
        switch (flicker_hist[i]) {
            case AS734_FLICKER_UNKNOWN:   count_off++;   break;
            case AS734_FLICKER_NONE:      count_none++;  break;
            case AS734_FLICKER_100HZ:     count_100hz++; break;
            case AS734_FLICKER_120HZ:     count_120hz++; break;
            default:                      count_unknown++; break;
        }
    }

    float total = (float)total_packets;
    feature_out_vec[17] = (float)count_off / total;      
    feature_out_vec[18] = (float)count_none / total;     
    feature_out_vec[19] = (float)count_100hz / total;    
    feature_out_vec[20] = (float)count_120hz / total;    
    feature_out_vec[21] = (float)(count_100hz + count_120hz) / total; 
    feature_out_vec[22] = (float)count_unknown / total;  
}

void Extract_Audio_Features_Window(int16_t* audio_samples, uint32_t total_samples, float* feature_out_vec) {
    // 1. Convert int16 hardware amplitudes to normalized float32
    for (uint32_t i = 0; i < total_samples; i++) {
        audio_float_scratch[i] = (float32_t)audio_samples[i] / 32768.0f;
    }

    // --- TIME DOMAIN FEATURES ---
    float32_t rms_val;
    arm_rms_f32(audio_float_scratch, total_samples, &rms_val);
    feature_out_vec[23] = (float)rms_val;

    float32_t energy_sum = 0.0f;
    for (uint32_t i = 0; i < total_samples; i++) {
        energy_sum += audio_float_scratch[i] * audio_float_scratch[i];
    }
    feature_out_vec[24] = (float)log1pf(energy_sum / (float32_t)total_samples);

    float32_t abs_mean_val = 0.0f;
    for (uint32_t i = 0; i < total_samples; i++) {
        abs_mean_val += fabsf(audio_float_scratch[i]);
    }
    feature_out_vec[25] = (float)(abs_mean_val / (float32_t)total_samples);

    float32_t mean_val = 0.0f, std_val = 0.0f;
    arm_mean_f32(audio_float_scratch, total_samples, &mean_val);
    arm_std_f32(audio_float_scratch, total_samples, &std_val);
    feature_out_vec[26] = (float)std_val;

    float32_t peak_val = 0.0f;
    uint32_t peak_idx;
    arm_max_f32(audio_float_scratch, total_samples, &peak_val, &peak_idx);
    feature_out_vec[27] = (float)fabsf(peak_val);

    feature_out_vec[28] = feature_out_vec[27] / (feature_out_vec[23] + EPS);

    uint32_t silent_samples = 0;
    for (uint32_t i = 0; i < total_samples; i++) {
        if (fabsf(audio_float_scratch[i]) < 0.005f) silent_samples++;
    }
    feature_out_vec[29] = (float)silent_samples / (float)total_samples;

    uint32_t zero_crossings = 0;
    for (uint32_t i = 1; i < total_samples; i++) {
        if ((audio_samples[i] >= 0 && audio_samples[i-1] < 0) || 
            (audio_samples[i] < 0 && audio_samples[i-1] >= 0)) {
            zero_crossings++;
        }
    }
    feature_out_vec[30] = (float)zero_crossings / (float)(total_samples - 1);


    // --- FREQUENCY DOMAIN FEATURES (BLOCK-WISE 8192-SAMPLE HANN WINDOWED FFT) ---
    static float32_t fft_input_block[AUDIO_FFT_SIZE];
    static float32_t fft_output_scratch[AUDIO_FFT_SIZE];
    static float32_t magnitudes[AUDIO_FFT_SIZE / 2 + 1];
    
    // Setup the CMSIS DSP RFFT Handler
    arm_rfft_fast_instance_f32 fft_handler;
    arm_rfft_fast_init_f32(&fft_handler, AUDIO_FFT_SIZE);

    // Frequency bands accumulator
    float low_band_energy = 0.0f;       // 20 - 250 Hz
    float mid_band_energy = 0.0f;       // 250 - 1000 Hz
    float high_band_energy = 0.0f;      // 1000 - 4000 Hz
    float very_high_band_energy = 0.0f; // 4000 - 7900 Hz
    
    float centroid_num = 0.0f;
    float centroid_den = 0.0f;

    // Loop through the entire context buffer in blocks of 8192 samples
    for (uint32_t start = 0; start < total_samples; start += AUDIO_FFT_SIZE) {
        uint32_t block_size = total_samples - start;
        
        // Isolate block and handle trailing zero padding if required
        if (block_size >= AUDIO_FFT_SIZE) {
            memcpy(fft_input_block, &audio_float_scratch[start], AUDIO_FFT_SIZE * sizeof(float32_t));
        } else {
            memcpy(fft_input_block, &audio_float_scratch[start], block_size * sizeof(float32_t));
            memset(&fft_input_block[block_size], 0, (AUDIO_FFT_SIZE - block_size) * sizeof(float32_t));
        }

        // Apply Hann Window directly on the isolated block
        for (uint32_t i = 0; i < AUDIO_FFT_SIZE; i++) {
            float32_t hann = 0.5f * (1.0f - cosf(2.0f * PI * (float)i / (float)(AUDIO_FFT_SIZE - 1)));
            fft_input_block[i] *= hann;
        }

        // Compute FFT
        arm_rfft_fast_f32(&fft_handler, fft_input_block, fft_output_scratch, 0);
        
        // Generate Complex Magnitudes
        arm_cmplx_mag_f32(fft_output_scratch, magnitudes, AUDIO_FFT_SIZE / 2);
        magnitudes[0] = fabsf(fft_output_scratch[0]); // Fix CMSIS DC extraction anomaly

        // Process inside the block
        for (uint32_t bin = 0; bin < AUDIO_FFT_SIZE / 2; bin++) {
            float freq = (float)bin * BIN_RESOLUTION;
            float mag = magnitudes[bin];
            float power = mag * mag;

            centroid_num += freq * mag;
            centroid_den += mag;

            if (freq >= 20.0f && freq < 250.0f) {
                low_band_energy += power;
            } else if (freq >= 250.0f && freq < 1000.0f) {
                mid_band_energy += power;
            } else if (freq >= 1000.0f && freq < 4000.0f) {
                high_band_energy += power;
            } else if (freq >= 4000.0f && freq < 7900.0f) {
                very_high_band_energy += power;
            }
        }
    }

    float total_band_energy = low_band_energy + mid_band_energy + high_band_energy + very_high_band_energy + EPS;

    // Index 31: audio_low_high_ratio
    feature_out_vec[31] = (low_band_energy + EPS) / (high_band_energy + very_high_band_energy + EPS);

    // Index 32: audio_spectral_centroid_hz
    feature_out_vec[32] = centroid_num / (centroid_den + EPS);

    // Low band parameters
    feature_out_vec[33] = log1pf(low_band_energy);
    feature_out_vec[34] = low_band_energy / total_band_energy;

    // Mid band parameters
    feature_out_vec[35] = log1pf(mid_band_energy);
    feature_out_vec[36] = mid_band_energy / total_band_energy;

    // High band parameters
    feature_out_vec[37] = log1pf(high_band_energy);
    feature_out_vec[38] = high_band_energy / total_band_energy;

    // Very high band parameters
    feature_out_vec[39] = log1pf(very_high_band_energy);
    feature_out_vec[40] = very_high_band_energy / total_band_energy;
}

void Apply_Normalization(float* feature_vec) {
    for (int i = 0; i < 41; i++) {
        feature_vec[i] = (feature_vec[i] - model_normalization_means[i]) / model_normalization_stds[i];
    }
}