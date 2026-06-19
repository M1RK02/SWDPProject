/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-19T11:52:22+0200
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
  0xf10e222e4402e177U, 0xb74ff4273ee43012U, 0xcdea154b09b7be14U, 0xb17f1c46cfd43342U,
  0xfb22ce0fc5fa27ffU, 0x2722e73875405ee0U, 0xd6b212031b07d5a9U, 0xf6b2fba811f8381dU,
  0x7f11d2051cf33a1aU, 0xe7f1d029e03354b0U, 0xac1ad9adc3d2c40eU, 0x45f8d8f02930233eU,
  0x51efeab636f3e106U, 0xd3eacc0fc3ff3efbU, 0xe424864f01e21c81U, 0xfccd45117f3fe2d9U,
  0xe2dcf50e12db545fU, 0x1c7be163012e5b9U, 0x4ef515e72c262e2cU, 0xbbfa2a1a4305677bU,
  0x595161eafce5181cU, 0xa6a6ffe2bfd9442cU, 0xcb3b1de21868023dU, 0x2bf2e5593b4cdf44U,
  0x247f3c2f67059ca5U, 0x9c9192945d622b2U, 0xccd3f49112c20746U, 0xa5d781582306ebe4U,
  0xd6c7b0916c0b7078U, 0x2f0d4a20f2d0f61bU, 0x5e7f1a500aff274aU, 0xb279a2d41f8175aU,
  0x9df6d39bc0bbcc9eU, 0xc6f52719c708fb1dU, 0x24faff2063e71deaU, 0xe3da423c0cf80118U,
  0xe0fb17c7cede2139U, 0x1275fa3c0abd56e5U, 0xbc6f22ac1bd103c3U, 0x6c536018aaf604dU,
  0xc72cf1ca114b7fe1U, 0xf6f545eabcfbc105U, 0xb3e4e332115a36d6U, 0x2fc7101e2ac9bd2bU,
  0xd281c5245dea09eaU, 0xef202420d0f4ecfeU, 0xecd0eb028db1c617U, 0xf118ba7bf9164ac1U,
  0xb7a4abe407aa31f1U, 0xed81bbdcef1f080bU, 0xa9d03516ddf3f52dU, 0xcf759c3fc18aa715U,
  0x5ad25a75b41f2300U, 0xf0cafbdd9276cc33U, 0xb9a39b1fb581cd4eU, 0xbd3d768b5b3d1015U,
  0xc3cb81b0b9fade96U, 0x705080a2ea11e0bU, 0xd8e3241334df3be9U, 0x13fefb0a10e314a1U,
  0x3aece3d6ee02f1dcU, 0x3d677f7d11e225ebU, 0xe3e81c1f9ae9faeeU, 0xd39280c2720db25U,
  0xdbdab79a983ab7feU, 0xd6f4d63ca7566323U, 0xe2c9ddd64af91618U, 0x260007f5d702b6ecU,
  0xfc567140ded9f97fU, 0x28ba233ef6b9f4bcU, 0x943543909ae1c51aU, 0xede754c4172b1620U,
  0xf4cee99bcd04e13fU, 0x14d9b98f810596c2U, 0xcbcbfd232b30223eU, 0x2742054ff4f4000fU,
  0x34c34219f3f6e6d8U, 0x2d17d016613593ceU, 0x5a12e5ebe82d51e8U, 0xf0462fe8b3b6b606U,
  0xab3117810518d843U, 0x2b40c51ebc2a7c1cU, 0x41ebdb16e8ad9281U, 0x71d6178e9ac0a2d0U,
  0xf15658d0429dbbd1U, 0xdf2b0dede4e3d5bbU, 0x27e0aff653e45917U, 0x56dff23bca2cde16U,
  0x5d541daf15bc1fb8U, 0x938011d0c88cb55U, 0x81ecc8e69eb312dfU, 0x211bc6b2ff96f58eU,
  0x302b139e3b26bdf1U, 0xd6ccbcc2633814dcU, 0xa7c14abdc7433844U, 0xd0248d571dcb4ee8U,
  0x5de2def03f014981U, 0x4ecc0cd781c1f73bU, 0xcada313857768c91U, 0xb9ca6c51dd00fb88U,
  0xa8f444de0e19e2c4U, 0x57a339d407b01fcbU, 0x1cc7899060cce537U, 0x6822220c1bb3f1d1U,
  0x9853f30281e22009U, 0xdd0ce2d8cb23b9fbU, 0x3f06bf25d704130fU, 0xed8714d911f92365U,
  0xb0ca415f0838cb9eU, 0x29f6a61b81fdc4f7U, 0x101d31f002b10622U, 0xd906d818ad4da4dcU,
  0xf20ebab8cce96659U, 0x2f5a90ceddfbbc04U, 0xd17f8551269a5803U, 0xdbe700f2fab29e46U,
  0x3036c918f5b7c7a6U, 0xa9e66101e95b5492U, 0xb608a8144c84315U, 0xa80bd1a7a3d60837U,
  0xdf69e313bdb82b94U, 0x990b249808af5cdcU, 0x11a32ec51bfd3b00U, 0x3c19c681bae00e4bU,
  0x1d234526a9dcfbf0U, 0xa80f47f045a793e3U, 0xdfac2f0411e9f39aU, 0xeafdc49dac5209d6U,
  0xd00eeca04cf2c7edU, 0x81a841be1b1e1cd6U, 0xf0bdbc1e5ca5c34dU, 0xb82fda443ca50358U,
  0x37396364b53f1bcaU, 0x5cf954a82d74addU, 0x3744c0d24c222a0dU, 0xb1d8991accd41a81U,
  0x4721540b3ddf29f5U, 0x3dd8d3cbdf8173dbU, 0x964c25b421d1190bU, 0x6046646818f8fbd0U,
  0xa8ac3163fe8d43a0U, 0x7f599bf450333fcfU, 0x125dda0a3ddee9e3U, 0x3bcf794e345de03fU,
  0x9fb82f92d71ae437U, 0x40025a8580f37f3U, 0x9ae6901bf5b5b3f3U, 0x8157d4ccfb3e3b29U,
  0x8137c52f92f4ea2bU, 0x28fc1185c1aaeff6U, 0x20f3357f1261ad1aU, 0x2871f223f6351a16U,
  0x1726c13543d3ed40U, 0x301345642005e62dU, 0xebda134b114247faU, 0x4ddf556d3a48b11dU,
  0x948196e640234addU, 0xb03c9aca57efcb9bU, 0x3bbb45ec15f08a0cU, 0xb7f909a2ea1200bU,
  0x91fdb99f210bbbfdU, 0xe8a90b84c8e40992U, 0xc6ed3976e5e431e7U, 0xccbffdf3e1ad3314U,
  0x5aefffffe8cU, 0x48c000004dfU, 0x596000002b9U, 0x15c00000566U,
  0x3c200000483U, 0x4b80000028cU, 0x3150000060dU, 0xffffff83000004f9U,
  0x6c600000385U, 0x1ecfffffe7aU, 0x5d00000383U, 0x1260000065dU,
  0x4c00000591U, 0x3f000000c6U, 0x39d0000015bU, 0x47a00000500U,
  0x360f222596603459U, 0x5ec608a6e1b7f0afU, 0x7fc62cf8f723a65dU, 0x6fdf4703edfa0739U,
  0x7fbc1e349b512af6U, 0x2aaeef204dd9eae7U, 0x55beebae080e5f09U, 0xe73b7af6d0b9bd5eU,
  0x812b550848a1a143U, 0x10ac9cbc36e0c8ddU, 0xfa5d7d6e3a77907U, 0x41878bb700ac64afU,
  0x364359df44e4252cU, 0x1832eb6af310ba1eU, 0x4dfc08d5111a7f19U, 0xfa580c53ff1ee555U,
  0xdd59244a7fa7624bU, 0xcff8104fed2e1de2U, 0xde52f823f918beb2U, 0xb24b3fb084d3e33U,
  0x191bd7fb2bd975e1U, 0x1ff35505284b7f31U, 0xfa72d44cf1d7130dU, 0xb0d357e24e03d6c4U,
  0x3f49d57a71c74f28U, 0x7f3ef0d115e23ef0U, 0x5e03e6a6e2f9dfd9U, 0x291d5fea2a6858d6U,
  0xec3740245802721bU, 0xd426bb7f0c1ec8fcU, 0xb829d2cf31df3f3aU, 0x8510f1eebdc3251U,
  0x1a7f00f344d052e1U, 0xdf60c53ae728ffcdU, 0xf913411707e7e1f3U, 0x650f12061156fe02U,
  0xdb8fe0d5486cf9d0U, 0x9f5cd8de084e42d7U, 0x3e276f433d9ddd6aU, 0x40f542bccd7f1169U,
  0xe9bab6fb31020de7U, 0xe5a33ef635427f03U, 0xf96b0c7ffe220ab0U, 0xdcb3fab4f4d7f0deU,
  0xcefa3c4a0c2bee1cU, 0xfe57d7fb693df521U, 0x1f591e7f5fe0ebf4U, 0xcb8c3d4f53adbceU,
  0x7be1bb6d0b3c130eU, 0x7edc170bac385503U, 0xe72cb106c17f7454U, 0xadb730aaaf09ce6U,
  0xd08a54dadc60e3ffU, 0xdaee14e770e41f5bU, 0x290fea0e11077f17U, 0xc47030f3217f23bU,
  0x2c9df12381b9cff7U, 0x780bf3c0a2194daeU, 0x71ea33066175efe5U, 0xf43750974c5ebe44U,
  0x32b5bfe4ce09f8cbU, 0x60e6468d74326e19U, 0x1e2722311c2de448U, 0x15b62f81ea2d5ce9U,
  0x43d000000deU, 0x3eaffffffabU, 0x2b2ffffff74U, 0x36d0000005cU,
  0x24affffff9eU, 0x1c600000454U, 0x4f6ffffff4dU, 0x466fffffe65U,
  0xfb3622dab7e85045U, 0x2c39814cb681e3e8U, 0x422e266f0e148de6U, 0xcdbe81f111e0e67cU,
  0x3981dcab32e63cf2U, 0xb2cd410d95ad2ba6U, 0xa2145be0b94786b9U, 0x4db53bdd34441c81U,
  0xb142abaff5088101U, 0x40edf9ab1ab7513cU, 0xffffff3cfffffed0U, 0xe900000156U,
  0xffffff6aU,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

