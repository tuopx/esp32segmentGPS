
#include <TM1637Display.h>
#include <gpsUblox.h>
#include <gpsPayloads.h>
#include <singleButton.h>

// Module connection pins (Digital Pins)
#define DISPLAYCLK 0
#define DISPLAYDIO 1

#define GPSRX 8
#define GPSTX 7
#define GPSBAUD 115200

#define BUTTON1 5
#define BUTTON2 6

//back and fort output smoothing factor. factor * gps output rate = time to hold the same speed
//i.e. 20 * 100ms = 2000ms
#define outputSmoothing 10

//display setup and - char for no fix scenario
TM1637Display display(DISPLAYCLK, DISPLAYDIO);
const uint8_t DASH[] = {SEG_G};

GpsUblox gps(GPSRX, GPSTX, GPSBAUD);
SingleButton btn1(BUTTON1);
SingleButton btn2(BUTTON2);

uint8_t currentBrightness = 0x7;
uint8_t speed_kmh_rounded = 0;
float speed_kmh = 0.0;
uint8_t noFixCnt = 0;
uint8_t posCnt = 0;
uint8_t lastKmh = 0;
uint8_t KmhDiff = 0;
uint8_t KmhDiffCounter = 0;

//debug
//uint32_t mtime;
//#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))
//uint32_t counter = 0;
//int fakes[] = { 26, 30, 31, 30, 31, 30, 31, 31, 31, 31, 31, 31, 31, 30,30,30, 32, 32, 32, 32};

void displayOnOff() 
{
  if (currentBrightness == 0x8) {
    currentBrightness = 0x7;
    display.setBrightness(currentBrightness);
    display.showNumberDec(speed_kmh_rounded, 0, 3, 0);
  } else {
    currentBrightness = 0x8;
    display.setBrightness(currentBrightness, false);
    display.showNumberDec(speed_kmh_rounded, 0, 3, 0);
  }
}

void displayIncrease() 
{
  if (currentBrightness >= 0x6) {
    currentBrightness = 0x0;
    display.setBrightness(currentBrightness);
    display.showNumberDec(speed_kmh_rounded, 0, 3, 0);
  } else {
    currentBrightness++;
    display.setBrightness(currentBrightness);
    display.showNumberDec(speed_kmh_rounded, 0, 3, 0);
  }
}

void setup()
{
  //Serial.begin(115200);
  display.setBrightness(currentBrightness);
  display.clear();
  //mtime = millis();
}

void loop()
{
/*   if (millis()-mtime > 1000) {
    Serial.println(counter);
    counter = 0;
    mtime = millis();
  }
  counter++; */


  if (btn1.read()) {
    displayOnOff();
  }

  if (btn2.read()) {
    displayIncrease();
  }

  if ( gps.processGPS() ) {
    //Serial.println(millis()-mtime);
    //mtime = millis();
    //Serial.println("in gps");
    
    if (bitRead(gps.ubxMessage.navPVT.flags, 0) != 1) {
      //Fix not valid
      if (noFixCnt == 10) {
        noFixCnt = 0;
        if (posCnt == 4) {
          posCnt = 0;
        }
        display.clear();
        display.setSegments(DASH, 1, posCnt);
        posCnt++;
      }
      //Serial.println("no fix");
      noFixCnt++;

    } else {
      speed_kmh = (gps.ubxMessage.navPVT.gSpeed/1000.0f)*3.6f;
      noFixCnt = 0;
      posCnt = 0;

      // speed is fluctuating a bit while stopped, so change speeds under 4kmh to 0kmh
      // otherwise always round up (i.e. 100.2 rounded to 101 instead of 100)
      if (speed_kmh < 4) {
        speed_kmh_rounded = 0;
      } else {
        speed_kmh_rounded = speed_kmh + 1.0;
      }

      //speed fluctuation checks
      KmhDiff = abs(lastKmh - speed_kmh_rounded);

      //Serial.print("kmh diff: ");
      //Serial.println(KmhDiff);

      //smooth the output value in case speed fluctuates between 2 values quickly (i.e. 59 & 60 kmh)
      if (KmhDiff == 1) {
        if (KmhDiffCounter > outputSmoothing-2) {
          KmhDiffCounter = 0;
          lastKmh = speed_kmh_rounded;

          //Serial.println("release hold");
        } else {
          //Serial.print("release holding: ");
          //Serial.println(KmhDiffCounter);

          speed_kmh_rounded = lastKmh;
          KmhDiffCounter++;
        }
      } else  {
        KmhDiffCounter = 0;
        lastKmh = speed_kmh_rounded;
      }

      display.showNumberDec(speed_kmh_rounded, 0, 3, 0);
      //Serial.println("fixed");
    }
  }

}
