#include "application_functions.h"

/**************************************************************************************************************
                                            UNIVERSAL SLOTS
**************************************************************************************************************/

/* ## Executes when the application closes
    - Waits till all the threads ends */
void quit(AtkWindow* win, gpointer arg)
{
    DATA* app = arg;

    pthread_join(app->threads.start, NULL);
    printf("Joined Thread 1\n");

    pthread_join(app->threads.fill_dest_date, NULL);
    printf("Joined Thread 2\n");

    pthread_join(app->threads.get_train_lst, NULL);
    printf("Joined Thread 3\n");

    pthread_join(app->threads.get_seat_data, NULL);
    printf("Joined Thread 4\n");

    /* Removing all the reports */
    remove("rsc/report.html"); remove("rsc/report.pdf"); remove("rsc/report.png");

    gtk_main_quit();
}

/* ## Used by scrollWidgets to resize images */
void size_allocate(GtkWidget* w, GtkAllocation* allocation, gpointer arg)
{
    /* Getting all the data */
    GHashTable *tbl = ((DATA*)arg)->pixbuffs;
    GList *lst = g_hash_table_lookup(tbl,gtk_widget_get_name(w)); 
    GtkImage *img = (GtkImage*) g_list_nth_data(lst, 1);
    GdkPixbuf *pix = (GdkPixbuf*) g_list_nth_data(lst, 2);
    GdkPixbuf *img_pix = gtk_image_get_pixbuf(img);

    /* Changing the size */
    if(allocation->width != gdk_pixbuf_get_width(img_pix) || allocation->height != gdk_pixbuf_get_height(img_pix))
    {
        gtk_image_set_from_pixbuf(img,gdk_pixbuf_scale_simple(pix,allocation->width,allocation->height,GDK_INTERP_BILINEAR));
    }
}

void size_allocate_flowbox(GtkWidget* w, GtkAllocation* allocation, gpointer arg)
{
    GtkImage *img = GTK_IMAGE(g_list_nth_data(
                                gtk_container_get_children(GTK_CONTAINER(g_list_nth_data(
                                gtk_container_get_children(GTK_CONTAINER(w) // Scrolled Window
                                ),0)) // Viewport
                                ),0));
    GdkPixbuf *img_pix = gtk_image_get_pixbuf(img);
    const GdkPixbuf *pix;

    W_choose_seats *d = arg;

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


/**************************************************************************************************************
                                            WELCOME SCREEN
**************************************************************************************************************/

/*  ## This is an unusual function as it uses GtkDialog.
        - In this function a new builder object is created due to a wierd property of dialog that it works only once if it created as other widgets, to tackle that each time the button is clicked a new dialog is created and is shown then destroyed immediately after it is closed
        - The image in this Dialog box does not support dynamic scaling implemented to others
        - The "Ok" button works because a response id is set in glade file */
void welcome_info(GtkButton* btn, gpointer data)
{
    GtkBuilder *b = gtk_builder_new_from_resource("/UI/UI.glade");
    GtkDialog *dig = GTK_DIALOG(gtk_builder_get_object(b,"Info_dialog"));
    gtk_dialog_run(dig);
    g_object_unref(b);
    gtk_widget_destroy(GTK_WIDGET(dig));
}

/* ## Activated when the Book Ticket button is clicked
    - This function initiate the fill_dest_date thread */
void book_tic(GtkButton* btn, gpointer arg)
{
    DATA *app = arg;
    gtk_stack_set_transition_type(GTK_STACK(app->stack),GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    pthread_create(&(app->threads.fill_dest_date),NULL,get_dest_date,&(app->choose_train));
    gtk_stack_set_visible_child(GTK_STACK(app->stack),app->choose_train.scr);
    gtk_widget_grab_focus(app->choose_train.dest);
}

/**************************************************************************************************************
                                            CHOOSE TRAIN SCREEN
**************************************************************************************************************/
    
/* ##This function is called when the get trains buttton is clicked which is in the choose train page
        - Query the db and get all the available trains
        - Adds those details to the listbox*/
void get_available_trains(GtkButton* btn, gpointer data)
{
    DATA *app = data;

    app->choose_train.selected_date = "", app->choose_train.selected_dest = "";

    app->choose_train.selected_date = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(app->choose_train.date));
    app->choose_train.selected_dest = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(app->choose_train.dest));

    // Get the data from the db
    pthread_create(&(app->threads.get_train_lst),NULL,get_available_trains_lst,&(app->choose_train));
    pthread_join(app->threads.get_train_lst,NULL);
    
    // Remove all the data from the list_box if existed
    gtk_container_foreach(GTK_CONTAINER(app->choose_train.lst_box),rem_container_wgts,app->choose_train.lst_box);

    fill_list_box(&(app->choose_train));

    // All set and show the widgets
    gtk_revealer_set_transition_duration(GTK_REVEALER(app->choose_train.revealer),3000);
    gtk_revealer_set_reveal_child(GTK_REVEALER(app->choose_train.revealer),TRUE);
    app->choose_train.is_revealer_visible = true;
    gtk_widget_show_all(app->choose_train.lst_box);
}

