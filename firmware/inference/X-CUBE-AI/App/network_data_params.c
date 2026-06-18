/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-18T18:41:35+0200
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[265] = {
  0xd57fe661e80077e9U, 0xa6fee75cbd490403U, 0xff26c727b4cd0ac9U, 0x294271322a449b38U,
  0xdf25d62d0a21db2aU, 0xb4d401ee2d1c9ad9U, 0xf3bb2b7fb521160cU, 0x31100517670a76a4U,
  0xb1adcac23e4501acU, 0x1de4d0f7aea930fdU, 0x8e0d0b3a0ae5c642U, 0x533697bb48cebcb0U,
  0x49577f263b1ffde3U, 0xf29c902b9ca6022U, 0x4f999244de5ab94eU, 0xe74207c6e1bfb39cU,
  0x3c21c0e839e9bdc7U, 0x208157c97c3865e2U, 0xceb19593ed0430eU, 0x6f66a7e2c6a28390U,
  0x641927b9394aeb15U, 0xb75ac1755f29b9e8U, 0xa2650a1edf6bdc37U, 0xbeddd96a3b0dd4ecU,
  0xf84b5e8157d9f009U, 0x8e629bbd1e6db0aU, 0xdacd1c88e7a13573U, 0x4463282fe27544fdU,
  0xebdd4df6f523c9b0U, 0xf14a15dc81b1873aU, 0xc5db102ed0de0f06U, 0xa34a379930c9ca6bU,
  0x3825cdb1da07c5acU, 0xdc022752e446837fU, 0x9a10d20b03433ba6U, 0x1af81b220f2619b1U,
  0xd7c3f760d114047fU, 0xbfc0e52937d52a3fU, 0xd8a1dad80a5d0d22U, 0xeae50b58da22c8c5U,
  0x185c4d5626f62b07U, 0xf93f5bc90f4c627fU, 0xce0c2547d41e3c44U, 0x1e160ff6f65a0509U,
  0xc4670425651cae4aU, 0x13c8d3bcb13d10cbU, 0x49d5b8d7af00fd02U, 0x2de5f3ea2cca7ac1U,
  0xcae7687fa52e133bU, 0x42f8f04b3322e3c8U, 0xacdf881c378d07c6U, 0x2a14e9c6af955f67U,
  0xfa12c9e4f11e3308U, 0x569d20bc6fcb881U, 0x3e9e760f3d15a929U, 0x7e3a7f01d39ad1a9U,
  0x799d979b7f1de144U, 0x627720720e4a022eU, 0xf61bc820dd57da93U, 0xe3154f60512087d2U,
  0x7455973f1d9f2b32U, 0xf6f32115071e08acU, 0x263f2f12aff1f701U, 0x920cf2f13117b44aU,
  0x31c6fb2e2dbeabcdU, 0x22e159af44a881f8U, 0xd5c1bf503b09e0e6U, 0xc03de6b6c828b4f1U,
  0xcaad55331b36d1efU, 0xa4f89cd11c5f0d55U, 0xd313d181fa15f03dU, 0xd3dfb3c74b5ab1b9U,
  0x13f5ed0e4df8a7f9U, 0x407f881741bafeedU, 0x3e09330cdb4d1a5dU, 0xf6181e393952aa30U,
  0xfd0cf3e41d4720ceU, 0xe08d01ede9fa0f0bU, 0x7f100decc554d638U, 0xbcf53ed727837196U,
  0xb8e3ae15e5cef70fU, 0xd0cff42eee581958U, 0xcbaa2c84093c3aa7U, 0xb5269eff1de4ede7U,
  0x3159a6298161b1b1U, 0x15a218403aa9e6ebU, 0xe825cb14faad0fe1U, 0xc402f9fe5c1316d7U,
  0xb98be7d5e007ab42U, 0x68fb18ee5b911f81U, 0x77f5ddb3454bfae1U, 0x248be7d348262f0cU,
  0x3f2d15dae05ce2bU, 0xcb7f504df14a16a2U, 0xf58b08195a750cfbU, 0x5e3edf2c14fc0deeU,
  0x2756a07536dbb7e4U, 0xe6835d20981b1dacU, 0x96bfa29c1a7f3bddU, 0xa9acd0e42005db3dU,
  0x6f50301c620320eU, 0xdbec5ceaf91191f8U, 0x56eccbdbaa1d9ee3U, 0x893eade3814310fbU,
  0xfa244ddd3d4b6b0eU, 0x225f271e4afe2312U, 0xd8706f3b2ed94c02U, 0x35ad213415bc22d2U,
  0xd92a2172f52b2521U, 0xe98501d182168cfaU, 0xc4c03aea1a851e81U, 0x36a849bcb2e2b5b4U,
  0x34ff06d5a5a4ea14U, 0xfb59b508c0cbc616U, 0x45d7df5f552a363aU, 0x31e432a8afebf564U,
  0xef00f18a9581b724U, 0xc9db19fd32083d2dU, 0xa53f5a562322adf5U, 0xbfb0e3dc8250a2aU,
  0xb103205a2e6581a2U, 0xe3cab72f24c397c6U, 0x3de1092a04ace774U, 0xbaae39f4eec149daU,
  0xfdfd1d771b816f7U, 0xd8f2f7db0b729b6bU, 0x81e621c7132d9c21U, 0x28fdf32be787c127U,
  0xe57f3939d57ce800U, 0xfc6105a0130423d6U, 0x12ed353f8937d03eU, 0x4b4617fe1c8e3382U,
  0x43471390d43c230aU, 0x67a32b5577ea47e3U, 0xd0d77b551843c23bU, 0x35d3d2642962a286U,
  0x621ceb9c9fbd2fc5U, 0xfc19a381d442052bU, 0x3aef84e3038cfb55U, 0x2cc93128e78fb8f0U,
  0x527fad460723d378U, 0xdae540091fb4ce2aU, 0x9ed545c7e31e37c8U, 0x500341bae827bd0aU,
  0x22e5f85db4574460U, 0x404f311dc77468a7U, 0xfe05b31027b4dfabU, 0x4708080fa9b9f63aU,
  0x6a91a929633d7f00U, 0x11b30ddd4c6053dU, 0x59bcc74df452c1U, 0x7f5040532190ebfdU,
  0x16124429efc9f5e4U, 0xe210c4f6b03ce7ddU, 0xcc3ba212e507dafaU, 0x4c5edb22d9444bdfU,
  0x3ce73135aad2dbedU, 0xfd2ab6d181ac10fdU, 0x33e250b0e52734faU, 0xd04734e1e11dada2U,
  0xfa3641ff0759aa9dU, 0xe798e053a18dd235U, 0x6c0d7ff1fabb9539U, 0xbacfaf73f2b2272dU,
  0x36e000004dfU, 0x5fbffffff17U, 0x2ca00000266U, 0x44400000493U,
  0x20a0000044cU, 0x3e0000045fU, 0x2ce000003c7U, 0x4e800000334U,
  0x3fb0000042eU, 0x24dfffffb66U, 0x3ac0000047dU, 0x22800000271U,
  0x5d800000526U, 0x3b100000411U, 0x38200000679U, 0xffffff3c000001c7U,
  0x4ce85c2adc5ef945U, 0x45a4792a005226e6U, 0x72118e214bfd3e93U, 0x54f7f3617f3d789bU,
  0x81b911cada03b7c0U, 0xddf1d7f38eb1b1f9U, 0x6bf71061fc1d197U, 0xc2fdcbf88a6a28c0U,
  0xde060e559c314eaaU, 0xc9eceba415fb0d43U, 0x548199b20abed732U, 0xe7fa054b1c420132U,
  0x4a5bf3cbed0c1807U, 0x295c0a1ce0c0041fU, 0xfb30e6c2b2fb0828U, 0xf25fa54cf9e3bc7fU,
  0x7f0ea72f99193605U, 0xcd03fbd7c3512e7dU, 0x34ca0c3b26cf4a41U, 0xc8c001592b265dd9U,
  0x980afa540cbec7bdU, 0xf2f60855c32d042aU, 0x957f115d40ab5922U, 0x9b2555231bd539aeU,
  0xe750e82d164364c2U, 0x3126c7cfa71ab349U, 0xb8ba10fd68ffd97fU, 0xcd56d1196c7c11aU,
  0x7f2bc1f8dd18dc6bU, 0xecf3fd29e14d9874U, 0x6a87cab14bf7b22U, 0x19be22249f302724U,
  0x13a62a72efe22a58U, 0x5b35986e57eda27fU, 0xccbbd4511455c9f7U, 0xbd05741de272268eU,
  0x1b062cb8695d2a5aU, 0x67193d1d301db47dU, 0xc1db4eb5ddfebaecU, 0xc340a04eef78f27fU,
  0xc35121c594ee8150U, 0xf6fc96ca7fbdc1e8U, 0xd4df2e7e9ed95df5U, 0x50f62f65bb337f71U,
  0x458283e09127934eU, 0xcbee61e6c7a9501aU, 0x479a4c7dba26f799U, 0xeaf08147639918feU,
  0x17bc0d180205d4d7U, 0x56ce1f59ebaf19edU, 0x7f66b32c6acaaba2U, 0x1310691b38d84fefU,
  0xfee0fdedd2bac367U, 0xd6f20848d009e570U, 0xb1b14a8faee1af0U, 0x3bd15edec37f41f9U,
  0x88521f167bbabcb1U, 0xe1be03dc2d7aec95U, 0x16097b567fee6d32U, 0xf96bc139105ac3b8U,
  0xda0c32386a163ea6U, 0x7f490c635e4109b1U, 0xe04912b6e5f09234U, 0x2d06db6cd851874bU,
  0xffffff21000001d1U, 0x609fffffc0cU, 0x1660000054dU, 0x1a4000003bbU,
  0x391ffffff35U, 0x4910000048aU, 0x240000001cdU, 0x712000003dcU,
  0x2c26fb681113403U, 0xf9fcb650bb16150eU, 0x78f8e712f0b5beb7U, 0x819d640c8a704831U,
  0xf1cabd1c8a03493aU, 0x97c4c3527f14d687U, 0xf829e58135e9c1e8U, 0x6600c2d6c4ad5781U,
  0x487c2be02fef14abU, 0xa642fdb934819bd1U, 0xffffff67fffffdcfU, 0x32f00000109U,
  0xfffffef3U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

