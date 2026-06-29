#include <WiFi.h>
#include <WebServer.h>

// 替换为你自己的2.4G WiFi名称、密码（不能5G、不能中文空格）
const char* ssid     = "SuperLUO";
const char* password = "12345678";

#define LED_PIN 2
WebServer server(80);
int brightness = 0;

// 网页前端：实时同步滑块，拖动即时发送，无延迟
String htmlPage = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 实时无极调光</title>
<style>
*{box-sizing:border-box}
body{text-align:center;margin-top:100px;font-family:Arial;}
#lightSlider{width:85%;height:32px;margin:40px auto;}
#numShow{font-size:60px;color:#0066ff;font-weight:bold;margin:20px 0;}
h2{font-size:28px;color:#333;}
</style>
</head>
<body>
<h2>网页实时平滑调光</h2>
<input type="range" id="lightSlider" min="0" max="255" value="0">
<div>当前亮度：<span id="numShow">0</span></div>

<script>
const slider = document.getElementById("lightSlider");
const numShow = document.getElementById("numShow");

// 实时发送亮度函数（无延迟）
function sendBright(val){
  let xhr = new XMLHttpRequest();
  xhr.open("GET", "/set?b="+val, false); // 同步请求，实时生效
  xhr.send();
}

// 拖动过程持续触发，实时更新
slider.addEventListener("input", function(){
  let val = this.value;
  numShow.innerText = val;
  sendBright(val);
});
</script>
</body>
</html>
)HTML";

// 首页页面
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// 接收亮度参数，立刻更新PWM
void handleSetBright() {
  if (server.hasArg("b")) {
    brightness = server.arg("b").toInt();
    analogWrite(LED_PIN, brightness);
    Serial.print("实时亮度更新：");
    Serial.println(brightness);
  }
  server.send(200, "text/plain", "ok");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, brightness);
  Serial.begin(115200);
  Serial.println("=== ex07 实时平滑调光启动 ===");

  // WiFi连接
  WiFi.begin(ssid, password);
  Serial.print("连接WiFi：");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  Serial.println("\nWiFi连接成功！");
  Serial.print("设备访问IP：");
  Serial.println(WiFi.localIP());

  // 注册网页接口
  server.on("/", handleRoot);
  server.on("/set", handleSetBright);
  server.begin();
  Serial.println("Web服务已就绪");
}

void loop() {
  // 持续处理网页请求，无阻塞延时，保证亮度平滑跟随
  server.handleClient();
}