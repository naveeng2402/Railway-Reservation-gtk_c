#ifndef SLOTS
#define SLOTS

#include <gtk/gtk.h>
#include <pthread.h>
#include "structures.h"
#include "functions.h"

void quit(AtkWindow *win, gpointer data)
{
    DATA *d = data;
    pthread_join(d->start_thread,NULL);
    printf("Joined Thread 1\n");
    pthread_join(d->choose_train.dest_date_thread, NULL);
    printf("Joined Thread 2\n");
    pthread_join(d->choose_train.get_trains_thread,NULL);
    printf("Joined Thread 3\n");
    pthread_join(d->choose_seats.get_seats_thread, NULL);
    printf("Joined Thread 4\n");
    pthread_join(d->view_tic.bk_tic_thread, NULL);
    printf("Joined Thread 5\n");
    pthread_join(d->tic_dets.create_html_thread,NULL);
    printf("Joined Thread 6\n");
    pthread_join(d->download_tic.check_num_thread,NULL);
    printf("Joined Thread 7\n");
    gtk_main_quit();
}

void size_allocate(GtkWidget* w, GtkAllocation* allocation, gpointer data)
{
    // Getting all the data
    GHashTable *tbl = ((DATA*)data)->pixbuffs;
    GList *lst = g_hash_table_lookup(tbl,gtk_widget_get_name(w)); 
    GtkImage *img = (GtkImage*) g_list_nth_data(lst, 1);
    GdkPixbuf *pix = (GdkPixbuf*) g_list_nth_data(lst, 2);
    GdkPixbuf *img_pix = gtk_image_get_pixbuf(img);

    // printf("\t\tSIGNAL\n");
    // printf(" %s - %d\n", gtk_widget_get_name(w),GTK_IS_IMAGE(img));
    // printf(" %s - %d\n", gtk_widget_get_name(w),GTK_IS_SCROLLED_WINDOW(w));

    // Changing the size
    if(allocation->width != gdk_pixbuf_get_width(img_pix) || allocation->height != gdk_pixbuf_get_height(img_pix))
    {
        gtk_image_set_from_pixbuf(img,gdk_pixbuf_scale_simple(pix,allocation->width,allocation->height,GDK_INTERP_BILINEAR));
    }
}

void size_allocate_flowbox(GtkWidget* w, GtkAllocation* allocation, gpointer data)
{
    GtkImage *img = GTK_IMAGE(g_list_nth_data(
                                gtk_container_get_children(GTK_CONTAINER(g_list_nth_data(
                                gtk_container_get_children(GTK_CONTAINER(w) // Scrolled Window
                                ),0)) // Viewport
                                ),0));
    GdkPixbuf *img_pix = gtk_image_get_pixbuf(img);
    const GdkPixbuf *pix;

    W_choose_seats *d = data;

    if ((strcmp(gtk_widget_get_name(w),"avail"))==0)
    {   
        // printf("\nAvail\n");
        pix = d->pix[AVAIL_SEAT];
    }
    else if ((strcmp(gtk_widget_get_name(w),"booked"))==0)
    {
        // printf("\nBooked\n");
        pix = d->pix[BOOKED_SEAT];
    }
    else if ((strcmp(gtk_widget_get_name(w),"win"))==0)
    {
        // printf("\nWin\n");
        pix = d->pix[WIN_SEAT];
    }

    if(allocation->width != gdk_pixbuf_get_width(img_pix) || allocation->height != gdk_pixbuf_get_height(img_pix))
    {
        gtk_image_set_from_pixbuf(img,gdk_pixbuf_scale_simple(pix,allocation->width,allocation->height,GDK_INTERP_BILINEAR));
    }
    
}

void welcome_info(GtkButton* btn, gpointer data)
{
    /*
        This is an unusual function as it uses GtkDialog.
          * In this function a new builder object is created due to a wierd property of dialog that it works only once if it created as other widgets, to tackle that each time the button is clicked a new dialog is created and is shown then destroyed immediately after it is closed
          * The image in this Dialog box does not support dynamic scaling implemented to others
          * The "Ok" button works because a response id is set in glade file
    */

    GtkBuilder *b = gtk_builder_new_from_resource("/UI/UI.glade");
    GtkDialog *dig = GTK_DIALOG(gtk_builder_get_object(b,"Info_dialog"));
    gtk_dialog_run(dig);
    // printf("Destroying Dialog");
    g_object_unref(b);
    gtk_widget_destroy(GTK_WIDGET(dig));
}

