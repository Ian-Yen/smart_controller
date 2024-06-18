#include <IRremote.h>

int RECV_PIN = 7;
int IR_LED_PIN = 5;

IRrecv irrecv(RECV_PIN);
decode_results res;

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("IR Receiver is Enabled");
}

void loop() {
  if (irrecv.decode(&res)) {
    Serial.println(res.value, HEX);

    irrecv.resume(); // 准备接收下一个值
  }
  delay(100);
}