#ifndef FUNCTIONS
#define FUNCTIONS

#include <gtk/gtk.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <time.h>
#include "structures.h"
#include "slots.h"
#include "callbacks.h"

#ifdef _WIN32 // Adding the module for sleep function(Platform dependent) 
    #include <Windows.h>
#endif

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

    // Widgets for enter_details
    d->enter_details.scr = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_scr"));
    d->enter_details.pass_dets = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_passengers_stack"));
    d->enter_details.back = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_scr_back_btn"));
    d->enter_details.book = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_scr_continue_btn"));
    d->enter_details.contact_name = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_contact_name"));
    d->enter_details.contact_number = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_contact_m_no"));
    d->enter_details.contact_mail = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_contact_email"));

    // Widgets for check_details
    d->check_details.scr = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr"));
    d->check_details.check_pass_dets = GTK_WIDGET(gtk_builder_get_object(b,"check_details_passengers_lst_box"));
    d->check_details.back = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr_back_btn"));
    d->check_details.confirm = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr_continue_btn"));
    d->check_details.contact_name_lbl = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr_contact_name_lbl"));
    d->check_details.contact_m_no_lbl = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr_m_no_lbl"));
    d->check_details.contact_email_lbl = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr_email_lbl"));

    // Widgets for view ticket
    d->view_tic.scr = GTK_WIDGET(gtk_builder_get_object(b,"view_ticket_scr"));
    d->view_tic.ok = GTK_WIDGET(gtk_builder_get_object(b,"view_ticket_scr_ok_btn"));
    d->view_tic.download = GTK_WIDGET(gtk_builder_get_object(b,"view_ticket_scr_dwnld_btn"));
    d->view_tic.web_view = GTK_WIDGET(gtk_builder_get_object(b,"web_view"));

    // Widgets for download tic
    d->download_tic.scr = GTK_WIDGET(gtk_builder_get_object(b,"download_tic"));
    d->download_tic.back = GTK_WIDGET(gtk_builder_get_object(b,"download_scr_back_btn"));
    d->download_tic.get_tic = GTK_WIDGET(gtk_builder_get_object(b,"download_scr_get_tic_btn"));
    d->download_tic.tic_num = GTK_WIDGET(gtk_builder_get_object(b,"download_scr_tic_num"));
    d->download_tic.m_num = GTK_WIDGET(gtk_builder_get_object(b,"download_scr_m_no"));

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
 
    // For Choose train scr
    populate_tbl(b,tbl,"choose_train_scr_back_scrl","choose_train_scr_back_ico","back");
    
    // For choose seats scr
    populate_tbl(b,tbl,"choose_seat_scr_back_scrl","choose_seat_scr_back_ico","back");
    populate_tbl(b,tbl,"choose_seat_scrl","choose_seat_scrl_ico","continue");
    populate_tbl(b,tbl,"choose_seat_avail_scrl","choose_seat_avail_ico","seat_avail");
    populate_tbl(b,tbl,"choose_seat_booked_scrl","choose_seat_booked_ico","seat_booked");
    populate_tbl(b,tbl,"choose_seat_win_scrl","choose_seat_win_ico","win_seat");

    // For enter details
    populate_tbl(b,tbl,"enter_details_scr_back_scrl","enter_details_scr_back_ico","back");
    populate_tbl(b,tbl,"enter_details_bk_scrl","enter_details_bk_ico","book_ticket");

    // For check details
    populate_tbl(b,tbl,"check_details_scr_back_scrl","check_details_scr_back_ico","back");
    populate_tbl(b,tbl,"check_details_scr_confirm_scrl","check_details_scr_confirm_ico","continue");

    // For view details
    populate_tbl(b,tbl,"view_ticket_scr_dwnld_scrl","view_ticket_scr_dwnld_ico","continue");
    populate_tbl(b,tbl,"view_ticket_scr_ok_scrl","view_ticket_scr_ok_ico","continue");

    // For Download Ticket 
    populate_tbl(b,tbl,"download_ticket_scr_back_scrl","download_ticket_scr_back_ico","back");

}

