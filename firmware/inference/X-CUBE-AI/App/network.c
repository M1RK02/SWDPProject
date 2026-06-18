/**
  ******************************************************************************
  * @file    network.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-18T18:41:35+0200
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "network.h"
#include "network_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_network
 
#undef AI_NETWORK_MODEL_SIGNATURE
#define AI_NETWORK_MODEL_SIGNATURE     "0x0d415474bacf8fbeda0e976c23beb354"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2026-06-18T18:41:35+0200"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_NETWORK_N_BATCHES
#define AI_NETWORK_N_BATCHES         (1)

static ai_ptr g_network_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_network_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  serving_default_context_features0_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 41, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  gemm_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 32, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 5, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  nl_3_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 5, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  gemm_0_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 1312, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  gemm_0_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 32, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 512, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 80, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 5, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  gemm_0_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 201, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 112, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 41, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  nl_3_scratch0_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 248, AI_STATIC)

/**  Array metadata declarations section  *************************************/
/* Int quant #0 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_0_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.02505643665790558f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #1 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_0_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 32,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0032014106400310993f, 0.003070445265620947f, 0.002311729360371828f, 0.0034010345116257668f, 0.0024313798639923334f, 0.002717933151870966f, 0.0033099816646426916f, 0.0033919878769665956f, 0.003249948611482978f, 0.0023444113321602345f, 0.0023895991034805775f, 0.0023680669255554676f, 0.0030239501502364874f, 0.003069430822506547f, 0.0031683004926890135f, 0.003450695425271988f, 0.0030471952632069588f, 0.0027008692268282175f, 0.0022912193089723587f, 0.0030222998466342688f, 0.003271754365414381f, 0.0029351064004004f, 0.00312817539088428f, 0.002905698027461767f, 0.0036805765703320503f, 0.002836240455508232f, 0.002748010912910104f, 0.002885451540350914f, 0.003063404466956854f, 0.0028774896636605263f, 0.003466585651040077f, 0.0027647640090435743f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #2 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_1_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.02981637418270111f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #3 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_1_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0033903394360095263f, 0.002657915698364377f, 0.0029573787469416857f, 0.004071732517331839f, 0.0039594657719135284f, 0.0031043505296111107f, 0.0036673566792160273f, 0.003496142104268074f, 0.0032172289211302996f, 0.0035518119111657143f, 0.003358294488862157f, 0.0030464795418083668f, 0.003469966584816575f, 0.00369662931188941f, 0.003326192032545805f, 0.0032552804332226515f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #4 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_2_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.07307854294776917f),
    AI_PACK_INTQ_ZP(3)))

/* Int quant #5 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_2_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 5,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005559441167861223f, 0.004254207015037537f, 0.0049261124804615974f, 0.005121572874486446f, 0.005121967289596796f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0)))

/* Int quant #6 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_3_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00390625f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #7 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(serving_default_context_features0_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.038736261427402496f),
    AI_PACK_INTQ_ZP(-49)))

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  gemm_0_bias, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 4, 4, 128, 128),
  1, &gemm_0_bias_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  gemm_0_output, AI_STATIC,
  1, 0x1,
  AI_SHAPE_INIT(4, 1, 32, 1, 1), AI_STRIDE_INIT(4, 1, 1, 32, 32),
  1, &gemm_0_output_array, &gemm_0_output_array_intq)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  gemm_0_scratch0, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 201, 1, 1), AI_STRIDE_INIT(4, 2, 2, 402, 402),
  1, &gemm_0_scratch0_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  gemm_0_weights, AI_STATIC,
  3, 0x1,
  AI_SHAPE_INIT(4, 41, 32, 1, 1), AI_STRIDE_INIT(4, 1, 41, 1312, 1312),
  1, &gemm_0_weights_array, &gemm_0_weights_array_intq)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_bias, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &gemm_1_bias_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_output, AI_STATIC,
  5, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 1, 1, 16, 16),
  1, &gemm_1_output_array, &gemm_1_output_array_intq)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_scratch0, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 112, 1, 1), AI_STRIDE_INIT(4, 2, 2, 224, 224),
  1, &gemm_1_scratch0_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_weights, AI_STATIC,
  7, 0x1,
  AI_SHAPE_INIT(4, 32, 16, 1, 1), AI_STRIDE_INIT(4, 1, 32, 512, 512),
  1, &gemm_1_weights_array, &gemm_1_weights_array_intq)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_bias, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 4, 4, 20, 20),
  1, &gemm_2_bias_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_output, AI_STATIC,
  9, 0x1,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 1, 1, 5, 5),
  1, &gemm_2_output_array, &gemm_2_output_array_intq)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_scratch0, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 41, 1, 1), AI_STRIDE_INIT(4, 2, 2, 82, 82),
  1, &gemm_2_scratch0_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_weights, AI_STATIC,
  11, 0x1,
  AI_SHAPE_INIT(4, 16, 5, 1, 1), AI_STRIDE_INIT(4, 1, 16, 80, 80),
  1, &gemm_2_weights_array, &gemm_2_weights_array_intq)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  nl_3_output, AI_STATIC,
  12, 0x1,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 1, 1, 5, 5),
  1, &nl_3_output_array, &nl_3_output_array_intq)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  nl_3_scratch0, AI_STATIC,
  13, 0x0,
  AI_SHAPE_INIT(4, 1, 248, 1, 1), AI_STRIDE_INIT(4, 4, 4, 992, 992),
  1, &nl_3_scratch0_array, NULL)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  serving_default_context_features0_output, AI_STATIC,
  14, 0x1,
  AI_SHAPE_INIT(4, 1, 41, 1, 1), AI_STRIDE_INIT(4, 1, 1, 41, 41),
  1, &serving_default_context_features0_output_array, &serving_default_context_features0_output_array_intq)



/**  Layer declarations section  **********************************************/



