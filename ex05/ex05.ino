// ex05 三档触摸调速呼吸灯
#define TOUCH_PIN T0
#define LED_PIN 2

bool lastTouch = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

int speedLevel = 1; // 1/2/3三档速度
int brightness = 0;
int fadeStep = 1;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // 触摸防抖+档位切换逻辑
  unsigned long currentTime = millis();
  int touchVal = touchRead(TOUCH_PIN);
  bool curTouch = (touchVal < 400);

  if (currentTime - lastDebounceTime > debounceDelay) {
    if (curTouch && !lastTouch) {
      speedLevel++;
      if (speedLevel > 3) speedLevel = 1; // 循环1-3
      lastDebounceTime = currentTime;
    }
  }
  lastTouch = curTouch;

  // PWM呼吸逻辑
  analogWrite(LED_PIN, brightness);
  brightness += fadeStep;

  // 亮度边界反转
  if (brightness <= 0 || brightness >= 255) {
    fadeStep = -fadeStep;
  }

  // 根据档位控制呼吸速度
  int delayTime;
  switch(speedLevel){
    case 1: delayTime = 10; break; // 慢
    case 2: delayTime = 5; break; // 中
    case 3: delayTime = 2; break;  // 快
  }
  delay(delayTime);
}