/* Fill the flowboxes in the choose seat */
void fill_flowboxes(W_choose_seats *data)
{
    GtkWidget *box, *aspect, *label;
    GtkWidget *scroll, *img;
    int is_booked, is_window;
    char *seat_status=""; /* avail, booked, win */

    data->pix = calloc(3, sizeof(GdkPixbuf*)); /*avail, booked, window*/
    
    /* Loading the images to the array */
    data->pix[AVAIL_SEAT] = gdk_pixbuf_new_from_resource("/icons/seat_avail.svg",NULL);
    data->pix[BOOKED_SEAT] = gdk_pixbuf_new_from_resource("/icons/seat_booked.svg",NULL);
    data->pix[WIN_SEAT] = gdk_pixbuf_new_from_resource("/icons/win_seat.svg",NULL);

    /* Removing widgets if existed */
    gtk_container_foreach(GTK_CONTAINER(data->ac),rem_container_wgts,data->ac);
    gtk_container_foreach(GTK_CONTAINER(data->non_ac),rem_container_wgts,data->non_ac);
    gtk_container_foreach(GTK_CONTAINER(data->ac_sleeper),rem_container_wgts,data->ac_sleeper);
    gtk_container_foreach(GTK_CONTAINER(data->non_ac_sleeper),rem_container_wgts,data->non_ac_sleeper);

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

/* Deselects a flowbox child */
void flowbox_deselect(GtkFlowBox* fbox, GtkFlowBoxChild* child, gpointer data)
{   
    /* Deselects the seat no if the seat is booked */
    GtkWidget *box = GTK_WIDGET(g_list_nth_data(
                        gtk_container_get_children(GTK_CONTAINER(child)), // FlowBox child
                        0));

    if ((strcmp(gtk_widget_get_name(box),"booked"))==0)
    {
        gtk_flow_box_unselect_child(fbox, child);
    }
    
}

/* Fills the stack inside the enter details scr */
void fill_det_stack(W_enter_details *data)
{
    GtkWidget *name_lbl, *age_lbl, *gen_lbl; /* Lables */
    GtkWidget *grid; /* Grid_layout */
    char *stack_child = malloc(1); /* name and title of the stack child */

    for (int i = 0; i < data->no_of_pass; i++)
    {
        stack_child = g_strdup_printf("SEAT %s",data->seat_nos[i]);

        name_lbl = gtk_label_new(NULL);
        gtk_label_set_use_markup(GTK_LABEL(name_lbl), TRUE);
        gtk_label_set_markup(GTK_LABEL(name_lbl), "<span size=\"15000\" >Passenger Name</span>");
        gtk_widget_set_halign(name_lbl, GTK_ALIGN_END);

        age_lbl = gtk_label_new(NULL);
        gtk_label_set_use_markup(GTK_LABEL(age_lbl), TRUE);
        gtk_label_set_markup(GTK_LABEL(age_lbl), "<span size=\"15000\" >Passenger's Age</span>");
        gtk_widget_set_halign(age_lbl, GTK_ALIGN_END);

        gen_lbl = gtk_label_new(NULL);
        gtk_label_set_use_markup(GTK_LABEL(gen_lbl), TRUE);
        gtk_label_set_markup(GTK_LABEL(gen_lbl), "<span size=\"15000\" >Gender</span>");
        gtk_widget_set_halign(gen_lbl, GTK_ALIGN_END);

        data->pass_name[i] = gtk_entry_new();
        gtk_widget_set_halign(data->pass_name[i], GTK_ALIGN_START);

        data->pass_age[i] = gtk_entry_new();
        gtk_widget_set_halign(data->pass_age[i], GTK_ALIGN_START);

        data->pass_gen[i] = gtk_combo_box_text_new();
        gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(data->pass_gen[i]),0,"Male");
        gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(data->pass_gen[i]),1,"Female");
        gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(data->pass_gen[i]),2,"Transgender");
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(data->pass_gen[i]), GTK_SENSITIVITY_AUTO);
        gtk_combo_box_set_active(GTK_COMBO_BOX(data->pass_gen[i]), 0);
        gtk_widget_set_halign(data->pass_gen[i], GTK_ALIGN_START);

        grid = gtk_grid_new();
        gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
        gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
        gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 20);

        /* Adding the widgets to the grid */
        gtk_grid_attach(GTK_GRID(grid),name_lbl,0,0,1,1);
        gtk_grid_attach(GTK_GRID(grid),age_lbl,0,1,1,1);
        gtk_grid_attach(GTK_GRID(grid),gen_lbl,0,2,1,1);
        gtk_grid_attach(GTK_GRID(grid),data->pass_name[i],1,0,1,1);
        gtk_grid_attach(GTK_GRID(grid),data->pass_age[i],1,1,1,1);
        gtk_grid_attach(GTK_GRID(grid),data->pass_gen[i],1,2,1,1);

        /* Add grid to stack */
        gtk_stack_add_titled(GTK_STACK(data->pass_dets),grid,stack_child,stack_child);
    }

    gtk_widget_show_all(data->pass_dets);

}

