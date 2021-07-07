#include "application_functions.h"

/* Getting pointors for required widgets */
void get_widgets(GtkBuilder* b, DATA* app)
{   
    //APP roots
    app->win = GTK_WIDGET(gtk_builder_get_object(b,"win"));
    app->stack = GTK_WIDGET(gtk_builder_get_object(b,"stack"));

    //Widgets for load_scr
    app->load.scr = GTK_WIDGET(gtk_builder_get_object(b,"load_scr"));
    app->load.title = GTK_WIDGET(gtk_builder_get_object(b,"load_scr_title_lbl"));
    app->load.progress_bar = GTK_WIDGET(gtk_builder_get_object(b,"load_scr_progress_bar"));

    // Widgets for welcome_scr
    app->welcome.scr = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr"));
    app->welcome.admin = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr_admin_btn"));
    app->welcome.info = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr_info_btn"));
    app->welcome.bk_tic = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr_book_tic_btn"));
    app->welcome.dwnld_tic = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr_dwnld_tic_btn"));
    app->welcome.cncl_tic = GTK_WIDGET(gtk_builder_get_object(b,"welcome_scr_cncl_tic_btn"));

    // Widgets for choose_train
    app->choose_train.scr = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr"));
    app->choose_train.back = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_back_btn"));
    app->choose_train.dest = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_dest"));
    app->choose_train.date = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_date"));
    app->choose_train.get_train = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_get_train"));
    app->choose_train.revealer = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_revealer"));
    app->choose_train.lst_box = GTK_WIDGET(gtk_builder_get_object(b,"choose_train_scr_lst_box"));

    // Widgets for choose_seats
    app->choose_seats.scr = GTK_WIDGET(gtk_builder_get_object(b,"choose_seats_scr"));
    app->choose_seats.back = GTK_WIDGET(gtk_builder_get_object(b,"choose_seat_scr_back_btn"));
    app->choose_seats.cont = GTK_WIDGET(gtk_builder_get_object(b,"choose_seat_scr_continue_btn"));
    app->choose_seats.title = GTK_WIDGET(gtk_builder_get_object(b,"choose_seats_title_lbl"));
    app->choose_seats.ac    = GTK_WIDGET(gtk_builder_get_object(b,"AC"));
    app->choose_seats.non_ac = GTK_WIDGET(gtk_builder_get_object(b,"Non_AC"));
    app->choose_seats.ac_sleeper = GTK_WIDGET(gtk_builder_get_object(b,"AC_Sleeper"));
    app->choose_seats.non_ac_sleeper = GTK_WIDGET(gtk_builder_get_object(b,"Non_AC_Sleeper"));

   // Widgets for enter_details
    app->enter_details.scr = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_scr"));
    app->enter_details.pass_dets = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_passengers_stack"));
    app->enter_details.back = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_scr_back_btn"));
    app->enter_details.book = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_scr_continue_btn"));
    app->enter_details.contact_name = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_contact_name"));
    app->enter_details.contact_number = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_contact_m_no"));
    app->enter_details.contact_mail = GTK_WIDGET(gtk_builder_get_object(b,"enter_details_contact_email"));

    // Widgets for check_details
    app->check_details.scr = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr"));
    app->check_details.check_pass_dets = GTK_WIDGET(gtk_builder_get_object(b,"check_details_passengers_lst_box"));
    app->check_details.back = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr_back_btn"));
    app->check_details.confirm = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr_continue_btn"));
    app->check_details.contact_name_lbl = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr_contact_name_lbl"));
    app->check_details.contact_m_no_lbl = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr_m_no_lbl"));
    app->check_details.contact_email_lbl = GTK_WIDGET(gtk_builder_get_object(b,"check_details_scr_email_lbl"));

    // Widgets for view ticket
    app->view_ticket.scr = GTK_WIDGET(gtk_builder_get_object(b,"view_ticket_scr"));
    app->view_ticket.ok = GTK_WIDGET(gtk_builder_get_object(b,"view_ticket_scr_ok_btn"));
    app->view_ticket.download = GTK_WIDGET(gtk_builder_get_object(b,"view_ticket_scr_dwnld_btn"));
    app->view_ticket.web_view = GTK_WIDGET(gtk_builder_get_object(b,"web_view"));

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
}

/* Removes the child from a container */
void rem_container_wgts(GtkWidget *w, gpointer data)
{
    GtkContainer *c = data;
    gtk_container_remove(c,w);

}

/* ## Creates a html files based on the TICKET DETAILS structures returned by SQL_get_ticket_data()
    - The file path is "rsc/report.html" */
