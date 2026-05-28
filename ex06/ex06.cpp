#define LED1 2
#define LED2 18
int fre = 200;
int re = 8;
int dutycycle = 0;
void setup() {
  Serial.begin(115200);
  ledcAttach(LED1,fre,re);
  ledcAttach(LED2,fre,re);
}

void loop() {
  for(;dutycycle<=255;dutycycle++)
  {
    ledcWrite(LED1,dutycycle);
    ledcWrite(LED2,255-dutycycle);
    delay(20);
  }
  for(;dutycycle>=0;dutycycle--)
  {
    ledcWrite(LED1,dutycycle);
    ledcWrite(LED2,255-dutycycle);
    delay(20);
  }
  Serial.print("");
}

