#ifndef FUNCTIONS
#define FUNCTIONS

#include <gtk/gtk.h>
#include <sqlite3.h>
#include <time.h>
#include "structures.h"
#include "slots.h"
#include "callbacks.h"

G_MODULE_EXPORT void size_allocate_flowbox(GtkWidget*, GtkAllocation*, gpointer); // This is declared here as the g_signal_connect fails

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

    // Widgets for choose_seats
    d->choose_seats.scr = GTK_WIDGET(gtk_builder_get_object(b,"choose_seats_scr"));
    d->choose_seats.back = GTK_WIDGET(gtk_builder_get_object(b,"choose_seat_scr_back_btn"));
    d->choose_seats.cont = GTK_WIDGET(gtk_builder_get_object(b,"choose_seat_scr_continue_btn"));
    d->choose_seats.title = GTK_WIDGET(gtk_builder_get_object(b,"choose_seats_title_lbl"));
    d->choose_seats.ac    = GTK_WIDGET(gtk_builder_get_object(b,"AC"));
    d->choose_seats.non_ac = GTK_WIDGET(gtk_builder_get_object(b,"Non_AC"));
    d->choose_seats.ac_sleeper = GTK_WIDGET(gtk_builder_get_object(b,"AC_Sleeper"));
    d->choose_seats.non_ac_sleeper = GTK_WIDGET(gtk_builder_get_object(b,"Non_AC_Sleeper"));

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
    populate_tbl(b,tbl,"choose_seat_scr_back_scrl","choose_seat_scr_back_ico","back");
    populate_tbl(b,tbl,"choose_seat_scrl","choose_seat_scrl_ico","continue");
    populate_tbl(b,tbl,"choose_seat_avail_scrl","choose_seat_avail_ico","seat_avail");
    populate_tbl(b,tbl,"choose_seat_booked_scrl","choose_seat_booked_ico","seat_booked");
    populate_tbl(b,tbl,"choose_seat_win_scrl","choose_seat_win_ico","win_seat");

}

/* Fill the flowboxes in the choose seat */
void fill_flowboxes(W_choose_seats *data)
{
    GtkWidget *box, *aspect, *label;
    GtkWidget *scroll, *viewport, *img;
    int is_booked, is_window;
    char *seat_status=""; /* avail, booked, win */

    data->pix = calloc(3, sizeof(GdkPixbuf*)); /*avail, booked, window*/
    
    /* Loading the images to the array */
    data->pix[AVAIL_SEAT] = gdk_pixbuf_new_from_resource("/icons/seat_avail.svg",NULL);
    data->pix[BOOKED_SEAT] = gdk_pixbuf_new_from_resource("/icons/seat_booked.svg",NULL);
    data->pix[WIN_SEAT] = gdk_pixbuf_new_from_resource("/icons/win_seat.svg",NULL);

    for (int i = 0; i < 100; i++)
    {
        /* Setting the variables for iteratin */
        is_window = atoi(data->seats[i][SEAT_IS_WINDOW]); is_booked = atoi(data->seats[i][SEAT_IS_BOOKED]);
        seat_status = (is_booked == 1)?"booked":((is_window == 0)?"avail":"win");

        // printf("%-3s : %s\n",data->seats[i][SEAT_NO],seat_status);
        
        label = gtk_label_new(g_strdup_printf("SEAT %s", data->seats[i][SEAT_NO]));

        /* Widgets for dynamic scaling images */
        img = gtk_image_new();
        gtk_image_set_from_pixbuf(GTK_IMAGE(img), data->pix[AVAIL_SEAT]);

        scroll = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scroll), GTK_SHADOW_NONE);
        gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(scroll), 1);
        gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(scroll), 1);
        gtk_widget_set_name(scroll, seat_status);
        g_signal_connect(G_OBJECT(scroll), "size-allocate", G_CALLBACK(size_allocate_flowbox), data);
        gtk_container_add(GTK_CONTAINER(scroll), img);

        aspect = gtk_aspect_frame_new(NULL, 0.50, 0.50, 1, TRUE);
        gtk_frame_set_shadow_type(GTK_FRAME(aspect), GTK_SHADOW_NONE);
        gtk_container_add(GTK_CONTAINER(aspect), scroll);

        /* Box containing all the widgets that is placed in the flowbox */
        box = gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
        gtk_widget_set_hexpand(box, TRUE);
        gtk_widget_set_name(box, seat_status);
        gtk_box_set_homogeneous(GTK_BOX(box),FALSE);
        gtk_container_add(GTK_CONTAINER(box),aspect);
        gtk_box_set_child_packing(GTK_BOX(box),aspect,TRUE,TRUE,0,GTK_PACK_START);
        gtk_container_add(GTK_CONTAINER(box),label);

        /* Choosing the appropriate flowbox */
        if (i>=0 && i < 25)
        {
            gtk_container_add(GTK_CONTAINER(data->ac), box);
        }
        else if (i>=25 && i < 50)
        {
            gtk_container_add(GTK_CONTAINER(data->non_ac), box);
        }
        else if (i>=50 && i < 75)
        {
            gtk_container_add(GTK_CONTAINER(data->ac_sleeper), box);
        }
        else if (i>=75 && i < 100)
        {
            gtk_container_add(GTK_CONTAINER(data->non_ac_sleeper), box);
        }
    }

    // Showing the widgets
    gtk_widget_show_all(data->ac);
    gtk_widget_show_all(data->non_ac);
    gtk_widget_show_all(data->ac_sleeper);
    gtk_widget_show_all(data->non_ac_sleeper);

}