/* Fills the lst_box inside check details scr and view ticket scr */
void fill_check_scr_lst_box(DATA *data)
{
    DATA *d = data;
    GtkLabel *seat_no, *name, *age, *gender; /* Lables that are inside the row */
    GtkWidget *row, *box;
    const gchar *lbl_content;
    int size = 15000;

    // Remove all the data from the list_box if existed
    gtk_container_foreach(GTK_CONTAINER(d->check_details.check_pass_dets),rem_container_wgts,d->check_details.check_pass_dets);

        /* Getting the header */
    // seat number
    seat_no = GTK_LABEL(gtk_label_new(""));
    gtk_label_set_use_markup(seat_no,TRUE);
    gtk_label_set_markup(seat_no,"<b><span size=\"15000\">Seat No</span></b>");
    gtk_widget_set_margin_start(GTK_WIDGET(seat_no),30);

    // name
    name = GTK_LABEL(gtk_label_new(""));
    gtk_label_set_use_markup(name,TRUE);
    gtk_label_set_markup(name,"<b><span size=\"15000\">Name</span></b>");

    // age
    age = GTK_LABEL(gtk_label_new(""));
    gtk_label_set_use_markup(age,TRUE);
    gtk_label_set_markup(age,"<b><span size=\"15000\">Age</span></b>");

    // gender
    gender = GTK_LABEL(gtk_label_new(""));
    gtk_label_set_use_markup(gender,TRUE);
    gtk_label_set_markup(gender,"<b><span size=\"15000\">Gender</span></b>");

    // Row for list box
    row = gtk_list_box_row_new();
    gtk_widget_set_size_request(row,50,60);
    gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row),FALSE);
    gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row),FALSE);

    // GtkBox for lables
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
    gtk_box_set_homogeneous(GTK_BOX(box),TRUE);
    gtk_box_pack_start(GTK_BOX(box),GTK_WIDGET(seat_no),TRUE,TRUE,0); 
    gtk_box_pack_start(GTK_BOX(box),GTK_WIDGET(name),TRUE,TRUE,0); 
    gtk_box_pack_start(GTK_BOX(box),GTK_WIDGET(age),TRUE,TRUE,0); 
    gtk_box_pack_start(GTK_BOX(box),GTK_WIDGET(gender),TRUE,TRUE,0); 

    // Adding wids to the parents
    gtk_container_add(GTK_CONTAINER(row),box);
    gtk_list_box_insert(GTK_LIST_BOX(d->check_details.check_pass_dets),row,-1);

    for (int i = 0; i < d->enter_details.no_of_pass; i++)
    {
        // seat number
        lbl_content = g_strdup_printf("<b><span size=\"%d\">%s</span></b>", size, d->enter_details.seat_nos[i]);

        seat_no = GTK_LABEL(gtk_label_new(""));
        gtk_label_set_use_markup(seat_no,TRUE);
        gtk_label_set_markup(seat_no,lbl_content);
        gtk_widget_set_margin_start(GTK_WIDGET(seat_no),30);

        // name
        lbl_content = g_strdup_printf("<b><span size=\"%d\">%s</span></b>", size, gtk_entry_get_text(GTK_ENTRY(d->enter_details.pass_name[i])));

        name = GTK_LABEL(gtk_label_new(""));
        gtk_label_set_use_markup(name,TRUE);
        gtk_label_set_markup(name,lbl_content);

        // Age
        lbl_content = g_strdup_printf("<b><span size=\"%d\">%s</span></b>", size, gtk_entry_get_text(GTK_ENTRY(d->enter_details.pass_age[i])));

        age = GTK_LABEL(gtk_label_new(""));
        gtk_label_set_use_markup(age,TRUE);
        gtk_label_set_markup(age,lbl_content);

        // Gender
        lbl_content = g_strdup_printf("<b><span size=\"%d\">%s</span></b>", size, gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(d->enter_details.pass_gen[i])));

        gender = GTK_LABEL(gtk_label_new(""));
        gtk_label_set_use_markup(gender,TRUE);
        gtk_label_set_markup(gender,lbl_content);

        // Row for list box
        row = gtk_list_box_row_new();
        gtk_widget_set_size_request(row,50,60);
        gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row),FALSE);
        gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row),FALSE);

        // GtkBox for lables
        box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
        gtk_box_set_homogeneous(GTK_BOX(box),TRUE);
        gtk_box_pack_start(GTK_BOX(box),GTK_WIDGET(seat_no),TRUE,TRUE,0); 
        gtk_box_pack_start(GTK_BOX(box),GTK_WIDGET(name),TRUE,TRUE,0); 
        gtk_box_pack_start(GTK_BOX(box),GTK_WIDGET(age),TRUE,TRUE,0); 
        gtk_box_pack_start(GTK_BOX(box),GTK_WIDGET(gender),TRUE,TRUE,0); 

        // Adding wids to the parents
        gtk_container_add(GTK_CONTAINER(row),box);
        gtk_list_box_insert(GTK_LIST_BOX(d->check_details.check_pass_dets),row,-1);

    }

    gtk_widget_show_all(d->check_details.check_pass_dets);
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

    return 0;    

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
    char *sql="";
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

    // Removing all the widgets if already exis
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(data->dest));
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(data->date));

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

    return 0;    

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

    return 0;    

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

    sqlite3_close(db);
    
    // for (int i = 0; i < 100; i++)
    // {
    //     for (int j = 0; j < 4; j++)
    //     {
    //         printf("%s\t",data->seats[i][j]);
    //     }
    //     printf("\n");
    // }
    
    return 0;    

}

