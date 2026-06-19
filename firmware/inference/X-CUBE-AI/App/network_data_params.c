/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-19T10:47:19+0200
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
  0xea323f1d31f91e7fU, 0xd12df32e27f42c00U, 0xf0f10249fadac708U, 0xd45e1d35f8f42940U,
  0xf30edd03d80229fbU, 0x4a3cdd442f4266deU, 0xd3b026153b09a5abU, 0xed901c8521f54c3bU,
  0x7f0be11d26eb4920U, 0xf1ddc51dd22a58a2U, 0xa420e2acb2c5a9f9U, 0x68c1dbce3236155fU,
  0x5ec6e4ad2bc4f212U, 0x2903ba04a7003c40U, 0xfa0e816f2c0235a8U, 0xf7d702087f52f3e3U,
  0xefce0a1003dc6f6aU, 0x3dbcbd133133f5c6U, 0x5cf614de2825372bU, 0xd0e8190046fb5965U,
  0x5b3764ed01da220fU, 0x96c603d3d5d84b34U, 0xb05136e0206d0a58U, 0x31eddb5a3526c151U,
  0xd7f2b2560d6a8a7U, 0x14c00a0a2ce613b0U, 0xe1d9f7b818d70435U, 0xacdd81531f04faeaU,
  0xdfd2c7a0640e5a70U, 0x3a053b1ff2dee727U, 0x5d7f282bf6c7172dU, 0x2411d83253032447U,
  0xe607d498beffcccbU, 0xccf832fcc02af623U, 0xcdd000848f52af3U, 0xe3d71241edf7eb1aU,
  0x80017d8dce03433U, 0xa60f43b02c25af0U, 0xc97119aff7d601c9U, 0x8d4391195c06451U,
  0xc42bfed6044a7febU, 0xefed5dd2ae12a109U, 0x87cbcc2a18684ca7U, 0x48b21a1b40abb332U,
  0xcc819f3e7fde12d8U, 0xf84d5f45efd8140aU, 0xf3b2fb0ea0d1e634U, 0xdafae871c60813b5U,
  0xafa4c0e20a883e02U, 0xe581b8dcdd0d0efeU, 0xd9e43513d7e30725U, 0xb2608142c686a326U,
  0x50d22e758edb43f3U, 0x2cee36a0a74bf32cU, 0xbbb3b12dbb96d830U, 0xa12063a661341d15U,
  0x9cb689f80307c994U, 0x615e31a14952312U, 0xc5dc2ded58b330afU, 0x1befe90800cb1f81U,
  0x55f8d2d108022301U, 0x3e5c7f7f1ae65303U, 0xe2ff2b2daefc1ee3U, 0x441036fe392de733U,
  0xe5ebb8a19735ddf4U, 0xbcfac63bc15c7034U, 0xe4dff1de3bfe0312U, 0x3f7e4e5dff5a7ebU,
  0xb567840dccb0279U, 0x1eb92337e2ae21acU, 0x922d388198d9a7faU, 0xe2dd4fc52533171fU,
  0xf4b5f181b7f1b73eU, 0x14d4c58d861d86d5U, 0xb8bff8252e432457U, 0x1c3c0f4a07ee0510U,
  0x28c1351bc7fbc5c4U, 0x431ec0205c2d9dc3U, 0x3c0bdbe4c7283ee0U, 0xef432ddaa8abbc0fU,
  0xaf3119810f17d040U, 0x234cc729bd287d16U, 0xd220e2700929f8fU, 0x7ff023a591d4b5fdU,
  0xbe6c5fe44899aac5U, 0xa83e15f9beaea786U, 0x49dbc1d33b005446U, 0x58f7ed32f4f8b21eU,
  0x494fe6fafbc950afU, 0x143ac11e0c81cd28U, 0xbe0ae9fda4bd0ee5U, 0x2b59dfe1e7c619abU,
  0x71805ab2b15e034U, 0xdab8c5ba543c12d5U, 0xb8c048bee23b2f42U, 0xcc1d9f4815d440f1U,
  0x54e7e7ee32034281U, 0x49ce08d685b9f934U, 0xbddd1b304b728781U, 0xbcd25a4cda02fd8fU,
  0xb6ef39da0510e3ccU, 0x60a72acefdb217d1U, 0x28c4a8ad57c1dd2eU, 0x6f1114c90f9dd6c8U,
  0x8167ca0f8ccd0725U, 0xc1f0c4c4aa19a918U, 0x3e0aa03cbf0a180bU, 0xeb8919d813233771U,
  0xb1d344620a3ccf9bU, 0x19fc9f2881ffcaf2U, 0x41828e316bf0432U, 0xdafbcc22a74fa2c7U,
  0x3b52baa1c7df6652U, 0x21b9bcfb7fdb3fcU, 0xbd7fac3a03be4bc6U, 0xd7f1fccdfbd79a4aU,
  0x233cc920f3bab799U, 0xb9f3622aea544b96U, 0x104a8f8d44d83c21U, 0xb70ee4aaa6c90535U,
  0xe16601f5c1a14381U, 0xa906288f06b859e4U, 0x1dcf31f50e1c3a16U, 0x322bec93fae6d314U,
  0xf1845cfb5be0d16U, 0xb14a43cb41a981b6U, 0xe2ec482a31f6f693U, 0x1d00f3b0dc404503U,
  0xd514fd9f2cf5c2faU, 0x90cf4ddd37331fa9U, 0xe7bdab3963baf27fU, 0xa045c446498f006dU,
  0x4f3c7466a64523bcU, 0x8dbba3a93dc60cbU, 0x1c3ac4d8312d240bU, 0xbae7a418cfcc1185U,
  0x2934660f36e127e3U, 0x29dad3d8dd8163c4U, 0x90510eb522da0bf9U, 0x5b5f5d6317ea01d5U,
  0xb691475715814099U, 0x774294f0442e3db6U, 0x1960d40829e3c9caU, 0x54f168322858db38U,
  0xa2ad4fd0cb0bed4aU, 0x12f7288a3d0d3db8U, 0x83fe812f1095998dU, 0xad5aedb30170372eU,
  0x8e38c95ea0d60071U, 0x24fe2893bcb5e6e9U, 0x30002c7f1a60ab11U, 0x2168f31cfb1f1104U,
  0x151cc8323edbe933U, 0x3e5455820fbe71bU, 0xfa200d69384b57faU, 0x3ca23073391faf2dU,
  0xa281a70848fa54dbU, 0xc963afea6303e5b5U, 0x2acb7ce53810a4e5U, 0xf7f858e25ad17fdU,
  0x89ffbc9b05febae0U, 0xf9c81e9fc2f9f49fU, 0xc3ee1f2debfc4a05U, 0xe2d00300e3c42d21U,
  0x6ff0000001cU, 0x5c700000637U, 0x3cc00000228U, 0xd3000001dfU,
  0x75d0000056fU, 0x6e200000039U, 0x49800000486U, 0x2000000374U,
  0x7a9000006d4U, 0x1a5fffffe4aU, 0xfffffec400000625U, 0x4e60000073dU,
  0xfffffbc900000772U, 0x37d00000111U, 0x3aefffffe00U, 0x4b9000004f0U,
  0x370d2626a0552b46U, 0x52da05b2dbb9d5bbU, 0x7fba20eefb23aa4aU, 0x6fe74408f1f90124U,
  0x7fc81e3cc75837f6U, 0x27bfed1d41e8e2daU, 0x67ccf6c60c0a3626U, 0xf2237aebccb4b253U,
  0x8123520644a49c3fU, 0xea9a3ba36e1c4dfU, 0x6a6d3dbe4a87206U, 0x3c8291b701ab60b2U,
  0x285463d662dd3354U, 0x2e2dde74f727bd23U, 0x59000be50f1b7f21U, 0xe8540e43fd2dcf5dU,
  0xd160174d7f9b6260U, 0xb9dc115411300cdbU, 0xd34bf724f31da188U, 0x179ffbfa634437U,
  0x200ecad9fec679d4U, 0x70367191e427f2aU, 0xfb79d059edcc1e09U, 0xb2c657f04dffd4cbU,
  0x4452c77f4adf4142U, 0x6c1dd0de42e33602U, 0x6507ecb1dffde4cbU, 0x2e1f5fff116748c0U,
  0xdc3f493573097e41U, 0xcd1aa57f2633b005U, 0xad2ad2db3fd43058U, 0x555100fdcdb3a56U,
  0x267ff6e22cda3fedU, 0xe052ba2d132cebe9U, 0xdefa551019e3d7d3U, 0x57091c031275f7fdU,
  0xd39ff2ea737020d0U, 0xa75ddafcde5562dfU, 0x3b2e704a40acf671U, 0x410942cac97f0c74U,
  0xeabbbd0315f520dcU, 0xe6bb401125297ffeU, 0xf26a0079f22027b2U, 0xeac7f506facdf8f5U,
  0xce083546fa27072aU, 0xf852de217f28f926U, 0x195e1c7068d1fdd0U, 0x14b7ade1004adbe4U,
  0x7af1c7742e400c0dU, 0x7af71315a93d3409U, 0xf726bb05cb7f6360U, 0x1de56f0fadfaa0d9U,
  0xbb8864ddf2660609U, 0xdb071be85fdf2d60U, 0x3b18ee1a11047f1fU, 0x1c4dfbfb4926ef49U,
  0x28a1002699b8c5f6U, 0x6f24f7c7a81d37bcU, 0x7fe73a09607bf2ecU, 0x64d51ab4e6abd41U,
  0x38c8bddbc5321ab2U, 0x57d951b16d287f0cU, 0x213519391e3b2834U, 0x2ab632c1ea165cedU,
  0x33a000000afU, 0x398ffffff72U, 0x313fffffe40U, 0x36000000118U,
  0x330ffffff50U, 0x2760000043fU, 0x481ffffffa4U, 0x536ffffff60U,
  0xfe341ddac3e95749U, 0x29399944af81dfedU, 0x3c342974041792e9U, 0xb0a781f019d7e07bU,
  0x3d81f0b630e22bf0U, 0xc5c82f0f93ab41a2U, 0xac1360dec04f8db5U, 0x57a842d22f491481U,
  0xae3caaaaf50981fbU, 0x1eef06a91fa8573aU, 0xfffffec3ffffff07U, 0x12900000117U,
  0xffffff80U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

