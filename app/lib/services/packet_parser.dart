import 'dart:typed_data';
import '../models/lumos_packet.dart';

class PacketParser {
  static const int packetLength = 24;
  static const int _startByte  = 0x7B; // '{'
  static const int _typeByte   = 0x4C; // 'L'
  static const int _endByte    = 0x7D; // '}'

  static LumosPacket? parse(List<int> raw) {
    if (raw.length != packetLength) return null;
    if (raw[0] != _startByte)      return null;
    if (raw[1] != _typeByte)       return null;
    if (raw[23] != _endByte)       return null;

    // raw[2..22] = class_label + 10×uint16  (21 bytes, skip end marker)
    final bd = Uint8List.fromList(raw.sublist(2, 23)).buffer.asByteData();

    return LumosPacket(
      classLabel: bd.getUint8(0),
      f1:    bd.getUint16(1,  Endian.little),
      f2:    bd.getUint16(3,  Endian.little),
      f3:    bd.getUint16(5,  Endian.little),
      f4:    bd.getUint16(7,  Endian.little),
      f5:    bd.getUint16(9,  Endian.little),
      f6:    bd.getUint16(11, Endian.little),
      f7:    bd.getUint16(13, Endian.little),
      f8:    bd.getUint16(15, Endian.little),
      clear: bd.getUint16(17, Endian.little),
      nir:   bd.getUint16(19, Endian.little),
    );
  }
}