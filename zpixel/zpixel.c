#include "zpixel.h"
#include <stdlib.h>
#include <math.h>
#include<stdio.h>

/// @brief créé une zone de pixel (zpixel) sur une image
///        Cette zone est un groupement de plusieurs pixels de même caractéristiques.
/// @param size la taille de la zone de pixels ==> nombres de pixels dans la zone = size * size
/// @param x la position x de la zone sur l'image
/// @param y la position y de la zone sur l'image
/// @param r composante r de la zone de pixels
/// @param g composante g de la zone de pixels
/// @param b composante b de la zone de pixels
/// @return la zone de pixels créée
zpixel * createZpixel(int size, int x, int y, uint8_t r, uint8_t g, uint8_t b){
    if (size <= 0 || r <=0 || g < 0 || b < 0) {
        return NULL;
    }
    zpixel * zpix = (zpixel *) malloc(sizeof(zpixel));

    if (zpix == NULL) {
        return NULL;
    }

    zpix->size = size;
    setRGBPixel(zpix, r, g, b);
    setZpixelXY(zpix, x, y);
    
    return zpix;
}

void destroyPixel(zpixel * zpix){
    free(zpix);
    zpix = NULL;
}

/// @brief mets à jour les composantes r, g et b d'une zone de pixels
/// @param zpix la zone à modifier
/// @param r la nouvelle composante r de la zone de pixels
/// @param g la nouvelle composante g de la zone de pixels
/// @param b la nouvelle composante b de la zone de pixels
void setRGBPixel(zpixel * zpix, uint8_t r, uint8_t g, uint8_t b){
    zpix->r = r;
    zpix->g = g;
    zpix->b = b;
}

/// @brief mets à jour la position d'une zone de pixels
/// @param zpix la zone de pixels à modifier
/// @param x la nouvelle position x de la zone de pixels
/// @param y la nouvelle position y de la zone de pixels
void setZpixelXY(zpixel * zpix ,int x,int y){
    zpix->pointSuperieurGaucheX = x;
    zpix->pointSuperieurGaucheY = y;
}

/// @brief projette une zone de pixels sur une image
/// @param zpix la zone de pixels à projeter sur une image
/// @param im l'image sur la quelle la zone de pixels sera projétée
void projectionZpixel(zpixel * zpix,image * im){
    if(im == NULL || zpix == NULL) return;
    for(int i = zpix->pointSuperieurGaucheX; i < zpix->pointSuperieurGaucheX + zpix->size; i++){
        for(int j = zpix->pointSuperieurGaucheY; j < zpix->pointSuperieurGaucheY + zpix->size; j++){
            setPixels(im, i, j, zpix->r, zpix->g, zpix->b);    
        }
    }
}