void book_tic(GtkButton* btn, gpointer data)
{
    /* Activated when the Book Ticket button is clicked */
    DATA *d = data;
    gtk_stack_set_transition_type(GTK_STACK(d->stack),GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    pthread_create(&(d->choose_train.dest_date_thread),NULL,get_dest_date,&(d->choose_train));
    gtk_stack_set_visible_child(GTK_STACK(d->stack),d->choose_train.scr);
}

void back_to_welcome(GtkButton* btn, gpointer data)
{
    /* This fn is used to move from choose train scr to welcome scr
        * while moving all the entries from the comboboxes(Dropbox) are deleted to avoid duplicates when again entered 
    */
    DATA *d=data;
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(d->choose_train.dest));
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(d->choose_train.date));
    gtk_stack_set_visible_child(GTK_STACK(d->stack),d->welcome.scr);
}

void back_to_choose_train(GtkButton* btn, gpointer data)
{
    /* This fn is used to move from choose seat scr to choose train scr
    
    */
    DATA *d = data;

    gtk_container_foreach(GTK_CONTAINER(d->choose_seats.ac),rem_container_wgts,d->choose_seats.ac);
    gtk_container_foreach(GTK_CONTAINER(d->choose_seats.non_ac),rem_container_wgts,d->choose_seats.non_ac);
    gtk_container_foreach(GTK_CONTAINER(d->choose_seats.ac_sleeper),rem_container_wgts,d->choose_seats.ac_sleeper);
    gtk_container_foreach(GTK_CONTAINER(d->choose_seats.non_ac_sleeper),rem_container_wgts,d->choose_seats.non_ac_sleeper);
    gtk_stack_set_visible_child(GTK_STACK(d->stack),d->choose_train.scr);
}

void back_to_enter_details(GtkButton* btn,gpointer data)
{   
    /* This function used to move from check details to enter details */

    DATA *d = data;

    // Remove all the widgets from the stack
    gtk_container_foreach(GTK_CONTAINER(d->check_details.check_pass_dets),rem_container_wgts,d->check_details.check_pass_dets);

    gtk_stack_set_visible_child(GTK_STACK(d->stack), d->enter_details.scr);
}

void back_to_choose_seat(GtkButton* btn,gpointer data)
{
    /* This fn is used to move to from enter details to choose seats */
    DATA *d = data;
    gtk_container_foreach(GTK_CONTAINER(d->enter_details.pass_dets),rem_container_wgts,d->enter_details.pass_dets);
    gtk_stack_set_visible_child(GTK_STACK(d->stack),d->choose_seats.scr);
}

