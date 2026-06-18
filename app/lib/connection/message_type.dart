enum MsgType {
  /* --------------------------------- general -------------------------------- */
  battery(0xb0),
  not_worn(0xb1),
  calibration(0xb2),
  frame_not_active(0xb3),
  frame_moving(0xb4),
  /* ----------------------------------- ppg ---------------------------------- */
  ppg_ir(0xc0),
  // ppg_ir_filt(0xc1),
  ppg_beat(0xc2),
  /* ----------------------------------- ecg ---------------------------------- */
  ecg(0xd0),
  ecg_r2r(0xd1),
  ecg_start(0xd2),
  ecg_stop(0xd3),
  pat(0xd4),
  /* ----------------------------------- har ---------------------------------- */
  har(0xe0),
  /* ----------------------------------- end ---------------------------------- */
  end(0xff); // end tag

  final int description;
  const MsgType(this.description);
}