void* create_html(void* arg)
{
    DATA *d = arg;
    sqlite3* db;
    char* sql;

    /********* GETTING DATA ***********/
    sqlite3_open("rsc/data", &db);

    // Gettig all the details
    sql = g_strdup_printf(
            "SELECT tic.no_of_passengers, tmp.dates_val, tic.time_of_bk, des.price*tic.no_of_passengers, t.id, n.train_names, des.destination, dt.dates_val, ti.time, tic.contact_name, tic.mobile_no, tic.email_id \
            FROM TICKET AS tic JOIN TRAIN AS t ON t.id   = tic.ticket_train_id \
            JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id \
            JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id \
            JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id \
            JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id \
            JOIN DATES AS tmp ON tmp.id = tic.date_of_bk \
            WHERE tic.ticket_number=%d",
            d->tic_dets.tic_no);
    sqlite3_exec(db, sql,callback_get_ticket_details, &(d->tic_dets), NULL);

    // Getting passenger details
    d->tic_dets.passenger_details = calloc(atoi(d->tic_dets.details[PASS_NO]),sizeof(char**));
    sql = g_strdup_printf(
            "SELECT s.seat_no, p.passenger_name, p.age, g.gender, c.class \
            FROM PASSENGERS as p JOIN static_GENDER as g ON p.gender_id=g.id \
            JOIN SEAT as s ON s.id=p.passenger_seat \
            JOIN static_SEAT_CLASS as c ON c.id=s.class_id \
            WHERE p.ticket_id=%d",
            d->tic_dets.tic_no);
    d->tic_dets.count = 0;
    sqlite3_exec(db,sql,callback_get_passenger_details, &(d->tic_dets), NULL);
    sqlite3_close(db);
    for (int i = 0; i < atoi(d->tic_dets.details[PASS_NO]); i++)
    {
        for (int j = 0; j < PASS_DET_NOS; j++)
        {
            printf("%s\t",d->tic_dets.passenger_details[i][j]);
        }
        printf("\n");
    }

    /*************** GENERATE HTML *********************/
    char *html_header = malloc(1), *temp = malloc(1);
    int ind;

    html_header = "<html> \
            <head> \
                <style> \
                    table, th, td {border: 0.75mm solid black;} \
                </style> \
            </head> \
            <body style=\"width: 210mm; height:297mm; margin-left: auto;margin-right: auto;\"> \
                <div id = \"content\" style=\"margin: 1cm; margin-top: 0.7cm; border: 0.25cm solid black; height: 277mm;\"> \
                    <br> \
                    <h1 style=\"text-align: center;\"><strong>UTOPIAN RAILWAYS</strong></h1> \
                    <h3 style=\"text-align: center;\"><span style=\"text-decoration: underline;\"><strong>Ticket Details</strong></span></h3> \
                    <table style=\"width: 85%; border-collapse: collapse;margin-left: auto; margin-right: auto;\"> \
                        <tbody> \
                            <tr> \
                                <th>Ticket Number</th> \
                                <th>Number of Passengers</th> \
                                <th>Date of Booking</th> \
                                <th>Time of Booking</th> \
                                <th>Price</th> \
                            </tr> \
                            <tr>\n";
    html_header = g_strdup_printf("%s <td style=\"width: 20%%; text-align: center;\">%d</td>",html_header, d->tic_dets.tic_no);
    for (int i = 0; i < 4; i++)
    {
        html_header = g_strdup_printf("%s <td style=\"width: 20%%; text-align: center;\">%s</td>",html_header, d->tic_dets.details[i]);
    }

    temp = "</tbody></table> \
        <h3 style=\"text-align: center;\"><span style=\"text-decoration: underline;\"><strong>Train Details</strong></span></h3> \
        <table style=\"width: 85%; border-collapse: collapse;margin-left: auto; margin-right: auto;\"> \
            <tbody> \
                <tr> \
                    <th>Train Number</th> \
                    <th>Train Name</th> \
                    <th>From</th> \
                    <th>To</th> \
                    <th>Date of Departure</th> \
                    <th>Time of Departure</th> \
                </tr> \
                <tr>";

    html_header = g_strdup_printf("%s %s",html_header,temp);

    for (int i = 4; i <=9; i++)
    {   
        if (i==6)
        {
            html_header = g_strdup_printf("%s <td style=\"width: 16.66%%; text-align: center;\">Chennai Central</td>",html_header);
            continue;
        }
        
        ind = (i>6)?(i==7?6:i-1):i;

        html_header = g_strdup_printf("%s <td style=\"width: 16.66%%; text-align: center;\">%s</td>",html_header, d->tic_dets.details[ind]);
    }

    temp = "</tr> \
            </tbody> \
        </table> \
        <h3 style=\"text-align: center;\"><span style=\"text-decoration: underline;\"><strong>Contact Details</strong></span></h3> \
        <table style=\"width: 85%; border-collapse: collapse;margin-left: auto; margin-right: auto;\"> \
            <tbody> \
                <tr> \
                    <th>Contact Name</th> \
                    <th>Mobile Number</th> \
                    <th>Email Id</th> \
                </tr> \
                <tr> ";
    
    html_header = g_strdup_printf("%s %s",html_header, temp);

    for (int i = 9; i < 12; i++)
    {
        html_header = g_strdup_printf("%s <td style=\"width: 33.33%%; text-align: center;\">%s</td>",html_header, d->tic_dets.details[i]);   
    }

    temp = "</tr> \
            </tbody> \
        </table> \
        <h3 style=\"text-align: center;\"><span style=\"text-decoration: underline;\"><strong>Passenger Details</strong></span></h3> \
        <table style=\"width: 85%; border-collapse: collapse;margin-left: auto; margin-right: auto;\"> \
            <tbody> \
                <tr> \
                    <th>Seat Number</th> \
                    <th>Name</th> \
                    <th>Age</th> \
                    <th>Gender</th> \
                    <th>Class</th> \
                </tr>";
    html_header = g_strdup_printf("%s %s",html_header, temp);
    
    for (int i = 0; i < atoi(d->tic_dets.details[PASS_NO]); i++)
    {
        html_header = g_strdup_printf("%s <tr>\n", html_header);
        for (int j = 0; j < PASS_DET_NOS; j++)
        {
            html_header = g_strdup_printf("%s <td style=\"width: 20%%; text-align: center;\">%s</td>",html_header, d->tic_dets.passenger_details[i][j]);
        }
        html_header = g_strdup_printf("%s </tr>\n", html_header);
    }
    
    temp = "</tbody> \
            </table> \
            <br> \
            </div> \
            </body> \
            </html>";

    html_header = g_strdup_printf("%s %s",html_header, temp);

    // Write to a file
    FILE *f = fopen("rsc/temp.html", "w");
    fprintf(f, "%s", html_header);
    fclose(f);

    return 0;
}