void get_available_trains(GtkButton* btn, gpointer data)
{
    /* 
        This function is called when the get trains buttton is clicked which is in the choose train page
    */
    DATA *d = data;
    GtkWidget *train_id_lbl, *name_lbl, *time_lbl, *avail_seats_lbl, *info_lbl; /*Widgets for row*/
    GtkWidget *row, *box;
    const gchar *lbl_content;

    d->choose_train.selected_date = "", d->choose_train.selected_dest = "";

    d->choose_train.selected_date = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(d->choose_train.date));
    d->choose_train.selected_dest = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(d->choose_train.dest));

    // Get the data from the db
    pthread_create(&(d->choose_train.get_trains_thread),NULL,get_list_content,&(d->choose_train));
    pthread_join(d->choose_train.get_trains_thread,NULL);
    
    // Remove all the data from the list_box if existed
    gtk_container_foreach(GTK_CONTAINER(d->choose_train.lst_box),rem_container_wgts,d->choose_train.lst_box);

    // If no trains are available prompt its
    if (d->choose_train.len == 0)
    {
        row = gtk_list_box_row_new();
        gtk_widget_set_size_request(row,100,100);
        gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row),FALSE);
        gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row),FALSE);

        lbl_content = g_strdup_printf("<b><span size=\"30000\">Sorry no trains available to\n%s on %s</span></b>",d->choose_train.selected_dest,d->choose_train.selected_date);
        info_lbl = gtk_label_new("");

        gtk_label_set_use_markup(GTK_LABEL(info_lbl),TRUE);
        gtk_label_set_markup(GTK_LABEL(info_lbl),lbl_content);
        gtk_label_set_justify(GTK_LABEL(info_lbl),GTK_JUSTIFY_CENTER);

        gtk_container_add(GTK_CONTAINER(row),info_lbl);
        gtk_list_box_insert(GTK_LIST_BOX(d->choose_train.lst_box),row,-1);
    }
    else // Show the train details
        {
                /* Add the header */
            // Train id label
            train_id_lbl = gtk_label_new("");
            gtk_widget_set_margin_start(train_id_lbl,40);
            gtk_label_set_use_markup(GTK_LABEL(train_id_lbl),TRUE);
            gtk_label_set_markup(GTK_LABEL(train_id_lbl),"<b><span size=\"15000\">TRAIN NUMBER</span></b>");

            // Train name label
            name_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(name_lbl),TRUE);
            gtk_label_set_markup(GTK_LABEL(name_lbl),"<b><span size=\"15000\">TRAIN NAME</span></b>");
            
            // Train time label
            time_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(time_lbl),TRUE);
            gtk_label_set_markup(GTK_LABEL(time_lbl),"<b><span size=\"15000\">TIME</span></b>");

            // Available no of trains label
            avail_seats_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(avail_seats_lbl),TRUE);
            gtk_label_set_markup(GTK_LABEL(avail_seats_lbl),"<b><span size=\"15000\">AVAILABLE SEATS</span></b>");

            // Row for list box
            row = gtk_list_box_row_new();
            gtk_widget_set_size_request(row,50,60);
            gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row),FALSE);
            gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row),FALSE);

            // Gtk_box for the lables
            box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
            gtk_box_set_homogeneous(GTK_BOX(box),TRUE);
            gtk_box_pack_start(GTK_BOX(box),train_id_lbl,FALSE,TRUE,0);
            gtk_box_pack_start(GTK_BOX(box),name_lbl,TRUE,TRUE,0);
            gtk_box_pack_start(GTK_BOX(box),time_lbl,TRUE,TRUE,0);
            gtk_box_pack_start(GTK_BOX(box),avail_seats_lbl,TRUE,TRUE,0);

            // Adding wids to the parents
            gtk_container_add(GTK_CONTAINER(row),box);
            gtk_list_box_insert(GTK_LIST_BOX(d->choose_train.lst_box),row,-1);

        /* Add the train details */
        for (int i = 0; i < d->choose_train.len; i++)
        {
            // Print the data
            // for (int j = 0; j < 4; j++)
            // {
            //     printf("%s\t",d->choose_train.lst_box_content[i][j]);
            // }
            // printf("\n");

            train_id_lbl = gtk_label_new("");
            gtk_widget_set_margin_start(train_id_lbl,40);
            gtk_label_set_use_markup(GTK_LABEL(train_id_lbl),TRUE);
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",d->choose_train.lst_box_content[i][ID]);
            gtk_label_set_markup(GTK_LABEL(train_id_lbl),lbl_content);

            name_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(name_lbl),TRUE);
            gtk_label_set_ellipsize(GTK_LABEL(name_lbl),PANGO_ELLIPSIZE_END);
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",d->choose_train.lst_box_content[i][LST_BOX_NAME]);
            gtk_label_set_markup(GTK_LABEL(name_lbl),lbl_content);
            
            time_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(time_lbl),TRUE);
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",d->choose_train.lst_box_content[i][LST_BOX_TIME]);
            gtk_label_set_markup(GTK_LABEL(time_lbl),lbl_content);

            avail_seats_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(avail_seats_lbl),TRUE);
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",d->choose_train.lst_box_content[i][LST_BOX_AVAIL_SEATS]);
            gtk_label_set_markup(GTK_LABEL(avail_seats_lbl),lbl_content);

            row = gtk_list_box_row_new();
            gtk_widget_set_size_request(row,50,60);

            box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL,0);
            gtk_box_set_homogeneous(GTK_BOX(box),TRUE);
            gtk_box_pack_start(GTK_BOX(box),train_id_lbl,FALSE,TRUE,0);
            gtk_box_pack_start(GTK_BOX(box),name_lbl,TRUE,TRUE,0);
            gtk_box_pack_start(GTK_BOX(box),time_lbl,TRUE,TRUE,0);
            gtk_box_pack_start(GTK_BOX(box),avail_seats_lbl,TRUE,TRUE,0);

            gtk_container_add(GTK_CONTAINER(row),box);
            gtk_list_box_insert(GTK_LIST_BOX(d->choose_train.lst_box),row,-1);
        }
    }
    
    // All set and show the widgets
    gtk_revealer_set_transition_duration(GTK_REVEALER(d->choose_train.revealer),3000);
    gtk_revealer_set_reveal_child(GTK_REVEALER(d->choose_train.revealer),TRUE);
    gtk_widget_show_all(d->choose_train.lst_box);
    d->choose_train.is_revealer_visible = 1;
}

