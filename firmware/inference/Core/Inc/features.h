/*
 * features.h
 * Public interface for the TinyML feature extraction pipeline.
 */

#ifndef INC_FEATURES_H_
#define INC_FEATURES_H_

#include <stdint.h>
#include "as7341.h"

// --- Shared Window Dimensions ---
#define SPECTRAL_CHANNELS          10
#define WINDOW_PACKET_COUNT        9
#define BLOCK_AUDIO_SAMPLES (2034 * 3) // 6102
#define AUDIO_CIRC_BUF_SIZE        (BLOCK_AUDIO_SAMPLES * 2) // 12204 samples
#define TOTAL_WINDOW_SAMPLES       (BLOCK_AUDIO_SAMPLES * WINDOW_PACKET_COUNT) // 54,918

/* --- Public Function Prototypes --- */

/**
  * @brief  Extracts 17 Spectral Features from the 9-packet optical history matrix.
  */
void Extract_Spectro_Features_Window(uint16_t spectro_hist[WINDOW_PACKET_COUNT][SPECTRAL_CHANNELS], float* feature_out_vec);

/**
  * @brief  Extracts 4 Flicker Features from the 9-packet on-chip tracker history.
  */
void Extract_Flicker_Features_Window(AS7341_Flicker_t* flicker_hist, uint32_t total_packets, float* feature_out_vec);

/**
  * @brief  Extracts 18 Audio Features from the reconstructed 3.43-second wave.
  */
void Extract_Audio_Features_Window(int16_t* audio_samples, uint32_t total_samples, float* feature_out_vec);

/**
  * @brief  Standardizes the full 39-element feature map before running the NN model.
  */
void Apply_Normalization(float* feature_vec);

#endif /* INC_FEATURES_H_ */