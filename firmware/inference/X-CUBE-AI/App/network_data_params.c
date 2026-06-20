/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-20T13:42:21+0200
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
const ai_u64 s_network_weights_array_u64[257] = {
  0xcb1601072cadf25cU, 0xc54c072528eefbeaU, 0xa2c2ad5e01d1a40bU, 0x8142a21a11d7fbf1U,
  0x5f1cc516b5abd61eU, 0xa0d52924bf28483cU, 0x2a41d18842fb17f1U, 0x370ce3c416bbe3f8U,
  0x5c8468e3e715fbcbU, 0xc8d7fa8112f28bedU, 0x2a01162e9ee6a981U, 0x2b1ba6f238c831d5U,
  0xd60c3f162e95a0fbU, 0x28ce20ed1934ee3bU, 0x49fc6e2a021ba678U, 0xdbe60fdf4d5fefddU,
  0xc81ff7efc081cc8cU, 0x4c20331f59332db7U, 0xfc0329f7432846f6U, 0x4b6553da08121107U,
  0xb69013cbb9f76b23U, 0xce3523a8f4510137U, 0xf6c0a7482a29ea5bU, 0x286e33267f01bcf4U,
  0xe3a0ee41af52418U, 0x105fbd7d7b021edU, 0x4341d3b7d0fe0606U, 0xa13402ceb59bfeb4U,
  0xc0224b0d0808eb81U, 0x4e051b7f42639bc8U, 0xb7bfd1d21c31c618U, 0xcf08f225eb0ebc8bU,
  0xaf218ffe4ea1ef9U, 0xff05f8251bc5fdefU, 0xe9f281a12018d90fU, 0xe813e3f434ee091fU,
  0x34d0ffe101f90431U, 0x3833d9f82b17d549U, 0x6a3451120aa26b6U, 0xffe327fecffd97f4U,
  0xd4d1cc48e4593a81U, 0x2d86ef40519bbafcU, 0xd19985e460cef8c9U, 0xd64e23148f87cc5dU,
  0xeaca1ad8303384ccU, 0xb452ec0ee620d841U, 0x331ca2eedc12d7e6U, 0x2c780eecb88b81efU,
  0xdcb3c8dffc06f7ffU, 0xdefd1efbdb5aa24fU, 0xb238e71e38df2031U, 0xded5101217d8fce8U,
  0x7f0d4803b9d0960eU, 0x81080cb6e93069d1U, 0x207eeae32fed3e5U, 0xfa0c04fe29f116fcU,
  0xfe0604d6f2c1e70cU, 0xff5e9092b0104e4U, 0x307f626527f81df8U, 0xf5fc203ccfe413e5U,
  0x27192b1e061edb34U, 0x161be8c5b80dc8e9U, 0xe3f2db01ce351d2fU, 0xee332ffd0cf4e93aU,
  0x4a3f0f10be0de8f7U, 0x3cd4d87a504fc5bdU, 0xabd91c97495edde7U, 0xec05814970d3d216U,
  0x9617e415dad74c26U, 0xab409de6e59cf381U, 0x33b33371b1616dbU, 0x5cf3d24aeb1e200U,
  0xa9e48d9aea74df3dU, 0xcd24130381cf0b35U, 0xe5dfc62632e71300U, 0x3201d5d4ce072cf6U,
  0x10c91801c1180333U, 0xdc05db0d27fbf42aU, 0x36a7bd5b33f89284U, 0x43c2bd86819d96e3U,
  0xfb4842f9469ff0f9U, 0xee3614da0301e9cfU, 0xf60fc1e017b41448U, 0x2fc673d216652e67U,
  0xd6c72b5534de0399U, 0x7137f1368ff4499aU, 0x120da7da81bb91e9U, 0x6f43854c6eb93d0U,
  0x4a2400282a21fd81U, 0xe7f2174b07ccd0c2U, 0xe3dc41e4e3d34db7U, 0x382f53a3e62587edU,
  0x1d0ed13a58f502c6U, 0x43493a129cc75804U, 0x7f2d34e0019ced1bU, 0x79f4f3eed6c686a5U,
  0x660febb02c9dc4feU, 0x42929ba0d54525dcU, 0x5531138102aaead7U, 0x8763031291d40235U,
  0xb53deeeeb60ce6eaU, 0x376cf01007171302U, 0x11e1e78114fc1a15U, 0xb1d392ca365efd2bU,
  0xe11c3122997490ecU, 0x95ca0f2b20e0f6c4U, 0x5645dff6f60e9c75U, 0xac27dd1c3148dc0eU,
  0x21f615cc3120bdf2U, 0x4ce8e5fd0838f85dU, 0x8958195ea2d3ebdU, 0xd40e53d07618370dU,
  0x868d24cafe41c6fdU, 0xf8c48f02216bf24aU, 0xf80ae2de02b9ca4cU, 0x5ede35d54da6b543U,
  0x7fe021dc04aed0d1U, 0x3a200baadc6f3878U, 0x95e024231ccf4d1bU, 0xd01d0b81df0c9f1aU,
  0x2dde3023d112fb6U, 0xba1ae6d6ef1240e7U, 0x25d7b4edd8aa41dfU, 0xa8598d81f882d4dfU,
  0xe72ccac28df66ca4U, 0xfacf9235c4143fc3U, 0x8ac8182f6e50d02bU, 0x222d0ef1e2f4d53eU,
  0xabe23eb31817b7d0U, 0x2cda1ce7d5ad814aU, 0xd6c04dc036023009U, 0x8b10b0142efd7cbU,
  0x464402f3f0b2c05dU, 0xd54c9bc3fb63310U, 0x81fa52483a0bbbd1U, 0xbc2c4408d2e54605U,
  0x327f4731cd44196aU, 0xe4ee4102e5f1112eU, 0x5010306dae813d9U, 0xd315c402f7abbfbdU,
  0xe3400ac30134031bU, 0xddda8c37ddd60036U, 0xce0a14010bd8d2d8U, 0xf0e942f6157f1746U,
  0x113cd924f117ea21U, 0x20fbc9d5f8273c1bU, 0xe353b7020f938d1U, 0xcd30ced2d0155613U,
  0x22e82f0244d9db2eU, 0x29fa0c05f1b0e0e7U, 0xa7fb18c8c24b1d3U, 0x87c837d4fbff2cf1U,
  0xbd810fd9f20afa4dU, 0xe69884cc218cb232U, 0x2846d4f35a03d693U, 0x111d99af561ed2e6U,
  0x21000000509U, 0x32c0000043fU, 0x5fc0000027cU, 0xfffffda40000024bU,
  0x1cf000003acU, 0x4ef000001d6U, 0xffffff110000027eU, 0xfc00000351U,
  0x53c00000109U, 0x75cffffffc7U, 0x171000006d0U, 0xfffffede000002e4U,
  0x185000003cbU, 0x24b00000469U, 0xfffffe66ffffff96U, 0x5aa0000021dU,
  0x12061115c1552155U, 0x36ca03a1db99b4c7U, 0x61c21be0cb079b7fU, 0x49fd23efe6070c16U,
  0x7fb9c73ac5245211U, 0xf7d11a286fe7f6e1U, 0x3a41f2b8d5f13cd8U, 0xb5fd7bb6d983882cU,
  0x814a6f0454f1d638U, 0x45bda7e33602da01U, 0x6d9cc1c39e62fdbU, 0x5db6be041ccd45efU,
  0x7f544e03629a4a5bU, 0x4a3bfe7cf60c8d2bU, 0x67f8f09ef0ea5a3eU, 0xc155584701e1b378U,
  0xd3611d4a70932b21U, 0xba960245b84117dcU, 0xe538060f2319c8a0U, 0x21a8e28fc56717fU,
  0xdadcdd223fe3f9eU, 0x4d0d46eed6766030U, 0x17fac7100db321dU, 0xa2c33ef46efda4d5U,
  0x131ae16339f0041aU, 0x4cead59c21f17f07U, 0x1c0c0ab1bb14f8ecU, 0x42647fe217578a2U,
  0xfa31112f3dc96e30U, 0x9e21c1690829a214U, 0xc733dab92c810748U, 0xdf2f1613d8b93446U,
  0x3c7ff9ef30cf1dc3U, 0xc31ab32dca2f01d7U, 0xdcfa64fa17e806f6U, 0x490b0d2d07793d31U,
  0xea99dde563674ffeU, 0xd06be0ff28323fd4U, 0x3e2567402284b674U, 0x3d047f8bb6690642U,
  0xc3c804f6091806f3U, 0x10e6180616101cdcU, 0x150fdc7f2a04d7c3U, 0x28d0d207c8ceb827U,
  0xb9226118ef36d80aU, 0x2018b40f3813dbffU, 0xf3e8707fefe4a0U, 0x3ac7aa10e634ef0dU,
  0x3eebc05f1729f806U, 0x36dc1c0acf406308U, 0xe558cb0ac77f5234U, 0xefd334fbd2f5b5b6U,
  0xd2b362bedb5c313cU, 0xdb5e0eef7fbe0b59U, 0x41d8d52c11fb53efU, 0x313fffe72010d215U,
  0x6a1ec1dacc190e7U, 0x25ee0acdbf2261c7U, 0x412045fa2d7f0205U, 0xe3382aad4d5cf1fdU,
  0x13d1ebcefa34fec6U, 0x7fda2db8462966feU, 0xf1e1a300c60ff4aU, 0x16de13e5f92c50f0U,
  0xbcfffffd40U, 0x47a00000805U, 0x3fdffffff81U, 0xffffff6e000002c8U,
  0x21f00000005U, 0x58200000401U, 0x75bffffff87U, 0x652ffffff04U,
  0x73642e7a9aa2438U, 0x414b8161a29ce2ebU, 0x2e28056b0a2aa4fcU, 0xc5b785e414cbde7fU,
  0x31ac05c824ab39efU, 0xaed026ec81b42992U, 0xbc1344cce77f83cfU, 0x30ba40e85b4317aaU,
  0xb363ada9d1bb81fdU, 0x5ae7edb1efa75d38U, 0xffffff42fffffe99U, 0x1ef000000abU,
  0xfffffeb0U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