void choose_train_combobox_changed(GtkComboBox *cbox, gpointer data)
{
    /* This function is called when the combobox active element changed */
    if (((DATA*)data)->choose_train.is_revealer_visible == 1)
        get_available_trains(NULL,data);
}

void train_selected(GtkListBox* box, GtkListBoxRow *row, gpointer data)
{   
    /*  This fn is called when the train is selected in the choose train screen, This function does the following:
        * Gets the data(train id) in the row
        * Using that data gets the seats and stores it in DATA.W_choose_seat.train_data which is char array(string)
        * Fills the flowboxes with appropriate data
    */

    DATA *d = data;
    char *title_text = "";
    GList *lst = gtk_container_get_children(
                        GTK_CONTAINER
                        (
                            g_list_nth_data(gtk_container_get_children
                            (
                                GTK_CONTAINER(row) // Row
                            ),0)
                        ) /*Box*/ );

    d->choose_seats.count = 0;
    free(d->choose_seats.train_data);
    d->choose_seats.train_data = calloc(6,sizeof(char*)); /* ID, NAME, TIME, AVAIL_SEATS, DATE, DEST */
    g_list_foreach(lst,get_lst_data,&(d->choose_seats));

    d->choose_seats.train_data[TRAIN_DET_DATE] = calloc(strlen(d->choose_train.selected_date),sizeof(char));
    d->choose_seats.train_data[TRAIN_DET_DEST] = calloc(strlen(d->choose_train.selected_dest),sizeof(char));
    strcpy(d->choose_seats.train_data[TRAIN_DET_DATE],d->choose_train.selected_date);
    strcpy(d->choose_seats.train_data[TRAIN_DET_DEST],d->choose_train.selected_dest);

    // for (int i = 0; i < 6; i++)
    // {
    //     printf("%s\t",d->choose_seats.train_data[i]);
    // }
    // printf("\n");

    // Allocating memory
    free(d->choose_seats.seats);
    d->choose_seats.seats = calloc(100, sizeof(char**)); // 100 seats

    d->choose_seats.count = 0;
    pthread_create(&(d->choose_seats.get_seats_thread),NULL,get_seats_data,&(d->choose_seats));
    pthread_join(d->choose_seats.get_seats_thread, NULL);
    // printf("Row Clicked\n");
    fill_flowboxes(&(d->choose_seats));

    title_text = g_strdup_printf("<b><span size=\"30000\">%s (%s @ %s)</span></b>",d->choose_seats.train_data[TRAIN_DET_NAME],d->choose_seats.train_data[TRAIN_DET_DATE],d->choose_seats.train_data[TRAIN_DET_TIME]);
    gtk_label_set_markup(GTK_LABEL(d->choose_seats.title), title_text);
    gtk_stack_set_visible_child(GTK_STACK(d->stack), d->choose_seats.scr);
}

void flowbox_selection_changed(GtkFlowBox* fbox, gpointer data)
{
    /* This function is called when a flowbox is selected; it helps to make booked seats unselectable */
    DATA *d = data;

    gtk_flow_box_selected_foreach(fbox, flowbox_deselect, d);
}

