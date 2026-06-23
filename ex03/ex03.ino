// SOS灯光参数定义
const int ledPin = 2;
// 时间常量 单位ms
const unsigned long T_SHORT_ON  = 200;    // 短闪光时长
const unsigned long T_LONG_ON   = 600;    // 长闪光时长
const unsigned long T_GAP       = 200;    // 同字符内光点间隔
const unsigned long T_CHAR_GAP  = 500;    // S-O、O-S 字母间隔
const unsigned long T_WORD_GAP  = 2000;   // 整套SOS结束长停顿

// millis时间变量
unsigned long nowTime;
unsigned long lastTick = 0;
unsigned long waitDelay = 0;
bool ledStatus = LOW;

// 状态机枚举
enum State {
  IDLE,        // 空闲，等待下一轮SOS
  S1_SHORT,    // 第一个S：3短闪
  O_LONG,      // O：3长闪
  S2_SHORT     // 第二个S：3短闪
};
State curState = IDLE;
int flashCnt = 0; // 当前闪烁计数

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  Serial.begin(115200);
}

void loop() {
  nowTime = millis();
  // 等待延时未到，直接退出
  if (nowTime - lastTick < waitDelay) return;
  lastTick = nowTime;

  switch (curState) {
    case IDLE:
      // 开始新一轮SOS，进入第一个S
      curState = S1_SHORT;
      flashCnt = 0;
      waitDelay = 0;
      break;

    case S1_SHORT:
      if (flashCnt < 3) {
        if (!ledStatus) {
          // 点亮短灯
          digitalWrite(ledPin, HIGH);
          ledStatus = HIGH;
          waitDelay = T_SHORT_ON;
        } else {
          // 熄灭，计数+1，间隔等待
          digitalWrite(ledPin, LOW);
          ledStatus = LOW;
          flashCnt++;
          waitDelay = T_GAP;
        }
      } else {
        // 3次短闪完成，间隔后进入O
        digitalWrite(ledPin, LOW);
        curState = O_LONG;
        flashCnt = 0;
        waitDelay = T_CHAR_GAP;
      }
      break;

    case O_LONG:
      if (flashCnt < 3) {
        if (!ledStatus) {
          digitalWrite(ledPin, HIGH);
          ledStatus = HIGH;
          waitDelay = T_LONG_ON;
        } else {
          digitalWrite(ledPin, LOW);
          ledStatus = LOW;
          flashCnt++;
          waitDelay = T_GAP;
        }
      } else {
        // O完成，间隔进入第二个S
        digitalWrite(ledPin, LOW);
        curState = S2_SHORT;
        flashCnt = 0;
        waitDelay = T_CHAR_GAP;
      }
      break;

    case S2_SHORT:
      if (flashCnt < 3) {
        if (!ledStatus) {
          digitalWrite(ledPin, HIGH);
          ledStatus = HIGH;
          waitDelay = T_SHORT_ON;
        } else {
          digitalWrite(ledPin, LOW);
          ledStatus = LOW;
          flashCnt++;
          waitDelay = T_GAP;
        }
      } else {
        // 完整SOS播放完毕，长停顿后回到空闲
        digitalWrite(ledPin, LOW);
        curState = IDLE;
        waitDelay = T_WORD_GAP;
        Serial.println("=== 一组SOS发送完成 ===");
      }
      break;
  }
}