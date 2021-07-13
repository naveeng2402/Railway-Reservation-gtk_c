#include "application_functions.h"

/*************************************************************************************************
                                    SQLITE CALLBACK FUNCTIONS
*************************************************************************************************/
int callback_str(void *arg, int argc, char **argv, char **azColName)
{
    PRAM *data = arg;
    // data->str = realloc(data->str, strlen(argv[0])*sizeof(char));
    strcpy(data->str,argv[0]);
    return 0;
}

/* For single column multi_rows */
int callback_col(void *arg, int argc, char **argv, char **azColName)
{
    PRAM *data = arg;
    
    for (int i = 0; i < argc; i++)
    {
        data->arr[data->count] = calloc(strlen(argv[i]), sizeof(char));
        strcpy(data->arr[data->count], argv[i]);
    }

    data->count++;
    return 0;    
}

/* For single row multi_columns */
int callback_row(void *arg, int argc, char **argv, char **azColName)
{
    PRAM* data = arg;
    for (int i = 0; i < argc; i++)
    {
        data->arr[data->count] = calloc(strlen(argv[i]), sizeof(char));
        strcpy(data->arr[data->count], argv[i]);
        data->count++;
    }

    return 0;
}

int callback_arr_2d(void *arg, int argc, char **argv, char **azColName)
{   
    PRAM *data = arg;

    data->arr_2d[data->count] = calloc(argc, sizeof(char*));
    for (int i = 0; i < argc; i++)
    {
        data->arr_2d[data->count][i] = calloc(strlen(argv[i]), sizeof(char));
        strcpy(data->arr_2d[data->count][i], argv[i]);
    }
    
    data->count++;
    return 0;
}

/*************************************************************************************************
                                    SQLITE HELPER FUNCTIONS
*************************************************************************************************/

/* Gets the dates for 3 days and add it to db */
void add_dates(sqlite3* db, START_LOAD* data)
{
    /* Local variables */
    time_t *tme = malloc(sizeof(time_t)), *unix_time=calloc(3,sizeof(time_t));
    struct tm *tm;
    char *today = malloc(1);
    char **dates= calloc(3,sizeof(char*));
    char *sql = malloc(1);

    PRAM *arg = malloc(sizeof(PRAM));
    arg->str = NULL; arg->arr = NULL; arg->arr_2d = NULL;

    /* Variables used in SQL_add_start_data */
    data->dates_id = calloc(3,sizeof(char*));
    data->is_dates_already_in_db = calloc(3,sizeof(char*));

    /* Getting Current time */
    time(tme); // time() function returns the value of time in seconds since 0 hours, 0 minutes, 0 seconds, January 1, 1970
    tm = localtime(tme); /* Converts unix time into human readable time */
    strftime(today,-1,"%d-%m-%Y",tm);

    /* Getting dates for 3 days from now */
    for (int i = 0; i < 3; i++)
    {
        dates[i] = malloc(1);
        tm->tm_mday += i==0?0:1;
        tm->tm_hour = 23; tm->tm_min=59; tm->tm_sec=59;
        *tme=mktime(tm);  /* Does the dates arithmetic */
        tm=localtime(tme);
        strftime(dates[i],-1,"%d-%m-%Y",tm);
        unix_time[i]=*tme;
    }

    /*Checking whether the date already exists*/
    for (int i = 0; i < 3; i++)
    {
        data->is_dates_already_in_db[i]  = malloc(sizeof(char));
        arg->str = data->is_dates_already_in_db[i];
        sql = g_strdup_printf("SELECT count(*) FROM DATES WHERE dates_val=\"%s\"",dates[i]);
        sqlite3_exec(db,sql,callback_str,arg,NULL);
    }
    arg->str = NULL;

    /* Inserting dates */
    for (int i = 0; i < 3; i++)
    {
        sql = g_strdup_printf("INSERT OR IGNORE INTO DATES (\"dates_val\",\"unix_time\") VALUES (\"%s\",%ld);", dates[i],unix_time[i]);
        sqlite3_exec(db, sql, NULL, NULL, NULL);        
    }

    /* Gets dates_id */
    for (int i = 0; i < 3; i++)
    {
        data->dates_id[i]  = malloc(sizeof(char));
        arg->str = data->dates_id[i];
        sql = g_strdup_printf("SELECT id FROM DATES WHERE dates_val=\"%s\"",dates[i]);
        sqlite3_exec(db,sql,callback_str, arg,NULL);
    }

    free(tme); free(unix_time); free(today); free(dates); free(sql); free(arg);
}

