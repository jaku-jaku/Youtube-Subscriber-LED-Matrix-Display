#ifndef PIXEL_NUM_H_
#define PIXEL_NUM_H_

#if defined(__AVR__)
    #include <avr/pgmspace.h>
#elif defined(__PIC32MX__)
    #define PROGMEM
#elif defined(__arm__)
    #define PROGMEM
#endif

#define PIXEL_NUM_W 3
#define PIXEL_NUM_H 5
const bool bitmapNumber[10][PIXEL_NUM_W*PIXEL_NUM_H] PROGMEM={
    //0
    {
        1, 1, 1,
        1, 0, 1,
        1, 0, 1,
        1, 0, 1,
        1, 1, 1,
    },
    //1
    {
        0, 1, 0,
        1, 1, 0,
        0, 1, 0,
        0, 1, 0,
        1, 1, 1,
    },
    //2
    {
        1, 1, 1,
        0, 0, 1,
        1, 1, 1,
        1, 0, 0,
        1, 1, 1,
    },
    //3
    {
        1, 1, 1,
        0, 0, 1,
        1, 1, 1,
        0, 0, 1,
        1, 1, 1,
    },
    //4
    {
        1, 0, 1,
        1, 0, 1,
        1, 1, 1,
        0, 0, 1,
        0, 0, 1,
    },
    //5
    {
        1, 1, 1,
        1, 0, 0,
        1, 1, 1,
        0, 0, 1,
        1, 1, 1,
    },
    //6
    {
        1, 1, 1,
        1, 0, 0,
        1, 1, 1,
        1, 0, 1,
        1, 1, 1,
    },
    //7
    {
        1, 1, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
        0, 0, 1,
    },
    //8
    {
        1, 1, 1,
        1, 0, 1,
        1, 1, 1,
        1, 0, 1,
        1, 1, 1,
    },
    //9
    {
        1, 1, 1,
        1, 0, 1,
        1, 1, 1,
        0, 0, 1,
        1, 1, 1,
    },
};

#define bitmapSpecialChar_W 5
#define bitmapSpecialChar_H PIXEL_NUM_H
const bool bitmapSpecialChar_K[bitmapSpecialChar_W*bitmapSpecialChar_H] PROGMEM={
        1, 0, 0, 1, 0, 
        1, 0, 1, 0, 0,
        1, 1, 0, 0, 0,
        1, 0, 1, 0, 0,
        1, 0, 0, 1, 0,
};

const bool bitmapSpecialChar_M[bitmapSpecialChar_W*bitmapSpecialChar_H] PROGMEM={
        1, 1, 1, 1, 1, 
        1, 0, 1, 0, 1, 
        1, 0, 1, 0, 1, 
        1, 0, 1, 0, 1, 
        1, 0, 1, 0, 1, 
};

const bool bitmapSpecialChar_B[bitmapSpecialChar_W*bitmapSpecialChar_H] PROGMEM={
        1, 1, 1, 0, 0,
        1, 0, 0, 1, 0,
        1, 1, 1, 0, 0,
        1, 0, 0, 1, 0,
        1, 1, 1, 0, 0,
};


#endif // PIXEL_NUM_H_
