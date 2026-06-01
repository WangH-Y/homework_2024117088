#include <WiFi.h>
#include <WebServer.h>

const char* ap_ssid = "Wang";
const char* ap_pass = "hanxiaorui";

const int LED_PIN = 2;
const int TRIGGER_PIN = 4;

bool isArmed = false;
bool isAlarming = false;

WebServer server(80);

String makePage() {
  String status;
  if (isAlarming) {
    status = "<span style='color:red'>? 报警中！</span>";
  } else if (isArmed) {
    status = "<span style='color:blue'>? 已布防</span>";
  } else {
    status = "<span>已撤防</span>";
  }

  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>安防系统</title>
</head>
<body style="text-align:center; margin-top:50px;">
  <h1>ESP32 报警系统</h1>
  <h2>状态：)rawliteral" + status + R"rawliteral(</h2>

  <a href="/arm"><button style="padding:15px 30px; font-size:20px;">布防</button></a>
  <a href="/disarm"><button style="padding:15px 30px; font-size:20px;">撤防</button></a>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot()  { server.send(200, "text/html; charset=utf-8", makePage()); }
void handleArm()   { isArmed = true;  isAlarming = false; digitalWrite(LED_PIN, LOW); server.sendHeader("Location","/"); server.send(303); }
void handleDisarm(){ isArmed = false; isAlarming = false; digitalWrite(LED_PIN, LOW); server.sendHeader("Location","/"); server.send(303); }

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, INPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.println("AP启动");
  Serial.println("IP: 192.168.4.1");

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();

  static unsigned long lastBlink = 0;
  bool ledState = 0;

  // 布防检测
  if (isArmed && !isAlarming) {
    if (digitalRead(TRIGGER_PIN) == HIGH) {
      isAlarming = true;
    }
  }

  // ===================== 高频闪灯核心 =====================
  if (isAlarming) {
    if (millis() - lastBlink >= 50) {  // 30ms 切换 = 高频快闪
      lastBlink = millis();
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
}