/* This function is called when the combobox active element changed */
void choose_train_combobox_changed(GtkComboBox *cbox, gpointer data)
{
    if (((DATA*)data)->choose_train.is_revealer_visible == true)
        get_available_trains(NULL,data);
}

/*  ## This fn is called when the train is selected in the choose train screen, This function does the following:
    - Gets the data(train id) in the row
    - Using that data gets the seats and stores it in DATA.W_choose_seat.train_data which is char array(string)
    - Fills the flowboxes with appropriate data */
void train_selected(GtkListBox* box, GtkListBoxRow *row, gpointer arg)
{   

    DATA *app = arg;
    char *title_text = "";

    PRAM* c_arg = malloc(sizeof(PRAM));
    c_arg->str = NULL; c_arg->arr = NULL; c_arg->arr_2d = NULL; c_arg->count = 0;

    /* Get individidual labels from list box as glist */
    GList *lst = gtk_container_get_children(
                        GTK_CONTAINER
                        (
                            g_list_nth_data(gtk_container_get_children
                            (
                                GTK_CONTAINER(row) // Row
                            ),0)
                        ) /*Box*/ );



    /* Get data */
    free(app->choose_seats.train_details);
    app->choose_seats.train_details = calloc(6,sizeof(char*)); /* ID, NAME, TIME, AVAIL_SEATS, DATE, DEST */
    c_arg->arr = app->choose_seats.train_details;
    g_list_foreach(lst,get_train_details_glist,c_arg);
    c_arg->arr = NULL; c_arg->count = 0;

    app->choose_seats.train_details[TRAIN_DET_DATE] = calloc(strlen(app->choose_train.selected_date),sizeof(char));
    strcpy(app->choose_seats.train_details[TRAIN_DET_DATE],app->choose_train.selected_date);
    app->choose_seats.train_details[TRAIN_DET_DEST] = calloc(strlen(app->choose_train.selected_dest),sizeof(char));
    strcpy(app->choose_seats.train_details[TRAIN_DET_DEST],app->choose_train.selected_dest);

    pthread_create(&(app->threads.get_seat_data),NULL,get_seats_data,&(app->choose_seats));
    pthread_join(app->threads.get_seat_data, NULL);

    fill_flowboxes(&(app->choose_seats));

    title_text = g_strdup_printf("<b><span size=\"30000\">%s (%s @ %s)</span></b>",app->choose_seats.train_details[TRAIN_DET_NAME],app->choose_seats.train_details[TRAIN_DET_DATE],app->choose_seats.train_details[TRAIN_DET_TIME]);
    gtk_label_set_markup(GTK_LABEL(app->choose_seats.title), title_text);
    gtk_stack_set_visible_child(GTK_STACK(app->stack), app->choose_seats.scr);

    free(c_arg);
}

/**************************************************************************************************************
                                            CHOOSE SEAT SCREEN
**************************************************************************************************************/

/* This function is called when a flowbox is selected; it helps to make booked seats unselectable */
void flowbox_selection_changed(GtkFlowBox* fbox, gpointer arg)
{
    DATA *app = arg;
    gtk_flow_box_selected_foreach(fbox, flowbox_deselect, app);
}

/* ## This fn is called when the continue btn in choose seats is clicked, This function does the following:
    - If no seats are selected it prompts the user
    - Gets the no of seats and stores it in data.enter_details.no_of_pass
    - Using the data the stack is  filled */
