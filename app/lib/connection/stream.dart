import 'dart:async';

class MyStream {
  StreamController controller = StreamController.broadcast();
  StreamController controllerSend = StreamController.broadcast();

  void setNum(List<int> data) {
    controller.add(data);
  }

  void sendData(List<int> data) {
    controllerSend.add(data);
  }
}