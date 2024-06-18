#include <ESP8266WiFi.h>
#include <ArduinoWiFiServer.h>
#include <IRremote.h>

#ifndef STASSID
#define STASSID "OPPO Reno4 5G"
#define STAPSK "abcdefgh"
#endif

const uint16_t IR_LED_PIN = D1; // ESP8266上的引腳
const uint16_t IR_rec_pin = D2; // ESP8266上的引腳
const char* ssid = STASSID;
const char* password = STAPSK;
const uint16_t port = 80; 
WiFiServer server(port);
IPAddress ipG;
IRsend irsend(IR_LED_PIN);
IRrecv irrecv(IR_rec_pin);
decode_results res;
int var[5] = {100, 100, 100, 100, 100};


void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  server.begin();

  IPAddress ip = WiFi.localIP();
  ipG = ip;
  Serial.println();
  Serial.println("Connected to WiFi network.");
  Serial.print("To access the server, connect with Telnet client to ");
  Serial.print(ip);
  server.begin();  // 啟動TCP伺服器
  irrecv.enableIRIn(); // 启动红外接收器
  Serial.println("TCP server started");
}

void loop() {
  WiFiClient client = server.available();  // 檢查是否有客戶端連接

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // Serial.write(c);
        if(c == '$'){
          Serial.println(currentLine);
          if (currentLine == "detect 0"){
            delay(3000);
            if (irrecv.decode(&res)) {
              var[0] = res.value;
              // Serial.println(resultToHumanReadableBasic(&res));
              irrecv.resume();
            }
          }if (currentLine == "detect 1"){
            delay(3000);
            if (irrecv.decode(&res)) {
              Serial.println(var[0]);
              var[1] = res.value;
              irrecv.resume();
            }
          }if (currentLine == "detect 2"){
            delay(3000);
            if (irrecv.decode(&res)) {
              var[2] = res.value;
              irrecv.resume();
            }
          }if (currentLine == "detect 3"){
            delay(3000);
            if (irrecv.decode(&res)) {
              var[3] = res.value;
              irrecv.resume();
            }
          }if (currentLine == "detect 4"){
            delay(3000);
            if (irrecv.decode(&res)) {
              var[4] = res.value;
              irrecv.resume();
            }
          }
          if (currentLine == "0"){
            irsend.sendSony(var[0], 12);
          }if (currentLine == "1"){
            irsend.sendSony(var[1], 12);
          }if (currentLine == "2"){
            irsend.sendSony(var[2], 12);
          }if (currentLine == "3"){
            irsend.sendSony(var[3], 12);
          }if (currentLine == "4"){
            irsend.sendSony(var[4], 12);
          }
          
          currentLine = "";
        }else{
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}