/* Get ids associated with default trains that will be used to insert data for trains  */
void get_def_ids(sqlite3* db, PRAM* arg)
{
    arg->count = 0;
    char *sql = "SELECT DEFAULT_TRAINS.id, Def_Train_Name.name_id, Def_Train_Dest.dest_id, Def_Train_Time.time_id \
            FROM DEFAULT_TRAINS JOIN Def_Train_Dest ON DEFAULT_TRAINS.id=Def_Train_Dest.train_id JOIN DEST ON DEST.id = Def_Train_Dest.dest_id \
            JOIN Def_Train_Name ON Def_Train_Name.train_id=DEFAULT_TRAINS.id JOIN NAME ON Def_Train_Name.name_id=NAME.id \
            JOIN Def_Train_Time ON Def_Train_Time.train_id=DEFAULT_TRAINS.id JOIN TIMES ON Def_Train_Time.time_id=TIMES.id";
    sqlite3_exec(db, sql, callback_arr_2d, arg, NULL);
    arg->arr_2d = NULL; arg->count = 0;
}

/* Adds the seats to for each train */
void add_seats(sqlite3* db, char* train_id)
{
    int is_window = 0, class_id = 1, offset = 0;
    char *sql = malloc(1);

    for (int i = 0; i < 4; i++) // Class (4)
    {
        for (int j = 1; j <= 25; j++) // 25 Seats per class
        {
            is_window = (j%5==0 || j%5==1)?1:0; //Conditional Operator

            sql = g_strdup_printf(
                     "INSERT INTO SEAT(\"seat_no\", \"is_window\", \"class_id\", \"train_id\") VALUES(%d,%d,%d,%s);", offset+j,is_window,class_id,train_id
                    );
            sqlite3_exec(db,sql,NULL,NULL,NULL);
        }
        offset += 25;
        class_id ++;
    }

    free(sql);
}

/* Adds trains to the db */
void add_trains(sqlite3* db, START_LOAD* data, char* dates_id, int def_train) /* def_trains is used as a index */
{
    char *sql = malloc(1), *err = malloc(1);
    PRAM *arg = malloc(sizeof(PRAM));

    sql = "INSERT INTO TRAIN (\"is_train_full\") VALUES(0)";
    sqlite3_exec(db,sql,NULL,NULL,&err);
    printf("error : %s \n", err);

    sql = "SELECT seq FROM sqlite_sequence WHERE name=\"TRAIN\"";
    data->train_id = malloc(sizeof(char));
    arg->str = data->train_id;
    sqlite3_exec(db,sql,callback_str,arg,NULL);
    arg->str = NULL;

    sql = g_strdup_printf(
            "INSERT INTO Train_Dates(\"train_id\", \"dates_id\") VALUES(%s, %s);\
            INSERT INTO Train_Dest (\"train_id\", \"dest_id\") VALUES(%s, %s); \
            INSERT INTO Train_Name (\"train_id\", \"name_id\") VALUES(%s, %s); \
            INSERT INTO Train_Time (\"train_id\", \"time_id\") VALUES(%s, %s);",
            data->train_id, dates_id,
            data->train_id, data->default_train_details[def_train][DEF_DEST_ID],
            data->train_id, data->default_train_details[def_train][DEF_NAME_ID],
            data->train_id, data->default_train_details[def_train][DEF_TIME_ID]
            );
    sqlite3_exec(db,sql,NULL,NULL,NULL);

    add_seats(db,data->train_id);

    free(sql); free(arg);
}

/*************************************************************************************************
                                    SQLITE CONSOLIDATED FUNCTIONS
*************************************************************************************************/

