#ifndef FUNCTIONS
#define FUNCTIONS

#include <gtk/gtk.h>
#include <sqlite3.h>
#include <time.h>
#include "structures.h"
#include "slots.h"
#include "callbacks.h"

/* Getting pointors for required widgets */
void get_widgets(GtkBuilder* b, DATA* d)
{   
    //APP roots
    d->win = GTK_WIDGET(gtk_builder_get_object(b,"win"));
    d->stack = GTK_WIDGET(gtk_builder_get_object(b,"stack"));

    //Widgets for load_scr
    d->load.scr = GTK_WIDGET(gtk_builder_get_object(b,"load_scr"));
    d->load.title = GTK_WIDGET(gtk_builder_get_object(b,"load_scr_title_lbl"));
    d->load.progress_bar = GTK_WIDGET(gtk_builder_get_object(b,"load_scr_progress_bar"));

    // Widgets for welcome_scr
    d->welcome.scr = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr"));
    d->welcome.admin = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr_admin_btn"));
    d->welcome.info = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr_info_btn"));
    d->welcome.bk_tic = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr_book_tic_btn"));
    d->welcome.dwnld_tic = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr_dwnld_tic_btn"));
    d->welcome.cncl_tic = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr_cncl_tic_btn"));

    // Widgets for choose_train
    d->choose_train.scr = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr"));
    d->choose_train.back = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_back_btn"));
    d->choose_train.dest = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_dest"));
    d->choose_train.date = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_date"));
    d->choose_train.get_train = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_get_train"));
    d->choose_train.revealer = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_revealer"));
    d->choose_train.lst_box = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_lst_box"));

}

/* Adding data to the hashtable */
void populate_tbl(GtkBuilder* b, GHashTable* tbl, char* scrl_name, char* img_name, char* img_path)
{
    GtkWidget *scrl, *img;
    GdkPixbuf *pix;
    GList *lst = NULL;

    // Getting the widgets and setting the pixbuff
    scrl = GTK_WIDGET(gtk_builder_get_object(b,scrl_name));
    img  = GTK_WIDGET(gtk_builder_get_object(b,img_name));
    pix = gdk_pixbuf_new_from_resource(g_strdup_printf("/icons/%s.svg",img_path),NULL);

    // printf("Working in %s\n",gtk_widget_get_name(scrl));

    gtk_image_set_from_pixbuf(GTK_IMAGE(img),gdk_pixbuf_copy(pix));

    // Making the list
    lst = g_list_append(lst, scrl);
    lst = g_list_append(lst, img);
    lst = g_list_append(lst, pix);

    // Adding the list to hash table
    g_hash_table_insert(tbl,(gpointer)gtk_widget_get_name(scrl),lst);
}

/* Getting Images and setting their pixbuff and scaling */
void get_imgs(GtkBuilder* b,GHashTable* tbl)
{
    // For load_scr
    populate_tbl(b,tbl,"load_img_scrl","load_img","train");    

    // For welcome_scr
    populate_tbl(b,tbl,"welcome_admin_scrl","welcome_admin_ico","admin");
    populate_tbl(b,tbl,"welcome_info_scrl","welcome_info_ico","info");
    populate_tbl(b,tbl,"welcome_scr_book_tic_scrl","welcome_scr_book_tic_img","book_ticket");
    populate_tbl(b,tbl,"welcome_scr_dwnld_tic_scrl","welcome_scr_dwnld_tic_img","view_ticket");
    populate_tbl(b,tbl,"welcome_scr_cncl_tic_scrl","welcome_scr_cncl_tic_img","cancel_ticket");
    populate_tbl(b,tbl,"choose_train_scr_back_scrl","choose_train_scr_back_ico","back");

}

/*************************************************************************************
                            Sqlite Functions
**************************************************************************************/

/* Gets the dates for 3 days */
void add_dates(sqlite3* db, int *dates_id, int *is_already_in_db)
{
    time_t *tme = malloc(sizeof(time_t)), *unix_time=calloc(3,sizeof(time_t));
    struct tm *tm;
    char *today = malloc(1);
    char **dates= calloc(3,sizeof(char*));
    char *sql = malloc(1);

    /* Getting Current time */
    time(tme); // time() function returns the value of time in seconds since 0 hours, 0 minutes, 0 seconds, January 1, 1970
    tm = localtime(tme); /* Converts unix time into human readable time */
    strftime(today,-1,"%d-%m-%Y",tm);

    // printf("From get_dates: TODAY = %s\n", today);

    for (int i = 0; i < 3; i++) /* Getting dates for 3 days from now */
    {
        dates[i] = malloc(1);
        tm->tm_mday += i==0?0:1;
        tm->tm_hour = 23; tm->tm_min=0; tm->tm_sec=0;
        *tme=mktime(tm);  /* Does the dates arithmetic */
        tm=localtime(tme);
        strftime(dates[i],-1,"%d-%m-%Y",tm);
        unix_time[i]=*tme;
    }

    /*Checking whether the date already exists*/
    for (int i = 0; i < 3; i++)
    {
        sql = g_strdup_printf("SELECT count(*) FROM DATES WHERE dates_val=\"%s\"",dates[i]);
        sqlite3_exec(db,sql,callback_get_id,&(is_already_in_db[i]),NULL);
    }

    /* Inserting dates */
    for (int i = 0; i < 3; i++)
    {
        sql = g_strdup_printf("INSERT OR IGNORE INTO DATES (\"dates_val\",\"unix_time\") VALUES (\"%s\",%ld);", dates[i],unix_time[i]);
        sqlite3_exec(db, sql, NULL, NULL, NULL);        
    }
    
    /* Gets dates_id */
    for (int i = 0; i < 3; i++)
    {
        sql = g_strdup_printf("SELECT id FROM DATES WHERE dates_val=\"%s\"",dates[i]);
        sqlite3_exec(db,sql,callback_get_id,&(dates_id[i]),NULL);
    }
    

}

