#include <FastLED.h>

#include <Wire.h> // must be included here so that Arduino library object file references work
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

#define NUM_LEDS 300
#define DATA_PIN 6

uint8_t thisdelay = 5;                                        // A delay value for the sequence(s)
uint8_t thishue = 0;                                          // Starting hue value.
uint8_t deltahue = 10;                                        // Hue change between pixels.
uint8_t colorNumber = 0;

CRGB leds[NUM_LEDS];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(64);

  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  Serial.println();

  if (!Rtc.IsDateTimeValid())
  {
      // Common Cuases:
      //    1) first time you ran and the device wasn't running yet
      //    2) the battery on the device is low or even missing

      Serial.println("RTC lost confidence in the DateTime!");

      // following line sets the RTC to the date & time this sketch was compiled
      // it will also reset the valid flag internally unless the Rtc device is
      // having an issue

      Rtc.SetDateTime(compiled);
  }

  if (!Rtc.GetIsRunning())
  {
      Serial.println("RTC was not actively running, starting now");
      Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled)
  {
      Serial.println("RTC is older than compile time!  (Updating DateTime)");
      Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
      Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled)
  {
      Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }

  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  Rtc.Enable32kHzPin(false);
  Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

  bool changed = false;
}

void loop() {
  // put your main code here, to run repeatedly:
   // it
if (!Rtc.IsDateTimeValid())
  {
      // Common Cuases:
      //    1) the battery on the device is low or even missing and the power line was disconnected
    Serial.println("RTC lost confidence in the DateTime!");
    lastOn();
    FastLED.show();
    delay(1000);
    allOff();
    FastLED.show();
    delay(1000);
  }

  RtcDateTime now = Rtc.GetDateTime();
  allOff();

  // Always on
  wordIt();
  wordIs();

  // Minutes
  int roundedMinutes = roundTo(now.Minute(), 5);
  // Hours
  int currentHour;
  if (now.Hour() > 12){
    currentHour = now.Hour() -12;
  } else {
    currentHour = now.Hour();
  }

  
  // Daylight savings time.
  if (now.Month() <= 3 || now.Month() >= 10){ // Not between the 3th month and 10th
    Serial.println(currentHour);
    if(now.Month() == 3 && now.Day() >= 25){ // Start day
      currentHour--;
    } else if(now.Month() == 10 && now.Day() <= 28){ // End day
      currentHour--;
    } else if(now.Month() <= 4 || now.Month() >= 11){ // Any day inbetween
      currentHour--;
    }
  }

  switch(roundedMinutes){
    case 0:
      wordOclock();
      break;
    case 5:
      wordFive();
      wordMinutes();
      wordPast();
      break;
    case 10:
      wordTen();
      wordMinutes();
      wordPast();
      break;
    case 15:
      wordQuarter();
      wordPast();
      break;
    case 20:
      wordTwenty();
      wordMinutes();
      wordPast();
      break;
    case 25:
      wordTwenty();
      wordFive();
      wordMinutes();
      wordPast();
      break;
    case 30:
      wordHalf();
      wordPast();
      break;
    case 35:
      wordTwenty();
      wordFive();
      wordMinutes();
      wordTo();
      currentHour++;
      break;
    case 40:
      wordTwenty();
      wordMinutes();
      wordTo();
      currentHour++;
      break;
    case 45:
      wordQuarter();
      wordTo();
      currentHour++;
      break;
    case 50:
      wordTen();
      wordMinutes();
      wordTo();
      currentHour++;
      break;
    case 55:
      wordFive();
      wordMinutes();
      wordTo();
      currentHour++;
      break;
    case 60:
      wordOclock();
      currentHour++;
      break;
    default:
      Serial.println("This isnt a valid minute");
      Serial.println(roundedMinutes);
      break;
  }
  // Hour conditions after hour changes
  if (currentHour == 13) {
    currentHour = 1;
  }
  if (currentHour == -1){
    currentHour = 0;
  }

  switch(currentHour) {
    case 0:
      wordHourTwelve();
      break;
    case 1:
      wordHourOne();
      break;
    case 2:
      wordHourTwo();
      break;
    case 3:
      wordHourThree();
      break;
    case 4:
      wordHourFour();
      break;
    case 5:
      wordHourFive();
      break;
    case 6:
      wordHourSix();
      break;
    case 7:
      wordHourSeven();
      break;
    case 8:
      wordHourEight();
      break;
    case 9:
      wordHourNine();
      break;
    case 10:
      wordHourTen();
      break;
    case 11:
      wordHourEleven();
      break;
    case 12:
      wordHourTwelve();
      break;
    default:
      Serial.println("This is not a valid hour!");
      Serial.println(currentHour);
      break;
  }

  if ((now.Month() == 1) && (now.Day() == 25)){
    wordHappy();
    wordBirth();
    wordDay();
    wordAdam();
  }

  if ((now.Month() == 4) && (now.Day() == 13)){
    wordHappy();
    wordBirth();
    wordDay();
    wordRuth();
  }

  if ((now.Month() == 10) && (now.Day() == 21)){
    wordHappy();
    wordAnniversary();
  }

//  wordAdam();
//  wordRuth();

//  wordHappy();
//  wordBirth();
//  wordDay();

//  wordAnniversary();

  FastLED.show();
}