void create_report_html(TICKET_DETAILS* details)
{
    char *html, *temp;

    html = "<html> \
            <head> \
                <style> \
                    table, th, td {border: 0.75mm solid black; text-align: center} \
                </style> \
            </head> \
            <body style=\"background-color: rgb(255, 255, 255);\"> \
                <div id = \"content\" style=\"border: 0.25cm solid black;\"> \
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
                            <tr>";

    /* Adding ticket details */
    html = g_strdup_printf("%s <td style=\"width: 20%%; text-align: center;\">%s</td>",html, details->tic_details[TICKET_NUMBER]);
    for (int i = 1; i < TIC_DET_NCOLS; i++)
    {
        html = g_strdup_printf("%s <td style=\"width: 20%%; text-align: center;\">%s</td>",html, details->tic_details[i]);
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
    html = g_strdup_printf("%s %s",html,temp);

    /* Adding train details */
    for (int i = 0; i < (TIC_TRAIN_NCOLS-1); i++)
    {
        html = g_strdup_printf("%s <td style=\"width: 16.66%%; text-align: center;\">%s</td>",html, details->tic_train_details[i]);
        if (i == TRAIN_NAME) // For inserting from station in the index of 3
            html = g_strdup_printf("%s <td style=\"width: 16.66%%; text-align: center;\">%s</td>",html, details->tic_train_details[FROM_STN]);
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
    html = g_strdup_printf("%s %s",html, temp);

    /* Adding contact details */
    for (int i = 0; i < CONTACT_NCOLS; i++)
    {
        html = g_strdup_printf("%s <td style=\"width: 33.33%%; text-align: center;\">%s</td>", html, details->contact_details[i]);   
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
    html = g_strdup_printf("%s %s",html, temp);

    /* Adding passenger details */
    for (int i = 0; i < atoi(details->tic_details[NO_OF_PASSENGERS]); i++)
    {
        html = g_strdup_printf("%s <tr>", html);
        for (int j = 0; j < PASS_NCOLS; j++)
        {
            html = g_strdup_printf("%s <td style=\"width: 20%%; text-align: center;\">%s</td>",html, details->passenger_details[i][j]);
        }
        html = g_strdup_printf("%s </tr>", html);
    }

    temp = "</tbody> \
            </table> \
            <br> \
            </div> \
            </body> \
            </html>";
    html = g_strdup_printf("%s %s",html, temp);

    /* Writing the html to a file(report.html) */
    FILE *f = fopen("rsc/report.html", "w");
    fprintf(f, "%s", html);
    fclose(f);
}

/* Generates rsc/report.png and rsc/report.pdf from rsc/report.html using python(weasyprint module) */
void generate_reports()
{   
    GBytes *data = g_resources_lookup_data("/scripts/reports.py",0,NULL);
    const char* py = g_bytes_get_data(data,NULL);
    Py_Initialize();
    PyRun_SimpleString(py);
    Py_Finalize();
}

/************************************************************************************************************
                                    HELPER FUNCTIONS
************************************************************************************************************/

/**************** Load ********************/
/* Checks id the database is present */
int check_db_present()
{
    FILE *file;
    if ((file = fopen("rsc/data", "r")))
    {
        fclose(file);
        return true;
    }
    return false;
}

/**************** Choose train ********************/

/* Fills the GtkListBox inside GtkRevealer */
void fill_list_box(W_choose_train* scr)
{
    GtkWidget *train_id_lbl, *name_lbl, *time_lbl, *avail_seats_lbl, *info_lbl; /*Widgets for row*/
    GtkWidget *row, *box; /* Container Widgets */
    const gchar *lbl_content;

    if (atoi(scr->len) == 0)
    {
        row = gtk_list_box_row_new();
        gtk_widget_set_size_request(row,100,100);
        gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(row),FALSE);
        gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(row),FALSE);

        lbl_content = g_strdup_printf("<b><span size=\"30000\">Sorry no trains available to\n%s on %s</span></b>",scr->selected_dest,scr->selected_date);
        info_lbl = gtk_label_new("");

        gtk_label_set_use_markup(GTK_LABEL(info_lbl),TRUE);
        gtk_label_set_markup(GTK_LABEL(info_lbl),lbl_content);
        gtk_label_set_justify(GTK_LABEL(info_lbl),GTK_JUSTIFY_CENTER);

        gtk_container_add(GTK_CONTAINER(row),info_lbl);
        gtk_list_box_insert(GTK_LIST_BOX(scr->lst_box),row,-1);
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
        gtk_list_box_insert(GTK_LIST_BOX(scr->lst_box),row,-1);

        /* Add the train details */
        for (int i = 0; i < atoi(scr->len); i++)
        {
            train_id_lbl = gtk_label_new("");
            gtk_widget_set_margin_start(train_id_lbl,40);
            gtk_label_set_use_markup(GTK_LABEL(train_id_lbl),TRUE);
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",scr->lst_box_content[i][LST_BOX_TRAIN_ID]);
            gtk_label_set_markup(GTK_LABEL(train_id_lbl),lbl_content);

            name_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(name_lbl),TRUE);
            gtk_label_set_ellipsize(GTK_LABEL(name_lbl),PANGO_ELLIPSIZE_END);
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",scr->lst_box_content[i][LST_BOX_NAME]);
            gtk_label_set_markup(GTK_LABEL(name_lbl),lbl_content);
            
            time_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(time_lbl),TRUE);
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",scr->lst_box_content[i][LST_BOX_TIME]);
            gtk_label_set_markup(GTK_LABEL(time_lbl),lbl_content);

            avail_seats_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(avail_seats_lbl),TRUE);
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",scr->seats_available[i]);
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
            gtk_list_box_insert(GTK_LIST_BOX(scr->lst_box),row,-1);
        }
    }

}

