/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-18T17:41:57+0200
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
  0xc2600f94d7bb55d1U, 0xde1014f51de7bb15U, 0xeaac7faad0a230bfU, 0x27606066d3ca1af5U,
  0xde1cd705f92b3bfeU, 0x47ffd40520f5cb28U, 0xef5b4c2914fffa44U, 0x9f3f0350279abcd2U,
  0xf5e7f4c891c21daU, 0x332dc4931e98403dU, 0x40e4e9a3c63ec574U, 0x37ba431e0a46d1e2U,
  0xfc0fb76285d3d9daU, 0xa18100af16f9e1c9U, 0xf2136e3f50f0b2fU, 0xbb90e4eeacdc5423U,
  0xf83bbecaaf0cd202U, 0xc6427cf8d037f323U, 0xcad20b3be08f9881U, 0x352bb1fbf2093edfU,
  0xb241516032f54641U, 0x1df1fdc6961907deU, 0xc9d2b92b4468274bU, 0xc159a97144737c7U,
  0x3d47151f9a6a7fc1U, 0xd12822ce48c0b2d5U, 0x4bdb42ee2ced57c8U, 0x3908d406dc1ead3aU,
  0xd4e31a7f52192211U, 0x1dee9cf540a0c455U, 0xc9f49a0423dc11c7U, 0xc2ac90ada90d1a0bU,
  0x4b143b14f57f246dU, 0xd367bddb32e60d6fU, 0x15084b6d4d2e69c9U, 0x81ee022ad05fe983U,
  0xc90a60198e09514U, 0x55b1fcd74e28ee9eU, 0xdef842130ef34bdfU, 0x6a3e5e13281c39c5U,
  0x2da10d17a6f50e39U, 0xcfe4bdcfd422555dU, 0x3fe2c90f8b5f43d3U, 0xd8b0b41425f2a0abU,
  0x33bba296f93e3a4dU, 0xd9406681021e65ffU, 0xebb961f64270387U, 0xd071c11ce7a30fddU,
  0x18ae030c323ea7ddU, 0xab06fedcabb2c9c9U, 0xb9d0818addc58a4bU, 0xc246401d9592941cU,
  0x2181bd9eaad4e9b5U, 0xb3d6e965d2ccc08fU, 0xe910b4d731fec6e9U, 0x33cdd91bb509b111U,
  0xe09c649cca9b31fU, 0x120f1807c1c3494fU, 0xa9350d90f5eadefcU, 0x55a688d6ecd4b4f9U,
  0xca51f6e46d8163ccU, 0x47267f05e344ea58U, 0xaa6368e9124b373eU, 0xda0a49a63a4f63edU,
  0x1c491021b41843f9U, 0x2e094a19d65ef313U, 0x51cb931be5c5fbc5U, 0xdff1d2f4b5a8e4fdU,
  0x5f852d819ae60ae6U, 0x47b50436f6d221a1U, 0x83d530cf98add141U, 0x2c4a623c6f8fbe6U,
  0x9e67e99faef89dc7U, 0xa981e50ebb361c2fU, 0x82e4863cee33ae0U, 0xc522f4a9b3fcc332U,
  0xe0b2ac9133590f6cU, 0x1fa4acb6d0b6268aU, 0xffdad05ff11c390dU, 0xa1f4d362bae58165U,
  0xd14c2c174c2d1c05U, 0xe612b2bb2c94c7f4U, 0x2361b92d9d8ed53eU, 0xd2bdce003c9bacc7U,
  0x56577f1d6318ac6cU, 0x15e8d5d6c537253cU, 0xc7c013f424069742U, 0xe73b4cd96f37f4d6U,
  0xc7e95b3871edbed8U, 0x18ad1b814202073eU, 0xcd5f493ae1b8d4e3U, 0xf21f75fe998123c3U,
  0x44c796fb5c675691U, 0x57ac24ee55f80035U, 0x9b601f7f49189bcfU, 0x94ac4bc710c53d4fU,
  0x18520ef4b3b0feU, 0xe57fbbe8f296092cU, 0xff1b320d48e65d55U, 0x3375b64ba44572fdU,
  0xee4203f8b9c812ddU, 0x85ca6673e838fe13U, 0xf108adac2e35ef3bU, 0x224bd16948581770U,
  0xf681d8c1102245b2U, 0xd800f8155f4fc641U, 0x72a8d741df4c16c4U, 0xc8b2f3367f092ffeU,
  0xec0de0f3bae8e226U, 0x662640001d13133eU, 0xd9dd020e208120d3U, 0xc1b90e27f6d3e6ffU,
  0x55ca0f3ac7e54efbU, 0xead0fe0006ff341eU, 0x4565d32bee1d544fU, 0xf3d86e26883cfd7fU,
  0xb0a46ae1805c3c9U, 0x3d0d03ef58be199dU, 0xefd4810e108b475U, 0x81612f41e83f0d34U,
  0x9f200f121a62db00U, 0x599efae736e8a428U, 0x32ef40e7f9d835c3U, 0x1ee7da54361e76feU,
  0xb1d115f6265b0d09U, 0xbed2d8d1c96ced7fU, 0xa508f6e4ffeeb90dU, 0xe4084fe703cb992cU,
  0x7b695b22d7666be6U, 0xc7af1d0d0c340099U, 0x3f1b46c96f8401e4U, 0xea006af18f111791U,
  0xe67d310037b3d481U, 0x7c4dcadcd15d453U, 0xf6c3fceb3b2d04f4U, 0x27db292c2b4b2426U,
  0xe32ad5d3ac97a5e6U, 0x28df37cffb0181d9U, 0xd3a4114720653dcU, 0xfc42d6e052f6f3bfU,
  0x3181ee8d70be3ec0U, 0x1204fa2f2bc44f37U, 0x2c0660fcd52cc88eU, 0xa45b35c22cb3ed04U,
  0x6a140fed6185e515U, 0x52e1c2062b1bc55U, 0x3ed415a6c068e2deU, 0xb298823cf37f2b5cU,
  0xf68ecc9f526cbce7U, 0x81e33c5f24c0e0eeU, 0xde1db5a8d3bf1ed3U, 0x22ece58e319ce9bU,
  0xf40d020ce2f141d6U, 0xd358ce02b5d612cbU, 0x473b81f434db97d8U, 0xb82ed025c5cb1c0bU,
  0xddb1afe905eacf5eU, 0xd80cadbaf1fdf5e3U, 0x4cd153e1006e558fU, 0x1ac80d3e6f14caefU,
  0xed574cc8131115caU, 0x691fbd05c9aa4fcdU, 0x3d0b81fc29025aU, 0x4e53e511b64d6927U,
  0x3fc000000a7U, 0x1ab0000040eU, 0xfffffee600000429U, 0x4fffffff19U,
  0xffffffb9000004cdU, 0x32e000003d9U, 0x547000004b3U, 0x1afffffe3aU,
  0x277ffffff8bU, 0x4c600000444U, 0x56c000000b8U, 0x35500000591U,
  0x60c00000198U, 0x21a00000613U, 0x27f0000014eU, 0xffffffe70000064fU,
  0xf505ae5006ce7805U, 0x17244b1e314fe239U, 0x7fbec44e03a71c39U, 0xab2357cfd8ce58f6U,
  0xf2d40ff2c8b83960U, 0xc4504573775db60bU, 0x49260504dfcb7b32U, 0xb39f567c95da7fbeU,
  0x4c4339363f4a37daU, 0x7af91761349d5f5U, 0x7f0f5a4867d3c409U, 0x664543c9273e1134U,
  0xee5965571cf8fbb1U, 0xc2130f4c54a623baU, 0x34e8a87fe0bad3fdU, 0x2eb7454d61da49b7U,
  0xe400e9e9ef7ffd89U, 0xccec29000934716eU, 0xcc7b73fd28f52543U, 0xf47836cf536fecc7U,
  0xe02b5bce7f718d3U, 0x4a762adef64e1914U, 0xa1414c244dbaf9aeU, 0x7ae67f58fefaa0c0U,
  0x76261b2c644be8c5U, 0xf44c78a5f436e1e4U, 0xa558693ae21df5edU, 0xd2114702087f3fdeU,
  0xe1ccfe0df34ff063U, 0x27ca250c3b6c7fdaU, 0x6fbd99d86dc70fcbU, 0xabe9daab0c552682U,
  0x1de7d5f9fa503b17U, 0x23d332d63e07df08U, 0x2d323a072341042cU, 0xc07fa62e1161255dU,
  0x56f3e926c9423f3aU, 0x53d90845181d2b5eU, 0xe81098747f03eb5fU, 0xd5ed9ba1f669ff92U,
  0xd3d53ceec2d10219U, 0x16f92228f3c81a2dU, 0xf1d8c760fb283932U, 0x6db3ad084081664fU,
  0xd55f6152082a17e5U, 0xe2ee0b5bf5f7ed5cU, 0x46a0723f77b7fd2U, 0x6d5db56412d75f6aU,
  0x2ad84fa4de31d45aU, 0x2c1b0dd15253297fU, 0xcb753633a8cb9e7U, 0xf3d522326b5ab1eaU,
  0xfdc83a64d5420903U, 0xef4b08370ffa2f49U, 0xf145cdf97f3ca6edU, 0xffedd8c7cf0b13acU,
  0x320f9bb5895ee27fU, 0x6ce246991311e3a2U, 0x8febc2157a0e43fbU, 0xabbda10453bbd39bU,
  0xf77fa7690a2a20deU, 0xdc66b179e6a56626U, 0xfe3496b209de71e3U, 0xa7d1b80da5d6d3f1U,
  0x1e2000001faU, 0xfffffff3ffffffacU, 0x1fe0000048fU, 0x2fc000005ebU,
  0x21000000460U, 0x30b000001e3U, 0x133ffffff7eU, 0x1e9fffffed8U,
  0xacb425085111c68dU, 0x103a4a3f1e2f5581U, 0xa681db8537fc0551U, 0x44bbbbb226f7ab0cU,
  0x4e95b7812b29e870U, 0x2f9d50f7c49c6010U, 0x91629a1cecfec08cU, 0xfaa488db36810a35U,
  0xfa347afefea1e225U, 0x39a5814201c4fe8eU, 0x29ffffff63U, 0x154ffffff5eU,
  0xffffff5fU,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

