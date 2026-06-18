import 'dart:async';

import 'package:smart_wearables_app/connection/message_type.dart';

class MyStream {
  StreamController controller = StreamController.broadcast();
  StreamController controllerBattery = StreamController.broadcast();
  StreamController controllerSend = StreamController.broadcast();

  void setNum(List<int> data) {
    if (data[0] == MsgType.battery.description) {
      controllerBattery.add(data);
    } else {
      controller.add(data);
      // debugPrint("Count ${data.toString()}");
    }
  }

  // void setBatteryLevel(List<int> data) {
  //   controllerBattery.add(data);
  //   // debugPrint("Count ${data.toString()}");
  // }

  void sendData(List<int> data) {
    controllerSend.add(data);
    // debugPrint("Sent ${data.toString()}");
  }
}
