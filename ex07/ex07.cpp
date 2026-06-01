#include <WiFi.h>
#include <WebServer.h>

// AP 热点信息
const char* ap_ssid = "Wang";
const char* ap_pass = "hanxiaorui";

const int LED_PIN = 2;
WebServer server(80);

// 生成网页
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>LED 亮度控制</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:50px;">
  <h1>LED 亮度调节</h1>
  <p>当前亮度：<span id="brightVal">0</span></p>

  <input type="range" min="0" max="255" id="slider" style="width:80%; height:30px;">
  <br><br>

  <a href="/on"><button style="padding:10px 20px;">点亮 LED</button></a>
  <a href="/off"><button style="padding:10px 20px;">熄灭 LED</button></a>

  <script>
    const slider = document.getElementById('slider');
    const brightVal = document.getElementById('brightVal');

    // 滑动条变化时发送数据
    slider.addEventListener('input', function() {
      let value = this.value;
      brightVal.innerText = value;
      // 发送到 ESP32
      fetch('/set?bright=' + value);
    });
  </script>
</body>
</html>
)rawliteral";
  return html;
}

// 首页
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 点亮
void handleOn() {
  analogWrite(LED_PIN, 255);
  server.sendHeader("Location", "/");
  server.send(303);
}

// 熄灭
void handleOff() {
  analogWrite(LED_PIN, 0);
  server.sendHeader("Location", "/");
  server.send(303);
}

// 接收滑动条亮度
void handleSetBright() {
  if (server.hasArg("bright")) {
    int bright = server.arg("bright").toInt();
    if (bright < 0) bright = 0;
    if (bright > 255) bright = 255;
    analogWrite(LED_PIN, bright);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  // 开启 AP 模式
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_pass);

  Serial.println("AP 已开启");
  Serial.print("WiFi 名称："); Serial.println(ap_ssid);
  Serial.print("WiFi 密码："); Serial.println(ap_pass);
  Serial.print("访问地址：http://");
  Serial.println(WiFi.softAPIP());

  // 网页路由
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/set", handleSetBright);

  server.begin();
}

void loop() {
  server.handleClient();
}