/* Creates a new sqlite3 database for the application */
void SQL_create_db()
{
    /* Getting sql file from resource */
    GBytes *data;
    char* err = malloc(1);
    const char* sql;

    data = g_resources_lookup_data("/scripts/sql.sql",0,NULL);
    sql=g_bytes_get_data(data, NULL);

    /* Creating the directory */
    system("mkdir rsc");

    /* Creatig the db */
    sqlite3* db;
    sqlite3_open("rsc/data",&db);
    sqlite3_exec(db, sql, NULL, NULL, &err);
    sqlite3_close(db);
    free(err);
}

/* Adds data to the db when the application starts */
void SQL_add_start_data()
{

    sqlite3* db;
    START_LOAD *data = malloc(sizeof(START_LOAD));
    PRAM *arg = malloc(sizeof(PRAM));

    data->len = malloc(sizeof(char)); // Allcocating memory to len as it defines the lenght of others

    /* Get the length of default trains */
    sqlite3_open("rsc/data",&db);
    arg->str = data->len;
    const char *sql = "\
        SELECT count(*) \
        FROM DEFAULT_TRAINS JOIN Def_Train_Dest ON DEFAULT_TRAINS.id=Def_Train_Dest.train_id JOIN DEST ON DEST.id = Def_Train_Dest.dest_id \
        JOIN Def_Train_Name ON Def_Train_Name.train_id=DEFAULT_TRAINS.id JOIN NAME ON Def_Train_Name.name_id=NAME.id \
        JOIN Def_Train_Time ON Def_Train_Time.train_id=DEFAULT_TRAINS.id JOIN TIMES ON Def_Train_Time.time_id=TIMES.id";
    sqlite3_exec(db, sql, callback_str, arg, NULL);
    arg->str = NULL;

    add_dates(db, data);

    data->default_train_details = calloc(atoi(data->len), sizeof(char**));
    arg->arr_2d = data->default_train_details;
    get_def_ids(db, arg);

    /* Add trains for eaach day with using default trains as template */
    for (int i = 0; i < 3; i++) /* DATES */
    {   
        /* Checks if the dates already exists */
        if (atoi(data->is_dates_already_in_db[i]) != 0)
            continue;
        
        for (int j = 0; j < atoi(data->len) ; j++) /* No of default trains */
        {
           add_trains(db, data, data->dates_id[i],j);
        }        
    }

    sqlite3_close(db);
    free(data); free(arg);
}

/* Gets all the destinations and 2 dates from today  */
void SQL_get_dest_date(W_choose_train* scr)
{
    sqlite3* db;
    char *date_len=malloc(sizeof(char)), *dest_len=malloc(sizeof(char)), *sql="";
    
    PRAM* arg = malloc(sizeof(PRAM));
    arg->str = NULL; arg->arr = NULL; arg->arr_2d = NULL; arg->count = 0;

    time_t *now=malloc(sizeof(time_t));
    time(now);

    sqlite3_open("rsc/data", &db);

    // Getting the lengths of date and dest
    arg->str = date_len;
    sql=g_strdup_printf("SELECT count(*) FROM DATES WHERE unix_time>%ld",*now);
    sqlite3_exec(db,sql,callback_str,arg,NULL);
    arg->str = dest_len;
    sqlite3_exec(db,"SELECT count(*) FROM DEST",callback_str,arg,NULL);
    arg->str = NULL;

    // Allocation of memory
    scr->date_val = calloc(atoi(date_len),sizeof(char*));
    scr->dest_val = calloc(atoi(dest_len),sizeof(char*));

    // Getting the values for dates and destinations
    arg->count = 0;
    arg->arr = scr->dest_val;
    sqlite3_exec(db, "SELECT DISTINCT destination FROM DEST",callback_col,arg,NULL);
    arg->arr = NULL;

    arg->count=0;
    arg->arr = scr->date_val;
    sql = g_strdup_printf("SELECT DISTINCT dates_val FROM DATES WHERE unix_time>%ld",*now);
    sqlite3_exec(db,sql,callback_col,arg,NULL);
    arg->arr = NULL;
    
    scr->date_dest_len[0] = atoi(dest_len); scr->date_dest_len[1] = atoi(date_len);
    
    sqlite3_close(db);
    free(arg); free(now); free(date_len); free(dest_len);
}

/* ## This function is called when the get trains buttton is clicked which is in the choose train page, this does the following :
    - Gets the data from the sql which is used to fill the list_box in the choose train page
    - The details are Train no, Time, Name, No of available seats
    - The data is stored in a 2d array DATA.W_choose_train.lst_box_content */
