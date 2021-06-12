#ifndef STRUCTURES
#define STRUCTURES

#include <gtk/gtk.h>

// Structure Containing widgets of Load_scr
typedef struct _load_scr
{
    GtkWidget *scr; // The grid to the screen

    GtkWidget *title, *load; //lables
    GtkWidget *load_img_scrl; //For Image Scaling (scroll window)
    GtkWidget *progress_bar;
}W_load_scr;

// Structure Containing all the data
typedef struct _data
{
    GtkWidget *win ; GtkWidget *stack;
    
    W_load_scr load;
    
    GHashTable *pixbuffs; // Hash table to store gdkpixbuffs, GtkScrolledWindow and GtkImages  
}DATA;

#endif