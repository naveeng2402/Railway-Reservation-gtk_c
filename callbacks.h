#ifndef CALLBACKS
#define CALLBACKS

#include <stdlib.h>

int callback_get_id(void* data, int argc, char **argv, char **azColName) /* Gets a value from one row and one column */
{    
    int *d = data;
    *d = atoi(argv[0]);
    return 0;
}

int callback_get_default_ids(void* data, int argc, char **argv, char **azColName)
{
    START_LOAD *d = data;

    d->def_id[d->count]  = atoi(argv[ID]);
    d->name_id[d->count] = atoi(argv[NAME_ID]);
    d->dest_id[d->count] = atoi(argv[DEST_ID]);
    d->time_id[d->count] = atoi(argv[TIME_ID]);
    
    d->count++;
    return 0;
}

int callback_get_train_ids(void* data, int argc, char **argv, char **azColName)
{
    START_LOAD *d = data;
    d->train_ids[d->count] = atoi(argv[0]);
    d->count++;
    return 0;
}

int callback_get_dest(void *data, int argc, char **argv, char **azColName)
{
    W_choose_train *d = data;
    d->dest_val[d->count] = calloc(strlen(argv[0]), sizeof(char));
    strcpy(d->dest_val[d->count],argv[0]);

    d->count++;
    return 0;
}

int callback_get_date(void *data, int argc, char **argv, char **azColName)
{
    W_choose_train *d = data;
    d->date_val[d->count] = calloc(strlen(argv[0]), sizeof(char));
    strcpy(d->date_val[d->count],argv[0]);

    d->count++;
    return 0;
}

int callback_get_lstbox_content(void *data, int argc, char **argv, char **azColName)
{
    W_choose_train *d = data;

    d->lst_box_content[d->count] = calloc(4, sizeof(char*));

    for (int i = 0; i < argc; i++)
    {
        d->lst_box_content[d->count][i] = calloc(strlen(argv[i]),sizeof(char));
        strcpy(d->lst_box_content[d->count][i],argv[i]);
    }

    d->count++;
    return 0;
}

int callback_get_avail_seats(void *data, int argc, char **argv, char **azColName)
{
    char *d = data;
    strcpy(d,argv[0]);
    return 0;
}

int callback_get_seat_data(void *data, int argc, char **argv, char **azColName)
{
    W_choose_seats *d = data;

    d->seats[d->count] = calloc(argc,sizeof(char*));
    for (int i = 0; i < argc; i++)
    {
        d->seats[d->count][i] = calloc(strlen(argv[i]),sizeof(char));
        strcpy(d->seats[d->count][i],argv[i]);
    }
    
    d->count++;
    return 0;
}

int callback_get_ticket_details(void *data, int argc, char **argv, char **azColName)
{
    HTML *d = data;
    
    d->details = calloc(argc,sizeof(char*));

    for (int i = 0; i < argc; i++)
    {
        d->details[i] = calloc(strlen(argv[i]),sizeof(char));
        strcpy(d->details[i],argv[i]);
    }
    return 0;
}

int callback_get_passenger_details(void *data, int argc, char **argv, char **azColName)
{
    HTML *d = data;
    
    d->passenger_details[d->count] = calloc(argc,sizeof(char*));

    for (int i = 0; i < argc; i++)
    {
        d->passenger_details[d->count][i] = calloc(strlen(argv[i]),sizeof(char));
        strcpy(d->passenger_details[d->count][i], argv[i]);
    }
    d->count++;
    return 0;
}

int callback_get_m_no(void *data, int argc, char **argv, char **azColName)
{
    char** m_no = data;
    m_no[1] = calloc(strlen(argv[0]),sizeof(char));
    strcpy(m_no[1],argv[0]);
    return 0;
}

void rem_container_wgts(GtkWidget *w, gpointer data)
{
    /* Removes the child from a container */
    GtkContainer *c = data;
    gtk_container_remove(c,w);
}

void get_seat_nums(gpointer lst_item, gpointer data)
{   
    W_enter_details* d = data;

    const char *seat_no;
    GtkWidget *lbl; // Label
    GtkWidget *fbox_child = lst_item; // FlowBoxChild

    lbl = GTK_WIDGET(g_list_nth_data(gtk_container_get_children(
                    GTK_CONTAINER(g_list_nth_data(gtk_container_get_children(GTK_CONTAINER(fbox_child)),0)) /*FlowBoxChild*/
                        ),1));
    seat_no = &(gtk_label_get_text(GTK_LABEL(lbl))[5]);

    d->seat_nos[d->count] = calloc(strlen(seat_no),sizeof(char));
    strcpy(d->seat_nos[d->count],seat_no);    

    d->count++;
}

#endif