void choose_seat_continue_clicked(GtkButton* btn,gpointer data)
{
    /* This fn is called when the continue btn in choose seats is clicked, This function does the following:
        * Gets the no of seats and stores it in data.enter_details.no_of_pass
        * Using the data the stack is  filled
    */
    
    DATA *d = data;

    /* Getting the selected list from all the 4 flowbox and concating them into a single doubly linked list */
    GList *selected_seats = g_list_concat(g_list_concat(g_list_concat(gtk_flow_box_get_selected_children(GTK_FLOW_BOX(d->choose_seats.ac)),gtk_flow_box_get_selected_children(GTK_FLOW_BOX(d->choose_seats.non_ac))), gtk_flow_box_get_selected_children(GTK_FLOW_BOX(d->choose_seats.ac_sleeper))), gtk_flow_box_get_selected_children(GTK_FLOW_BOX(d->choose_seats.non_ac_sleeper)));

    d->enter_details.no_of_pass = g_list_length(selected_seats);

    if (d->enter_details.no_of_pass == 0)
    {
        GtkWidget *dig = GTK_WIDGET(gtk_builder_get_object(gtk_builder_new_from_resource("/UI/UI.glade"),"Message_dig"));
        gtk_message_dialog_format_secondary_markup(GTK_MESSAGE_DIALOG(dig),"Please select atleast 1 seat");
        gtk_dialog_run(GTK_DIALOG(dig));
        gtk_widget_destroy(GTK_WIDGET(dig));
    }
    else
    {   
        // Allocating memory for entry widgets
        d->enter_details.seat_nos  = calloc(d->enter_details.no_of_pass, sizeof(char*));
        d->enter_details.pass_name = calloc(d->enter_details.no_of_pass, sizeof(GtkWidget*));
        d->enter_details.pass_age = calloc(d->enter_details.no_of_pass, sizeof(GtkWidget*));
        d->enter_details.pass_gen = calloc(d->enter_details.no_of_pass, sizeof(GtkWidget*));

        // Gets the selected seat numbers and adds it to a array
        d->enter_details.count = 0;
        g_list_foreach(selected_seats, get_seat_nums, &(d->enter_details));

        // Remove all the widgets from the stack if existed
        gtk_container_foreach(GTK_CONTAINER(d->enter_details.pass_dets),rem_container_wgts,d->enter_details.pass_dets);

        // Fill the stack in enter details scr
        fill_det_stack(&(d->enter_details));

        gtk_entry_set_text(GTK_ENTRY(d->enter_details.contact_name),"");
        gtk_entry_set_text(GTK_ENTRY(d->enter_details.contact_number),"");
        gtk_entry_set_text(GTK_ENTRY(d->enter_details.contact_mail),"");

        gtk_stack_set_visible_child(GTK_STACK(d->stack),d->enter_details.scr);
    }
}

void enter_details_continue_clicked(GtkButton* btn,gpointer data)
{
    DATA *d = data;
    int no_of_entries = 3+((d->enter_details.no_of_pass)*2), is_empty_index = 2;
    int *is_empty = calloc(no_of_entries,sizeof(int)), is_all_good = 1;

    is_empty[0] = (strcmp(gtk_entry_get_text(GTK_ENTRY(d->enter_details.contact_name)),"")==0)?1:0;
    is_empty[1] = (strcmp(gtk_entry_get_text(GTK_ENTRY(d->enter_details.contact_number)),"")==0)?1:0;
    is_empty[2] = (strcmp(gtk_entry_get_text(GTK_ENTRY(d->enter_details.contact_mail)),"")==0)?1:0;

    for (int i = 0; i < d->enter_details.no_of_pass; i++)
    {
        is_empty[++is_empty_index] = (strcmp(gtk_entry_get_text(GTK_ENTRY(d->enter_details.pass_name[i])),"")==0)?1:0;
        is_empty[++is_empty_index] = (strcmp(gtk_entry_get_text(GTK_ENTRY(d->enter_details.pass_age[i])),"")==0)?1:0;
    }
    
    for (int i = 0; i < no_of_entries; i++)
    {
        if (is_empty[i]==1)
        {
            is_all_good = 0;
            break;
        }
        
    }
    
    if (is_all_good == 0)
    {
        GtkWidget *dig = GTK_WIDGET(gtk_builder_get_object(gtk_builder_new_from_resource("/UI/UI.glade"),"Message_dig"));
        gtk_message_dialog_format_secondary_markup(GTK_MESSAGE_DIALOG(dig),"Please Enter all the details");
        gtk_dialog_run(GTK_DIALOG(dig));
        gtk_widget_destroy(GTK_WIDGET(dig));
    }
    else
    {
        // Filling the contact details
        gtk_label_set_text(GTK_LABEL( d->check_details.contact_name_lbl), gtk_entry_get_text(GTK_ENTRY(d->enter_details.contact_name)));
        gtk_label_set_text(GTK_LABEL( d->check_details.contact_m_no_lbl), gtk_entry_get_text(GTK_ENTRY(d->enter_details.contact_number)));
        gtk_label_set_text(GTK_LABEL( d->check_details.contact_email_lbl), gtk_entry_get_text(GTK_ENTRY(d->enter_details.contact_mail)));

        // Remove all the widgets from the lat_box if existed
        gtk_container_foreach(GTK_CONTAINER(d->check_details.check_pass_dets),rem_container_wgts,d->check_details.check_pass_dets);

        // Fill the stack
        fill_check_scr_lst_box(d);
        
        gtk_stack_set_visible_child(GTK_STACK(d->stack),d->check_details.scr);
    }
}

