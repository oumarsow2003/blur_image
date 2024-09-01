#ifndef _ZPIXEL_H
#define _ZPIXEL_H
#include <stdint.h>
#include "./image/image.h"

typedef struct{
    int size;
    int pointSuperieurGaucheX;
    int pointSuperieurGaucheY;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    double degradation;
}zpixel;

zpixel * createZpixel(int size, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void destroyPixel(zpixel * zpixel);
void setRGBPixel(zpixel * zpixel, uint8_t r, uint8_t g, uint8_t b);
void setZpixelXY(zpixel * zpixel, int x, int y);
void projectionZpixel(zpixel * zpixel, image * im);
#endif