void flowbox_deselect(GtkFlowBox* fbox, GtkFlowBoxChild* child, gpointer data)
{   
    /* Deselects the seat no if the seat is booked */
    GtkWidget *box = GTK_WIDGET(g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(child)),0));

    if ((strcmp(gtk_widget_get_name(box),"booked"))==0)
    {
        gtk_flow_box_unselect_child(fbox, child);
    }
    
}


/* Get the data from GList */
void get_lst_data(gpointer data, gpointer pram)
{
    /* train_data -> ID, NAME, TIME, AVAIL_SEATS */
    W_choose_seats *d = pram;
    const char *text = gtk_label_get_text(GTK_LABEL(data));
    d->train_data[d->count] = calloc(strlen(text),sizeof(char));
    strcpy(d->train_data[d->count],text);
    d->count++;
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
    /* Gets the ids to fill trains using default trains tables */
    char *sql = "SELECT DEFAULT_TRAINS.id, Def_Train_Name.name_id, Def_Train_Dest.dest_id, Def_Train_Time.time_id \
                FROM DEFAULT_TRAINS JOIN Def_Train_Dest ON DEFAULT_TRAINS.id=Def_Train_Dest.train_id JOIN DEST ON DEST.id = Def_Train_Dest.dest_id \
                JOIN Def_Train_Name ON Def_Train_Name.train_id=DEFAULT_TRAINS.id JOIN NAME ON Def_Train_Name.name_id=NAME.id \
                JOIN Def_Train_Time ON Def_Train_Time.train_id=DEFAULT_TRAINS.id JOIN TIMES ON Def_Train_Time.time_id=TIMES.id";
    sqlite3_exec(db,sql,callback_get_default_ids,data,NULL);    
}

void add_seats(sqlite3* db, int train_id)
{
    /* Adds the seats to for each train */
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
    /* Adds trains to the db */
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
            data->train_id, dates_id,
            data->train_id, data->dest_id[def_train],
            data->train_id, data->name_id[def_train],
            data->train_id, data->time_id[def_train]
            );
    sqlite3_exec(db,sql,NULL,NULL,NULL);

    add_seats(db,data->train_id);
}

void update_is_train_full(sqlite3* db, START_LOAD* data)
{   
    /*BUGGY AND NOT USED*/
    char *sql;
    int result;

    /* Getting Train ids that are not full */
    data->count = 0;
    sql = "SELECT id FROM TRAIN WHERE is_train_full=0";
    sqlite3_exec(db,sql,callback_get_train_ids, data, NULL); /* The bug in the function that can't be found */

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
    
    // update_is_train_full(db,data);

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
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(data->date),"Select Date");
    gtk_combo_box_set_active(GTK_COMBO_BOX(data->date),0);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(data->dest),"Select Destination");
    gtk_combo_box_set_active(GTK_COMBO_BOX(data->dest),0);

    for (int i = 0; i < date_len; i++) // Dates
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(data->date),data->date_val[i]);
    }
    for (int i = 0; i < dest_len; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(data->dest),data->dest_val[i]);
    }

    sqlite3_close(db);
}

