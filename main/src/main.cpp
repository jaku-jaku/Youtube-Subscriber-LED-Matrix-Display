#include <Arduino.h>
#include "proj_config.h"
#include "neon_common.h"
#include "pixelNum.h"

#include <IOTAppStory.h>
#include <WiFiClientSecure.h>
#include "SNTPtime.h"
#include "YoutubeApi.h"
#include "youtubeBtn.h"
// Adafruit_NeoMatrix example for single NeoPixel Shield.
// By Marc MERLIN <marc_soft@merlins.org>
// Contains code (c) Adafruit, license BSD

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define API_KEY "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
WiFiClientSecure client; //For ESP8266 boards
YoutubeApi api(API_KEY, client);

Adafruit_NeoMatrix *matrix_ptr = new Adafruit_NeoMatrix(MAT_WIDTH, MAT_HEIGHT, GPIO_LED_MATRIX,  MAT_FIRST_PIXEL_POS, MAT_NEO_PIXEL_TYPE);

// Fill the screen with multiple levels of white to gauge the quality
void display_four_white(uint8_t mw, uint8_t mh) {
    matrix_ptr->clear();
    matrix_ptr->fillRect(0,0, mw,mh, LED_WHITE_HIGH);
    matrix_ptr->drawRect(1,1, mw-2,mh-2, LED_WHITE_MEDIUM);
    matrix_ptr->drawRect(2,2, mw-4,mh-4, LED_WHITE_LOW);
    matrix_ptr->drawRect(3,3, mw-6,mh-6, LED_WHITE_VERYLOW);
    matrix_ptr->show();
}

void display_scrollText(uint8_t mw, uint8_t mh) {
    matrix_ptr->clear();
    matrix_ptr->setTextWrap(false);  // we don't wrap text so it scrolls nicely
    matrix_ptr->setTextSize(1);
    matrix_ptr->setRotation(0);
    for (int8_t x=7; x>=-42; x--) {
	matrix_ptr->clear();
	matrix_ptr->setCursor(x,0);
	matrix_ptr->setTextColor(LED_GREEN_HIGH);
	matrix_ptr->print("Hello");
	if (mh>11) {
	    matrix_ptr->setCursor(-20-x,mh-7);
	    matrix_ptr->setTextColor(LED_ORANGE_HIGH);
	    matrix_ptr->print("World");
	}
	matrix_ptr->show();
       delay(50);
    }

    matrix_ptr->setRotation(3);
    matrix_ptr->setTextColor(LED_BLUE_HIGH);
    for (int8_t x=7; x>=-45; x--) {
	matrix_ptr->clear();
	matrix_ptr->setCursor(x,mw/2-4);
	matrix_ptr->print("Rotate");
	matrix_ptr->show();
       delay(50);
    }
    matrix_ptr->setRotation(0);
    matrix_ptr->setCursor(0,0);
    matrix_ptr->show();
}

