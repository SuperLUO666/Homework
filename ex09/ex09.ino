#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "SuperLUO";
const char* password = "12345678";

#define TOUCH_PIN 4
WebServer server(80);

String htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>触摸传感器仪表盘</title>
<style>
body{text-align:center;margin-top:120px;font-size:30px;}
#num{font-size:80px;color:#ff3333;font-weight:bold;}
</style>
</head>
<body>
<h2>触摸传感器实时数值</h2>
<div id="num">0</div>
<script>
function getData(){
  fetch("/data")
  .then(res=>res.text())
  .then(val=>{
    document.getElementById("num").innerText = val;
  })
}
// 每200ms刷新一次数据
setInterval(getData, 200);
</script>
</body>
</html>
)HTML";

void handleRoot(){
  server.send(200, "text/html", htmlPage);
}
// 返回触摸原始值
void handleData(){
  int val = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(val));
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){delay(500);Serial.print(".");}
  Serial.println("\n访问IP："+WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();
}

void loop() {
  server.handleClient();
}
