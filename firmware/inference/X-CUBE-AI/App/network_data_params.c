/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-06-20T17:07:47+0200
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
  0xd51914fb29b5ba15U, 0x9445ef2210ea12c1U, 0xb7c7c147f6e2810eU, 0xbc1fa4eee6050cfbU,
  0x2a08d7f4c2bdf11fU, 0x85ec032fa4490204U, 0x3843fd8541b45939U, 0x3b0adcc810b5e9e6U,
  0x5f8e63e6b7233de4U, 0xbfcfdf81ece399f1U, 0x2eed1830b104d093U, 0x2722a0e024dc21c8U,
  0xe30c41272ea6b1edU, 0x1ed210eb190fee52U, 0x4f066a1e0610a07fU, 0xbfd111e535470ff7U,
  0xda2306f1bc81a9c4U, 0x1603272041271cb4U, 0xfe0e23052b2440e5U, 0x4d445bdcf2090a0eU,
  0xad8bf1d5c1eb6110U, 0xe0223a93f950f91eU, 0xfbc7b256293df76eU, 0x286d332c7f04c507U,
  0xfb1a0ae21af91c0dU, 0xfe22d9b7d6ae3cd9U, 0x3734dfa2bdfa0a19U, 0xab1b06decaac27c8U,
  0xe82d45120210d681U, 0x50f8107f5478c3cbU, 0xe1d002e0214bc122U, 0xc708f129be0ad79aU,
  0x11fd20f0dbe320e1U, 0xf104012725c005ddU, 0xdae781971013d80aU, 0xf317e6bb4f08fc1aU,
  0x29d205e8fce72d25U, 0x242cd0f31517d741U, 0xfaa33a0529b420bcU, 0x2e72201d000a9f9U,
  0xcfdfd04ac14e4581U, 0x249af3303fb1c21fU, 0xe0a898dd4efe13d3U, 0xd43d230baeaed73eU,
  0xe7d630cd272481caU, 0xf25aea1af01ed209U, 0x3115b5f2e011dae2U, 0x3066ffe7bb9d96f7U,
  0xffd5d9cff603ef02U, 0xadfc160cdb5abe3cU, 0xb328d90533c42a13U, 0xe9df0d1a1cddffe8U,
  0x7f0b49ffb8dca41aU, 0x810207abec3072d2U, 0xec0bdba928efd4e3U, 0xee0910f530d921deU,
  0x100f10dc08c2ed19U, 0x16eee7103107fde1U, 0x4c7f6c6d2eed1aeaU, 0xfc153440cadd25e7U,
  0x2f162efd0817ea28U, 0xe11e3c3b609c5edU, 0xddf7df0ae02f2034U, 0xb3c08f2ffecde1fU,
  0x2c48ffdeb510d3deU, 0x52d6c77f685ea6a2U, 0xa6cb12885869d6e3U, 0xea27917769d2d21fU,
  0xa31bed17cecd5b41U, 0xbc46a5fc0fa8fe81U, 0x11231b42121f0adeU, 0xc63029d2c8f012U,
  0xcade9da8eb67f13bU, 0xfb23381581cb0e2dU, 0xe7eded282ede1b14U, 0x30fdcfcecb0b3408U,
  0x12d62103b91b003aU, 0xe211d91b3a05e62eU, 0x48d9da5f11dc9181U, 0x63c9f0b081c6a3edU,
  0xb5a5fd433acf211U, 0x51d16b2b1a4e4d2U, 0xdff0b9d632b03b50U, 0xf9bc61dc1d5b133fU,
  0xdcba193c1000e79aU, 0x622b042f98ef4681U, 0xe202c3dea0d7c5feU, 0x260e2a50d302e3e9U,
  0x37420a352c320d81U, 0xe51e2d4ffbc79eabU, 0xe4d445e6ddcf65a0U, 0x3b416283e62b81eaU,
  0xe8ffc4485eeafeb7U, 0x34383124c5ce34e3U, 0x7f3afafa03b5001eU, 0x4cfafc01cfd284a1U,
  0x46f9f7bf2cb8c7e7U, 0x30b29898f34732dbU, 0x5906f78e08c5f3ceU, 0x81680bc28394e72eU,
  0x9f01f001ce36f8ebU, 0x3d5cdb2406140e01U, 0x13d0f38a23072923U, 0xbfd3a6df5b65103fU,
  0xed2b2531817890ffU, 0x9bc4102d2be202cbU, 0x6649c1f50518997bU, 0xb820d3323e52d911U,
  0x1bcafeaa0e11bcfcU, 0x57f0edf10a18f151U, 0x892878af1353abfU, 0xc9235eb97f24390aU,
  0xb2b925d4cf3bcff9U, 0x13d7cbf9326ee636U, 0x1627bfbc289bea2eU, 0x63c92bba74dfdc53U,
  0x7fe617f636ddb5e5U, 0x30150390d8352663U, 0x9ef83f081ad54bfcU, 0xc2030486e00d8130U,
  0x1acdcffb1e1c37b4U, 0xa613f2cff20e47edU, 0x37ef8ac20aa65dceU, 0xad789481f288ce00U,
  0xfa49e1d18afe6ec2U, 0xf1cc8b3cc63013abU, 0x81c5072d6d57d83aU, 0x103b1cfacaf8ce43U,
  0xbab856981524c7c7U, 0x2bd923e5d3b6833fU, 0xd6b058aa3e0831e9U, 0x16c521f440fbd5c1U,
  0x4d2c09f0f5b6d167U, 0x1472c2b62ea33e1bU, 0x81f34d402e04afc5U, 0xdd3c2403cad84f11U,
  0x1e7f6545bb2e3679U, 0xf1e041f3def1181cU, 0xfd081bd3dad41fabU, 0xb8fcc71e0ca1c6a0U,
  0xe02f10c317330027U, 0xcbe98133d7c2fb52U, 0xd200251affcab7edU, 0x5fe2df6195b152fU,
  0xfa3ff529ea09e81aU, 0x5e8d4dafa252f13U, 0x113b5f4f1dfb28c4U, 0xc228b2ee00145bffU,
  0x18f030fd1edcdd2dU, 0x2afc3314edb1d9eeU, 0xff6ba193812da8f2U, 0x9fc642b90f0838fcU,
  0xb3971c30c80dfd59U, 0x6a386d21b81a12bU, 0x224bcdeb5a07e4a4U, 0x1611a4af5221d1f0U,
  0x23700000599U, 0x24600000396U, 0x53e0000008fU, 0xffffff71000001f1U,
  0x1a0000002e3U, 0x46b000001c2U, 0xfffffdce000002c4U, 0xffffff2000000294U,
  0x770ffffffedU, 0x564fffffeb6U, 0xffffff0b000005ccU, 0xeb00000241U,
  0xfffffeab000005b1U, 0x26500000260U, 0xffffffb00000003bU, 0x4f400000069U,
  0x16fb0406ac5b3860U, 0x3dc307a6e0919dbaU, 0x6dba1cddd103817dU, 0x56032cece40f1522U,
  0x7fc0d828b8356111U, 0xf701f8187dc6fa06U, 0x3c18f1b5f2e723c8U, 0xb5126dbed2969730U,
  0x813c63083aead533U, 0x23c3c6de28fcfbfbU, 0x13d5e6181fdd400cU, 0x48b7c1020ebc15f0U,
  0x7d6546ed54b15064U, 0x534ae97c0a16a43cU, 0x65f6eab7080a5904U, 0xd8695a3cffebb87fU,
  0xd85b273e738e2c25U, 0x9a930e5aba4c1cd8U, 0xdc4f070afe1fdaa0U, 0xfd209b1cfa52637fU,
  0xf1b4f8e723f057b7U, 0x58105700c07c5a23U, 0x77ca67ffeba532dU, 0x92b7350175f193cbU,
  0xf82bf0633da7ff1dU, 0x3be0cb8d1fc372f5U, 0x19eb0395a801f2f9U, 0x61c4df0087a7fa1U,
  0xb3b0c264fd37f31U, 0x951e9d7b363a981dU, 0xac47d0b7399c0a29U, 0xeb3f260ad8b94951U,
  0x3a7ce8df2ec218b3U, 0xad0bb93cc93ef4c8U, 0xd00c72fcf8db18f2U, 0x5109251d1b7f372cU,
  0xe5a5c7ec5564560bU, 0xd762dbff1e333cd0U, 0x3b1760433781ad67U, 0x3fff6c9dae6f0c57U,
  0xc0ce09f9fc0d0305U, 0x4df09140a092cc4U, 0x2a0ce47f1a05e1c5U, 0x2c94cb0bb9ca9e2fU,
  0xac0c7f40f944c811U, 0x1d1eaa193924cbe5U, 0x504f97d76d108daU, 0x37b1b00cf423a70cU,
  0x42ebb95c2a26ebf7U, 0x41db2508c8426609U, 0xe25ecd05c17f503fU, 0xf0cb39fcc9f2b5adU,
  0xc1ad70b6c8594051U, 0xe572f8d97fc3176aU, 0x4fddd91a24f245f5U, 0x3040f7e22e10b72aU,
  0x499df18b3b495d9U, 0x2fd113d0c42063ccU, 0x452547f8307ffdf4U, 0xdc2e2da74d63f705U,
  0x39bd8dbea060db0U, 0x5ee04fa61d2f6fe4U, 0x16152a1d086a067fU, 0x1d71dc3d53a6fcbU,
  0xf1fffffcf6U, 0x4f200000641U, 0x51700000001U, 0x6a00000101U,
  0x18bfffffedaU, 0x5e4000003e8U, 0x6efffffff91U, 0x432fffffec3U,
  0x82943efa9bf1136U, 0x4447815f9da0e6e8U, 0x322b096f0f27aefaU, 0xcbb781e30cc8e37bU,
  0x3da7f7ba2da144e8U, 0xafc032de81a9238cU, 0xb51147c1f07e81caU, 0x19af51e35d47209cU,
  0xba72b3aecdbc81ffU, 0x64e5edabedab5f38U, 0xffffff24fffffe82U, 0x293000000e8U,
  0xfffffe70U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

