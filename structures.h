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
    int is_revealer_visible;
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

// Structure Containing widgets of enter_details
typedef struct _enter_details
{
    GtkWidget *scr; // Grid of the screen

    GtkWidget *pass_dets; // Stack 
    GtkWidget *back, *book; // Buttons
    GtkWidget *contact_name, *contact_number, *contact_mail; // Entries

    int count; // used in callbacks
    int no_of_pass; // no of passengers
    char **seat_nos; // a array of selected seat numbers
    GtkWidget **pass_name, **pass_age, **pass_gen; // arrays of passenger detail entries

}W_enter_details;

// Structure Containing widgets of check details
typedef struct _check_details
{
    GtkWidget *scr; // Grid of the screen

    GtkWidget *check_pass_dets; // List Box
    GtkWidget *back, *confirm; // Buttons
    GtkWidget *contact_name_lbl, *contact_m_no_lbl, *contact_email_lbl; // Labels
    
    int count; // used in callbacks
}W_check_details;

// Structure Containing widgets of check details
typedef struct _view_ticket
{
    GtkWidget *scr; // Grid to the screen

    GtkWidget *details_box; // Box contained inside scrolled window

    GtkWidget *ok, *download; // Buttons
    GtkWidget *web_view;

    pthread_t bk_tic_thread; // Thread that books ticket
    int count; // used in callbacks

}W_view_ticket;

typedef struct _download_tic
{
    GtkWidget *scr;
    GtkWidget *tic_num, *m_num;
    GtkWidget *back, *get_tic;

}W_download_tic;

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

typedef struct _html
{
    pthread_t create_html_thread;
    char **details, ***passenger_details;
    int tic_no, count;
}HTML;

// Structure Containing all the data
typedef struct _data
{
    pthread_t start_thread; // Threads

    GtkWidget *win ; GtkWidget *stack;
    
    W_load_scr load;
    W_welcome_scr welcome;
    W_choose_train choose_train;
    W_choose_seats choose_seats;
    W_enter_details enter_details;
    W_check_details check_details;
    W_view_ticket view_tic;
    W_download_tic download_tic;

    HTML tic_dets;
    
    GHashTable *pixbuffs; // Hash table to store gdkpixbuffs, GtkScrolledWindow and GtkImages  
}DATA;

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

enum html
{
    PASS_NO,
    BK_DATE,
    BK_TIME,
    PRICE,
    TRIAN_ID,
    TRAIN_NAME,
    DESTINATION,
    DEP_DATE,
    DEP_TIME,
    CONTACT_NAME,
    CONTACT_NUMBER,
    CONTACT_EMAIL,
    HTML_NOS
};

enum passenger_details
{
    PASS_SEAT_NO,
    PASS_NAME,
    PASS_AGE,
    PASS_GENDER,
    PASS_CLASS,
    PASS_DET_NOS
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