void SQL_get_available_trains_lst(W_choose_train* scr)
{

    sqlite3 *db;

    char *sql="";

    PRAM* arg = malloc(sizeof(PRAM));
    arg->str = NULL; arg->arr = NULL; arg->arr_2d = NULL; arg->count = 0;

    sqlite3_open("rsc/data", &db);

    /* Get number of trains */
    scr->len = malloc(sizeof(char));
    arg->str = scr->len;
    sql = g_strdup_printf("SELECT count(DISTINCT t.id) FROM \
                            SEAT AS s JOIN static_SEAT_CLASS AS sc ON s.class_id=sc.id \
                            JOIN TRAIN AS t ON t.id=s.train_id \
                            JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id \
                            JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id \
                            JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id \
                            JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id \
                            WHERE des.destination=\"%s\" AND dt.dates_val=\"%s\" AND s.is_booked=0",
                            scr->selected_dest,scr->selected_date);
    sqlite3_exec(db, sql, callback_str,arg,NULL);
    arg->str = NULL;

    if (atoi(scr->len)==0)     
    {
        ;
    }
    else
    {
        /* Getting the train details */
        scr->lst_box_content = calloc(atoi(scr->len),sizeof(char**));
        arg->arr_2d = scr->lst_box_content;
        sql = g_strdup_printf("SELECT DISTINCT t.id, ti.time, n.train_names FROM \
                            SEAT AS s JOIN static_SEAT_CLASS AS sc ON s.class_id=sc.id \
                            JOIN TRAIN AS t ON t.id=s.train_id \
                            JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id \
                            JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id \
                            JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id \
                            JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id \
                            WHERE des.destination=\"%s\" AND dt.dates_val=\"%s\" AND s.is_booked=0",
                            scr->selected_dest,scr->selected_date);
        sqlite3_exec(db,sql,callback_arr_2d,arg,NULL);
        arg->arr_2d = NULL; arg->count = 0;
        
        /* Get the number of available seats */
        for (int i = 0; i < atoi(scr->len); i++)
        {
            arg->count = i;
            sql = g_strdup_printf("SELECT count(*) FROM \
                            SEAT WHERE is_booked=0 and train_id=\"%s\"",scr->lst_box_content[i][LST_BOX_TRAIN_ID]);
            scr->seats_available = calloc(atoi(scr->len), sizeof(char*)); /*seat is a 3 digit number*/
            arg->arr = scr->seats_available;
            sqlite3_exec(db,sql,callback_col,arg,NULL);
        }
        
    }

    sqlite3_close(db);
    free(arg);
}

/* ## Gets the seats data and store it in a 3d array (seats) */
void SQL_get_seats_data(W_choose_seats* scr)
{
    sqlite3 *db;

    char* sql="";

    PRAM* arg = malloc(sizeof(PRAM));
    arg->str = NULL; arg->arr = NULL; arg->arr_2d = NULL; arg->count = 0;

    sqlite3_open("rsc/data", &db);

    /* Get seats data */
    free(scr->seats);
    scr->seats = calloc(100, sizeof(char**)); // 100 seats
    arg->arr_2d = scr->seats;
    sql = g_strdup_printf("SELECT s.seat_no, sc.class, s.is_booked, s.is_window FROM \
                        SEAT AS s JOIN static_SEAT_CLASS AS sc ON s.class_id=sc.id \
                        JOIN TRAIN AS t ON t.id=s.train_id \
                        JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id /*joining train dates*/ \
                        JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id /*joining train name*/ \
                        JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id /*joining train destination*/ \
                        JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id /*joining train time*/ \
                        WHERE t.id = %s",scr->train_details[TRAIN_DET_TRAIN_ID]);
    sqlite3_exec(db,sql,callback_arr_2d,arg,NULL);

    sqlite3_close(db);
    free(arg);
}

/* ## Gets the data from db and previous screens and use them to make necessary changes to db and books the ticket
    - Return the ticket_id of the booked ticket */
