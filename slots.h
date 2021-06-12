#ifndef slots
#define slots

#include <gtk/gtk.h>
#include "structures.h"
#include "debug.h"

void size_allocate(GtkWidget* w, GtkAllocation* allocation, gpointer data)
{
    // Getting all the data
    GHashTable *tbl = ((DATA*)data)->pixbuffs;
    GList *lst = g_hash_table_lookup(tbl,gtk_widget_get_name(w)); 
    GtkImage *img = (GtkImage*) g_list_nth_data(lst, 1);
    GdkPixbuf *pix = (GdkPixbuf*) g_list_nth_data(lst, 2);
    GdkPixbuf *img_pix = gtk_image_get_pixbuf(img);

    // Changing the size
    if(allocation->width != gdk_pixbuf_get_width(img_pix) || allocation->height != gdk_pixbuf_get_height(img_pix))
    {
        gtk_image_set_from_pixbuf(img,gdk_pixbuf_scale_simple(pix,allocation->width,allocation->height,GDK_INTERP_BILINEAR));
    }
}

#endif