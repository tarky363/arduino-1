

#include <Arduino.h>
#include <ADNS2610.h>

#define LED 13

byte frame[FRAMELENGTH];

void toggleLED()
{
  static bool led_state = false;
  
  led_state = !led_state;
  digitalWrite(LED, led_state ? HIGH : LOW);
}

void setup()
{
  pinMode(SCLK, OUTPUT);
  pinMode(SDIO, OUTPUT);
  pinMode(LED, OUTPUT);

  Serial.begin(9600);
  delay(100);
  Serial.print("ADNS2610\n");

  mouseInit();
  dumpDiag();
  
  for (int i = 0; i < 10; i++) {
    toggleLED();
    delay(100);
  }
}

void loop()
{
  readFrame(frame);
  toggleLED();

  for(int i = 0; i < FRAMELENGTH; i++)
  {
    Serial.print((char) (frame[i] & 0x3F));
    //Serial.print(char(0x40));
  }
  Serial.print(char(0x80)); // end of frame marker
}

// FIN