char* SQL_book_ticket(DATA* app)
{
    sqlite3 *db;
    char* sql = malloc(1), *err = malloc(1);

    PRAM* arg = malloc(sizeof(PRAM));
    arg->str = NULL; arg->arr = NULL; arg->arr_2d = NULL; arg->count = 0;

    /* Values for ticket table */
    time_t *now = malloc(sizeof(time_t));
    struct tm *tm;
    char *bk_date_id = malloc(1),  *bk_time=malloc(1), *todate=malloc(1);
    const char *cntct_name, *m_no, *email, *train_id;    

    /* Values for passenger table */
    char *seat_id = malloc(1), *ticket_id = malloc(1);
    const char *name, *age, *gender; 
    int gender_id;

    time(now);
    tm = localtime(now);
    strftime(bk_time,-1,"%H.%M",tm); // Getting current time 
    strftime(todate,-1,"%d-%m-%Y",tm); // Grtting current date

    cntct_name = gtk_entry_get_text(GTK_ENTRY(app->enter_details.contact_name));
    m_no = gtk_entry_get_text(GTK_ENTRY(app->enter_details.contact_number));
    email = gtk_entry_get_text(GTK_ENTRY(app->enter_details.contact_mail));
    train_id = app->choose_seats.train_details[TRAIN_DET_TRAIN_ID];

    sqlite3_open("rsc/data", &db);

    /* Get bk_date_id */
    arg->str = bk_date_id;
    sql = g_strdup_printf("SELECT id from DATES WHERE dates_val = \"%s\"", todate);
    sqlite3_exec(db, sql, callback_str, arg, &err);
    arg->str = NULL;

    /* Adding data to ticket table */
    sql = g_strdup_printf(
            "INSERT INTO TICKET \
            (\"no_of_passengers\", \"contact_name\", \"mobile_no\", \"email_id\", \"time_of_bk\", \"date_of_bk\", \"ticket_train_id\") \
            VALUES (%d, \"%s\", \"%s\", \"%s\", %s, %s, %s)",
            app->enter_details.no_of_pass, cntct_name,m_no,email,bk_time,bk_date_id,train_id);
    sqlite3_exec(db,sql,NULL,NULL,&err);

    /* Get ticket id */
    arg->str = ticket_id;
    sqlite3_exec(db, "SELECT seq FROM sqlite_sequence WHERE name = \"TICKET\"",callback_str,arg,&err);
    arg->str = NULL;

    /* Adding data to passenger table */
    for (int i = 0; i < app->enter_details.no_of_pass; i++)
    {   
        arg->str = seat_id;
        sql = g_strdup_printf("SELECT id FROM SEAT WHERE train_id=%s AND seat_no=%s",train_id,app->enter_details.seat_nos[i]);
        sqlite3_exec(db,sql,callback_str, arg,&err);

        sql = g_strdup_printf("UPDATE SEAT set is_booked = 1 WHERE id=%s",seat_id);
        sqlite3_exec(db,sql,NULL,NULL,NULL);

        gender = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(app->enter_details.pass_gen[i]));

        gender_id = (strcmp(gender,"Male")==0)?MALE:((strcmp(gender,"Female")==0)?FEMALE:TRANSGENDER);
        name = gtk_entry_get_text(GTK_ENTRY(app->enter_details.pass_name[i]));
        age = gtk_entry_get_text(GTK_ENTRY(app->enter_details.pass_age[i]));

        sql = g_strdup_printf("INSERT INTO PASSENGERS (\"passenger_name\",\"age\",\"gender_id\",\"passenger_seat\",\"ticket_id\") VALUES (\"%s\",\"%s\",%d,%s,%s)",name,age,gender_id,seat_id,ticket_id);
        sqlite3_exec(db,sql,NULL,NULL,&err);
    }
    arg->str = NULL;

    free(arg); free(sql);
    free(now); free(bk_time); free(bk_date_id); free(todate);
    free(seat_id);

    return ticket_id;
}

/* ## Gets the ticket id and gets all the data about it to generate ticket
    - Returns a struct TICKET_DETAILS containing all necessay data
    - This data is used by report generation functions -> create_report_txt() and create_report_html */
