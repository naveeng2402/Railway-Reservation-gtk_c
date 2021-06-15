#include <gtk/gtk.h>
#include <pthread.h>
#include "structures.h"
#include "slots.h"
#include "functions.h"

#ifdef _WIN32 // Adding the module for sleep function(Platform dependent) 
    #include <Windows.h>
#else
#include <unistd.h>
#endif

/* Function Declarations */
void get_widgets(GtkBuilder*, DATA*);
void populate_tbl(GtkBuilder*, GHashTable*, char*, char*,char*);
void get_imgs(GtkBuilder*, GHashTable*);

void SLEEP(int);

/* Callback Declarations */


/* Slots */
G_MODULE_EXPORT void size_allocate(GtkWidget*, GtkAllocation*, gpointer);
G_MODULE_EXPORT void welcome_info(GtkButton*,gpointer);

/* Thread Functions */
void* start_load(void*);

int main(int argc, char *argv[])
{
    GtkBuilder *builder;
    DATA *data = g_slice_new(DATA);
    pthread_t start_thread;

    data->pixbuffs = g_hash_table_new(g_str_hash,NULL); // Initialising the hash table

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_resource("/UI/UI.glade");

    get_widgets(builder,data); // Getting all the required widgets    
    get_imgs(builder,data->pixbuffs); // Getting all the scrolls imgs and pixbuffs and storing it in a hash table(GHashTable) containing doubly linked list(GList)

    gtk_builder_connect_signals(builder,data); // Connecting all the signals
   
    // Free up the memory taken by the builder
    g_object_unref(builder);

    g_signal_connect(G_OBJECT (data->win), "destroy",G_CALLBACK(gtk_main_quit), NULL);

    // Creating the thread
    pthread_create(&start_thread,NULL,start_load,data);
    
    gtk_widget_show(data->win);

    gtk_main();

    return 0;
}