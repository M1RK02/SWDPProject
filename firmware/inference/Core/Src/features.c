#include "main.h"
#include "features.h"
#include "as7341.h"
#include "arm_math.h"
#include <math.h>

#define EPS 1e-8f

// --- FFT Configuration to match Python prototype ---
#define AUDIO_FFT_SIZE 512
#define BIN_RESOLUTION (16000.0f / (float)AUDIO_FFT_SIZE)

// --- ⚙️ PYTHON GENERATED MEAN VALUES FROM YOUR NEW feature_normalization.json ---
static const float model_normalization_means[39] = {
    7.92914629f, // [0] lumos_total_log
    6.85052824f, // [1] lumos_clear_log
    0.333235085f, // [2] lumos_nir_clear_ratio
    0.153810471f, // [3] lumos_clear_std_ratio
    0.203442737f, // [4] lumos_visible_std_ratio
    0.404858708f, // [5] lumos_blue_red_ratio
    0.878961027f, // [6] lumos_green_red_ratio
    0.0615974031f, // [7] lumos_dark_fraction
    0.0586501695f, // [8] lumos_saturation_fraction
    0.107435599f, // [9] lumos_f1_clear_ratio
    0.128238767f, // [10] lumos_f2_clear_ratio
    0.136300251f, // [11] lumos_f3_clear_ratio
    0.175746784f, // [12] lumos_f4_clear_ratio
    0.312788904f, // [13] lumos_f5_clear_ratio
    0.377664059f, // [14] lumos_f6_clear_ratio
    0.246293753f, // [15] lumos_f7_clear_ratio
    0.323619008f, // [16] lumos_f8_clear_ratio
    0.000957854558f, // [17] flicker_sensor_off_fraction
    0.896256804f, // [18] flicker_none_fraction
    0.102785118f, // [19] flicker_100hz_fraction
    0.102785118f, // [20] flicker_detected_fraction
    0.0198098868f, // [21] audio_rms
    0.000961849582f, // [22] audio_log_energy
    0.0146218641f, // [23] audio_abs_mean
    0.0198095683f, // [24] audio_std
    0.121829957f, // [25] audio_peak
    6.5986619f, // [26] audio_crest_factor
    0.423671097f, // [27] audio_silence_fraction
    0.0672174394f, // [28] audio_zero_crossing_rate
    516.356262f, // [29] audio_low_high_ratio
    1122.94141f, // [30] audio_spectral_centroid_hz
    5.9243784f, // [31] audio_low_band_log
    0.610638201f, // [32] audio_low_band_ratio
    4.89343452f, // [33] audio_mid_band_log
    0.296831727f, // [34] audio_mid_band_ratio
    3.29496598f, // [35] audio_high_band_log
    0.0821418837f, // [36] audio_high_band_ratio
    1.28739297f, // [37] audio_very_high_band_log
    0.0103879087f // [38] audio_very_high_band_ratio
};

// --- ⚙️ PYTHON GENERATED STANDARD DEVIATION VALUES FROM YOUR NEW feature_normalization.json ---
static const float model_normalization_stds[39] = {
    2.67949677f, // [0] lumos_total_log
    2.53916264f, // [1] lumos_clear_log
    0.292120397f, // [2] lumos_nir_clear_ratio
    0.254173934f, // [3] lumos_clear_std_ratio
    0.252810389f, // [4] lumos_visible_std_ratio
    0.237827465f, // [5] lumos_blue_red_ratio
    0.418987602f, // [6] lumos_green_red_ratio
    0.238302335f, // [7] lumos_dark_fraction
    0.21502994f, // [8] lumos_saturation_fraction
    0.115437917f, // [9] lumos_f1_clear_ratio
    0.113741271f, // [10] lumos_f2_clear_ratio
    0.161012545f, // [11] lumos_f3_clear_ratio
    0.15994224f, // [12] lumos_f4_clear_ratio
    0.209642962f, // [13] lumos_f5_clear_ratio
    0.23847574f, // [14] lumos_f6_clear_ratio
    0.216114938f, // [15] lumos_f7_clear_ratio
    0.235399306f, // [16] lumos_f8_clear_ratio
    0.0102718947f, // [17] flicker_sensor_off_fraction
    0.291700333f, // [18] flicker_none_fraction
    0.291687906f, // [19] flicker_100hz_fraction
    0.291687906f, // [20] flicker_detected_fraction
    0.0239953697f, // [21] audio_rms
    0.00341155427f, // [22] audio_log_energy
    0.0176457241f, // [23] audio_abs_mean
    0.0239949934f, // [24] audio_std
    0.143867537f, // [25] audio_peak
    4.02168465f, // [26] audio_crest_factor
    0.25026843f, // [27] audio_silence_fraction
    0.0524318703f, // [28] audio_zero_crossing_rate
    2612.68237f, // [29] audio_low_high_ratio
    515.489014f, // [30] audio_spectral_centroid_hz
    1.8296299f, // [31] audio_low_band_log
    0.284965187f, // [32] audio_low_band_ratio
    2.37785554f, // [33] audio_mid_band_log
    0.218445107f, // [34] audio_mid_band_ratio
    2.08876967f, // [35] audio_high_band_log
    0.109314412f, // [36] audio_high_band_ratio
    1.40210736f, // [37] audio_very_high_band_log
    0.0312638581f // [38] audio_very_high_band_ratio
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

        for (int ch = 0; ch < 8; ch++) {
            current_visible_sum += (float)spectro_hist[w][ch];
        }

        if (current_clear < 10.0f && current_visible_sum < 80.0f) dark_frames_count++;
        if (current_clear >= 55000.0f) saturation_frames_count++;

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

    float red_sum = channel_means[6] + channel_means[7];
    if (red_sum < 1.0f) {
        feature_out_vec[5] = 0.0f;
        feature_out_vec[6] = 0.0f;
    } else {
        feature_out_vec[5] = (channel_means[0] + channel_means[1]) / red_sum;
        feature_out_vec[6] = (channel_means[3] + channel_means[4]) / red_sum;
    }

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
    feature_out_vec[20] = (float)(count_100hz + count_120hz) / total; 
}

