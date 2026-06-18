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

// --- ⚙️ ACTUAL PYTHON GENERATED MEAN VALUES FROM feature_normalization.json ---
static const float model_normalization_means[41] = {
    6.261284828186035f,     // 0: lumos_total_log
    5.287024021148682f,     // 1: lumos_clear_log
    0.38745948672294617f,   // 2: lumos_nir_clear_ratio
    0.12853290140628815f,   // 3: lumos_clear_std_ratio
    0.1347494274377823f,    // 4: lumos_visible_std_ratio
    0.26937344670295715f,   // 5: lumos_blue_red_ratio
    0.791897714138031f,     // 6: lumos_green_red_ratio
    0.21896223723888397f,   // 7: lumos_dark_fraction
    0.0f,                   // 8: lumos_saturation_fraction
    0.06574340909719467f,   // 9: lumos_f1_clear_ratio
    0.09522206336259842f,   // 10: lumos_f2_clear_ratio
    0.15613852441310883f,   // 11: lumos_f3_clear_ratio
    0.13386811316013336f,   // 12: lumos_f4_clear_ratio
    0.3147699534893036f,    // 13: lumos_f5_clear_ratio
    0.3402782380580902f,    // 14: lumos_f6_clear_ratio
    0.21239124238491058f,   // 15: lumos_f7_clear_ratio
    0.23333987593650818f,   // 16: lumos_f8_clear_ratio
    0.0008149959612637758f, // 17: flicker_sensor_off_fraction
    0.7951643466949463f,    // 18: flicker_none_fraction
    0.2040206491947174f,    // 19: flicker_100hz_fraction
    0.0f,                   // 20: flicker_120hz_fraction
    0.2040206491947174f,    // 21: flicker_detected_fraction
    0.0f,                   // 22: flicker_unknown_fraction
    0.02849610149860382f,   // 23: audio_rms
    0.0014387154951691628f, // 24: audio_log_energy
    0.02176782116293907f,   // 25: audio_abs_mean
    0.02849576435983181f,   // 26: audio_std
    0.15940487384796143f,   // 27: audio_peak
    6.023674488067627f,     // 28: audio_crest_factor
    0.24530065059661865f,   // 29: audio_silence_fraction
    0.040955640375614166f,  // 30: audio_zero_crossing_rate
    116.31025695800781f,    // 31: audio_low_high_ratio
    959.8027954101562f,     // 32: audio_spectral_centroid_hz
    9.165566444396973f,     // 33: audio_low_band_log
    0.5133225917816162f,    // 34: audio_low_band_ratio
    8.856313705444336f,     // 35: audio_mid_band_log
    0.4342832565307617f,    // 36: audio_mid_band_ratio
    6.1673455238342285f,    // 37: audio_high_band_log
    0.04915877804160118f,   // 38: audio_high_band_ratio
    3.098005771636963f,     // 39: audio_very_high_band_log
    0.0032353370916098356f  // 40: audio_very_high_band_ratio
};

// --- ⚙️ ACTUAL PYTHON GENERATED STANDARD DEVIATION VALUES FROM feature_normalization.json ---
static const float model_normalization_stds[41] = {
    3.0582730770111084f,   // 0: lumos_total_log
    2.7367405891418457f,   // 1: lumos_clear_log
    0.36162877082824707f,  // 2: lumos_nir_clear_ratio
    0.24729187786579132f,  // 3: lumos_clear_std_ratio
    0.23384395241737366f,  // 4: lumos_visible_std_ratio
    0.17691883444786072f,  // 5: lumos_blue_red_ratio
    0.48702681064605713f,  // 6: lumos_green_red_ratio
    0.412811815738678f,    // 7: lumos_dark_fraction
    1.0f,                  // 8: lumos_saturation_fraction
    0.08030950278043747f,  // 9: lumos_f1_clear_ratio
    0.09512348473072052f,  // 10: lumos_f2_clear_ratio
    0.13650795817375183f,  // 11: lumos_f3_clear_ratio
    0.14567534625530243f,  // 12: lumos_f4_clear_ratio
    0.22072772681713104f,  // 13: lumos_f5_clear_ratio
    0.23018866777420044f,  // 14: lumos_f6_clear_ratio
    0.18775592744350433f,  // 15: lumos_f7_clear_ratio
    0.1737399846315384f,   // 16: lumos_f8_clear_ratio
    0.009481087327003479f, // 17: flicker_sensor_off_fraction
    0.3863869309425354f,   // 18: flicker_none_fraction
    0.38670122623443604f,  // 19: flicker_100hz_fraction
    1.0f,                  // 20: flicker_120hz_fraction
    0.38670122623443604f,  // 21: flicker_detected_fraction
    1.0f,                  // 22: flicker_unknown_fraction
    0.025143807753920555f, // 23: audio_rms
    0.0029872418381273746f,// 24: audio_log_energy
    0.019446400925517082f, // 25: audio_abs_mean
    0.025143524631857872f, // 26: audio_std
    0.14241459965705872f,  // 27: audio_peak
    2.98909068107605f,     // 28: audio_crest_factor
    0.14869262278079987f,  // 29: audio_silence_fraction
    0.030030982568860054f, // 30: audio_zero_crossing_rate
    255.53562927246094f,   // 31: audio_low_high_ratio
    375.89459228515625f,   // 32: audio_spectral_centroid_hz
    1.7730233669281006f,   // 33: audio_low_band_log
    0.27855613827705383f,  // 34: audio_low_band_ratio
    1.5847792625427246f,   // 35: audio_mid_band_log
    0.25440531969070435f,  // 36: audio_mid_band_ratio
    2.005514621734619f,    // 37: audio_high_band_log
    0.06278987228870392f,  // 38: audio_high_band_ratio
    1.9382314682006836f,   // 39: audio_very_high_band_log
    0.00698096351698041f   // 40: audio_very_high_band_ratio
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