void displayText(String tt) {
  matrix_ptr->setTextWrap(false);
  matrix_ptr->setBrightness(40);
  matrix_ptr->fillScreen(0);
  matrix_ptr->setTextColor(LED_RED_MEDIUM);
  matrix_ptr->setCursor(0, 0);
  matrix_ptr->print(tt);
  matrix_ptr->show();
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return matrix_ptr->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return matrix_ptr->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return matrix_ptr->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void fillRainbow(void)
{
	matrix_ptr->clear();
	for(int i = 0; i < 8 ; i++)
	{
		for (int j =0; j<32; j++)
		{
			matrix_ptr->drawPixel(j,i, Wheel((uint8_t)(i+j*8)));
		}
	}
	matrix_ptr->show();
}



/*
    32 bit - 10 bit (for youtube logo)
    = 22 bit
    
    3 pix + 1 pix padding / #
    > 0 ~ 99,999
        all number: 5 numbers ... 2 pix 
    > 100, 000 ~ 9, 999, 999  (100k - 9,999 k)
        4 number + k: 4*4 + 4 = 20 pixs (2 pix space)
    > 10, 000, 000 ~ 9, 999, 999, 999 (10m - 9,999m)
        4 number + m: 4*4 + 5 = 21 pixs (1 pix space)
    > 10, 000, 000, 000 ~ 9, 999, 999, 999, 999 (10b - 9,999b)
        4 number + m: 4*4 + 4 = 20 pixs (2 pix space)
*/
const int kFactor[7] = {1, 10, 100, 1000, 10000, 100000, 1000000};
#define DIGIT_PADDING 1
void displayDigits(uint8_t xOffset, uint8_t yOffset, uint16_t val, uint8_t numberDigits, uint16_t clr, uint16_t bkg_clr, const bool* tag)
{
	uint8_t number = 0;
	uint8_t dx = xOffset;
	uint8_t dy = yOffset;
	bool valid = false;
	for (int k = 1; k <= numberDigits; k ++)
	{
		number = (val/kFactor[numberDigits - k])%10;
		if (number != 0 && valid != true)
		{
			valid = true; // begin to display numbers
		}

		for (int i=0; i<PIXEL_NUM_W; i++)
		{
			for (int j=0; j<PIXEL_NUM_H; j++)
			{
				if (valid == false) // draw background
				{
					matrix_ptr->drawPixel(i+dx, j+dy, bkg_clr);
				}
				else if (bitmapNumber[number][PIXEL_NUM_W*j+i])
				{
					matrix_ptr->drawPixel(i+dx, j+dy, clr);
				}
				else
				{
					matrix_ptr->drawPixel(i+dx, j+dy, bkg_clr);
				}
			}
		}	
		dx = dx + PIXEL_NUM_W + DIGIT_PADDING;
	}

	if (tag != nullptr)
	{
		for (int i=0; i<bitmapSpecialChar_W; i++)
		{
			for (int j=0; j<bitmapSpecialChar_H; j++)
			{
				if (tag[bitmapSpecialChar_W*j+i])
				{
					matrix_ptr->drawPixel(i+dx, j+dy, clr);
				}
				else
				{
					matrix_ptr->drawPixel(i+dx, j+dy, bkg_clr);
				}
			}
		}	
	}
}
void displayNumber(uint64_t value)
{
    uint16_t round_val = 0;
    uint8_t case_type = 0;
    if (value < 100000) 
    {
        round_val = value;
    }
    else if (value < 1e7) // k
    {
        round_val = value/1e3;
        case_type = 1;
	}
    else if (value < 1e10) // m
    {
        round_val = value/1e6;
        case_type = 2;
    }
    else // b
    {
        round_val = value/1e9;
        case_type = 3;
    }
    
    switch (case_type)
    {
        case 0:
            displayDigits(10, 1, round_val, 5, LED_WHITE_HIGH, LED_BLACK, nullptr);
            break;
        case 1:
            displayDigits(10, 1, round_val, 4, LED_WHITE_HIGH, LED_BLACK, bitmapSpecialChar_K);
            break;
        case 2:
            displayDigits(10, 1, round_val, 4, LED_WHITE_HIGH, LED_BLACK, bitmapSpecialChar_M);
            break;
        case 3:
            displayDigits(10, 1, round_val, 4, LED_WHITE_HIGH, LED_BLACK, bitmapSpecialChar_B);
            break;
        default:
            break;
    }
}

void showUTubeBtn(){
	 matrix_ptr->fillRect(0, 0, BMAP24_UTUBE_W, BMAP24_UTUBE_H, LED_BLACK);
	uint32_t clr32 = 0; 
	uint8_t b = 0; 
	uint8_t g = 0; 
	uint8_t r = 0; 
	uint16_t clr_gfx = 0;

	for (int i=0; i<BMAP24_UTUBE_W; i++)
	{
		for (int j=0; j<BMAP24_UTUBE_H; j++)
		{
			clr32 = bitmap24_utube[BMAP24_UTUBE_W*j+i];
			b = (clr32&0x0000FF);
			g = (clr32&0x00FF00)>>8;
			r = (clr32&0xFF0000)>>16;
			clr_gfx = NEON_COMMON_convert8BitRGB2GFX(r, g, b);
			matrix_ptr->drawPixel(i, j, clr_gfx);
		}
	}
}

void display_Utube_And_NumberText(int value, int xOffset) {
	matrix_ptr->fillRect(xOffset, 0, MAT_WIDTH - xOffset, MAT_HEIGHT, LED_BLACK);
	matrix_ptr->setTextWrap(false);  // we don't wrap text so it scrolls nicely
	matrix_ptr->setTextSize(1);
	matrix_ptr->setRotation(0);
	matrix_ptr->setCursor(xOffset,0);
	matrix_ptr->setTextColor(LED_WHITE_MEDIUM);
	matrix_ptr->print(String(value));
}

void updateSubs() {
//   if (api.getChannelStatistics(CHANNEL_ID))
//   {
//     // get subscribers from YouTube
//     //      Serial.println("Get Subs");
//     subscribers.actual = api.channelStats.subscriberCount;
//     displayNeo(subscribers.actual, subscribers.actual - subscribers.old[timeNow.hour]);
//     Serial.print("Subs ");
//     Serial.print(subscribers.actual);
//     Serial.print(" yesterday ");
//     Serial.println(subscribers.old[timeNow.hour]);
//     if (subscribers.last > 0) {
//       if (subscribers.actual > subscribers.last ) {
//         beepUp();
//         if (subscribers.actual % 10 <= subscribers.last % 10) for (int ii = 0; ii < 1; ii++) beepUp();
//         if (subscribers.actual % 100 <= subscribers.last % 100) starwars();
//         if (subscribers.actual % 1000 <= subscribers.last % 1000) for (int ii = 0; ii < 2; ii++) starwars();
//       }
//       else {
//         if (subscribers.actual < subscribers.last ) beepDown();
//       }
//     }
//     subscribers.last = subscribers.actual;
//     //     debugPrint();
//   }
}

void beepUp()
{
	digitalWrite(GPIO_BUZZER, HIGH);
	delay(200);
	digitalWrite(GPIO_BUZZER, LOW);
	delay(50);
}

void updateDisplay(uint64_t currentSubNumber)
{
	static int oldNumber = 0;
	if(currentSubNumber != oldNumber)
	{
		oldNumber = currentSubNumber;
		matrix_ptr->clear();
		showUTubeBtn();
		displayNumber(currentSubNumber);
		// display_Utube_And_NumberText(1000, 10);
		matrix_ptr->show();
		beepUp();
	}
}

// =====
#define COMPDATE __DATE__ __TIME__
#define MODEBUTTON 35U
// IotAppStory.com library
// IOTAppStory IAS(COMPDATE, MODEBUTTON);  // Initialize IotAppStory
/*
void IAS_init()
{
	IAS.preSetDeviceName("YouTubeNeo");
	IAS.preSetAutoUpdate(true);                      // automaticUpdate (true, false)
	IAS.preSetAutoConfig(false);                      // automaticConfig (true, false)
	String mySSID="VIRGIN839";
	String myPASSWORD="2193029103";
	IAS.preSetWifi(mySSID, myPASSWORD);

	IAS.onModeButtonShortPress([]() {
		Serial.println(F(" If mode button is released, I will enter in firmware update mode."));
		Serial.println(F("*-------------------------------------------------------------------------*"));
	});

	IAS.onModeButtonLongPress([]() {
		Serial.println(F(" If mode button is released, I will enter in configuration mode."));
		Serial.println(F("*-------------------------------------------------------------------------*"));
	});

	IAS.onFirmwareUpdateCheck([]() {
		Serial.println(F(" Checking if there is a firmware update available........."));
		Serial.println(F("*-------------------------------------------------------------------------*"));
		displayText("Checking");

	});

	IAS.onFirmwareUpdateDownload([]() {
		Serial.println(F(" Downloading and Installing firmware update."));
		Serial.println(F("*-------------------------------------------------------------------------*"));
		displayText("Update");
	});

	IAS.onFirmwareUpdateError([]() {
		Serial.println(F(" Update failed...Check your logs"));
		Serial.println(F("*-------------------------------------------------------------------------*"));
	});

	IAS.onConfigMode([]() {
		Serial.println(F(" Starting configuration mode. Search for my WiFi and connect to 192.168.4.1."));
		Serial.println(F("*-------------------------------------------------------------------------*"));
		displayText("Config");
	});


	IAS.onFirmwareUpdateCheck([]() {
		Serial.println(F(" Checking if there is a firmware update available."));
		Serial.println(F("*-------------------------------------------------------------------------*"));
		displayText("Checking Update");
	});

	IAS.begin('P');
	IAS.setCallHome(false);                // Set to true to enable calling home frequently (disabled by default)
	IAS.setCallHomeInterval(86400);   // Call home interval in seconds
}
*/

struct subscriberStruc {
  long last;
  long actual;
  long old[24];
} subscribers;

strDateTime timeNow;
int lastHour = 0;
SNTPtime NTPch("ch.pool.ntp.org");
unsigned long entrySubscriberLoop, entryNTPLoop, entryDispLoop;
#define SUBSCRIBER_INTERVAL 30000
#define NTP_LOOP_INTERVAL 	60000
#define DISP_LOOP_INTERVAL 	500

void setup() {
    Serial.begin(115200);
    matrix_ptr->begin();
    matrix_ptr->setTextWrap(false);
    matrix_ptr->setBrightness(MAT_BRIGHTNESS);
    matrix_ptr->fillScreen(LED_WHITE_HIGH);
    matrix_ptr->show();
	
	pinMode(GPIO_BUZZER, OUTPUT);
	beepUp();
    delay(500);
	beepUp();
	delay(500);
    matrix_ptr->clear();
}


// ================================================ LOOP =================================================

void loop() {
	
	// IAS.loop();                                       
	//-------- Your Sketch starts from here ---------------
	if (millis() - entryDispLoop > DISP_LOOP_INTERVAL) 
	{
		entryDispLoop = millis();
		updateDisplay(12324);
	}

	if (millis() - entryNTPLoop > NTP_LOOP_INTERVAL) 
	{
		//   Serial.println("NTP Loop");
		entryNTPLoop = millis();
		timeNow = NTPch.getTime(1.0, 1); // get time from internal clock
		NTPch.printDateTime(timeNow);
		if (timeNow.hour != lastHour ) 
		{
			// Serial.println("New hour!!!!!!!");
			// subscribers.old[lastHour] = subscribers.actual;
			// subscribers.last = subscribers.actual;
			lastHour = timeNow.hour;
		}
	}

	if (millis() - entrySubscriberLoop > SUBSCRIBER_INTERVAL) 
	{
		//   Serial.println("Subscriber Loop");
		entrySubscriberLoop = millis();
		updateSubs();
	}
}
