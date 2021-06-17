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

#endif