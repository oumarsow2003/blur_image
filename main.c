#include <stdio.h>
#include "image/image.h"
#include "zpixel/zpixel.h"
#include "arbre_zpixels/arbre_zpixels.h"
#include <math.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#define IMAGE_DEGRADEE_FILENAME "image_degradee2.bmp"
#define GLADE_FILENAME "glade.glade"

/*
    Structure qui permet de faire passer les informations
    entre les différentes fonctions du programme.
    L'arbre est donc modifié en fonction du type de dégradation.
    im représente l'image de type image à dégrader
*/
typedef struct {
    GtkBuilder * builder;
    char * filename;
    image * im;
    arbre_zpixel * arbre;
} ImgData;

/*
    Fonction qui crée une nouvelle image dégradée
    en utilisant le seuil et aussi l'arbre qui est crée
    en fonction du type de dégradation choisi
*/
void update_image(ImgData *imgData, int value_int) {
    (void) value_int;
    image *degradee = creer_tampon(imgData->arbre, imgData->im, value_int);
    ecrireImageBMP(IMAGE_DEGRADEE_FILENAME, degradee);

    GtkImage *gtkImage_deg = GTK_IMAGE(gtk_builder_get_object(imgData->builder, "image_finale"));
    gtk_image_set_from_file(gtkImage_deg, IMAGE_DEGRADEE_FILENAME);
}
/*
    Fonction qui gère les changements de valeurs du seuil
    Cette fonction est apellée à chaque changement de la
    valeur du seuil ou du type de dégradation.
    L'arbre est donc recrée avec comme critère le critère 
    sélectionné dans l'interface.
*/
void on_value_changed(GtkRange *range, gpointer data) {
    ImgData *imgData = (ImgData*)data;

    double value = gtk_range_get_value(range);
    int value_int = round(value);

    image * im = imgData->im;

    //  Création de l'arbre en fonction du critère sélectionné
    arbre_zpixel * arbre = createArbreZPixel(im, 0, 0, fmax(im->hauteur, im->largeur));

    imgData->arbre = arbre;

    //  Génération de la nouvelle image avec comme seuil de dégradation la valeur du scale
    update_image(imgData, value_int);
}

void on_window_destroy(gpointer data) {
    ImgData *imgData = (ImgData*)data;
    destroyImage(imgData->im);
    gtk_main_quit();
}

/*
    Fais le lien entre les composants type degradation et seuil de l'interface avec
    les différentes fonctions du programme.
*/
void connect_signals(ImgData *imgData) {
    GtkScale *scale = GTK_SCALE(gtk_builder_get_object(imgData->builder, "scale"));
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(imgData->builder, "window"));

    g_signal_connect(scale, "value-changed", G_CALLBACK(on_value_changed), imgData);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), imgData);
}
/*
    Fonction qui permet l'ouverture du gestionnaire de fichier
    L'utilisateur choisit un fichier et ainsi une nouvelle
    image dégradée est créée en fonction du critère de dégradation
    choisi et du seuil de dégradation.
    Les informations sont stockées dans une structure UserData
*/
void open_file_chooser(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;
    GtkBuilder *builder = (GtkBuilder*)data;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(gtk_widget_get_toplevel(widget)),
                                         action,
                                         "_Cancel",
                                         GTK_RESPONSE_CANCEL,
                                         "_Open",
                                         GTK_RESPONSE_ACCEPT,
                                         NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_ACCEPT) {
        char * filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        // Charger l'image et créer l'arbre
        image * im = chargerImageBMP(filename);
        if (im == NULL) return;
        //Choix du critère de dégradation par taille comme critère par défaut
        arbre_zpixel *arbre = createArbreZPixel(im, 0, 0, fmax(im->hauteur, im->largeur));

        // Stockage de l'image et de l'arbre dans UserData
        ImgData * imgData = g_malloc(sizeof(ImgData));
        imgData->builder = builder;
        imgData->filename = filename;
        imgData->im = im;
        imgData->arbre = arbre;

        connect_signals(imgData);

        arbre = createArbreZPixel(im, 0, 0, fmax(im->hauteur, im->largeur));

        GtkScale *scale = GTK_SCALE(gtk_builder_get_object(builder, "scale"));

        double value = gtk_range_get_value(GTK_RANGE(scale));
        int value_int = round(value);
        update_image(imgData, value_int);

        GtkImage *gtkImage = GTK_IMAGE(gtk_builder_get_object(builder, "image_initiale"));
        GtkImage *gtkImage_deg = GTK_IMAGE(gtk_builder_get_object(builder, "image_finale"));
        gtk_image_set_from_file(gtkImage, filename);
        gtk_image_set_from_file(gtkImage_deg, IMAGE_DEGRADEE_FILENAME);

        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void on_finApropos_select(GtkWidget *widget, gpointer data) {
    (void) widget;
    GtkWidget *window = GTK_WIDGET(data);
    gtk_widget_destroy(window);
}

void on_Apropos_select(GtkWidget *widget, gpointer *data) {
    (void) widget;
    (void) data;
    GtkBuilder *builder = gtk_builder_new();
    GObject *object;
    gtk_builder_add_from_file(builder, GLADE_FILENAME, NULL);
    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "Apropos"));

    object = gtk_builder_get_object(builder, "finApropos");
    g_signal_connect(object, "clicked", G_CALLBACK(on_finApropos_select), window);

    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    GtkWidget *apropos;
    GObject *object;

    gtk_init(&argc, &argv);

    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, GLADE_FILENAME, NULL);

    GtkWidget *window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    
    apropos = GTK_WIDGET(gtk_builder_get_object(builder, "apropos"));
    g_signal_connect(apropos, "activate", G_CALLBACK(on_Apropos_select), NULL);

    object = gtk_builder_get_object(builder, "open_file");
    g_signal_connect(object, "activate", G_CALLBACK(open_file_chooser), builder);

    gtk_widget_show_all(window);

    gtk_main();

    g_object_unref(builder);

    return 0;
}