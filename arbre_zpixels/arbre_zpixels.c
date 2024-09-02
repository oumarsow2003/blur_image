#include "arbre_zpixels.h"
#include <math.h>

arbre_zpixel* createArbreZPixel(image *im ,int x , int y , int size){
    int maxDimension = (im->hauteur > im->largeur) ? im->hauteur : im->largeur;
    if (size < maxDimension) {
        return NULL;
    }
    int taille = getNextPowerOfTwo(size);
    return createArbreZPixelR(im, x, y, taille);
}

/* Création d'un arbre de zpixel de taille size*/
arbre_zpixel* createArbreZPixelR(image *im, int x, int y, int size) {
    if (x < 0 || y < 0 || size == 0 || x >= im->hauteur || y >= im->largeur) {
        return NULL;
    }
    zpixel *zpix = createZpixel(size, x, y, 100, 100, 100);
    if (zpix == NULL) {
        return NULL;
    }

    arbre_zpixel* noeud = g_node_new(zpix);
    if (noeud == NULL) {
        return NULL;
    }

    if (size == 1) {
        // On récupère les pixels de l'image à la position x et y
        getPixels(im, x, y, &zpix->r, &zpix->g, &zpix->b);
    } else {
        arbre_zpixel* filsGaucheHaut = createArbreZPixelR(im, x, y, size / 2);
        arbre_zpixel* filsDroiteHaut = createArbreZPixelR(im, x + size / 2, y, size / 2);
        arbre_zpixel* filsGaucheBas = createArbreZPixelR(im, x, y + size / 2, size / 2);
        arbre_zpixel* filsDroiteBas = createArbreZPixelR(im, x + size / 2, y + size / 2, size / 2);

        zpixel* pixels[4];
        int nombreEnfants = 0;

        if (filsGaucheHaut) {
            g_node_append(noeud, filsGaucheHaut);
            pixels[nombreEnfants++] = (zpixel *) filsGaucheHaut->data;
        }
        if (filsDroiteHaut) {
            g_node_append(noeud, filsDroiteHaut);
            pixels[nombreEnfants++] = (zpixel *) filsDroiteHaut->data;
        }
        if (filsGaucheBas) {
            g_node_append(noeud, filsGaucheBas);
            pixels[nombreEnfants++] = (zpixel *) filsGaucheBas->data;
        }
        if (filsDroiteBas) {
            g_node_append(noeud, filsDroiteBas);
            pixels[nombreEnfants++] = (zpixel *) filsDroiteBas->data;
        }

        calculerMoyenneCouleurs(pixels, nombreEnfants, &zpix->r, &zpix->g, &zpix->b);
        
        zpix->degradation = zpix->size;
    }
    return noeud;
}

void calculerMoyenneCouleurs(zpixel** pixels, int nombreEnfants, u_int8_t* r, u_int8_t* g, u_int8_t* b) {
    int totalR = 0, totalG = 0, totalB = 0;
    
    for (int i = 0; i < nombreEnfants; i++) {
        totalR += pixels[i]->r;
        totalG += pixels[i]->g;
        totalB += pixels[i]->b;
    }

    if (nombreEnfants > 0) {
        *r = totalR / nombreEnfants;
        *g = totalG / nombreEnfants;
        *b = totalB / nombreEnfants;
    }
}


void affichageZpixelsInferieurSeuil(arbre_zpixel* arbre, image* im, double seuil) {
    if (arbre == NULL || seuil < 0) {
        return;
    }
    zpixel* zpix = arbre->data;
    if (zpix->degradation <= seuil) {
        projectionZpixel(zpix, im);
    } else {
        arbre_zpixel* child = arbre->children;
        while (child) {
            affichageZpixelsInferieurSeuil(child, im, seuil);
            child = child->next;
        }
    }
}

//Renvoie une image contenant les zpixels dont la dégradation est inférieur au seuil
image * creer_tampon(arbre_zpixel* arbre, image * im, double seuil){
    int hauteur = im->hauteur;
    int largeur = im->largeur;
    int rowstride = im->rowstride;

    image * tampon  = createImage(hauteur, largeur, rowstride);
    affichageZpixelsInferieurSeuil(arbre, tampon, seuil);

    return tampon;
}

int isPowerOfTwo(int n) {
    return (n & (n - 1)) == 0;
}

int getNextPowerOfTwo(int n) {
    if (isPowerOfTwo(n)) { return n;}
    int power = 1;
    while (power < n) {
        power <<= 1;
    }
    return power;
}