void check_details_continue_clicked(GtkButton* btn, gpointer data)
{
    /* Moves from check details to view ticket */ 
    DATA *d = data;

    /* Changing to load screen */
    gtk_label_set_markup(GTK_LABEL(d->load.title),"<b><span size=\"30000\">Booking your ticket\n\n</span></b>");
    gtk_stack_set_visible_child(GTK_STACK(d->stack),d->load.scr);

    pthread_create(&(d->view_tic.bk_tic_thread),NULL,book_ticket,d);

    // set web uri
    char* file = malloc(1), *cwd = malloc(1);
    getcwd(cwd,-1);
    file = g_strdup_printf("file://%s/rsc/temp.html",cwd);
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(d->view_tic.web_view), file);
}

void view_ticket_ok(GtkButton* btn,gpointer data)
{
    DATA *d = data;
    gtk_stack_set_visible_child(GTK_STACK(d->stack),d->welcome.scr);
}

void download_scr_get_tic_btn_clicked(GtkButton* btn,gpointer data)
{
    DATA *d = data;
    d->tic_dets.tic_no = atoi(gtk_entry_get_text(GTK_ENTRY(d->download_tic.tic_num)));
    d->download_tic.num = gtk_entry_get_text(GTK_ENTRY(d->download_tic.m_num));

    // Change to load scr
    gtk_label_set_markup(GTK_LABEL(d->load.title),"<b><span size=\"30000\">Getting Your Ticket\n\n</span></b>");
    // gtk_stack_set_visible_child(GTK_STACK(d->stack), d->load.scr);
    pthread_create(&(d->download_tic.check_num_thread), NULL, check_num, data);

    // pthread_create(&(d->tic_dets.create_html_thread), NULL, create_html, data);
}


void save_tic(GtkButton* btn,gpointer data)
{
    DATA* d = data;
    char *filename;

    GtkWidget *dig = gtk_file_chooser_dialog_new("SAVE TICKET",GTK_WINDOW(d->win),GTK_FILE_CHOOSER_ACTION_SAVE,"Cancel",
                                      GTK_RESPONSE_CANCEL,
                                      "SAVE",
                                      GTK_RESPONSE_ACCEPT,NULL);
    int res = gtk_dialog_run(GTK_DIALOG(dig));

    if (res == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dig);
        filename = gtk_file_chooser_get_uri(chooser);
        printf("%s\n",filename);
    }
    gtk_widget_destroy(dig);

    // copy file
    #ifdef _WIN32
        asdf
    #else
        system(g_strdup_printf("cp rsc/temp.html %s", &(filename[7])));
    #endif
}

void download_tic(GtkButton* btn,gpointer data)
{
    DATA *d = data;
    gtk_stack_set_transition_type(GTK_STACK(d->stack),GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_visible_child(GTK_STACK(d->stack),d->download_tic.scr);   
}

#endif