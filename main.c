#include <gtk/gtk.h>
#include "structures.h"
#include "slots.h"
// #include "debug.h"

/* Function Declarations */
void get_widgets(GtkBuilder*, DATA*);
void get_imgs(GtkBuilder*, GHashTable*);
void populate_tbl(GtkBuilder*, GHashTable*, char*, char*,char*);
void set_img(DATA*);

/* Callback Declarations */


/* Signals */
G_MODULE_EXPORT void size_allocate(GtkWidget*, GtkAllocation*, gpointer);

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    DATA *data = g_slice_new(DATA);

    data->pixbuffs = g_hash_table_new(g_str_hash,NULL); // Initialising the hash table

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_resource("/UI/UI.glade");

    get_widgets(builder,data); // Getting all the required widgets    
    get_imgs(builder,data->pixbuffs); // Getting all the scrolls imgs and pixbuffs and storing it in a hash table(GHashTable) containing doubly linked list(GList)

    gtk_builder_connect_signals(builder,data); // Connecting all the signals
   
    // Free up the memory taken by the builder
    g_object_unref(builder);

    g_signal_connect(G_OBJECT (data->win), "destroy",G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show(data->win);

    gtk_main();

    return 0;
}

void get_widgets(GtkBuilder* b, DATA* d)
{   
    //APP roots
    d->win = GTK_WIDGET(gtk_builder_get_object(b,"win"));
    d->stack = GTK_WIDGET(gtk_builder_get_object(b,"stack"));

    //Widgets for load_scr
    d->load.scr = GTK_WIDGET(gtk_builder_get_object(b,"load_scr"));
    d->load.progress_bar = GTK_WIDGET(gtk_builder_get_object(b,"load_scr_progress_bar"));

}

/* Getting Images and setting their pixbuff and scaling */
void get_imgs(GtkBuilder* b,GHashTable* tbl)
{
    populate_tbl(b,tbl,"load_img_scrl","load_img","train");    
}

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
