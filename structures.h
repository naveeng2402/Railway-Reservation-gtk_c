#ifndef STRUCTURES
#define STRUCTURES

#define true 1
#define false 0

/***********************************************************************************************
                                    SCREEN WIDGETS
 **********************************************************************************************/
/* Widgets for Load Screen */
typedef struct _load_scr
{
    GtkWidget *scr;

    GtkWidget *title, *load; // Lables
    GtkWidget *progress_bar;
}W_load_scr;

/* Widgets for Welcome Screen */
typedef struct _welcome_scr
{
    GtkWidget *scr;

    GtkWidget *admin, *info, *bk_tic,
              *dwnld_tic, *cncl_tic; // Buttons
}W_welcome_scr;

/* Widgets for ChooseTrain Screen */
typedef struct _choose_train
{
    GtkWidget *scr; // Grid of the screen

    GtkWidget *back, *get_train; // buttons
    GtkWidget *dest, *date; // combobox
    GtkWidget *revealer;
    GtkWidget *lst_box;

    char* len; // length of available trains
    int is_revealer_visible;
    int date_dest_len[2];  // Lenght of date and dest
    char *selected_dest, *selected_date;
    char **dest_val, **date_val, ***lst_box_content, **seats_available/* available seats in list box */; // arrays to store data 
    /* ***lst_box_content is a 2d array of string the cols of each arrays are listed in the enum lstbox_content */

}W_choose_train;

/* Widgets for ChooseSeats Screen */
typedef struct _choose_seats
{
    GtkWidget* scr; // Grid of the screen

    GtkWidget *back, *cont; // Buttons
    GtkWidget *title; // Label
    GtkWidget *ac, *non_ac, *ac_sleeper, *non_ac_sleeper; // Flowbox

    GdkPixbuf **pix; /* Array to store pix_buffs -> avail, booked, window, {enum -> seat_class_pixbuf} */
    char ***seats, **train_details; // arrays
    /* enums -> seat, train_details */
}W_choose_seats;

/* Widgets for EnterDetails Screen */
typedef struct _enter_details
{
    GtkWidget *scr; // Grid of the screen

    GtkWidget *pass_dets; // Stack 
    GtkWidget *back, *book; // Buttons
    GtkWidget *contact_name, *contact_number, *contact_mail; // Entries

    int no_of_pass; // no of passengers
    char **seat_nos; // a array of selected seat numbers
    GtkWidget **pass_name, **pass_age, **pass_gen; // arrays of passenger detail entries
}W_enter_details;

/* Widgets for CheckDetails Screen */
typedef struct _check_details
{
    GtkWidget *scr; // Grid of the screen

    GtkWidget *check_pass_dets; // List Box
    GtkWidget *back, *confirm; // Buttons
    GtkWidget *contact_name_lbl, *contact_m_no_lbl, *contact_email_lbl; // Labels
}W_check_details;

/* Widgets for ViewTicket Screen */
typedef struct _view_ticket
{
    GtkWidget *scr; // Grid to the screen

    GtkWidget *details_box; // Box contained inside scrolled window

    GtkWidget *ok, *download; // Buttons
    GtkWidget *web_view;

    char* label_content;
}W_view_ticket;

/* Widgets for DownloadTicket Screen */
typedef struct _dwnld_tic
{
    GtkWidget *scr; // Box to the screen

    GtkWidget *tic_num, *mobile_num; // Entries
    GtkWidget *back, *get_tic; // Buttons
    GtkWidget *msg_lbl; // Lable

    GtkWidget *revealer; 
    GtkWidget *tic_img, *dwnld; // GtkRevealer children

}W_dwnld_tic;

/***********************************************************************************************
                                    SQLITE WIDGETS
 **********************************************************************************************/
typedef struct _start_load
{
    char **is_dates_already_in_db, **dates_id;
    char ***default_train_details; /* enum def_train_details */
    char *train_id; /* Used in add trains for adding seats */
    char *len;    
}START_LOAD;

/***********************************************************************************************
                                    MISC STRUCTURES
 **********************************************************************************************/
typedef struct _threads
{
    pthread_t start;
    pthread_t fill_dest_date; /* Welcome scr */
    pthread_t get_train_lst, get_seat_data; /* choose train scr */
    pthread_t book_ticket; /* View ticket scr */
    pthread_t get_tic; /* Download ticket */
}THREADS;

/* The structure that is passed to sqlite callbacks */
typedef struct _callback_pram
{
    char *str, **arr, ***arr_2d;
    int count; /* Count is used to keep track of iteration in callbacks */
}PRAM;

/* Structure containig ticket_details */
typedef struct _ticket_details
{
    char **tic_details, **tic_train_details, **contact_details;
    char ***passenger_details; 
    /* enums are defined in the variable names */
}TICKET_DETAILS;

/***********************************************************************************************
                                    CONSOLIDATED STRUCTURE
 **********************************************************************************************/
typedef struct _data
{
    THREADS threads;

    GtkWidget *win, *stack; // Root Widgets

    W_load_scr load;
    W_welcome_scr welcome;
    W_choose_train choose_train;
    W_choose_seats choose_seats;
    W_enter_details enter_details;
    W_check_details check_details;
    W_view_ticket view_ticket;
    W_dwnld_tic dwnld_tic;

    GHashTable *pixbuffs; // Hash table to store gdkpixbuffs, GtkScrolledWindow and GtkImages  
}DATA;

/***********************************************************************************************
                                    ENUMERATORS
 **********************************************************************************************/

enum def_train_details
{
    DEF_ID,
    DEF_NAME_ID,
    DEF_DEST_ID,
    DEF_TIME_ID,
    DEF_NCOLS
};

enum lstbox_content
{
    LST_BOX_TRAIN_ID,
    LST_BOX_TIME,
    LST_BOX_NAME,
    LST_BOX_NCOLS
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

enum seat_class_pixbuf
{
    AVAIL_SEAT,
    BOOKED_SEAT,
    WIN_SEAT 
};

enum seat
{
    SEAT_NO,
    SEAT_CLASS,
    SEAT_IS_BOOKED,
    SEAT_IS_WINDOW
};

enum gender
{
    GENDER_ZERO,
    MALE,
    FEMALE,
    TRANSGENDER
};

enum tic_details
{
    TICKET_NUMBER,
    NO_OF_PASSENGERS,
    BK_DATE,
    BK_TIME,
    PRICE,
    TIC_DET_NCOLS
};

enum tic_train_details
{
    TRAIN_ID,
    TRAIN_NAME,
    /* FROM_STN, */ /* From station should be here when generating report but for iteration purpose it is moved to last index */
    TO_STN,
    DEP_DATE,
    DEP_TIME,
    FROM_STN,
    TIC_TRAIN_NCOLS
};

enum contact_details
{
    CONTACT_NAME,
    MOBILE_NUM,
    EMAIL,
    CONTACT_NCOLS
};

enum passenger_details
{
    PASS_SEAT_NO,
    PASS_NAME,
    PASS_AGE,
    PASS_GEN,
    PASS_SEAT_CLASS,
    PASS_NCOLS,
};

enum get_tic_db
{
    ENTRY_EMPTY,
    INVALID_TICKET_NO,
    INVALID_MOBILE_NO,
    VALID_DATA,
};

#endif