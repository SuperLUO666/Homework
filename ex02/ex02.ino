const int ledPin = 2;
unsigned long currentTime = 0;
unsigned long lastTime = 0;
const long interval = 500; // 500ms切换一次，1Hz闪烁
bool ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  currentTime = millis(); // 获取系统运行毫秒，非阻塞
  if (currentTime - lastTime >= interval) {
    lastTime = currentTime;
    // 翻转LED状态
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    if(ledState) Serial.println("LED ON");
    else Serial.println("LED OFF");
  }
  // 此处可添加其他代码，不会被delay阻塞
}