AI_STATIC_CONST ai_i32 nl_3_nl_params_data[] = { 1255479808, 23, -248 };
AI_ARRAY_OBJ_DECLARE(
    nl_3_nl_params, AI_ARRAY_FORMAT_S32,
    nl_3_nl_params_data, nl_3_nl_params_data, 3, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_3_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_3_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  nl_3_layer, 3,
  SM_TYPE, 0x0, NULL,
  sm, forward_sm_integer,
  &nl_3_chain,
  NULL, &nl_3_layer, AI_STATIC, 
  .nl_params = &nl_3_nl_params, 
  .axis = AI_SHAPE_CHANNEL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_2_weights, &gemm_2_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_2_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_2_layer, 2,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_2_chain,
  NULL, &nl_3_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_1_weights, &gemm_1_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_1_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_1_layer, 1,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_1_chain,
  NULL, &gemm_2_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &serving_default_context_features0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_0_weights, &gemm_0_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_0_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_0_layer, 0,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_0_chain,
  NULL, &gemm_1_layer, AI_STATIC, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 2116, 1, 1),
    2116, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 1000, 1, 1),
    1000, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &serving_default_context_features0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &nl_3_output),
  &gemm_0_layer, 0xeb30d6c7, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 2116, 1, 1),
      2116, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 1000, 1, 1),
      1000, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_IN_NUM, &serving_default_context_features0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_NETWORK_OUT_NUM, &nl_3_output),
  &gemm_0_layer, 0xeb30d6c7, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_network_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    serving_default_context_features0_output_array.data = AI_PTR(g_network_activations_map[0] + 956);
    serving_default_context_features0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 956);
    gemm_0_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 552);
    gemm_0_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 552);
    gemm_0_output_array.data = AI_PTR(g_network_activations_map[0] + 520);
    gemm_0_output_array.data_start = AI_PTR(g_network_activations_map[0] + 520);
    gemm_1_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 552);
    gemm_1_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 552);
    gemm_1_output_array.data = AI_PTR(g_network_activations_map[0] + 776);
    gemm_1_output_array.data_start = AI_PTR(g_network_activations_map[0] + 776);
    gemm_2_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 520);
    gemm_2_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 520);
    gemm_2_output_array.data = AI_PTR(g_network_activations_map[0] + 992);
    gemm_2_output_array.data_start = AI_PTR(g_network_activations_map[0] + 992);
    nl_3_scratch0_array.data = AI_PTR(g_network_activations_map[0] + 0);
    nl_3_scratch0_array.data_start = AI_PTR(g_network_activations_map[0] + 0);
    nl_3_output_array.data = AI_PTR(g_network_activations_map[0] + 992);
    nl_3_output_array.data_start = AI_PTR(g_network_activations_map[0] + 992);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool network_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_network_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    gemm_0_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_0_weights_array.data = AI_PTR(g_network_weights_map[0] + 0);
    gemm_0_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 0);
    gemm_0_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_0_bias_array.data = AI_PTR(g_network_weights_map[0] + 1312);
    gemm_0_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 1312);
    gemm_1_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_1_weights_array.data = AI_PTR(g_network_weights_map[0] + 1440);
    gemm_1_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 1440);
    gemm_1_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_1_bias_array.data = AI_PTR(g_network_weights_map[0] + 1952);
    gemm_1_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 1952);
    gemm_2_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_2_weights_array.data = AI_PTR(g_network_weights_map[0] + 2016);
    gemm_2_weights_array.data_start = AI_PTR(g_network_weights_map[0] + 2016);
    gemm_2_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_2_bias_array.data = AI_PTR(g_network_weights_map[0] + 2096);
    gemm_2_bias_array.data_start = AI_PTR(g_network_weights_map[0] + 2096);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_network_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 2032,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xeb30d6c7,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_network_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_NETWORK_MODEL_NAME,
      .model_signature   = AI_NETWORK_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 2032,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0xeb30d6c7,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_network_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_network_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_network_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_network_create(network, AI_NETWORK_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_network_data_params_get(&params) != true) {
    err = ai_network_get_error(*network);
    return err;
  }
#if defined(AI_NETWORK_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_NETWORK_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_network_init(*network, &params) != true) {
    err = ai_network_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_network_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_network_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_network_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_network_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= network_configure_weights(net_ctx, params);
  ok &= network_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_network_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_network_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_NETWORK_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