void choose_seat_continue_clicked(GtkButton* btn,gpointer pram)
{
    
    DATA *app = pram;

    PRAM* arg = malloc(sizeof(PRAM));
    arg->str = NULL; arg->arr = NULL; arg->arr_2d = NULL; arg->count = 0;

    /* Getting the selected list from all the 4 flowbox and concating them into a single doubly linked list */
    GList *selected_seats = g_list_concat(g_list_concat(g_list_concat(gtk_flow_box_get_selected_children(GTK_FLOW_BOX(app->choose_seats.ac)),gtk_flow_box_get_selected_children(GTK_FLOW_BOX(app->choose_seats.non_ac))), gtk_flow_box_get_selected_children(GTK_FLOW_BOX(app->choose_seats.ac_sleeper))), gtk_flow_box_get_selected_children(GTK_FLOW_BOX(app->choose_seats.non_ac_sleeper)));

    app->enter_details.no_of_pass = g_list_length(selected_seats);

    if (app->enter_details.no_of_pass == 0)
    {
        GtkWidget *dig = GTK_WIDGET(gtk_builder_get_object(gtk_builder_new_from_resource("/UI/UI.glade"),"Message_dig"));
        gtk_message_dialog_format_secondary_markup(GTK_MESSAGE_DIALOG(dig),"Please select atleast 1 seat");
        gtk_dialog_run(GTK_DIALOG(dig));
        gtk_widget_destroy(GTK_WIDGET(dig));
    }
    else
    {   
        // Allocating memory for entry widgets
        app->enter_details.seat_nos  = calloc(app->enter_details.no_of_pass, sizeof(char*));
        app->enter_details.pass_name = calloc(app->enter_details.no_of_pass, sizeof(GtkWidget*));
        app->enter_details.pass_age = calloc(app->enter_details.no_of_pass, sizeof(GtkWidget*));
        app->enter_details.pass_gen = calloc(app->enter_details.no_of_pass, sizeof(GtkWidget*));

        // Gets the selected seat numbers and adds it to a array
        arg->arr = app->enter_details.seat_nos;
        g_list_foreach(selected_seats, get_seat_nums, arg);
        arg->arr = NULL, arg->count = 0;

        // Remove all the widgets from the stack if existed
        gtk_container_foreach(GTK_CONTAINER(app->enter_details.pass_dets),rem_container_wgts,app->enter_details.pass_dets);

        // Fill the stack in enter details scr
        fill_det_stack(&(app->enter_details));

        gtk_entry_set_text(GTK_ENTRY(app->enter_details.contact_name),"");
        gtk_entry_set_text(GTK_ENTRY(app->enter_details.contact_number),"");
        gtk_entry_set_text(GTK_ENTRY(app->enter_details.contact_mail),"");

        gtk_stack_set_visible_child(GTK_STACK(app->stack),app->enter_details.scr);
    }

    free(arg);
}

/**************************************************************************************************************
                                            ENTER DETAILS SCREEN
**************************************************************************************************************/

/* ## This function is called when the continue btn in enter details is clicked, this does the following
    - If all the details are not entered it prompts the user
    - Using the data gathered in the previous screens, ticket details are displayed*/
void enter_details_continue_clicked(GtkButton* btn,gpointer data)
{
    DATA *app = data;
    int no_of_entries = 3+((app->enter_details.no_of_pass)*2), is_empty_index = 2;
    int *is_empty = calloc(no_of_entries,sizeof(int)), is_all_good = true;

    /* Check if all the details are entered */
    is_empty[0] = (strcmp(gtk_entry_get_text(GTK_ENTRY(app->enter_details.contact_name)),"")==0)?true:false;
    is_empty[1] = (strcmp(gtk_entry_get_text(GTK_ENTRY(app->enter_details.contact_number)),"")==0)?true:false;
    is_empty[2] = (strcmp(gtk_entry_get_text(GTK_ENTRY(app->enter_details.contact_mail)),"")==0)?true:false;

    for (int i = 0; i < app->enter_details.no_of_pass; i++)
    {
        is_empty[++is_empty_index] = (strcmp(gtk_entry_get_text(GTK_ENTRY(app->enter_details.pass_name[i])),"")==0)?true:false;
        is_empty[++is_empty_index] = (strcmp(gtk_entry_get_text(GTK_ENTRY(app->enter_details.pass_age[i])),"")==0)?true:false;
    }
    
    for (int i = 0; i < no_of_entries; i++)
    {
        if (is_empty[i]==true)
        {
            is_all_good = false;
            break;
        }
        
    }
    
    /* Doing the work */
    if (is_all_good == false)
    {
        GtkWidget *dig = GTK_WIDGET(gtk_builder_get_object(gtk_builder_new_from_resource("/UI/UI.glade"),"Message_dig"));
        gtk_message_dialog_format_secondary_markup(GTK_MESSAGE_DIALOG(dig),"Please Enter all the details");
        gtk_dialog_run(GTK_DIALOG(dig));
        gtk_widget_destroy(GTK_WIDGET(dig));
    }
    else
    {
        // Filling the contact details
        gtk_label_set_text(GTK_LABEL( app->check_details.contact_name_lbl), gtk_entry_get_text(GTK_ENTRY(app->enter_details.contact_name)));
        gtk_label_set_text(GTK_LABEL( app->check_details.contact_m_no_lbl), gtk_entry_get_text(GTK_ENTRY(app->enter_details.contact_number)));
        gtk_label_set_text(GTK_LABEL( app->check_details.contact_email_lbl), gtk_entry_get_text(GTK_ENTRY(app->enter_details.contact_mail)));

        // Remove all the widgets from the lat_box if existed
        gtk_container_foreach(GTK_CONTAINER(app->check_details.check_pass_dets),rem_container_wgts,app->check_details.check_pass_dets);

        // Fill the stack
        fill_check_scr_lst_box(app);
        
        gtk_stack_set_visible_child(GTK_STACK(app->stack),app->check_details.scr);
    }

    free(is_empty);
}

