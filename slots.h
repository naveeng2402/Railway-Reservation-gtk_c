#ifndef SLOTS
#define SLOTS

#include <gtk/gtk.h>
#include "structures.h"

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

#endif