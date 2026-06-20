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
    7.48809481f, // [0] lumos_total_log
    6.4915309f, // [1] lumos_clear_log
    0.340682596f, // [2] lumos_nir_clear_ratio
    0.182996273f, // [3] lumos_clear_std_ratio
    0.212237597f, // [4] lumos_visible_std_ratio
    0.35450536f, // [5] lumos_blue_red_ratio
    0.857133031f, // [6] lumos_green_red_ratio
    0.0829528123f, // [7] lumos_dark_fraction
    0.0261605773f, // [8] lumos_saturation_fraction
    0.0744318664f, // [9] lumos_f1_clear_ratio
    0.0962068588f, // [10] lumos_f2_clear_ratio
    0.126042426f, // [11] lumos_f3_clear_ratio
    0.139463827f, // [12] lumos_f4_clear_ratio
    0.280059218f, // [13] lumos_f5_clear_ratio
    0.323803812f, // [14] lumos_f6_clear_ratio
    0.241179213f, // [15] lumos_f7_clear_ratio
    0.247934237f, // [16] lumos_f8_clear_ratio
    0.0014269409f, // [17] flicker_sensor_off_fraction
    0.857686579f, // [18] flicker_none_fraction
    0.140886575f, // [19] flicker_100hz_fraction
    0.140886575f, // [20] flicker_detected_fraction
    0.0236336794f, // [21] audio_rms
    0.00124205649f, // [22] audio_log_energy
    0.017452687f, // [23] audio_abs_mean
    0.0236333366f, // [24] audio_std
    0.139106065f, // [25] audio_peak
    6.112216f, // [26] audio_crest_factor
    0.362818718f, // [27] audio_silence_fraction
    0.0651287884f, // [28] audio_zero_crossing_rate
    573.83075f, // [29] audio_low_high_ratio
    1067.88843f, // [30] audio_spectral_centroid_hz
    6.32634211f, // [31] audio_low_band_log
    0.605807066f, // [32] audio_low_band_ratio
    5.29197168f, // [33] audio_mid_band_log
    0.303791016f, // [34] audio_mid_band_ratio
    3.58511066f, // [35] audio_high_band_log
    0.0805806518f, // [36] audio_high_band_ratio
    1.4486289f, // [37] audio_very_high_band_log
    0.00982138049f // [38] audio_very_high_band_ratio
};

// --- ⚙️ PYTHON GENERATED STANDARD DEVIATION VALUES FROM YOUR NEW feature_normalization.json ---
static const float model_normalization_stds[39] = {
    2.65739799f, // [0] lumos_total_log
    2.51192999f, // [1] lumos_clear_log
    0.299541682f, // [2] lumos_nir_clear_ratio
    0.270707577f, // [3] lumos_clear_std_ratio
    0.266386598f, // [4] lumos_visible_std_ratio
    0.235128403f, // [5] lumos_blue_red_ratio
    0.437445581f, // [6] lumos_green_red_ratio
    0.274467021f, // [7] lumos_dark_fraction
    0.144822612f, // [8] lumos_saturation_fraction
    0.0879014432f, // [9] lumos_f1_clear_ratio
    0.0844740644f, // [10] lumos_f2_clear_ratio
    0.139331743f, // [11] lumos_f3_clear_ratio
    0.125929788f, // [12] lumos_f4_clear_ratio
    0.194704577f, // [13] lumos_f5_clear_ratio
    0.205611676f, // [14] lumos_f6_clear_ratio
    0.2140809f, // [15] lumos_f7_clear_ratio
    0.14856936f, // [16] lumos_f8_clear_ratio
    0.0125104971f, // [17] flicker_sensor_off_fraction
    0.332666487f, // [18] flicker_none_fraction
    0.332844794f, // [19] flicker_100hz_fraction
    0.332844794f, // [20] flicker_detected_fraction
    0.0263030287f, // [21] audio_rms
    0.00386577309f, // [22] audio_log_energy
    0.0192646831f, // [23] audio_abs_mean
    0.0263026152f, // [24] audio_std
    0.153506383f, // [25] audio_peak
    2.9885323f, // [26] audio_crest_factor
    0.237991348f, // [27] audio_silence_fraction
    0.0538180619f, // [28] audio_zero_crossing_rate
    2182.67236f, // [29] audio_low_high_ratio
    543.352661f, // [30] audio_spectral_centroid_hz
    1.76976383f, // [31] audio_low_band_log
    0.287668437f, // [32] audio_low_band_ratio
    2.47799015f, // [33] audio_mid_band_log
    0.223544613f, // [34] audio_mid_band_ratio
    2.21050453f, // [35] audio_high_band_log
    0.111818537f, // [36] audio_high_band_ratio
    1.51651037f, // [37] audio_very_high_band_log
    0.0314609408f // [38] audio_very_high_band_ratio
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