/**************************************************************************************************************
                                            CHECK DETAILS SCREEN
**************************************************************************************************************/
void check_details_continue_clicked(GtkButton* btn, gpointer data)
{
    /* Moves from check details to view ticket */ 
    DATA *app = data;

    /* Changing to load screen */
    gtk_label_set_markup(GTK_LABEL(app->load.title),"<b><span size=\"25000\">Booking your ticket\n\n</span></b>");
    gtk_stack_set_visible_child(GTK_STACK(app->stack),app->load.scr);

    pthread_create(&(app->threads.book_ticket),NULL,book_and_get_ticket,app);
}

/**************************************************************************************************************
                                            VIEW TICKET SCREEN
**************************************************************************************************************/

/* ## Copy rsc/report.html to desired location */
void save_tic(GtkButton* btn,gpointer data)
{
    DATA* app = data;
    char *filename;

    GtkWidget *dig = gtk_file_chooser_dialog_new("SAVE TICKET",GTK_WINDOW(app->win),GTK_FILE_CHOOSER_ACTION_SAVE,"Cancel",
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

    /* copy file */
    download_html;
}

void view_ticket_ok(GtkButton* btn,gpointer data)
{
    DATA *app = data;
    gtk_stack_set_visible_child(GTK_STACK(app->stack),app->welcome.scr);
    /* Remove html */
    remove("rsc/report.html"); remove("rsc/report.pdf");
    app->choose_train.is_revealer_visible = false;
}

/**************************************************************************************************************
                                            BACK
**************************************************************************************************************/

    /* ## This fn is used to move from choose train scr to welcome scr
        - while moving all the entries from the comboboxes(Dropbox) are deleted to avoid duplicates when again entered 
        - And makes revealer invisible
    */
void back_to_welcome(GtkButton* btn, gpointer data)
{
    DATA *app=data;
    app->choose_train.is_revealer_visible = false;
    gtk_revealer_set_reveal_child(GTK_REVEALER(app->choose_train.revealer), FALSE);
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(app->choose_train.dest));
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(app->choose_train.date));
    gtk_stack_set_visible_child(GTK_STACK(app->stack),app->welcome.scr);
}

/* ## This fn is used to move from choose seat scr to choose train scr
    - While moving it removes all the flowbox entries */
void back_to_choose_train(GtkButton* btn, gpointer data)
{
    DATA *d = data;

    gtk_container_foreach(GTK_CONTAINER(d->choose_seats.ac),rem_container_wgts,d->choose_seats.ac);
    gtk_container_foreach(GTK_CONTAINER(d->choose_seats.non_ac),rem_container_wgts,d->choose_seats.non_ac);
    gtk_container_foreach(GTK_CONTAINER(d->choose_seats.ac_sleeper),rem_container_wgts,d->choose_seats.ac_sleeper);
    gtk_container_foreach(GTK_CONTAINER(d->choose_seats.non_ac_sleeper),rem_container_wgts,d->choose_seats.non_ac_sleeper);
    gtk_stack_set_visible_child(GTK_STACK(d->stack),d->choose_train.scr);
}

/* This fn is used to move to from enter details to choose seats */
void back_to_choose_seat(GtkButton* btn,gpointer data)
{
    DATA *app = data;
    gtk_container_foreach(GTK_CONTAINER(app->enter_details.pass_dets),rem_container_wgts,app->enter_details.pass_dets);
    gtk_stack_set_visible_child(GTK_STACK(app->stack),app->choose_seats.scr);
}

/* This function used to move from check details to enter details */
void back_to_enter_details(GtkButton* btn,gpointer data)
{   
    DATA *app = data;

    // Remove all the widgets from the stack
    gtk_container_foreach(GTK_CONTAINER(app->check_details.check_pass_dets),rem_container_wgts,app->check_details.check_pass_dets);
    gtk_stack_set_visible_child(GTK_STACK(app->stack), app->enter_details.scr);
}
