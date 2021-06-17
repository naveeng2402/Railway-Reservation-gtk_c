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

// Structure Containing widgets of choose train
typedef struct _choose_train
{
    GtkWidget *scr; // Grid of the screen

    GtkWidget *back, *get_train; // buttons
    GtkWidget *dest, *date; // combobox
    GtkWidget *revealer;
    GtkWidget *lst_box;

    pthread_t dest_date_thread; // Thread
    int count; // Used in callbacks
    char **dest_val, **date_val; // arrays to store data 

}W_choose_train;

// Structure Containing all the data
typedef struct _data
{
    pthread_t start_thread; // Threads

    GtkWidget *win ; GtkWidget *stack;
    
    W_load_scr load;
    W_welcome_scr welcome;
    W_choose_train choose_train;
    
    GHashTable *pixbuffs; // Hash table to store gdkpixbuffs, GtkScrolledWindow and GtkImages  
}DATA;

/*******************************************************************************************
                            SQLITE STRUCTURES
 *******************************************************************************************/

typedef struct _default_trains
{
    // char **dates;
    int *is_dates_already_in_db, *dates_id;
    int *def_id, *name_id, *dest_id, *time_id;
    int *train_ids, *is_train_full; /* used in update_is_train_full */
    int train_id; /* Used in add trains for adding seats */
    int len;
    int count; /* Count is used to keep track of iteration in callbacks */
    
}START_LOAD;

enum default_trains
{
    ID,
    NAME_ID,
    DEST_ID,
    TIME_ID
};

enum static_SEAT_CLASS
{
    CLASS_ZERO,
    AC,
    NON_AC,
    AC_SLEEPER,
    NON_AC_SLEEPER
};

enum static_GENDER
{
    GENDER_ZERO,
    MALE,
    FEMALE,
    TRANSGENDER
};

#endif