void get_len(sqlite3* db, START_LOAD* data)
{
    /* Get the lenght of the default trains */
    char *sql = "SELECT count(*) \
                FROM DEFAULT_TRAINS JOIN Def_Train_Dest ON DEFAULT_TRAINS.id=Def_Train_Dest.train_id JOIN DEST ON DEST.id = Def_Train_Dest.dest_id \
                JOIN Def_Train_Name ON Def_Train_Name.train_id=DEFAULT_TRAINS.id JOIN NAME ON Def_Train_Name.name_id=NAME.id \
                JOIN Def_Train_Time ON Def_Train_Time.train_id=DEFAULT_TRAINS.id JOIN TIMES ON Def_Train_Time.time_id=TIMES.id";
    sqlite3_exec(db,sql,callback_get_id,&(data->len),NULL);
}

void get_ids(sqlite3 *db, START_LOAD *data)
{
    /* Gets the ids to fill trains using default trains tabeles */
    char *sql = "SELECT DEFAULT_TRAINS.id, Def_Train_Name.name_id, Def_Train_Dest.dest_id, Def_Train_Time.time_id \
                FROM DEFAULT_TRAINS JOIN Def_Train_Dest ON DEFAULT_TRAINS.id=Def_Train_Dest.train_id JOIN DEST ON DEST.id = Def_Train_Dest.dest_id \
                JOIN Def_Train_Name ON Def_Train_Name.train_id=DEFAULT_TRAINS.id JOIN NAME ON Def_Train_Name.name_id=NAME.id \
                JOIN Def_Train_Time ON Def_Train_Time.train_id=DEFAULT_TRAINS.id JOIN TIMES ON Def_Train_Time.time_id=TIMES.id";
    sqlite3_exec(db,sql,callback_get_default_ids,data,NULL);    
}

void add_seats(sqlite3* db, int train_id)
{
    int is_window = 0, class_id = 1, offset = 0;
    char *sql = malloc(1);

    for (int i = 0; i < 4; i++) // Class (4)
    {
        for (int j = 1; j <= 25; j++) // 25 Seats per class
        {
            is_window = (j%5==0 || j%5==1)?1:0; //Conditional Operator

            sql = g_strdup_printf(
                     "INSERT INTO SEAT(\"seat_no\", \"is_window\", \"class_id\", \"train_id\") VALUES(%d,%d,%d,%d);", offset+j,is_window,class_id,train_id
                    );
            sqlite3_exec(db,sql,NULL,NULL,NULL);
        }
        offset += 25;
        class_id ++;
    }
}

void add_trains(sqlite3* db, START_LOAD* data, int dates_id, int def_train) /* def_trains is used as a index */
{
    char *sql = malloc(1);

    sql = "INSERT INTO TRAIN (\"is_train_full\") VALUES(0)";
    sqlite3_exec(db,sql,NULL,NULL,NULL);

    sql = "SELECT seq FROM sqlite_sequence WHERE name=\"TRAIN\"";
    sqlite3_exec(db,sql,callback_get_id,&(data->train_id),NULL);

    sql = g_strdup_printf(
            "INSERT INTO Train_Dates(\"train_id\", \"dates_id\") VALUES(%d, %d);\
            INSERT INTO Train_Dest (\"train_id\", \"dest_id\") VALUES(%d, %d); \
            INSERT INTO Train_Name (\"train_id\", \"name_id\") VALUES(%d, %d); \
            INSERT INTO Train_Time (\"train_id\", \"time_id\") VALUES(%d, %d);",
            data->train_id, data->dates_id[def_train],
            data->train_id, data->dest_id[def_train],
            data->train_id, data->name_id[def_train],
            data->train_id, data->time_id[def_train]
            );
    sqlite3_exec(db,sql,NULL,NULL,NULL);

    add_seats(db,data->train_id);
}

