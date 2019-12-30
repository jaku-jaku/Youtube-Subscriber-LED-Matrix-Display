#include "DisplayManager.h"
#include "proj_config.h"
#include "neon_common.h"
#include "pixelNum.h"
#include "youtubeBtn.h"

const int kFactor[7] = {1, 10, 100, 1000, 10000, 100000, 1000000};

DisplayManager::DisplayManager(FuncPtrVoidInt callback_beepUp, DisplayConfig_t* config)
{
    // create Neo matrix instance
    this->config_ = config;
    this->matrix_ = new Adafruit_NeoMatrix( config->width, 
                                            config->height, 
                                            config->gpio,
                                            config->orientation,
                                            config->neo_type);
    this->oldNumber_ = 0;
    this->callback_beepUp_ = callback_beepUp;
}

DisplayManager::~DisplayManager()
{
    delete(this->matrix_);
    this->matrix_ = nullptr;
}

void DisplayManager::beepUp(void)
{
    this->callback_beepUp_(this->config_->beepUpDuration);
}

void DisplayManager::setup(void)
{
    this->matrix_->begin();
    this->matrix_->setTextWrap(false);
    this->matrix_->setBrightness(this->config_->brightness);
    this->matrix_->fillScreen(LED_WHITE_HIGH);
    this->matrix_->show();
    beepUp();
    displayRainbow();
    beepUp();
	delay(500);
    this->matrix_->clear();
    beepUp();
}

bool DisplayManager::updateDisplay(uint64_t currentSubNumber, bool forceUpdate)
{
    bool result = false;
    // update upon number changes
	if((currentSubNumber != this->oldNumber_) || forceUpdate)
	{
		this->oldNumber_ = currentSubNumber;
		this->matrix_->clear();
		renderUTubeBtn();
		renderNumber(currentSubNumber);
		this->matrix_->show();
        result = true;
        // beep upon updates
        beepUp();
	}
    return result;
}

void DisplayManager::displayText(String tt) 
{
  this->matrix_->setTextWrap(false);
  this->matrix_->setBrightness(40);
  this->matrix_->fillScreen(0);
  this->matrix_->setTextColor(LED_RED_MEDIUM);
  this->matrix_->setCursor(0, 0);
  this->matrix_->print(tt);
  this->matrix_->show();
}

void DisplayManager::displayRainbow(void)
{
	this->matrix_->clear();
	for(int i = 0; i < 8 ; i++)
	{
		for (int j =0; j<32; j++)
		{
			this->matrix_->drawPixel(j,i, Wheel((uint8_t)(i+j*8)));
		}
	}
	this->matrix_->show();
}

// --- Private --- //
uint32_t DisplayManager::Wheel(byte WheelPos) 
{
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return this->matrix_->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return this->matrix_->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return this->matrix_->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
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
void DisplayManager::renderDigits(uint8_t xOffset, uint8_t yOffset, uint16_t val, uint8_t numberDigits, uint16_t clr, uint16_t bkg_clr, const bool* tag)
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
					this->matrix_->drawPixel(i+dx, j+dy, bkg_clr);
				}
				else if (bitmapNumber[number][PIXEL_NUM_W*j+i])
				{
					this->matrix_->drawPixel(i+dx, j+dy, clr);
				}
				else
				{
					this->matrix_->drawPixel(i+dx, j+dy, bkg_clr);
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
					this->matrix_->drawPixel(i+dx, j+dy, clr);
				}
				else
				{
					this->matrix_->drawPixel(i+dx, j+dy, bkg_clr);
				}
			}
		}	
	}
}

DisplayManager::DM_NUM_DISPLAY_t DisplayManager::renderNumber(uint64_t value)
{
    uint16_t round_val = 0;
    DM_NUM_DISPLAY_t case_type = DM_NUM_DISPLAY_NORM;
    if (value < 100000) 
    {
        round_val = value;
    }
    else if (value < 1e7) // k
    {
        round_val = value/1e3;
        case_type = DM_NUM_DISPLAY_K;
	}
    else if (value < 1e10) // m
    {
        round_val = value/1e6;
        case_type = DM_NUM_DISPLAY_M;
    }
    else // b
    {
        round_val = value/1e9;
        case_type = DM_NUM_DISPLAY_B;
    }
    
    switch (case_type)
    {
        case (DM_NUM_DISPLAY_NORM):
            renderDigits(10, 1, round_val, 5, LED_WHITE_HIGH, LED_BLACK, nullptr);
            break;
        case (DM_NUM_DISPLAY_K):
            renderDigits(10, 1, round_val, 4, LED_WHITE_HIGH, LED_BLACK, bitmapSpecialChar_K);
            break;
        case (DM_NUM_DISPLAY_M):
            renderDigits(10, 1, round_val, 4, LED_WHITE_HIGH, LED_BLACK, bitmapSpecialChar_M);
            break;
        case (DM_NUM_DISPLAY_B):
            renderDigits(10, 1, round_val, 4, LED_WHITE_HIGH, LED_BLACK, bitmapSpecialChar_B);
            break;
        default:
            break;
    }

    return case_type;
}

void DisplayManager::renderUTubeBtn(void){
	this->matrix_->fillRect(0, 0, BMAP24_UTUBE_W, BMAP24_UTUBE_H, LED_BLACK);
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
			this->matrix_->drawPixel(i, j, clr_gfx);
		}
	}
}

// ------- Testing Code ------ //

// Fill the screen with multiple levels of white to gauge the quality
void DisplayManager::TEST_display_four_white() {
    int mw = this->config_->width;
    int mh = this->config_->height;
    this->matrix_->clear();
    this->matrix_->fillRect(0,0, mw,mh, LED_WHITE_HIGH);
    this->matrix_->drawRect(1,1, mw-2,mh-2, LED_WHITE_MEDIUM);
    this->matrix_->drawRect(2,2, mw-4,mh-4, LED_WHITE_LOW);
    this->matrix_->drawRect(3,3, mw-6,mh-6, LED_WHITE_VERYLOW);
    this->matrix_->show();
}

void DisplayManager::TEST_display_scrollText() {
    this->matrix_->clear();
    this->matrix_->setTextColor(LED_WHITE_LOW);
    for (int8_t x=10; x>=-100; x--) {
        this->matrix_->clear();
        this->matrix_->setCursor(x,0);
        this->matrix_->setTextColor(Wheel((uint8_t)(x*2%255)));
        this->matrix_->print("Devon, Happy Birthday!");
        this->matrix_->show();
        delay(50);
    }
    delay(500);
}