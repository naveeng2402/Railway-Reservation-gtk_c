#include <gtk/gtk.h>
#include <pthread.h>
#include "structures.h"
#include "slots.h"
#include "functions.h"
#include "callbacks.h"

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

/* Sqlite declatations */
void add_dates(sqlite3* ,int*, int*);
void get_len(sqlite3*, START_LOAD*);
void get_ids(sqlite3*, START_LOAD*);
void add_seats(sqlite3*, int);
void add_trains(sqlite3*, START_LOAD*, int, int);
void update_is_train_full(sqlite3*, START_LOAD*);

/* Callback Declarations */
int callback_get_id(void *, int, char **, char **);
int callback_get_default_ids(void *, int, char **, char **);
int callback_get_train_ids(void *, int, char **, char **);
int callback_get_dest(void *, int, char **, char **);
int callback_get_date(void *, int, char **, char **);
int callback_get_lstbox_content(void *, int, char **, char **);
int callback_get_avail_seats(void *, int, char **, char **);

void rem_lst_wgts(GtkWidget*, gpointer);

/* Slots */
G_MODULE_EXPORT void quit(AtkWindow*, gpointer);
G_MODULE_EXPORT void size_allocate(GtkWidget*, GtkAllocation*, gpointer);
G_MODULE_EXPORT void welcome_info(GtkButton*,gpointer);
G_MODULE_EXPORT void book_tic(GtkButton*,gpointer);
G_MODULE_EXPORT void back_to_welcome(GtkButton*,gpointer);
G_MODULE_EXPORT void get_available_trains(GtkButton*,gpointer);
G_MODULE_IMPORT void train_selected(GtkListBoxRow*, gpointer);

/* Thread Functions */
void* start_load(void*);
void* get_dest_date(void*);
void* get_list_content(void*);

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

    gtk_widget_show(data->win);

    // Creating the thread
    pthread_create(&(data->start_thread),NULL,start_load,data);
    
    gtk_main();

    return 0;
}
