// ex06 双通道反相PWM，上电初始A灭、B全亮
#define LED_A 16
#define LED_B 17

int val = 0;    // 初始值0，上电A=0，B=255
int step = 1;

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  Serial.begin(115200);
  Serial.println("ex06启动，初始A灯灭，B灯全亮");
  // 上电瞬间直接输出初始亮度
  analogWrite(LED_A, val);
  analogWrite(LED_B, 255 - val);
}

void loop() {
  int lightA = val;
  int lightB = 255 - val;

  analogWrite(LED_A, lightA);
  analogWrite(LED_B, lightB);

  // 串口打印亮度，方便排错
  static unsigned long printTime = 0;
  if (millis() - printTime > 200) {
    Serial.print("A亮度：");
    Serial.print(lightA);
    Serial.print("  B亮度：");
    Serial.println(lightB);
    printTime = millis();
  }

  val = val + step;
  // 到达边界反转变化方向
  if (val <= 0 || val >= 255) {
    step = -step;
  }
  delay(8);
}