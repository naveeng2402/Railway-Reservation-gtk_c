#define ASDF

#include <gtk/gtk.h>

void print_lst(gpointer data, gpointer arg)
{
    char *a = (char *) data;
    printf("%s\t",a);
}

void print_keys(GHashTable* tbl)
{
    GList *lst = g_hash_table_get_keys(tbl);
    g_list_foreach(lst,print_lst,NULL);
}