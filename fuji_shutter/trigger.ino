#include <Arduino.h>
#include <TM1637Display.h>

// segment display pins
#define CLK 3
#define DIO 4

// The amount of time (in milliseconds) between tests
#define TEST_DELAY   1000

#define SHT 6
#define FCS 5
#define TRG 2

TM1637Display display(CLK, DIO);
uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };

int mode = 0;
int pot = 0;
int takes = 0;

void readPot(){
    // function that evaluates current setting based on potentiometer
    pot = analogRead(A3); 
    if((pot >= 0) && (pot < 128))           {mode = 0; pot = 5;}    // 5sec
    else if((pot >= 128) && (pot < 256))    {mode = 1; pot = 10;}   // 10sec
    else if((pot >= 256) && (pot < 384))    {mode = 2; pot = 30;}   // 30sec
    else if((pot >= 384) && (pot < 512))    {mode = 3; pot = 60;}   // 1min
    else if((pot >= 512) && (pot < 640))    {mode = 4; pot = 120;}  // 2min
    else if((pot >= 640) && (pot < 768))    {mode = 5; pot = 180;}  // 3min
    else if((pot >= 768) && (pot < 896))    {mode = 6; pot = 240;}  // 4min
    else                                    {mode = 7; pot = 300;}  // 5min

    Serial.print(pot);  Serial.print(" -> ");    Serial.println(mode);  
    // show selected mode as #4 numebr on the display
    display.showNumberDec(mode, false, 1, 3);
}

void checkSwitch(){
  // if the Trigger switch is off
  // wait and keep an eye on the mode setting
  while(digitalRead(TRG) == HIGH) {
      readPot();  
      delay(200);
    }
  }

void setup() {
  Serial.begin(9600);
  while (! Serial);
 
  pinMode(TRG, INPUT_PULLUP);

  pinMode(SHT, OUTPUT);
  pinMode(FCS, OUTPUT);

  digitalWrite(SHT, LOW);
  digitalWrite(FCS, LOW);
  
  display.setSegments(data);    // clear display
  display.setBrightness(0x0b);  // set display brightness
  
  // get potentiometer position before first loop
  readPot();
}

void loop() {
  
  checkSwitch();

  if (digitalRead(TRG) == LOW)
  {
    Serial.print("TRG ");
    digitalWrite(FCS, HIGH);
    Serial.print("> FCS ");
    delay(1500);

    digitalWrite(SHT, HIGH);
    Serial.println("> SHT");
    delay(500);
    digitalWrite(SHT, LOW);
    digitalWrite(FCS, LOW);
  
    takes++;
    // first two digits of the display shows number of takes (0-99)
    display.showNumberDec(takes, false, 2, 0);
  }

  // countdown till next photo
  for( int i = 0; i <= pot; i++ ){
    checkSwitch();
    if((pot-i) < 10 && (pot-i) >= 0){  // prevents negative values on display
        if(pot-i == 0){
          data[2] = 0x00;
          display.setSegments(data);
          display.showNumberDec(takes, false, 2, 0);
          display.showNumberDec(mode, false, 1, 3);
        }
        else display.showNumberDec(pot-i, false, 1, 2);
      }
    delay(TEST_DELAY);
  }
}
