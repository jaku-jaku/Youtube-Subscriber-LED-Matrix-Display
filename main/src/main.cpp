#include <Arduino.h>
#include "proj_config.h"
#include "DisplayManager.h"
#include <IOTAppStory.h>
#include <WiFiClientSecure.h>
#include "SNTPtime.h"
#include "YoutubeApi.h"
#include "proj_credential.h"
#include "task_helper.h"
#include "WiFi.h" // ESP32 WiFi include
 
#define ENABLE_WIFI_SYNC (1)

struct
{
	WiFiClientSecure* 		client;
	YoutubeApi* 			api;
	DisplayManager* 		mat_display;

	uint64_t 			num_Subscriber;
	LoopWatchDog* 		displayLoop;
	LoopWatchDog* 		youtubeLoop;
} m_data;

DisplayManager::DisplayConfig_t config = {	
	.beepUpDuration	= 250U,
	.brightness 	= MAT_BRIGHTNESS,
	.width			= MAT_WIDTH,
	.height			= MAT_HEIGHT,
	.gpio			= GPIO_LED_MATRIX,
	.orientation	= MAT_FIRST_PIXEL_POS,
	.neo_type		= MAT_NEO_PIXEL_TYPE,
};

void beepUp(int delay_ms)
{
	digitalWrite(GPIO_BUZZER, HIGH);
	delay(delay_ms);
	digitalWrite(GPIO_BUZZER, LOW);
	delay(delay_ms/5);
}

void setup() 
{
	// init devices
	m_data.mat_display 	= new DisplayManager(beepUp, &config);
	m_data.client 		= new WiFiClientSecure();
	m_data.api          = new YoutubeApi(API_KEY, *m_data.client);

	// init values
	m_data.num_Subscriber = 0;
	m_data.displayLoop = new LoopWatchDog(DISP_LOOP_INTERVAL);
	m_data.youtubeLoop = new LoopWatchDog(UATUBE_SYNC_LOOP_INTERVAL);
	
	// setup devices
    Serial.begin(115200);
	// init gpios
	pinMode(GPIO_BUZZER, OUTPUT);
	// setup display
	m_data.mat_display->setup();

#if (ENABLE_WIFI_SYNC)
	// connect wifi
	char ssid[] = WIFI_SSID; // your network SSID (name)
	char password[] = WIFI_PASSWORD; // your network key
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	
	uint8_t i = 0;
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print('.');
		delay(500);
	}
#endif
} 

void loop() 
{
	if (m_data.displayLoop->onTick())
	{	
		if(m_data.mat_display->updateDisplay(m_data.num_Subscriber, false))
		{
			// do nothing, display updated, since # is changed
		}
	}
#if (ENABLE_WIFI_SYNC)
	if (m_data.youtubeLoop->onTick())
	{
		// update subscriber count
		if (m_data.api->getChannelStatistics(CHANNEL_ID))
  		{
			m_data.num_Subscriber = m_data.api->channelStats.subscriberCount;
		}
	}
#endif //ENABLE_WIFI_SYNC
}
