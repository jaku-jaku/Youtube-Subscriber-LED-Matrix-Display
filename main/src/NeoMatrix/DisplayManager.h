#ifndef DISPLAY_MANAGER_H_
#define DISPLAY_MANAGER_H_

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <functional>

#define DIGIT_PADDING 1
class DisplayManager{
    public:
        typedef enum{
            DM_NUM_DISPLAY_NORM,
            DM_NUM_DISPLAY_K,
            DM_NUM_DISPLAY_M,
            DM_NUM_DISPLAY_B,
            DM_NUM_DISPLAY_UNKNOWN
        } DM_NUM_DISPLAY_t;
        typedef void (*FuncPtrVoidVoid)(void);

        DisplayManager(FuncPtrVoidVoid callback_beepUp);
        ~DisplayManager();

        void setup(void);
        bool updateDisplay(uint64_t currentSubNumber);
        void displayText(String tt);
        void displayRainbow(void);
    private:
        Adafruit_NeoMatrix* matrix_;
        uint64_t oldNumber_;
        FuncPtrVoidVoid callback_beepUp_;
        
        DM_NUM_DISPLAY_t renderNumber(uint64_t value);
        void renderUTubeBtn(void);
        void renderDigits(uint8_t xOffset, uint8_t yOffset, uint16_t val, uint8_t numberDigits, uint16_t clr, uint16_t bkg_clr, const bool* tag);
        uint32_t Wheel(byte WheelPos);

        // Test code
        void TEST_display_four_white(uint8_t mw, uint8_t mh);
        void TEST_display_scrollText(uint8_t mw, uint8_t mh);
};

#endif //DISPLAY_MANAGER_H_
