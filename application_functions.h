#ifndef APPLICATOIN_FUNCTIONS
#define APPLICATOIN_FUNCTIONS

/* Imports */
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

#ifdef _WIN32
    #include <Windows.h>
    #define download_html system(g_strdup_printf("copy rsc\\report.pdf %s /V", &(filename[7])))
#else
    #include <unistd.h>
    #define download_html system(g_strdup_printf("cp rsc/report.pdf %s", &(filename[7])))
#endif

#include "structures.h"

/* Functions */
void get_widgets(GtkBuilder*, DATA*);
void populate_tbl(GtkBuilder*, GHashTable*, char*, char*,char*);
void get_imgs(GtkBuilder*, GHashTable*);
void rem_container_wgts(GtkWidget*, gpointer);
void create_report_html(TICKET_DETAILS*);
void generate_reports();
void show_msg_dig(const char*, const char*);

/* Helper Functions */
int check_db_present();
void fill_list_box(W_choose_train*);
void get_train_details_glist(gpointer, gpointer);
void fill_flowboxes(W_choose_seats*);
void flowbox_deselect(GtkFlowBox*, GtkFlowBoxChild*, gpointer);
void get_seat_nums(gpointer, gpointer);
void fill_det_stack(W_enter_details*);
void fill_check_scr_lst_box(DATA*);

/* Slots */
                // Universal
G_MODULE_EXPORT void quit(AtkWindow*, gpointer);
G_MODULE_EXPORT void size_allocate(GtkWidget*, GtkAllocation*, gpointer);
G_MODULE_EXPORT void size_allocate_flowbox(GtkWidget*, GtkAllocation*, gpointer); 
G_MODULE_EXPORT void save_tic(GtkButton*, gpointer);

                // Welcome
G_MODULE_EXPORT void welcome_info(GtkButton*,gpointer);
G_MODULE_EXPORT void book_tic(GtkButton*,gpointer);

                // Choose Trains
G_MODULE_EXPORT void get_available_trains(GtkButton*,gpointer);
G_MODULE_EXPORT void choose_train_combobox_changed(GtkComboBox*,gpointer);
G_MODULE_EXPORT void train_selected(GtkListBox*, GtkListBoxRow*, gpointer);

                // Choose Seat
G_MODULE_EXPORT void flowbox_selection_changed(GtkFlowBox*, gpointer);
G_MODULE_EXPORT void choose_seat_continue_clicked(GtkButton*,gpointer);

                // Enter details
G_MODULE_EXPORT void enter_details_continue_clicked(GtkButton*,gpointer);

                // Check details
G_MODULE_EXPORT void check_details_continue_clicked(GtkButton*, gpointer);

                // View Details
G_MODULE_EXPORT void view_ticket_ok(GtkButton*, gpointer);

                // Download Ticket
G_MODULE_EXPORT void get_tic(GtkButton*, gpointer);

                // Back btns
G_MODULE_EXPORT void back_to_welcome_bk_tic(GtkButton*,gpointer);
G_MODULE_EXPORT void back_to_welcome_dwnld_scr(GtkButton*,gpointer);
G_MODULE_EXPORT void back_to_choose_train(GtkButton*,gpointer);
G_MODULE_EXPORT void back_to_choose_seat(GtkButton*,gpointer);
G_MODULE_EXPORT void back_to_enter_details(GtkButton*,gpointer);

/* Sqlite Functions */
void add_dates(sqlite3*, START_LOAD*);
void get_def_ids(sqlite3*, PRAM*);
void add_seats(sqlite3*, char*);
void add_trains(sqlite3*, START_LOAD*, char*, int);

void SQL_create_db();
void SQL_add_start_data();
void SQL_get_dest_date(W_choose_train*);
void SQL_get_available_trains_lst(W_choose_train*);
void SQL_get_seats_data(W_choose_seats*);
char* SQL_book_ticket(DATA*);
TICKET_DETAILS* SQL_get_ticket_data(char*);
int SQL_get_tic(W_dwnld_tic* scr);

/* Callback Functions */
int callback_str(void *, int, char **, char **);
int callback_col(void *, int, char **, char **);
int callback_row(void *, int, char **, char **);
int callback_arr_2d(void *, int, char **, char **);

/* Thread Functions */
void* start_load(void*);
void* get_dest_date(void*);
void* get_available_trains_lst(void*);
void* get_seats_data(void*);
void* book_and_get_ticket(void*);
void* get_tic_thread(void* arg);

/* Platform Functions */
void SLEEP(int);



#endif