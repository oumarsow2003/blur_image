#ifndef _ARBRE_Z_PIXELS_H
#define _ARBRE_Z_PIXELS_H
#include "../zpixel/zpixel.h"
#include <stdio.h>
#include <glib.h>
#include <stdarg.h>

typedef GNode arbre_zpixel;
typedef struct {
    image * im;
    double seuil;
}dataNode;

arbre_zpixel* createArbreZPixelR(image * im, int x, int y, int size);
arbre_zpixel* createArbreZPixel(image * im, int x, int y, int size);
void calculerMoyenneCouleurs(zpixel** pixels, int nombreEnfants, uint8_t* r, uint8_t* g, uint8_t* b);
void affichageZpixelsInferieurSeuil(arbre_zpixel* arbre, image * pixelsImage, double seuil);
int getNextPowerOfTwo(int n);
image * creer_tampon(arbre_zpixel* arbre, image * im, double seuil);

#endif 