TICKET_DETAILS* SQL_get_ticket_data(char* ticket_id)
{
    TICKET_DETAILS* ticket_details = malloc(sizeof(TICKET_DETAILS));
    char* from_stn = "Chennai Central"; 

    sqlite3* db;
    char* sql = malloc(1);
    
    PRAM* arg = malloc(sizeof(PRAM));
    arg->str = NULL; arg->arr = NULL; arg->arr_2d = NULL; arg->count = 0;

    sqlite3_open("rsc/data", &db);

    /* Getting ticket details */
    ticket_details->tic_details = calloc(TIC_DET_NCOLS, sizeof(char*));
        // Adding ticket id
        ticket_details->tic_details[0] = calloc(strlen(ticket_id), sizeof(char));
        strcpy(ticket_details->tic_details[0], ticket_id);
    arg->arr = &(ticket_details->tic_details[1]);
    arg->count = 0;
    sql = g_strdup_printf("\
            SELECT tic.no_of_passengers, tmp.dates_val, tic.time_of_bk, des.price*tic.no_of_passengers \
            FROM TICKET AS tic JOIN TRAIN AS t ON t.id   = tic.ticket_train_id \
            JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id \
            JOIN DATES AS tmp ON tmp.id = tic.date_of_bk \
            WHERE tic.ticket_number= %s", ticket_id);
    sqlite3_exec(db, sql, callback_row, arg, NULL);
    arg->arr = NULL; arg->count = 0;

    /* Getting train details */
    ticket_details->tic_train_details = calloc(TIC_TRAIN_NCOLS, sizeof(char*));
    arg->arr = ticket_details->tic_train_details;
    sql = g_strdup_printf("\
            SELECT t.id, n.train_names, des.destination, dt.dates_val, ti.time \
            FROM TICKET AS tic JOIN TRAIN AS t ON t.id   = tic.ticket_train_id \
            JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id \
            JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id \
            JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id \
            JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id \
            WHERE tic.ticket_number= %s", ticket_id);
    sqlite3_exec(db, sql, callback_row, arg, NULL);
    arg->arr = NULL; arg->count = 0;
        // Adding from station
        ticket_details->tic_train_details[FROM_STN] = calloc(strlen(from_stn), sizeof(char));
        strcpy(ticket_details->tic_train_details[FROM_STN], from_stn);

    /* Getting contact details */
    ticket_details->contact_details = calloc(CONTACT_NCOLS, sizeof(char*));
    arg->arr = ticket_details->contact_details;
    sql = g_strdup_printf("SELECT contact_name, mobile_no, email_id \
                            FROM TICKET WHERE ticket_number = %s", ticket_id);
    sqlite3_exec(db, sql, callback_row, arg, NULL);
    arg->arr = NULL; arg->count = 0;

    /* Getting passenger details */
    ticket_details->passenger_details = calloc(atoi(ticket_details->tic_details[NO_OF_PASSENGERS]), sizeof(char**));
    arg->arr_2d = ticket_details->passenger_details;
    sql = g_strdup_printf("\
            SELECT s.seat_no, p.passenger_name, p.age, g.gender, c.class \
            FROM PASSENGERS as p JOIN static_GENDER as g ON p.gender_id=g.id \
            JOIN SEAT as s ON s.id=p.passenger_seat \
            JOIN static_SEAT_CLASS as c ON c.id=s.class_id \
            WHERE p.ticket_id= %s", ticket_id);
    sqlite3_exec(db, sql, callback_arr_2d, arg, NULL);
    arg->arr_2d = NULL; arg->count = 0;

    sqlite3_close(db);
    free(sql); free(arg);

    return ticket_details;
}

