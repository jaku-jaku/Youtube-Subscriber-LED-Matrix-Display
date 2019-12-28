#include <Arduino.h>
#include "proj_config.h"
#include "DisplayManager.h"
#include <IOTAppStory.h>
#include <WiFiClientSecure.h>
#include "SNTPtime.h"
#include "YoutubeApi.h"
#include "proj_credential.h"
#include "task_helper.h"
struct
{
	WiFiClientSecure* 		client;
	YoutubeApi* 			api;
	DisplayManager* 		mat_display;

	uint64_t 			num_Subscriber;
	LoopWatchDog_t 		displayLoop;
} m_data;

void beepUp()
{
	digitalWrite(GPIO_BUZZER, HIGH);
	delay(200);
	digitalWrite(GPIO_BUZZER, LOW);
	delay(50);
}

void setup() 
{
	// init devices
	m_data.mat_display 	= new DisplayManager(beepUp);
	m_data.client 		= new WiFiClientSecure();
	m_data.api          = new YoutubeApi(API_KEY, *m_data.client);

	// init values
	m_data.num_Subscriber = 382000;
	m_data.displayLoop.init(DISP_LOOP_INTERVAL);

	// setup devices
    Serial.begin(115200);
	// init gpios
	pinMode(GPIO_BUZZER, OUTPUT);
	// setup display
	m_data.mat_display->setup();
}

void loop() 
{
	if (m_data.displayLoop.onTick())
	{	
		if(m_data.mat_display->updateDisplay(m_data.num_Subscriber))
		{
			// do nothing, display updated, since # is changed
		}
	}
}