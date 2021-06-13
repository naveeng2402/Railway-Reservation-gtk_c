#ifndef STRUCTURES
#define STRUCTURES

#include <gtk/gtk.h>

// Structure Containing widgets of Load_scr
typedef struct _load_scr
{
    GtkWidget *scr; // The grid to the screen

    GtkWidget *title, *load; //lables
    GtkWidget *progress_bar;
}W_load_scr;

// Structure Containing widgets of welcome_scr
typedef struct _welcome_scr
{
    GtkWidget *scr; // The grid of the screen

    GtkWidget *admin, *info, *bk_tic,
              *dwnld_tic, *cncl_tic;    // Buttons
}W_welcome_scr;

// Structure Containing all the data
typedef struct _data
{
    GtkWidget *win ; GtkWidget *stack;
    
    W_load_scr load;
    W_welcome_scr welcome;
    
    GHashTable *pixbuffs; // Hash table to store gdkpixbuffs, GtkScrolledWindow and GtkImages  
}DATA;

#endif