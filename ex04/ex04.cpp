 #define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 500 
bool cantouch=true;

bool ledState = false;
int touchValue;

// 中断服务函数 (ISR)
void gotTouch() {
  if(cantouch){
    ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
   Serial.print("中断 ");
   cantouch=false;
   }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
}

void loop() {
  touchValue = touchRead(TOUCH_PIN);
  if(touchValue<500)
  {
    cantouch=true;
  }
  Serial.print("Touch Value: ");
  Serial.println(touchValue);
  delay(100);
}
