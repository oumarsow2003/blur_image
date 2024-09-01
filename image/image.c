#include "image.h"
#include <stdlib.h>
#include <stdio.h>

/// @brief alloue de la mémoire pour accueillir l'image à flouter puis à afficher
/// @param nbLignes la longueur de l'image
/// @param nbColonnes la largeur de l'image avec une colonne => 3px (r, g, b)
/// @param r le rowstride (la largeur de l'image en octets) pour s'aligner au format BMP
/// @return l'image créée
image* createImage(int nbLignes, int nbColonnes, int r){
    if (r < nbColonnes*3 || nbLignes <= 0 || nbColonnes <= 0) {
        return NULL;
    }

    image * im = (image*) malloc(sizeof(image));
    if (im == NULL) {
        return NULL;
    }

    im->pixels = (uint8_t*) malloc(nbLignes*r);
    if (im->pixels == NULL) {
        free(im);
        return NULL;
    }

    im->hauteur = nbLignes;
    im->largeur = nbColonnes;
    im->rowstride = r;

    for (int i=0; i < nbLignes; i++) {
        for (int j=0; j < nbColonnes; j++) {
            setPixels(im, i, j, 0, 0, 0);
        }
    }

    return im;
}

void destroyImage(image * im){
    if(im!=NULL) return;
    free(im->pixels);
    free(im);
}

/// @brief affiche les pixels d'une image au format (rrr,ggg,bbb)
/// @param im l'image à afficher
void afficheImage(image *im) {
    if (im == NULL) {
        return;
    }
    uint8_t r, g, b;
    for (int i = 0; i < im->hauteur; i++) {
        for (int j = 0; j < im->largeur; j++) {
            getPixels(im, i, j, &r, &g, &b);
            printf(" (%03hhu,%03hhu,%03hhu) ", r, g, b);
        }
        printf("\n");
    }
}

/// @brief récupère les composantes r, g et b du pixel à la position i et j sur l'image
/// @param im l'image sur la quelle on veut récupéerer le pixel
/// @param i l'abcisse du pixel
/// @param j l'ordonnée du pixel
/// @param r variable qui contiendra la composante r du pixel
/// @param g variable qui contiendra la composante g du pixel
/// @param b variable qui contiendra la composante b du pixel
/// @return 1 si on arrive à récupérer le pixel, 0 sinon
int getPixels(image * im, int i, int j, uint8_t * r, uint8_t * g, uint8_t * b){
    if (i >= 0 && i < im->hauteur && j >= 0 && j < im->largeur) {
        *r = *(im->pixels+i*im->rowstride + 3*j);
        *g = *(im->pixels+i*im->rowstride + 3*j+1);
        *b = *(im->pixels+i*im->rowstride + 3*j+2);
        return 1;
    }
    return 0;
}

/// @brief met à jour les composantes r, g et b du pixel à la position i et j sur l'image
/// @param im l'image à modifier
/// @param i l'abcisse du pixel à modifier
/// @param j l'ordonnée du pixel à modifier
/// @param valr la nouvelle valeur r du pixel
/// @param valg la nouvelle valeur g du pixel
/// @param valb la nouvelle valeur b du pixel
/// @return 1 si le pixel à été modifié, 0 sinon
int setPixels(image * im,int i, int j , uint8_t valr , uint8_t valg ,uint8_t valb){
    if (i>=0 && i<im->hauteur && j>=0 && j<im->largeur) {
        *(im->pixels+i*im->rowstride + 3*j) = valr;
        *(im->pixels+i*im->rowstride + 3*j+1) = valg;
        *(im->pixels+i*im->rowstride + 3*j+2) = valb;
        return 1;
    }
    return 0;
}

/// @brief crée une image BMP à partir de la structure struct image
/// @param nomFichier le nom de l'image à créer
/// @param im la struct image pour la quelle on crée une image BMP
void ecrireImageBMP(const char *nomFichier, image *im) {
    FILE *fichier = fopen(nomFichier, "wb");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Initialisation des en-têtes BMP
    EnteteFichierBMP enteteFichier = {
        .signature = 0x4D42, // "BM"
        .tailleFichier = sizeof(EnteteFichierBMP) + sizeof(EnteteImageBMP) + im->largeur * im->hauteur * 3,
        .reserve1 = 0,
        .reserve2 = 0,
        .offsetDonnees = sizeof(EnteteFichierBMP) + sizeof(EnteteImageBMP)
    };

    EnteteImageBMP enteteImage = {
        .tailleEntete = sizeof(EnteteImageBMP),
        .largeur = im->largeur,
        .hauteur = im->hauteur,
        .nombrePlans = 1,
        .profondeurCouleur = 24, // 24 bits par pixel (RGB)
        .compression = 0,
        .tailleImage = im->largeur * im->hauteur*3,
        .resolutionHorizontale = 2835, // 72 DPI
        .resolutionVerticale = 2835,   // 72 DPI
        .nombreCouleursPalette = 0,
        .nombreCouleursImportantes = 0
    };

    // Écriture des en-têtes dans le fichier
    fwrite(&enteteFichier, sizeof(EnteteFichierBMP), 1, fichier);
    fwrite(&enteteImage, sizeof(EnteteImageBMP), 1, fichier);

    // Écriture des données de l'image dans le fichier
    fwrite(im->pixels, sizeof(uint8_t), im->largeur * im->hauteur * 3, fichier);

    fclose(fichier);
}

/// @brief ouvre une image BMP puis la transforme en une struct image
/// @param nomFichier le nom du fichier à ouvrir
/// @return l'image BMP transfromée en notre struct image ci-dessus
image* chargerImageBMP(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "rb");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    EnteteFichierBMP enteteFichier;
    fread(&enteteFichier, sizeof(EnteteFichierBMP), 1, fichier);
    if (enteteFichier.signature != 0x4D42) {
        fprintf(stderr, "Le fichier n'est pas au format BMP.\n");
        fclose(fichier);
        return NULL;
    }

    EnteteImageBMP enteteImage;
    fread(&enteteImage, sizeof(EnteteImageBMP), 1, fichier);
    int pad =(4-((enteteImage.largeur*24)%4))%4;

    image * im = createImage(enteteImage.hauteur, enteteImage.largeur, enteteImage.largeur * 3 + pad);

    if (im == NULL) {
        fprintf(stderr, "Erreur lors de l'allocation de la mémoire pour l'image.\n");
        fclose(fichier);
        return NULL;
    }

    fread(im->pixels, sizeof(uint8_t), enteteImage.largeur * enteteImage.hauteur * 3, fichier);

    fclose(fichier);

    return im;
}