/* Get the text from each label in glist and stores it in an array */
void get_train_details_glist(gpointer lst_data, gpointer pram)
{
    PRAM *arg = pram;
    const char *text = gtk_label_get_text(GTK_LABEL(lst_data));

    arg->arr[arg->count] = calloc(strlen(text), sizeof(char));
    strcpy(arg->arr[arg->count], text);

    arg->count++;
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
        /* Setting the variables for iteration */
        is_window = atoi(data->seats[i][SEAT_IS_WINDOW]); is_booked = atoi(data->seats[i][SEAT_IS_BOOKED]);
        seat_status = (is_booked == 1)?"booked":((is_window == 0)?"avail":"win");
        
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

/**************** Choose seat ********************/

/* Deselects a flowbox child if the seat is booked */
void flowbox_deselect(GtkFlowBox* fbox, GtkFlowBoxChild* child, gpointer arg)
{   
    GtkWidget *box = GTK_WIDGET(g_list_nth_data(
                        gtk_container_get_children(GTK_CONTAINER(child)), // FlowBox child
                        0));

    if ((strcmp(gtk_widget_get_name(box),"booked"))==0)
    {
        gtk_flow_box_unselect_child(fbox, child);
    }
    
}

/* Gets the seat number from selected flowbox child */
void get_seat_nums(gpointer lst_item, gpointer pram)
{   
    PRAM* arg = pram;

    const char *seat_no;
    GtkWidget *lbl; // Label
    GtkWidget *fbox_child = lst_item; // FlowBoxChild

    lbl = GTK_WIDGET(g_list_nth_data(gtk_container_get_children(
                    GTK_CONTAINER(g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(fbox_child)),0)) /*FlowBoxChild*/
                        ),1));
    seat_no = &(gtk_label_get_text(GTK_LABEL(lbl))[5]);

    arg->arr[arg->count] = calloc(strlen(seat_no),sizeof(char));
    strcpy(arg->arr[arg->count],seat_no);    

    arg->count++;
}

/* Fills the stack inside the enter details scr */
void fill_det_stack(W_enter_details *scr)
{
    GtkWidget *name_lbl, *age_lbl, *gen_lbl; /* Lables */
    GtkWidget *grid; /* Grid_layout */
    char *stack_child = malloc(1); /* name and title of the stack child */

    for (int i = 0; i < scr->no_of_pass; i++)
    {
        stack_child = g_strdup_printf("SEAT %s",scr->seat_nos[i]);

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

        scr->pass_name[i] = gtk_entry_new();
        gtk_widget_set_halign(scr->pass_name[i], GTK_ALIGN_START);

        scr->pass_age[i] = gtk_entry_new();
        gtk_widget_set_halign(scr->pass_age[i], GTK_ALIGN_START);

        scr->pass_gen[i] = gtk_combo_box_text_new();
        gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(scr->pass_gen[i]),0,"Male");
        gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(scr->pass_gen[i]),1,"Female");
        gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(scr->pass_gen[i]),2,"Transgender");
        gtk_combo_box_set_button_sensitivity(GTK_COMBO_BOX(scr->pass_gen[i]), GTK_SENSITIVITY_AUTO);
        gtk_combo_box_set_active(GTK_COMBO_BOX(scr->pass_gen[i]), 0);
        gtk_widget_set_halign(scr->pass_gen[i], GTK_ALIGN_START);

        grid = gtk_grid_new();
        gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
        gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
        gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
        gtk_grid_set_column_spacing(GTK_GRID(grid), 20);

        /* Adding the widgets to the grid */
        gtk_grid_attach(GTK_GRID(grid),name_lbl,0,0,1,1);
        gtk_grid_attach(GTK_GRID(grid),age_lbl,0,1,1,1);
        gtk_grid_attach(GTK_GRID(grid),gen_lbl,0,2,1,1);
        gtk_grid_attach(GTK_GRID(grid),scr->pass_name[i],1,0,1,1);
        gtk_grid_attach(GTK_GRID(grid),scr->pass_age[i],1,1,1,1);
        gtk_grid_attach(GTK_GRID(grid),scr->pass_gen[i],1,2,1,1);

        /* Add grid to stack */
        gtk_stack_add_titled(GTK_STACK(scr->pass_dets),grid,stack_child,stack_child);
    }

    gtk_widget_show_all(scr->pass_dets);

    free(stack_child);
}

/**************** Enter details ********************/

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

/************************************************************************************************************
                                    PLATFORM FUNCTIONS
************************************************************************************************************/
void SLEEP(int sec)
{
    #ifdef _WIN32
        Sleep(sec*1000);
    #else
        sleep(sec);
    #endif
}