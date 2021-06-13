#ifndef FUNCTIONS
#define FUNCTIONS

#include <gtk/gtk.h>
#include "structures.h"
#include "slots.h"

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

}

/* Make the system to sleep */
void SLEEP(int s)
{
    #ifdef _WIN32
    Sleep(s*1000);
    #else
    sleep(s);
    #endif
}

/*************************************************************************************
                            Thread Functions
***************************************************************************************/

void* start_load(void* arg)
{
    DATA *d = (DATA*) arg; // Typecasting the argument

    // printf("\nthread start\n");

    gtk_label_set_markup(GTK_LABEL(d->load.title),"<b><span size=\"30000\"> Welcome to \n Railway Reservation System </span></b>");

    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(d->load.progress_bar),0);
    SLEEP(1);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(d->load.progress_bar),0.5);
    SLEEP(1);
    gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(d->load.progress_bar),1);

    // Change the screen
    gtk_stack_set_visible_child(GTK_STACK(d->stack), d->welcome.scr);
}


#endif