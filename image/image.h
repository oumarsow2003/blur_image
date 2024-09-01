#ifndef _IMAGE_H
#define _IMAGE_H
#include <stdint.h>
typedef struct {
    int largeur;
    int hauteur;
    int rowstride;
    uint8_t * pixels;
}image;
#pragma pack(push, 1)

// Structure pour l'en-tête du fichier BMP
typedef struct {
    uint16_t signature;     // "BM" pour indiquer qu'il s'agit d'un fichier BMP
    uint32_t tailleFichier; // Taille totale du fichier en octets
    uint16_t reserve1;
    uint16_t reserve2;
    uint32_t offsetDonnees;  // Offset où commencent les données de l'image
} EnteteFichierBMP;

// Structure pour l'en-tête de l'image BMP
typedef struct {
    uint32_t tailleEntete;     // Taille de l'en-tête de l'image (40 octets)
    int32_t  largeur;
    int32_t  hauteur;
    uint16_t nombrePlans;
    uint16_t profondeurCouleur; // Nombre de bits par pixel (habituellement 24 bits pour RGB)
    uint32_t compression;
    uint32_t tailleImage;       // Taille de l'image en octets
    int32_t  resolutionHorizontale;
    int32_t  resolutionVerticale;
    uint32_t nombreCouleursPalette;
    uint32_t nombreCouleursImportantes;
} EnteteImageBMP;

#pragma pack(pop)
image * createImage(int,int,int);
void destroyImage(image * im);
void afficheImage(image * im);
int getPixels(image *,int x,int y,uint8_t* r,uint8_t* g,uint8_t* b);
int setPixels(image * ,int x ,int y,uint8_t r,uint8_t g,uint8_t b);
void ecrireImageBMP(const char *nomFichier, image *im);
image* chargerImageBMP(const char* nomFichier);
#endif