void update_is_train_full(sqlite3* db, START_LOAD* data)
{   
    char *sql;
    int result;

    /* Getting Train ids that are not full */
    data->count = 0;
    sql = "SELECT id FROM TRAIN WHERE is_train_full=0";
    sqlite3_exec(db,sql,callback_get_train_ids, data, NULL);

    /* Getting the number of trains that are not full */ 
    sql = "SELECT count(*) FROM TRAIN WHERE is_train_full=0";
    sqlite3_exec(db,sql,callback_get_id, &(data->len), NULL);

    for (int i = 0; i < data->len; i++)
    {
        sql = g_strdup_printf("SELECT count(*) FROM SEAT WHERE is_booked=0 AND train_id=%d",data->train_ids[i]);
        sqlite3_exec(db,sql,callback_get_id,&result,NULL);

        if (result == 0)
        {
            sql = g_strdup_printf("UPDATE TRAIN SET is_train_full=1 WHERE id=%d",data->train_ids[i]);
            sqlite3_exec(db,sql,NULL,NULL,NULL);
        }
        
    }
    

}

/*************************************************************************************
                            Thread Functions
***************************************************************************************/

/* Make the system to sleep */
void SLEEP(int s)
{
    #ifdef _WIN32
    Sleep(s*1000);
    #else
    sleep(s);
    #endif
}

void* start_load(void* arg)
{
    /*  This function is the one that is started at the time of initial loading, this does the following actions
            * Adds dates entires to the DATES table if not already present
            * Adds train entries to the TRAIN tabel using the DEFAULT set of tables as template and also generates its seats
    */

    DATA *d = (DATA*) arg; // Typecasting the argument

    gtk_label_set_markup(GTK_LABEL(d->load.title),"<b><span size=\"30000\"> Welcome to \n Railway Reservation System </span></b>");

    sqlite3 *db;
    START_LOAD *data = malloc(sizeof(START_LOAD));

    data->count = 0;

    sqlite3_open("rsc/data",&db); // Opening the Database

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(d->load.progress_bar),0.1);

    get_len(db, data); // Gets the length of the default trains

    /* Allocating memory */
    data->def_id   = calloc(data->len,sizeof(int));
    data->name_id  = calloc(data->len,sizeof(int));
    data->dest_id  = calloc(data->len,sizeof(int));
    data->time_id  = calloc(data->len,sizeof(int));
    data->dates_id = calloc(3,sizeof(int)); /* No of dates is fixed */
    data->is_dates_already_in_db = calloc(3,sizeof(int));
    data->train_ids = calloc(data->len*3,sizeof(int)); /* No of trains per date */
    data->is_train_full = calloc(data->len*3,sizeof(int));

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(d->load.progress_bar),0.3);

    add_dates(db,data->dates_id,data->is_dates_already_in_db);

    get_ids(db, data); // Gets the ids to be filled

    for (int i = 0; i < 3; i++) /* DATES */
    {   
        if (data->is_dates_already_in_db[i] != 0) /* Checks if the dates already exists */
        {
            continue;
        }
        
        for (int j = 0; j < data->len ; j++) /* No of default trains */
        {
           add_trains(db, data, data->dates_id[i],j);
        }        
    }

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(d->load.progress_bar),0.9);
    SLEEP(1);
    
    update_is_train_full(db,data);

    sqlite3_close(db); /* Closes the db */

    /* Change the screen */
    gtk_stack_set_visible_child(GTK_STACK(d->stack), d->welcome.scr);
}

void* get_dest_date(void* arg)
{
    /* This function is called during the page change from welcome_scr to choose_trains under book tickets, this does the following :
        * Gets all the destinations and adds it to the combobox 
        * Gets dates from now and 2 days after today and adds them to the combobox
    */

    W_choose_train *data = arg; 
    sqlite3* db;

    int date_len, dest_len;
    char *sql="", **dates_val, **dest_val;
    time_t *now=malloc(sizeof(time_t));

    time(now);

    sqlite3_open("rsc/data", &db);

    // Getting the length
    sql=g_strdup_printf("SELECT count(*) FROM DATES WHERE unix_time>%ld",*now);
    sqlite3_exec(db,sql,callback_get_id,&date_len,NULL);
    sqlite3_exec(db,"SELECT count(*) FROM DEST",callback_get_id,&dest_len,NULL);

    // Allocation of memory
    data->date_val = calloc(date_len,sizeof(char*));
    data->dest_val = calloc(dest_len,sizeof(char*));

    // Getting the values for dates and destinations
    data->count = 0;
    sqlite3_exec(db, "SELECT DISTINCT destination FROM DEST",callback_get_dest,data,NULL);

    data->count=0;
    sql = g_strdup_printf("SELECT DISTINCT dates_val FROM DATES WHERE unix_time>%ld",*now);
    sqlite3_exec(db,sql,callback_get_date,data,NULL);

    // Adding the data to combobox
    for (int i = 0; i < date_len; i++) // Dates
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(data->date),data->date_val[i]);
    }
    for (int i = 0; i < dest_len; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(data->dest),data->dest_val[i]);
    }
}

#endif