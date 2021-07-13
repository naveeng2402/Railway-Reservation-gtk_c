#include "application_functions.h"

/*  ### This function is the one that is started at the time of initial loading, this does the following actions
- Adds dates entires to the DATES table if not already present
- Adds train entries to the TRAIN tabel using the DEFAULT set of tables as template and also generates its seats
*/
void* start_load(void* arg)
{
    DATA *app = arg;
    int is_db_present = check_db_present();

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(app->load.progress_bar),0.1);

    gtk_label_set_markup(GTK_LABEL(app->load.title),"<b><span size=\"30000\"> Welcome to \n Railway Reservation System </span></b>");

    /* Creates a db if it does not exists */
    if(is_db_present == false)
        SQL_create_db();
    
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(app->load.progress_bar),0.3);

    /* Adding required data to db */
    SQL_add_start_data();
    
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(app->load.progress_bar),0.9);
    SLEEP(1);
    
    /* Change the screen */
    gtk_stack_set_visible_child(GTK_STACK(app->stack), app->welcome.scr);

    return 0;
}

/* ## This function is called during the page change from welcome_scr to choose_trains under book tickets, this does the following :
    - Gets all the destinations and adds it to the combobox 
    - Gets dates from now and 2 days after today and adds them to the combobox
*/
void* get_dest_date(void* arg)
{
    enum len{DEST, DATE};

    W_choose_train *scr = arg; 

    SQL_get_dest_date(scr);

    // Removing all the widgets if already exis
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(scr->dest));
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(scr->date));

    // Adding the data to combobox
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scr->date),"Select Date");
    gtk_combo_box_set_active(GTK_COMBO_BOX(scr->date),0);
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scr->dest),"Select Destination");
    gtk_combo_box_set_active(GTK_COMBO_BOX(scr->dest),0);

    for (int i = 0; i < scr->date_dest_len[DATE]; i++) // Dates
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scr->date),scr->date_val[i]);
    }

    for (int i = 0; i < scr->date_dest_len[DEST]; i++)
    {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(scr->dest),scr->dest_val[i]);
    }

    return 0;    
}

void* get_available_trains_lst(void* arg)
{
    SQL_get_available_trains_lst((W_choose_train*)arg);
    return 0;
}

void* get_seats_data(void* arg) 
{
    SQL_get_seats_data((W_choose_seats*)arg);
    return 0;
}

/* Books the ticket and get its data then genratare reports (.pdf and .png from it) */
void* book_and_get_ticket(void* arg)
{
    DATA* app = arg;

    char* ticket_id;
    TICKET_DETAILS* ticket_details;

    ticket_id = SQL_book_ticket(app);
    ticket_details = SQL_get_ticket_data(ticket_id);

    create_report_html(ticket_details);
    generate_reports();
    gtk_image_set_from_file(GTK_IMAGE(app->view_ticket.web_view),"rsc/report.png");
    
    /* Change the screen */
    gtk_stack_set_visible_child(GTK_STACK(app->stack), app->view_ticket.scr);

    /* Removing the reports */
    remove("rsc/report.png");

    return 0;
}

/* ## This fn is called when the get ticket button in download screen is clicked
    - Checks whether the ticket is present
    - if the ticket is present checks that the mobile is correct
    - if both the details are correct the reports are generated and shown else suitable error popups are given */
void* get_tic_thread(void* arg)
{
    W_tic_ops* scr = arg;

    int result = (SQL_get_tic(scr));
    
    if (result == VALID_DATA)
    {
        TICKET_DETAILS* dets = SQL_get_ticket_data((char*)gtk_entry_get_text(GTK_ENTRY(scr->tic_num)));
        create_report_html(dets);
        generate_reports();
        gtk_image_set_from_file(GTK_IMAGE(scr->tic_img), "rsc/report.png");
        remove("rsc/report.png");
        gtk_label_set_markup(GTK_LABEL(scr->msg_lbl), "");
        gtk_revealer_set_reveal_child(GTK_REVEALER(scr->revealer), TRUE);
    }
    else
    {
        const char* msg = (result == INVALID_TICKET_NO || result == ALPHABETS)?"<b>Please check your ticket number</b>":((result == INVALID_MOBILE_NO)?"<b>Please check the mobile number</b>":((result == CANCELLED_TICKET)?"<b>Please Check your Ticket Number, it seems to be Cancelled</b>":((result == OLD_TRAIN)?"<b>Please Check your Ticket Number,\nit seems the train has already departed</b>":"<b>Please enter all the details</b>")));
        gtk_label_set_markup(GTK_LABEL(scr->msg_lbl), msg);
    }
    return 0;
}

/* ## Cancels the ticket */
void* cancel_tic(void* arg)
{
    DATA *app = arg;

    SQL_cancel_tic((char*)gtk_entry_get_text(GTK_ENTRY(app->cancel_tic.tic_num)));

    gtk_stack_set_visible_child(GTK_STACK(app->stack), app->welcome.scr);
    // show_msg_dig("<b>Success</b>", "Ticket successfully cancelled"); /* Breaks the code */
    return 0;
}
