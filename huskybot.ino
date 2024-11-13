#include <ipst.h>
#include <HUSKYLENS.h>
#include <Wire.h>

HUSKYLENS huskylens;

int pos, a=0;
int min_S1 = 230;   //อ่านค่าเส้นสีดำ ด้านซ้าย
int max_S1 = 990;   //อ่านค่าพื้นสีขาว ด้านซ้าย
int min_S2 = 210;  //อ่านค่าเส้นสีดำ ด้านขวา
int max_S2 = 990;   //อ่านค่าพื้นสีขาว ด้านขวา
int maxSP1 = 40;       //ความเร็วสูงสุดที่ต้องการ
int maxSP2 = 40;       //ความเร็วสูงสุดที่ต้องการ
float k = 1.0;      //ค่าอัตราการขยายของความไวเซนเซอร์

#define val_S1 map(analog(1), min_S1, max_S1, 0, 100)
#define val_S2 map(analog(2), min_S2, max_S2, 0, 100)

const int buttonPin1 = 16;
const int buttonPin2 = 18;

int buttonState1 = 0;
int lastButtonState1 = 0;
unsigned long lastDebounceTime1 = 0;

int buttonState2 = 0;
int lastButtonState2 = 0;
unsigned long lastDebounceTime2 = 0;

unsigned long debounceDelay = 50;

int stateConBtn1 = 0;
int stateConBtn2 = 0;

int resultState = 0;

int qrData = 0;

int boxOne = 0;
int boxTwo = 0;

int collectThing = 0;

int debugOne = 1;

int resultLine = 0;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Wire.begin();
    while (!huskylens.begin(Wire)){
      Serial.println(F("Begin failed!"));
      Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
      Serial.println(F("2.Please recheck the connection."));
      delay(100);
    }
    servo(0,20);
    pinMode(buttonPin1, INPUT);
    lastButtonState1 = digitalRead(buttonPin1);
}

void loop() {
  if(debugOne == 1){
    servo(1,26);
    debugOne++;
  }
    glcd(0, 0, "L= %d  ", analog(1));
    glcd(1, 0, "R= %d  ", analog(2));
    glcd(2, 0, "but= %d  ",stateConBtn1);
    glcd(3, 0, "collectThing= %d  ",collectThing);
//    glcd(5, 0, "collectThing= %d  ",collectThing);
//    glcd(6, 0, "collectThing= %d  ",collectThing);

    int reading1 = digitalRead(buttonPin1);
    if (reading1 != lastButtonState1) {
        lastDebounceTime1 = millis();
    }
    if ((millis() - lastDebounceTime1) > debounceDelay) {
        if (reading1 != buttonState1) {
            buttonState1 = reading1;
            if (buttonState1 == LOW) {
                Serial.println("Button 1 Pressed");
                stateConBtn1 = 1;
                stateConBtn2 = 0;
            }
        }
    }
    lastButtonState1 = reading1;
    if (!huskylens.request()) Serial.println(F("Recheck the connection!"));
    else if(!huskylens.available())   ao();
    else {Serial.println(F("OK..."));}
    while (huskylens.available()){
        HUSKYLENSResult result = huskylens.read();
        if (result.command == COMMAND_RETURN_BLOCK) {
              Serial.println(String()+F("Block:xCenter=")+
              result.xCenter+F(",yCenter=")+ result.yCenter+F(",width=")
              + result.width+F(",height=")+ result.height+F(",ID=")
              + result.ID);
        }
         if(result.ID == 1 && a == 0 && stateConBtn1 == 1 && collectThing == 0){
               fd(45); delay(880);
               ao(); delay(300);
               servo(1,0);
               ao(); delay(500);
               bk(50);delay(900); 
               ao(); delay(500);
               motor(1,40);motor(2,-80); delay(500);
               ao(); delay(500);
               fd(40); delay(300);
               Line_T();
               if (collectThing == 1){
                break;
               }
           }else if(result.ID == 2 && collectThing == 1 && boxOne == 0){
            fd(40); delay(700);
            Line_T(); delay(3000);
            ao(); delay(400);
            servo(1,23);
            ao(); delay(500);
            bk(40); delay(1400);
            ao(); delay(1000);
            sr(40); delay(1000);
            boxOne = 1;
           }
    }
    if(stateConBtn1 == 1){
      Line_T();
    }else if (boxOne == 1){
      ao();
    }
    
}
void bb(int x) {
  bk(40);delay(x);
}
void Line_T() {
  if (val_S1 < 70 && val_S2 < 70){
    ao(); delay(1500);
    collectThing = 1;
  }else{
         int SP_M1 = constrain(val_S1 * k, 0, maxSP1);
         int SP_M2 = constrain(val_S2 * (k*2), 0, maxSP2);
         motor(1, (SP_M1 - 15));
         motor(2, (SP_M2 - 15));
  }
}
//    int SP_M1 = constrain(val_S1 * k, 0, maxSP1);
//    int SP_M2 = constrain(val _S2 * (k*2), 0, maxSP2);
//    motor(1, (SP_M1 - 15));
//    motor(2, (SP_M2 - 15));
