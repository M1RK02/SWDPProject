class LumosPacket {
  final int classLabel;
  final int f1, f2, f3, f4, f5, f6, f7, f8;
  final int clear, nir;

  const LumosPacket({
    required this.classLabel,
    required this.f1, required this.f2,
    required this.f3, required this.f4,
    required this.f5, required this.f6,
    required this.f7, required this.f8,
    required this.clear, required this.nir,
  });

  List<int> get channels => [f1, f2, f3, f4, f5, f6, f7, f8, clear, nir];
  List<String> get channelNames =>
      ['F1', 'F2', 'F3', 'F4', 'F5', 'F6', 'F7', 'F8', 'CLR', 'NIR'];
}