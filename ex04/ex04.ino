// ex04 触摸自锁开关 - 带串口调试版
#define TOUCH_PIN T0    // 触摸引脚GPIO4
#define LED_PIN 2       // LED输出引脚

bool ledState = LOW;    // LED状态：默认熄灭
bool lastTouch = false; // 上一轮触摸状态
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50; // 防抖50ms

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  digitalWrite(LED_PIN, ledState);
  Serial.println("===== ex04 触摸自锁调试启动 =====");
  Serial.print("触摸引脚：");
  Serial.println(TOUCH_PIN);
  Serial.print("LED引脚：");
  Serial.println(LED_PIN);
  Serial.println("操作说明：触摸金属片，观察串口输出");
}

void loop() {
  unsigned long currentTime = millis();
  // 读取触摸原始值
  int touchVal = touchRead(TOUCH_PIN);
  bool curTouch = (touchVal < 400);

  // 每200ms打印一次基础状态，防止刷屏
  static unsigned long printTimer = 0;
  if(currentTime - printTimer > 200){
    Serial.print("原始触摸值：");
    Serial.print(touchVal);
    Serial.print(" | 当前触摸判定：");
    Serial.print(curTouch ? "是触摸" : "未触摸");
    Serial.print(" | LED当前状态：");
    Serial.println(ledState ? "亮" : "灭");
    printTimer = currentTime;
  }

  // 防抖逻辑
  if (currentTime - lastDebounceTime > debounceDelay) {
    // 边缘检测：上一次没触摸，当前触摸 = 按下瞬间
    if (curTouch == true && lastTouch == false) {
      Serial.println("【触发有效触摸！翻转LED状态】");
      ledState = !ledState; // 翻转LED状态
      digitalWrite(LED_PIN, ledState);
      lastDebounceTime = currentTime;
    }
  }

  lastTouch = curTouch; // 更新上一次触摸状态
}
