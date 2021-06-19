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
    pthread_join(d->choose_train.dest_date_thread, NULL);
    pthread_join(d->choose_train.get_trains_thread,NULL);
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
    gtk_container_foreach(GTK_CONTAINER(d->choose_train.lst_box),rem_lst_wgts,d->choose_train.lst_box);

    // If no trains are available prompt its
    if (d->choose_train.len == 0)
    {
        row = gtk_list_box_row_new();
        gtk_widget_set_size_request(row,100,100);

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
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",d->choose_train.lst_box_content[i][NAME]);
            gtk_label_set_markup(GTK_LABEL(name_lbl),lbl_content);
            
            time_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(time_lbl),TRUE);
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",d->choose_train.lst_box_content[i][TIME]);
            gtk_label_set_markup(GTK_LABEL(time_lbl),lbl_content);

            avail_seats_lbl =  gtk_label_new("");
            gtk_label_set_use_markup(GTK_LABEL(avail_seats_lbl),TRUE);
            lbl_content = g_strdup_printf("<b><span size=\"15000\">%s</span></b>",d->choose_train.lst_box_content[i][AVAIL_SEATS]);
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
}

void train_selected(GtkListBoxRow *row, gpointer data)
{
    printf("Row Clicked\n");
}

#endif