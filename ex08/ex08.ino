#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "SuperLUO";
const char* password = "12345678";

#define LED_PIN 2
#define TOUCH_PIN 4

WebServer server(80);
bool isArm = false;    // 布防状态
bool isAlarm = false;  // 报警锁定

String htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>安防报警器</title>
<style>
button{font-size:26px;padding:15px 40px;margin:20px;}
</style>
</head>
<body style="text-align:center;margin-top:100px;">
<h2>物联网安防系统</h2>
<button onclick="fetch('/arm')">布防 ARM</button>
<button onclick="fetch('/disarm')">撤防 DISARM</button>
<p>当前状态：<span id="state">未布防</span></p>
<script>
function refreshStatus(){
  fetch("/status").then(res=>res.text()).then(t=>{
    document.getElementById("state").innerText = t;
  })
}
setInterval(refreshStatus, 500);
</script>
</body>
</html>
)HTML";

void handleRoot(){
  server.send(200, "text/html", htmlPage);
}
// 布防接口
void handleArm(){
  isArm = true;
  server.send(200, "text/plain", "armed");
}
// 撤防接口，重置报警
void handleDisarm(){
  isArm = false;
  isAlarm = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain", "disarmed");
}
// 返回当前状态给网页
void handleStatus(){
  String s;
  if(!isArm) s = "未布防";
  else if(isAlarm) s = "报警中！";
  else s = "已布防，等待触发";
  server.send(200, "text/plain", s);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){delay(500);Serial.print(".");}
  Serial.println("\nIP地址："+WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.on("/status", handleStatus);
  server.begin();
}

void loop() {
  server.handleClient();
  int touchVal = touchRead(TOUCH_PIN);
  bool touchNow = (touchVal < 400);

  // 布防且未报警时，触摸触发锁定报警
  if(isArm && !isAlarm && touchNow){
    isAlarm = true;
  }

  // 报警状态：高频闪烁
  if(isAlarm){
    digitalWrite(LED_PIN, HIGH);
    delay(80);
    digitalWrite(LED_PIN, LOW);
    delay(80);
  }else{
    digitalWrite(LED_PIN, LOW);
  }
}