int roundTo(int numToRound, int multiple)
{
  int result = numToRound + multiple/2;
  result -= result % multiple;
  return result;
}

void allOff() {
  for(int i = 0; i<299; i++) {
    leds[i] = CRGB::Black;
  }
}

void lastOn() {
  leds[51] = CRGB::White;
}

void wordIt() {
  leds[299] = CRGB::White;
  leds[298] = CRGB::White;
  leds[297] = CRGB::White;
}

void wordIs() {
  leds[294] = CRGB::White;
  leds[293] = CRGB::White;
}

void wordTen() {
  leds[290] = CRGB::White;
  leds[289] = CRGB::White;
  leds[288] = CRGB::White;
  leds[287] = CRGB::White;
}

void wordHalf() {
  leds[285] = CRGB::White;
  leds[284] = CRGB::White;
  leds[283] = CRGB::White;
  leds[282] = CRGB::White;
  leds[281] = CRGB::White;
}

void wordTwenty() {
  leds[276] = CRGB::White;
  leds[275] = CRGB::White;
  leds[274] = CRGB::White;
  leds[273] = CRGB::White;
  leds[272] = CRGB::White;
  leds[271] = CRGB::White;
  leds[270] = CRGB::White;
}

void wordQuarter() {
  leds[267] = CRGB::White;
  leds[266] = CRGB::White;
  leds[265] = CRGB::White;
  leds[264] = CRGB::White;
  leds[263] = CRGB::White;
  leds[262] = CRGB::White;
  leds[261] = CRGB::White;
  leds[260] = CRGB::White;
  leds[259] = CRGB::White;
  leds[258] = CRGB::White;
}

void wordFive() {
  leds[252] = CRGB::White;
  leds[251] = CRGB::White;
  leds[250] = CRGB::White;
  leds[249] = CRGB::White;
  leds[248] = CRGB::White;
  leds[247] = CRGB::White;
}

void wordMinutes() {
  leds[244] = CRGB::White;
  leds[243] = CRGB::White;
  leds[242] = CRGB::White;
  leds[241] = CRGB::White;
  leds[240] = CRGB::White;
  leds[239] = CRGB::White;
  leds[238] = CRGB::White;
  leds[237] = CRGB::White;
  leds[236] = CRGB::White;
}

void wordTo() {
  leds[227] = CRGB::White;
  leds[226] = CRGB::White;
}

void wordPast() {
  leds[224] = CRGB::White;
  leds[223] = CRGB::White;
  leds[222] = CRGB::White;
  leds[221] = CRGB::White;
  leds[220] = CRGB::White;
}

void wordHappy() {
  colorCycle(213, 219);
}

void wordHourOne() {
  leds[206] = CRGB::White;
  leds[205] = CRGB::White;
  leds[204] = CRGB::White;
  leds[203] = CRGB::White;
}

void wordBirth() {
  colorCycle(195, 200);
}

