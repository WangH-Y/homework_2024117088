#define LED1  2
#define TOUCH_PIN 4
#define THRESHOLD 500
int touchValue;
int fre = 100;
int re = 8;
int i =0;
bool canTouch = true;

void count() {
  if(canTouch){
     i++;
     if(i>=4)i-=3;
    canTouch=false;
   }
}

void shine(int i)
{
  for(int dutycycle = 0;dutycycle<=255;dutycycle+=10*i)
  {
    ledcWrite(LED1,dutycycle);
    delay(20);
  }
  for(int dutycycle = 255;dutycycle>=0;dutycycle-=10*i)
  {
    ledcWrite(LED1,dutycycle);
    delay(20);
  }
}

void setup() {
  Serial.begin(115200);
  ledcAttach(LED1,fre,re);
  touchAttachInterrupt(TOUCH_PIN,count, THRESHOLD);
}

void loop() {  
  touchValue = touchRead(TOUCH_PIN);
  if(touchValue<500)canTouch=true;
  count();
  shine(i);
  Serial.print("Touch Value: ");
  Serial.println(touchValue);
  Serial.print("i: ");
  Serial.println(i);
}