void* get_list_content(void* arg)
{
    /* This function is called when the get trains buttton is clicked which is in the choose train page, this does the following :
        * Gets the data from the sql which is used to fill the list_box in the choose train page
        * The details are Train no, Time, Name, No of available seats
        * The data is stored in a 2d array DATA.W_choose_train.lst_box_content
    */

    W_choose_train *data = arg;
    sqlite3 *db;

    char *sql="";

    sqlite3_open("rsc/data", &db);

    sql = g_strdup_printf("SELECT count(DISTINCT t.id) FROM \
                            SEAT AS s JOIN static_SEAT_CLASS AS sc ON s.class_id=sc.id \
                            JOIN TRAIN AS t ON t.id=s.train_id \
                            JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id \
                            JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id \
                            JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id \
                            JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id \
                            WHERE des.destination=\"%s\" AND dt.dates_val=\"%s\" AND s.is_booked=0",
                            data->selected_dest,data->selected_date);
    sqlite3_exec(db, sql, callback_get_id,&(data->len),NULL);
    // printf("len %d\n",data->len);

    if (data->len==0)     
    {
        printf("Sorry no trains are available to %s on %s\n",data->selected_dest,data->selected_date);
    }
    else
    {
        data->lst_box_content = calloc(data->len,sizeof(char**));
        data->count = 0;
        /* Getting the train details */
        sql = g_strdup_printf("SELECT DISTINCT t.id, ti.time, n.train_names FROM \
                            SEAT AS s JOIN static_SEAT_CLASS AS sc ON s.class_id=sc.id \
                            JOIN TRAIN AS t ON t.id=s.train_id \
                            JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id \
                            JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id \
                            JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id \
                            JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id \
                            WHERE des.destination=\"%s\" AND dt.dates_val=\"%s\" AND s.is_booked=0",
                            data->selected_dest,data->selected_date);
        sqlite3_exec(db,sql,callback_get_lstbox_content,data,NULL);
        
        /* Get the number of available seats */
        for (int i = 0; i < data->len; i++)
        {
            data->count = i;
            sql = g_strdup_printf("SELECT count(*) FROM \
                            SEAT WHERE is_booked=0 and train_id=\"%s\"",data->lst_box_content[i][LST_BOX_TRAIN_ID]);
            data->lst_box_content[i][LST_BOX_AVAIL_SEATS] = calloc(3,sizeof(char)); /*seat is a 3 digit number*/
            sqlite3_exec(db,sql,callback_get_avail_seats,data->lst_box_content[i][LST_BOX_AVAIL_SEATS],NULL);
        }
        
    }
    sqlite3_close(db);
}

void* get_seats_data(void* arg)
{
    /* This thread is called  when a train is choosed in the choose train and does the following functions
        * Gets the seats data and store it in a 3d array (seats)
    */

    W_choose_seats *data = arg;
    sqlite3 *db;

    char* sql="";

    sqlite3_open("rsc/data", &db);

    sql = g_strdup_printf("SELECT s.seat_no, sc.class, s.is_booked, s.is_window FROM \
                        SEAT AS s JOIN static_SEAT_CLASS AS sc ON s.class_id=sc.id \
                        JOIN TRAIN AS t ON t.id=s.train_id \
                        JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id /*joining train dates*/ \
                        JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id /*joining train name*/ \
                        JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id /*joining train destination*/ \
                        JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id /*joining train time*/ \
                        WHERE t.id = %s",data->train_data[TRAIN_DET_TRAIN_ID]);
    data->count = 0;
    sqlite3_exec(db,sql,callback_get_seat_data,data,NULL);

    // for (int i = 0; i < 100; i++)
    // {
    //     for (int j = 0; j < 4; j++)
    //     {
    //         printf("%s\t",data->seats[i][j]);
    //     }
    //     printf("\n");
    // }
    

}

#endif