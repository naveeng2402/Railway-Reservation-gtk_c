#include "application_functions.h"

int main(int argc, char **argv)
{
    GtkBuilder *builder;
    DATA *app = g_slice_new(DATA);

    gtk_init(&argc, &argv);

    /* Making necessary initialisations */
    app->pixbuffs = g_hash_table_new(g_str_hash,NULL);
    app->choose_train.is_revealer_visible = false;
    
    builder = gtk_builder_new_from_resource("/UI/UI.glade");

    get_widgets(builder, app);
    get_imgs(builder,app->pixbuffs);

    gtk_builder_connect_signals(builder, app);

    g_object_unref(builder);

    gtk_widget_show(app->win);

    /* Create and start loading thread */
    pthread_create(&(app->threads.start),NULL,start_load,app);

    gtk_main();

    return 0;
}