void* book_ticket(void* arg)
{
    DATA* data = arg;
    sqlite3* db;

    char* sql = "";

    time_t *now = malloc(sizeof(time_t));
    struct tm *tm;
    int bk_date_id; char  *bk_time=malloc(1), *todate=malloc(1);
    const char *cntct_name, *m_no, *email, *train_id;

    cntct_name = gtk_entry_get_text(GTK_ENTRY(data->enter_details.contact_name));
    m_no = gtk_entry_get_text(GTK_ENTRY(data->enter_details.contact_number));
    email = gtk_entry_get_text(GTK_ENTRY(data->enter_details.contact_mail));
    train_id = data->choose_seats.train_data[ID];

    time(now);
    tm = localtime(now);
    strftime(bk_time,-1,"%H.%M",tm); // Getting current time 
    strftime(todate,-1,"%d-%m-%Y",tm); // Grtting current date

    sqlite3_open("rsc/data", &db);

    sql = g_strdup_printf("SELECT id from DATES WHERE dates_val = \"%s\"", todate);
    sqlite3_exec(db, sql, callback_get_id, &bk_date_id, NULL);

    printf("%d\n", bk_date_id);

    // Adding data to ticket table
    sql = g_strdup_printf(
            "INSERT INTO TICKET \
            (\"no_of_passengers\", \"contact_name\", \"mobile_no\", \"email_id\", \"time_of_bk\", \"date_of_bk\", \"ticket_train_id\") \
            VALUES (%d, \"%s\", \"%s\", \"%s\", %s, %d, %s)",
            data->enter_details.no_of_pass, cntct_name,m_no,email,bk_time,bk_date_id,train_id);
    sqlite3_exec(db,sql,NULL,NULL,NULL);

    int seat_id, gender_id, tic_id;
    const char *name, *age; char *gender;

    sqlite3_exec(db, "SELECT seq FROM sqlite_sequence WHERE name = \"TICKET\"",callback_get_id,&tic_id,NULL);

    // Adding data to passenger table
    for (int i = 0; i < data->enter_details.no_of_pass; i++)
    {
        sql = g_strdup_printf("SELECT id FROM SEAT WHERE train_id=%s AND seat_no=%s",train_id,data->enter_details.seat_nos[i]);
        sqlite3_exec(db,sql,callback_get_id,&seat_id,NULL);

        sql = g_strdup_printf("UPDATE SEAT set is_booked = 1 WHERE id=%d",seat_id);
        sqlite3_exec(db,sql,NULL,NULL,NULL);

        gender = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(data->enter_details.pass_gen[i]));

        gender_id = (strcmp(gender,"Male")==0)?MALE:((strcmp(gender,"Female")==0)?FEMALE:TRANSGENDER);
        name = gtk_entry_get_text(GTK_ENTRY(data->enter_details.pass_name[i]));
        age = gtk_entry_get_text(GTK_ENTRY(data->enter_details.pass_age[i]));

        sql = g_strdup_printf("INSERT INTO PASSENGERS (\"passenger_name\",\"age\",\"gender_id\",\"passenger_seat\",\"ticket_id\") VALUES (\"%s\",\"%s\",%d,%d,%d)",name,age,gender_id,seat_id,tic_id);
        sqlite3_exec(db,sql,NULL,NULL,NULL);
    }
    sqlite3_close(db);

    data->tic_dets.tic_no = tic_id;
    pthread_create(&(data->tic_dets.create_html_thread), NULL, create_html, data);

    SLEEP(2);
    gtk_stack_set_visible_child(GTK_STACK(data->stack),data->view_tic.scr);
    return 0;
}

void* check_num(void* arg)
{
    DATA* data = arg;
    char* sql = "", **number=calloc(2,sizeof(char*));
    sqlite3* db;

    sqlite3_open("rsc/data", &db);

    sql = g_strdup_printf("SELECT mobile_no FROM TICKET WHERE ticket_number=%d",data->tic_dets.tic_no);
    sqlite3_exec(db,sql,callback_get_m_no,number, NULL);

    printf("%s\n", number[1]);
    if (strcmp(number[1],"")==0 || number[1] != data->download_tic.num)
    {
        printf("FALSE\n");
    }
    

    return 0;
}

#endif