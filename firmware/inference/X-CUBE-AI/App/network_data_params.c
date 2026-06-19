/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-19T18:40:31+0200
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
  0x30c13ffe818227fU, 0xe2491c3446122831U, 0xe1c8d62dcf080222U, 0xd14c170de2e51b2aU,
  0xd43dde11cbe5f3d6U, 0x131af1277f2641d3U, 0xcdb5040d1c01c8c3U, 0xebb505b01bfa3407U,
  0x550fed020ee72f08U, 0xede3c80df21f27b9U, 0xa926e890adbec9f1U, 0x46e5c6f12d271c39U,
  0x54a6e8b430e3f817U, 0x6facd12c4014333U, 0xe81e8149ffec2d83U, 0x2c75cf47f4bd90eU,
  0xd4cd0e0d1be06177U, 0x36a6c6244118dfb4U, 0x58f926ee42384520U, 0xc3f31a0d38f46d63U,
  0x577f36b9f5d33324U, 0x998806f9cecc4335U, 0xd2351cb0226bff31U, 0x17d2c73b1d1be855U,
  0x1478303662ebaba4U, 0x2bd9061134ed18b4U, 0xccf209dc1cf50a22U, 0xd1eb81301006d80bU,
  0xf3ede3cc34031941U, 0x1c18131ef7c50424U, 0x3f67082201fd143cU, 0x829ce193e0b217fU,
  0xef08d29bbdc7d6c6U, 0xc2ea21e1e7141effU, 0x2ccdff0936eb13e5U, 0xf3e82b45feceee39U,
  0x29fa33d58ba4f942U, 0x67a084b1bdb6bd1U, 0xb253ffb5f4dcfdcdU, 0xf9cf293f81aa4633U,
  0xc00d10ad0e1b70c5U, 0xfff444dbcdfcc71fU, 0xb1d0d853f66356e0U, 0x55bf43366685e237U,
  0xde81ee2072f72200U, 0xdf28352ec7f50300U, 0xcbbc1c4481e5eb2fU, 0xc9c5031deafcd8a8U,
  0xf5e4051bd2ae3904U, 0x15d1ffd3cef10aecU, 0xc6be20faf6d45b1fU, 0xda75b643c59fe530U,
  0x56f35056e0df1cf7U, 0x12c819cbba7fe030U, 0xf2d0b915c6a5d925U, 0xd41955c253240f15U,
  0xdff381ebeeded1adU, 0xfeaff0e18b71704U, 0xf1f60b1110fa2ddfU, 0xbeaee01fbe017c5U,
  0x2ae0f0d30e02fc08U, 0xc7f5e6216e51fe3U, 0x4172acfdf06eeU, 0xed4f2b1c0e0ddb28U,
  0xedf7d4c0bc07a6daU, 0xd005dff4d6303f07U, 0xd8fe07d80bf8f805U, 0xfe11a3feeaff88d5U,
  0xf944564dd5d00475U, 0x39c95a5302be46c0U, 0x81406d9bf60fbb13U, 0xeeea66db0e2e3a3dU,
  0xf6d1e09ed011f538U, 0xf0cc294903681e1U, 0xc5c8fb2553314a16U, 0x17270e25f9e9f90eU,
  0xfd42a1cd0e7c4e6U, 0x393ad130552b93a9U, 0x2c0cd0e0d42d37e6U, 0xe1432fdfbcacb70dU,
  0xae3623811a22cc40U, 0x2948ca21c5387815U, 0x4cf8e124dcc59481U, 0x45bf0799bcafa9edU,
  0xfa5150c936a6e7ecU, 0x628e7c0e0e9e8bdU, 0x25cba00e5bdb5406U, 0x76f6ee2acd4cef1dU,
  0x515312e7f8c86fb8U, 0xef2b990c24819c28U, 0x9a1cc7f0a4c32bf1U, 0x3e2ae1badb9be6b1U,
  0x2b1e1c8121079adaU, 0xf292b8a1583e120bU, 0xb5b94dbad71b504dU, 0xce37923f0ebe3706U,
  0x61eaecea360f4092U, 0x53daeed988a4f937U, 0xc7cf303e3060a5a4U, 0xb7bb7347db100da0U,
  0x9de73dde0713fab8U, 0x2d8160ca00b1f5c6U, 0x4dc0acab50c1d925U, 0x351510cb1ac3f3f6U,
  0x816af9f894c4f329U, 0xd7fcd8d2c340a239U, 0x4721da2cdcf60f30U, 0xf1931adc12c7145cU,
  0xc0d83557fd3de4b4U, 0x2402a82081f4bef2U, 0x81e24e727ba1127U, 0xeafdd91fa8379dc5U,
  0x4b5dd0b4cce35545U, 0x4cdc92c3f92ff423U, 0xfa638323e9ab31e8U, 0xd4ff04fa00ec934cU,
  0x5a49ce3c1fdfb48cU, 0x9bdd5fd8dc7e7a81U, 0xf86a8a8169e32320U, 0x833ae4b292cf1665U,
  0xc4782043dad20d9cU, 0x9a163c811fa967cfU, 0x3fbc3cd24f262001U, 0x451bde81a20a0132U,
  0xfa1838e004d8f829U, 0xd5ff1ec81ed2c3bcU, 0x16d714fe250c1ac7U, 0xde27e4cfd21e1bbaU,
  0xc312ed963deec4cdU, 0xa7ca4bea411af4b4U, 0x39dbb077f970c64U, 0xb239d14c459e1464U,
  0x5442565ab33823b9U, 0xf2dfab4881cf4ad5U, 0x281ab3c122151e06U, 0xb2f59b25c1c20e86U,
  0x2a1838ff35d317f4U, 0x24bdbff3d18b57b8U, 0x985dfecc3adb0e02U, 0x6f4e6b7b35ef14d7U,
  0xd393435d089057adU, 0x7f3284e3562340b8U, 0x205bcaff67ca143fU, 0x3ad75a2f385fdb3eU,
  0xcbcf2bd7e20cd61dU, 0x1eeb318b561e64bcU, 0x81fa951e0c8eb995U, 0x884ed9a005470ee2U,
  0x811fab428ef3e751U, 0x12fc3a95e5bbe5f2U, 0x20f5057c176a961dU, 0x226fe81df1341018U,
  0x162bb7373cc8fb40U, 0x8f235520df8ed3aU, 0xfa352c2f1c3b1f7fU, 0x34cdf02e18f8fd01U,
  0xe0aed7d220d12cfcU, 0xea10d1f524cbede7U, 0x37b347ff3b13cddfU, 0x3a7f8d8f36ac1a05U,
  0x9306c5a02304bfe9U, 0xebc4e5a8c008e7aeU, 0xacf70a21ebfa4f0cU, 0xc3cb050ddcc53530U,
  0x1e7ffffff11U, 0x22d00000212U, 0x5300000127U, 0x9800000018U,
  0x30800000070U, 0x227000000e1U, 0xbd000000ffU, 0xffffff21000001edU,
  0x31b0000003fU, 0x1c4ffffff5fU, 0xffffff57000001f2U, 0xffffffd200000398U,
  0x1000000d2U, 0x1ed00000072U, 0x1cffffffee2U, 0xa10000008aU,
  0x151d482694631554U, 0x58a8eba3d7b7c4a2U, 0x7fc22ff6f33eaa72U, 0x6feb3f0fe707003eU,
  0x7fcd2525d53c0e10U, 0x1b91e5206de2dcd8U, 0x45c6edc0091124ecU, 0xefed5ffad2c4c040U,
  0x812226fb3abdd725U, 0xad0aeec2beef1f3U, 0x4ea0317f8f83a00U, 0x1fd3cbd71be228c1U,
  0x624768d35ed12d62U, 0x2708ed7ffa1ca911U, 0x56edfec2fe266f12U, 0xeb39134dea05c656U,
  0xb35e3b526d977f4eU, 0xb6f7185bbb521ae4U, 0xde5a0513ef18bdabU, 0xf5489ceffd4a4d4dU,
  0xfd1acbe624cc7fdcU, 0xc294f23e2456823U, 0xfd6be750f1ca0012U, 0xb7e34eef4a04dfceU,
  0x2842c5785bb53622U, 0x7e0fc1b31ee01403U, 0x4704d884c4f3d7cdU, 0x122d67dd1b7f63cfU,
  0x32f471355ef6936U, 0xc204aa7f023e8df3U, 0xb315d4bd3fc21930U, 0xe2aff08cbcd3a50U,
  0x117f0df61bd64fc0U, 0xe25cba26e144e2e5U, 0xfdfc49f703d9eef6U, 0x4f1c1af4055e0023U,
  0xdb91d4e7787f1fdeU, 0xab53e3f4354f7dd2U, 0x3e4978574298e271U, 0x3ceb53bdc07f0d69U,
  0xefd9bb15281420e3U, 0xede83b153a287ff1U, 0xf558117d011f2fd6U, 0xe7f3eb13fad703f5U,
  0xa8ef244ac13fe4dbU, 0xf77fb8de3c3cef1aU, 0x1b5318776fc500f5U, 0x9d29ed2f13ecfe4U,
  0x7aeccb621333ed13U, 0x6ed70406c33d33ffU, 0xe924b8ffc67f5c52U, 0x13cc610ca3fb9fdcU,
  0xd1ae4fd4f170f315U, 0xda0509eb7fc13f50U, 0x2cf5d10e100f720dU, 0x1b36ec033e0be034U,
  0x1ca6091d81b3aff2U, 0x62f3e5c0ae1b31b8U, 0x69eb3101567df4f2U, 0xf63e4aa7486ebf42U,
  0x13c891e4e31e0da6U, 0x54f44eb270237f0bU, 0xf2c2442201aeb3cU, 0x9d128f1ee3463e8U,
  0x1ff000000a8U, 0x2c9000000feU, 0x3070000002eU, 0xa600000019U,
  0x36dffffffbdU, 0x235000006d0U, 0x639fffffe98U, 0x6c9fffffe81U,
  0x92916dec9e83e3dU, 0x20309e3dc481e500U, 0x4927317114f49eeaU, 0xb6b683f118ddef7fU,
  0x3093cab229ea2f02U, 0xbada2c0b81e621acU, 0xb8246ae1bd7f83a1U, 0x6e9b4bde3b4d3382U,
  0xa744b3acdf0481fcU, 0x26e7fba41da45424U, 0xffffff66fffffea9U, 0x28f000000f1U,
  0xfffffee5U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