void wordHourThree() {
  leds[194] = CRGB::White;
  leds[193] = CRGB::White;
  leds[192] = CRGB::White;
  leds[191] = CRGB::White;
  leds[190] = CRGB::White;
  leds[189] = CRGB::White;
  leds[188] = CRGB::White;
}

void wordDay() {
  colorCycle(180, 182);
}

void wordHourFour() {
  leds[178] = CRGB::White;
  leds[177] = CRGB::White;
  leds[176] = CRGB::White;
  leds[175] = CRGB::White;
  leds[174] = CRGB::White;
}

void wordHourEleven() {
  leds[172] = CRGB::White;
  leds[171] = CRGB::White;
  leds[170] = CRGB::White;
  leds[169] = CRGB::White;
  leds[168] = CRGB::White;
  leds[167] = CRGB::White;
  leds[166] = CRGB::White;
  leds[165] = CRGB::White;
}

void wordAnniversary() {
  colorCycle(141, 156);
}

void wordHourEight() {
  leds[135] = CRGB::White;
  leds[134] = CRGB::White;
  leds[133] = CRGB::White;
  leds[132] = CRGB::White;
  leds[131] = CRGB::White;
  leds[130] = CRGB::White;
}

void wordAdam() {
  colorCycle(124, 128);
}

void wordHourTwo() {
  leds[122] = CRGB::White;
  leds[121] = CRGB::White;
  leds[120] = CRGB::White;
  leds[119] = CRGB::White;
}

void wordHourNine() {
  leds[113] = CRGB::White;
  leds[112] = CRGB::White;
  leds[111] = CRGB::White;
  leds[110] = CRGB::White;
  leds[109] = CRGB::White;
}

void wordHourSeven() {
  leds[107] = CRGB::White;
  leds[106] = CRGB::White;
  leds[105] = CRGB::White;
  leds[104] = CRGB::White;
  leds[103] = CRGB::White;
  leds[102] = CRGB::White;
}

void wordHourFive() {
  leds[101] = CRGB::White;
  leds[100] = CRGB::White;
  leds[99] = CRGB::White;
  leds[98] = CRGB::White;
  leds[97] = CRGB::White;
  leds[96] = CRGB::White;
}

void wordHourTen() {
  leds[91] = CRGB::White;
  leds[90] = CRGB::White;
  leds[88] = CRGB::White;
}

void wordRuth() {
  colorCycle(81, 86);
}

void wordHourSix() {
  leds[78] = CRGB::White;
  leds[77] = CRGB::White;
  leds[76] = CRGB::White;
  leds[75] = CRGB::White;
  leds[74] = CRGB::White;
}

void wordHourTwelve() {
  leds[68] = CRGB::White;
  leds[67] = CRGB::White;
  leds[66] = CRGB::White;
  leds[65] = CRGB::White;
  leds[64] = CRGB::White;
  leds[63] = CRGB::White;
  leds[62] = CRGB::White;
  leds[61] = CRGB::White;
}

void wordOclock() {
  leds[58] = CRGB::White;
  leds[57] = CRGB::White;
  leds[56] = CRGB::White;
  leds[55] = CRGB::White;
  leds[54] = CRGB::White;
  leds[53] = CRGB::White;
  leds[52] = CRGB::White;
  leds[51] = CRGB::White;
}

CRGB wheel(int WheelPos) {
  CRGB color;
  if (85 > WheelPos) {
   color.r=0;
   color.g=WheelPos * 3;
   color.b=(255 - WheelPos * 3);;
  }
  else if (170 > WheelPos) {
   color.r=WheelPos * 3;
   color.g=(255 - WheelPos * 3);
   color.b=0;
  }
  else {
   color.r=(255 - WheelPos * 3);
   color.g=0;
   color.b=WheelPos * 3;
  }
  return color;
}

void colorCycle(int startLed, int endLed) {
  uint8_t currentColor = colorNumber;
  for(int i=startLed; i <= endLed; i++) {
    leds[i] = wheel(currentColor);
    currentColor = currentColor + 20;
  }
  colorNumber = colorNumber + 1;
}
