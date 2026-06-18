// lib/utils/sensor_utils.dart

/// Returns a readable name for a given sensor data type.
String getSensorNameFromType(String dataType) {
  switch (dataType) {
    case 'A':
      return 'Accelerometer';
    case 'G':
      return 'Gyroscope';

    // --- EXAMPLE ---
    // You can add your own sensor type here
    // case 'T':
    //   return 'Temperature';
    // ----------------------------

    default:
      // If the type isn't listed, show the raw code
      // or a default message.
      return 'Unknown ($dataType)';
  }
}