/* ## Gets the details of the ticket using ticket number and return appropriate errors */
int SQL_get_tic(W_tic_ops* scr)
{
    int ret_val;

    PRAM* arg = malloc(sizeof(PRAM));
    arg->str = NULL; arg->arr = NULL; arg->arr_2d = NULL; arg->count = 0;

    const char* tic_num = gtk_entry_get_text(GTK_ENTRY(scr->tic_num));
    const char* mobile_num = gtk_entry_get_text(GTK_ENTRY(scr->mobile_num));

    if ((strcmp(tic_num,"")==0) || (strcmp(mobile_num,"")==0))
    {
        return ENTRY_EMPTY;
    }

    for (int i = 0; i < strlen(tic_num); i++)
    {
        if (('0'>tic_num[i]) || (tic_num[i]>'9'))
        {
            return ALPHABETS;
        }
    }

    char* is_tic_present = malloc(1), *db_mobile_num = malloc(1), *is_cancelled = malloc(1), *is_old = malloc(1);
    time_t *now = malloc(sizeof(time_t));

    sqlite3* db;
    char* sql;

    sqlite3_open("rsc/data", &db);

    sql = g_strdup_printf("SELECT count(*) FROM TICKET WHERE ticket_number = %s", tic_num);
    arg->str = is_tic_present;
    sqlite3_exec(db, sql, callback_str, arg, NULL);
    arg->str = NULL;

    if (strcmp(is_tic_present, "0")==0)
    {
        ret_val =  INVALID_TICKET_NO;
    }
    else
    {
        sql = g_strdup_printf("SELECT is_cancelled FROM TICKET WHERE ticket_number = %s", tic_num);
        arg->str = is_cancelled;
        sqlite3_exec(db, sql, callback_str, arg, NULL);
        arg->str = NULL;

        if (atoi(is_cancelled) == 1)
        {
            ret_val = CANCELLED_TICKET;
        }
        else
        {
            if (strcmp(scr->name, "CANCEL") == 0)
            {
                time(now); 
                sql = g_strdup_printf("SELECT count(*) FROM TICKET as tic JOIN TRAIN as t ON tic.ticket_train_id=t.id JOIN Train_Dates as t_dt ON t_dt.train_id=t.id JOIN DATES as dt ON dt.id=t_dt.dates_id WHERE tic.ticket_number = %s AND dt.unix_time>%ld", tic_num, *now);
                arg->str = is_old;
                sqlite3_exec(db, sql, callback_str, arg, NULL);
                arg->str = NULL;

                if (atoi(is_old) == 0)
                {
                    sqlite3_close(db);
                    free(is_tic_present); free(is_cancelled); free(is_old); free(db_mobile_num); free(arg); free(now);
                    return OLD_TRAIN;
                }
            }
            sql = g_strdup_printf("SELECT mobile_no FROM TICKET WHERE ticket_number = %s", tic_num);
            arg->str = db_mobile_num;
            sqlite3_exec(db, sql, callback_str, arg, NULL);
            arg->str = NULL;
            if (strcmp(mobile_num, db_mobile_num)==0)
            {
                ret_val = VALID_DATA;
            }
            else
            {
                ret_val = INVALID_MOBILE_NO;
            }
        }
    }

    sqlite3_close(db);
    free(is_tic_present); free(is_cancelled); free(is_old); free(db_mobile_num); free(arg); free(now);   
    return ret_val;
}

/* ## Makes suitable changes to the db to cancel ticket */
void SQL_cancel_tic(char* tic_num)
{
    PRAM* arg = malloc(sizeof(PRAM));
    arg->str = NULL; arg->arr = NULL; arg->arr_2d = NULL; arg->count = 0;

    sqlite3* db;
    char* sql;

    char *len = malloc(1), **seat_ids;

    sqlite3_open("rsc/data", &db);

    /* Getting the number of passengers */
    sql = g_strdup_printf("SELECT count(*) FROM PASSENGERS WHERE ticket_id = %s",tic_num);
    arg->str = len;
    sqlite3_exec(db, sql, callback_str, arg, NULL);

    /* Getting their seat ids */
    seat_ids = calloc(atoi(len), sizeof(char*));
    sql = g_strdup_printf("SELECT passenger_seat FROM PASSENGERS WHERE ticket_id = %s", tic_num);
    arg->arr = seat_ids;
    sqlite3_exec(db, sql, callback_col, arg, NULL);

    /* Updating ticket */
    sql = g_strdup_printf("UPDATE TICKET SET is_cancelled = 1 WHERE ticket_number = %s", tic_num);
    sqlite3_exec(db, sql, NULL, NULL, NULL);

    for (int i = 0; i < atoi(len); i++)
    {
        sql = g_strdup_printf("UPDATE SEAT SET is_booked = 0 WHERE id = %s", seat_ids[i]);
        sqlite3_exec(db, sql, NULL, NULL, NULL);
    }

    sqlite3_close(db);
    free(len); free(seat_ids);   
    printf("\nTicket Cancelled Successfully\n");
}