void Extract_Audio_Features_Window(int16_t* audio_samples, uint32_t total_samples, float* feature_out_vec) {
    // --- TIME DOMAIN FEATURES (COMPUTED DIRECTLY ON INT16_T SAMPLES) ---
    int64_t sum_squares = 0;
    int64_t sum_abs = 0;
    int64_t sum_raw = 0;
    int16_t peak_raw = 0;
    uint32_t silent_samples = 0;

    for (uint32_t i = 0; i < total_samples; i++) {
        int16_t sample = audio_samples[i];
        int32_t abs_sample = abs(sample);
        sum_squares += (int64_t)sample * sample;
        sum_abs += abs_sample;
        sum_raw += sample;
        if (abs_sample > peak_raw) {
            peak_raw = abs_sample;
        }
        if (abs_sample < 164) { // 164 is approx 0.005 * 32768
            silent_samples++;
        }
    }

    float rms_val = sqrtf((float)sum_squares / ((float)total_samples * 1073741824.0f));
    feature_out_vec[21] = rms_val;

    float log_energy_val = log1pf((float)sum_squares / ((float)total_samples * 1073741824.0f));
    feature_out_vec[22] = log_energy_val;

    float abs_mean_val = (float)sum_abs / ((float)total_samples * 32768.0f);
    feature_out_vec[23] = abs_mean_val;

    float mean_val = (float)sum_raw / ((float)total_samples * 32768.0f);
    float variance = ((float)sum_squares / 1073741824.0f - (float)total_samples * mean_val * mean_val) / (float)(total_samples - 1);
    float std_val = sqrtf(fabsf(variance));
    feature_out_vec[24] = std_val;

    float peak_val = (float)peak_raw / 32768.0f;
    feature_out_vec[25] = peak_val;

    feature_out_vec[26] = peak_val / (rms_val + EPS);

    feature_out_vec[27] = (float)silent_samples / (float)total_samples;

    uint32_t zero_crossings = 0;
    for (uint32_t i = 1; i < total_samples; i++) {
        if ((audio_samples[i] >= 0 && audio_samples[i-1] < 0) || 
            (audio_samples[i] < 0 && audio_samples[i-1] >= 0)) {
            zero_crossings++;
        }
    }
    feature_out_vec[28] = (float)zero_crossings / (float)(total_samples - 1);


    // --- FREQUENCY DOMAIN FEATURES (BLOCK-WISE HANN WINDOWED FFT) ---
    static float32_t fft_input_block[AUDIO_FFT_SIZE];
    static float32_t fft_output_scratch[AUDIO_FFT_SIZE];
    static float32_t magnitudes[AUDIO_FFT_SIZE / 2 + 1];
    
    arm_rfft_fast_instance_f32 fft_handler;
    arm_rfft_fast_init_f32(&fft_handler, AUDIO_FFT_SIZE);

    float low_band_energy = 0.0f;       // 20 - 250 Hz
    float mid_band_energy = 0.0f;       // 250 - 1000 Hz
    float high_band_energy = 0.0f;      // 1000 - 4000 Hz
    float very_high_band_energy = 0.0f; // 4000 - 7900 Hz
    
    float centroid_num = 0.0f;
    float centroid_den = 0.0f;

    for (uint32_t start = 0; start < total_samples; start += AUDIO_FFT_SIZE) {
        uint32_t block_size = total_samples - start;
        uint32_t copy_size = (block_size >= AUDIO_FFT_SIZE) ? AUDIO_FFT_SIZE : block_size;
        
        for (uint32_t i = 0; i < copy_size; i++) {
            float32_t val = (float32_t)audio_samples[start + i] / 32768.0f;
            float32_t hann = 0.5f * (1.0f - cosf(2.0f * PI * (float)i / (float)(AUDIO_FFT_SIZE - 1)));
            fft_input_block[i] = val * hann;
        }
        if (copy_size < AUDIO_FFT_SIZE) {
            memset(&fft_input_block[copy_size], 0, (AUDIO_FFT_SIZE - copy_size) * sizeof(float32_t));
        }

        arm_rfft_fast_f32(&fft_handler, fft_input_block, fft_output_scratch, 0);
        
        arm_cmplx_mag_f32(fft_output_scratch, magnitudes, AUDIO_FFT_SIZE / 2);
        magnitudes[0] = fabsf(fft_output_scratch[0]); // Fix CMSIS DC extraction anomaly

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

    feature_out_vec[29] = (low_band_energy + EPS) / (high_band_energy + very_high_band_energy + EPS);
    feature_out_vec[30] = centroid_num / (centroid_den + EPS);

    feature_out_vec[31] = log1pf(low_band_energy);
    feature_out_vec[32] = low_band_energy / total_band_energy;

    feature_out_vec[33] = log1pf(mid_band_energy);
    feature_out_vec[34] = mid_band_energy / total_band_energy;

    feature_out_vec[35] = log1pf(high_band_energy);
    feature_out_vec[36] = high_band_energy / total_band_energy;

    feature_out_vec[37] = log1pf(very_high_band_energy);
    feature_out_vec[38] = very_high_band_energy / total_band_energy;
}

void Apply_Normalization(float* feature_vec) {
    for (int i = 0; i < 39; i++) {
        feature_vec[i] = (feature_vec[i] - model_normalization_means[i]) / model_normalization_stds[i];
    }
}