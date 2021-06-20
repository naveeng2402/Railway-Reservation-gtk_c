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

    pthread_t dest_date_thread, get_trains_thread; // Thread
    int count, len; // Used in callbacks
    char *selected_dest, *selected_date;
    char **dest_val, **date_val, ***lst_box_content; // arrays to store data 
    /* ***lst_box_content is a 2d array of string the cols of each arrays are listed in the enum lstbox_content */

}W_choose_train;

// Structure Containing widgets of choose train
typedef struct _choose_seats
{
    GtkWidget* scr; // Grid of the screen

    GtkWidget *back, *cont; // Buttons
    GtkWidget *title; // Label
    GtkWidget *ac, *non_ac, *ac_sleeper, *non_ac_sleeper; // Flowbox

    pthread_t get_seats_thread; // Threads
    GdkPixbuf **pix; /* Array to store pix_buffs -> avail, booked, window*/
    int count;
    char ***seats, **train_data; // arrays
}W_choose_seats;


// Structure Containing all the data
typedef struct _data
{
    pthread_t start_thread; // Threads

    GtkWidget *win ; GtkWidget *stack;
    
    W_load_scr load;
    W_welcome_scr welcome;
    W_choose_train choose_train;
    W_choose_seats choose_seats;
    
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

enum lstbox_content
{
    LST_BOX_TRAIN_ID,
    LST_BOX_TIME,
    LST_BOX_NAME,
    LST_BOX_AVAIL_SEATS
};

enum train_details
{
    TRAIN_DET_TRAIN_ID,
    TRAIN_DET_NAME,
    TRAIN_DET_TIME,
    TRAIN_DET_AVAIL_SEATS,
    TRAIN_DET_DATE,
    TRAIN_DET_DEST
};

enum seats
{
    SEAT_NO,
    SEAT_CLASS,
    SEAT_IS_BOOKED,
    SEAT_IS_WINDOW
};


enum Pixbuff
    {
        AVAIL_SEAT,
        BOOKED_SEAT,
        WIN_SEAT 
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