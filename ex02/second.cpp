// 定义LED引脚，ESP32通常板载LED连接在GPIO 2
const int ledPin = 2; 

void setup() {
  // 初始化串口通信，设置波特率为115200
  Serial.begin(115200);
  // 将LED引脚设置为输出模式
  pinMode(ledPin, OUTPUT);
}

void loop() {
// SOS 示例思路
// S: 短闪3次
for(int i=0; i<3; i++) { digitalWrite(ledPin, HIGH); delay(200); digitalWrite(ledPin, LOW); delay(200); }
delay(500); // 字母间隔
// O: 长闪3次
for(int i=0; i<3; i++) { digitalWrite(ledPin, HIGH); delay(600); digitalWrite(ledPin, LOW); delay(200); }
delay(500);
// S: 短闪3次
for(int i=0; i<3; i++) { digitalWrite(ledPin, HIGH); delay(200); digitalWrite(ledPin, LOW); delay(200); }
delay(2000); // 单词间隔
}
