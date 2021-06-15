## Table Of Contents
- [CREATING TABLES](#creating-tables)
- [INSERTING DATA](#inserting-data)
- [SELECTION TEMPLATES](#selection-templates)
  * [Selections](#selections)

---

# CREATING TABLES
``` sql
CREATE TABLE "DATES" (
"id"	integer PRIMARY KEY AUTOINCREMENT,
"dates_val"	text UNIQUE
);

CREATE TABLE "SEAT" (
    "id"	integer PRIMARY KEY AUTOINCREMENT,
    "seat_no"	integer,
    "is_booked"	integer DEFAULT 0,
    "is_window"	integer,
    "class_id"	integer,
    "train_id"	integer
);

CREATE TABLE "TRAIN" (
	"id"	integer PRIMARY KEY AUTOINCREMENT,
	"is_train_full"	INTEGER
	/*"is_default"	INTEGER*/
);

CREATE TABLE "Train_Dates" (
	"train_id"	INTEGER,
	"dates_id"	INTEGER
);

CREATE TABLE "DEST" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT,
	"destination"	TEXT,
	"price"	REAL,
    UNIQUE(destination , price)
);

CREATE TABLE "Train_Dest" (
	"train_id"	INTEGER,
	"dest_id"	INTEGER
);

CREATE TABLE "TIMES" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT,
	"time"	TEXT UNIQUE
);

CREATE TABLE "Train_Time" (
	"train_id"	INTEGER,
	"time_id"	INTEGER
);

CREATE TABLE "NAME" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT,
	"train_names"	TEXT UNIQUE
);

CREATE TABLE "Train_Name" (
	"train_id"	INTEGER,
	"name_id"	INTEGER
);

CREATE TABLE "static_SEAT_CLASS" (
    "id"	integer,
    "class"	text,
    PRIMARY KEY("id")
);


CREATE TABLE "TICKET" (
    "ticket_number"	integer PRIMARY KEY AUTOINCREMENT,
    "is_cancelled"	INTEGER DEFAULT 0,
    "no_of_passengers"	integer,
    "contact_name"	text,
    "mobile_no"	text, /*To avoid removal of initial zeros*/
    "email_id"	text,
    "time_of_bk"	text,
    "date_of_bk"	integer,
    "ticket_train_id"	integer
);

CREATE TABLE "PASSENGERS" (
    "id"	integer PRIMARY KEY AUTOINCREMENT,
    "passenger_name"	text,
    "age"	integer,
    "gender_id"	integer,
    "passenger_seat"	INTEGER,
    "ticket_id"	integer
);

CREATE TABLE static_GENDER (
    id integer PRIMARY KEY AUTOINCREMENT,
    gender text
);

        /* Default Trains */

CREATE TABLE "DEFAULT_TRAINS" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT
);

CREATE TABLE "Def_Train_Dest" (
	"train_id"	INTEGER,
	"dest_id"	INTEGER
);

CREATE TABLE "Def_Train_Name" (
	"train_id"	INTEGER,
	"name_id"	INTEGER
);

CREATE TABLE "Def_Train_Time" (
	"train_id"	INTEGER,
	"time_id"	INTEGER
);

```

# INSERTING DATA
```sql
INSERT INTO DATES ("dates_val") VALUES("02-06-2021");
INSERT INTO DATES ("dates_val") VALUES("03-06-2021");
INSERT INTO DATES ("dates_val") VALUES("04-06-2021");

INSERT INTO static_TRAIN_DET("name","dep_time","price") VALUES("Train 1","5.00 AM",300);
INSERT INTO static_TRAIN_DET("name","dep_time","price") VALUES("Train 2","10.00 PM",250);
INSERT INTO static_TRAIN_DET("name","dep_time","price") VALUES("Train 3","1.00 PM",100);

INSERT INTO static_SEAT_CLASS("class") VALUES("A/C");
INSERT INTO static_SEAT_CLASS("class") VALUES("Non-A/C");
INSERT INTO static_SEAT_CLASS("class") VALUES("A/C Sleeper");
INSERT INTO static_SEAT_CLASS("class") VALUES("Non-A/C Sleeper");

/* ONLY PRIMARY KEY EXISTS FOR TRAINS table*/

INSERT INTO Train_Dates("train_id","dates_id") VALUES(1,1);
INSERT INTO Train_Dates("train_id","dates_id") VALUES(2,1);
INSERT INTO Train_Dates("train_id","dates_id") VALUES(3,1);
INSERT INTO Train_Dates("train_id","dates_id") VALUES(4,2);
INSERT INTO Train_Dates("train_id","dates_id") VALUES(5,2);
INSERT INTO Train_Dates("train_id","dates_id") VALUES(6,2);
INSERT INTO Train_Dates("train_id","dates_id") VALUES(7,3);
INSERT INTO Train_Dates("train_id","dates_id") VALUES(8,3);
INSERT INTO Train_Dates("train_id","dates_id") VALUES(9,3);

INSERT INTO DEST ("destination","price") VALUES("Thiruvallur",30);
INSERT INTO DEST ("destination","price") VALUES("Madurai",300);
INSERT INTO DEST ("destination","price") VALUES("Kanchipuram",160);

INSERT INTO Train_Dest ("train_id","dest_id") VALUES (1,1);
INSERT INTO Train_Dest ("train_id","dest_id") VALUES (2,2);
INSERT INTO Train_Dest ("train_id","dest_id") VALUES (3,3);
INSERT INTO Train_Dest ("train_id","dest_id") VALUES (4,1);
INSERT INTO Train_Dest ("train_id","dest_id") VALUES (5,2);
INSERT INTO Train_Dest ("train_id","dest_id") VALUES (6,3);
INSERT INTO Train_Dest ("train_id","dest_id") VALUES (7,1);
INSERT INTO Train_Dest ("train_id","dest_id") VALUES (8,2);
INSERT INTO Train_Dest ("train_id","dest_id") VALUES (9,3);

INSERT INTO TIMES ("time") VALUES ("10:00");
INSERT INTO TIMES ("time") VALUES ("13:00");
INSERT INTO TIMES ("time") VALUES ("18:00");

INSERT INTO Train_Time ("train_id","time_id") VALUES (1,1);
INSERT INTO Train_Time ("train_id","time_id") VALUES (2,2);
INSERT INTO Train_Time ("train_id","time_id") VALUES (3,3);
INSERT INTO Train_Time ("train_id","time_id") VALUES (4,1);
INSERT INTO Train_Time ("train_id","time_id") VALUES (5,2);
INSERT INTO Train_Time ("train_id","time_id") VALUES (6,3);
INSERT INTO Train_Time ("train_id","time_id") VALUES (7,1);
INSERT INTO Train_Time ("train_id","time_id") VALUES (8,2);
INSERT INTO Train_Time ("train_id","time_id") VALUES (9,3);

INSERT INTO NAME ("train_names") VALUES ("Train 1");
INSERT INTO NAME ("train_names") VALUES ("Train 2");
INSERT INTO NAME ("train_names") VALUES ("Train 3");

INSERT INTO Train_Name ("train_id","name_id") VALUES (1,1);
INSERT INTO Train_Name ("train_id","name_id") VALUES (2,2);
INSERT INTO Train_Name ("train_id","name_id") VALUES (3,3);
INSERT INTO Train_Name ("train_id","name_id") VALUES (4,1);
INSERT INTO Train_Name ("train_id","name_id") VALUES (5,2);
INSERT INTO Train_Name ("train_id","name_id") VALUES (6,3);
INSERT INTO Train_Name ("train_id","name_id") VALUES (7,1);
INSERT INTO Train_Name ("train_id","name_id") VALUES (8,2);
INSERT INTO Train_Name ("train_id","name_id") VALUES (9,3);

/* NO need to pass is_booked as by default it is 0 */
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(1,0,1,1,1);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(2,0,0,2,1);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(3,0,0,3,1);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(7,0,1,4,2);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(8,0,0,1,2);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(9,0,0,2,2);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(20,0,1,3,3);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(21,0,0,4,3);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(22,0,0,1,3);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(30,0,1,2,4);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(31,0,0,3,4);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(32,0,0,4,4);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(40,1,1,1,5);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(41,1,0,2,5);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(42,0,0,3,5);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(50,0,1,4,6);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(51,0,0,1,6);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(52,0,0,2,6);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(60,1,1,3,7);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(61,0,0,4,7);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(62,0,0,1,7);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(70,0,1,2,8);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(71,1,0,3,8);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(72,1,0,4,8);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(80,0,1,1,9);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(81,0,0,2,9);
INSERT INTO SEAT("seat_no","is_booked","is_window","class_id","train_id") VALUES(82,0,0,3,9);
/* End of Train Model */ 

INSERT INTO static_GENDER ("gender") VALUES ("MALE");
INSERT INTO static_GENDER ("gender") VALUES ("FEMALE");
INSERT INTO static_GENDER ("gender") VALUES ("TRANSGENDER");

INSERT INTO TICKET ("no_of_passengers","contact_name","mobile_no","email_id","time_of_bk","date_of_bk", "ticket_train_id")
			VALUES (2,"Name 1",1234567890,"name1@email.com","5.31 PM",1,8);
INSERT INTO TICKET ("no_of_passengers","contact_name","mobile_no","email_id","time_of_bk","date_of_bk", "ticket_train_id")
			VALUES (2,"Name 2",0987654321,"name2@email.com","10.53 AM",1,5);
INSERT INTO TICKET ("no_of_passengers","contact_name","mobile_no","email_id","time_of_bk","date_of_bk", "ticket_train_id")
			VALUES (1,"Name 3",0246813579,"name3@email.com","6.24 PM",2,7);

INSERT INTO PASSENGERS ("passenger_name","age","gender_id","passenger_seat","ticket_id") VALUES ("Pass 1",13,1,71,1001);
INSERT INTO PASSENGERS ("passenger_name","age","gender_id","passenger_seat","ticket_id") VALUES ("Pass 2",35,2,72,1001);
INSERT INTO PASSENGERS ("passenger_name","age","gender_id","passenger_seat","ticket_id") VALUES ("Pass 3",10,2,40,1002);
INSERT INTO PASSENGERS ("passenger_name","age","gender_id","passenger_seat","ticket_id") VALUES ("Pass 4",30,1,41,1002);
INSERT INTO PASSENGERS ("passenger_name","age","gender_id","passenger_seat","ticket_id") VALUES ("Pass 5",55,1,61,1003);
```

# SELECTION TEMPLATES
> SELECTION TEMPLATES For Trains model (TRAIN, SEAT, DATES, static_SEAT_CLASS,static_TRAIN_DET) (for operations with seats)

```sql
SELECT {Columns} FROM
SEAT AS s JOIN static_SEAT_CLASS AS sc ON s.class_id=sc.id
JOIN TRAIN AS t ON t.id=s.train_id
JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id /*joining train dates*/
JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id /*joining train name*/
JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id /*joining train destination*/
JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id /*joining train time*/
WHERE {Conditions}

/*Example*/
SELECT dt.dates_val,des.destination, ti.time, n.train_names, s.seat_no, sc.class, s.is_booked, s.is_window FROM
SEAT AS s JOIN static_SEAT_CLASS AS sc ON s.class_id=sc.id
JOIN TRAIN AS t ON t.id=s.train_id
JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id /*joining train dates*/
JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id /*joining train name*/
JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id /*joining train destination*/
JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id /*joining train time*/
WHERE des.destination="Thiruvallur" AND dt.dates_val="03-06-2021"
```
>SELECTION TEMPLATE FOR WHOLE MODEL(for operations with ticket)
```sql
SELECT {Columns}
FROM PASSENGERS AS p JOIN TICKET AS tic ON tic.ticket_number=p.ticket_id
JOIN static_GENDER AS gen ON gen.id = p.gender_id
JOIN TRAIN AS t ON t.id   = tic.ticket_train_id
JOIN SEAT AS s ON t.id = s.train_id  AND s.id=p.passenger_seat
JOIN static_SEAT_CLASS AS sc ON sc.id  = s.class_id
JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id /*joining train dates*/
JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id /*joining train name*/
JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id /*joining train destination*/
JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id /*joining train time*/
JOIN DATES_BK AS tmp ON tmp.id = tic.date_of_bk
WHERE {Conditions}


/* Exmaple */
SELECT dt.dates_val, tmp.dates_val, n.train_names, ti.time, des.destination, tic.ticket_number,  tic.no_of_passengers, tic.contact_name, gen.gender,
	p.passenger_name,s.seat_no
FROM PASSENGERS AS p JOIN TICKET AS tic ON tic.ticket_number=p.ticket_id
JOIN static_GENDER AS gen ON gen.id = p.gender_id
JOIN TRAIN AS t ON t.id   = tic.ticket_train_id
JOIN SEAT AS s ON t.id = s.train_id  AND s.id=p.passenger_seat
JOIN static_SEAT_CLASS AS sc ON sc.id  = s.class_id
JOIN Train_Dates AS t_dt ON t.id=t_dt.train_id JOIN DATES AS dt ON dt.id=t_dt.dates_id /*joining train dates*/
JOIN Train_Name AS t_n ON t.id=t_n.train_id JOIN NAME AS n ON n.id=t_n.name_id /*joining train name*/
JOIN Train_Dest AS t_des ON t.id=t_des.train_id JOIN DEST AS des ON des.id=t_des.dest_id /*joining train destination*/
JOIN Train_Time AS t_ti ON t.id=t_ti.train_id JOIN TIMES AS ti ON ti.id=t_ti.time_id /*joining train time*/
JOIN DATES_BK AS tmp ON tmp.id = tic.date_of_bk
WHERE tic.ticket_number=1002
```
***`Note:`***
- ### While selecting the complete model a copy of `DATES` named `DATES_BK` is created and is droped after the selection
1. ```sql
    CREATE TABLE IF NOT EXISTS DATES_BK AS SELECT * FROM DATES;
    ```
2. ```sql
    DROP TABLE DATES_BK
    ```
- ### Use `DISTINCT` keywoed immediately after `SELECT` to remove duplicates in the view
- ```sql
    SELECT DISTINCT {Columns} FROM {TABLES}

> SELECTION TEMPLATE FOR DEFAULT_TRAINS

```SQL
SELECT {Columns}
FROM DEFAULT_TRAINS JOIN Def_Train_Dest ON DEFAULT_TRAINS.id=Def_Train_Dest.train_id JOIN DEST ON DEST.id = Def_Train_Dest.dest_id
JOIN Def_Train_Name ON Def_Train_Name.train_id=DEFAULT_TRAINS.id JOIN NAME ON Def_Train_Name.name_id=NAME.id 
JOIN Def_Train_Time ON Def_Train_Time.train_id=DEFAULT_TRAINS.id JOIN TIMES ON Def_Train_Time.time_id=TIMES.id 

/* Example */
SELECT DEFAULT_TRAINS.id, NAME.train_names, DEST.destination, TIMES.time, DEST.price
FROM DEFAULT_TRAINS JOIN Def_Train_Dest ON DEFAULT_TRAINS.id=Def_Train_Dest.train_id JOIN DEST ON DEST.id = Def_Train_Dest.dest_id
JOIN Def_Train_Name ON Def_Train_Name.train_id=DEFAULT_TRAINS.id JOIN NAME ON Def_Train_Name.name_id=NAME.id 
JOIN Def_Train_Time ON Def_Train_Time.train_id=DEFAULT_TRAINS.id JOIN TIMES ON Def_Train_Time.time_id=TIMES.id 
```

## Selections
```sql
/* Joining 3 tables and Printing values */ 
SELECT TRAIN.id, DATES.dates_val, static_TRAIN_DET.name
FROM TRAIN JOIN DATES ON DATES.id=TRAIN.date_id
JOIN static_TRAIN_DET on static_TRAIN_DET.id=TRAIN.train_det
WHERE DATES.dates_val = "04-06-2021"

/* Joining 3 tables and selecting a particular train(id) on a specific date */
SELECT t.id FROM
TRAIN AS t JOIN DATES AS d ON t.date_id=d.id
JOIN static_TRAIN_DET AS det on t.train_det=det.id
WHERE d.dates_val="03-06-2021" AND det.name="Train 1"

/* Joining all tables */
 SELECT p.passenger_name,
		p.age, gen.gender, s.seat_no, sc.class, dt.dates_val, tmp.dates_val,  tic.ticket_number, t.id, tic.is_cancelled
FROM TICKET AS tic JOIN PASSENGERS as p ON p.ticket_id=tic.ticket_number
JOIN static_GENDER 	   AS gen ON gen.id = p.gender_id
JOIN TRAIN 		       AS t   ON t.id   = tic.ticket_train_id
JOIN static_TRAIN_DET  AS det ON det.id = t.train_det
JOIN SEAT 			   AS s   ON t.id   = s.train_id  AND s.seat_no=p.passenger_seat
JOIN static_SEAT_CLASS AS sc  ON sc.id  = s.class_id
JOIN DATES			   AS dt  ON dt.id  = t.date_id 
JOIN DATES_VIEW        AS tmp ON tmp.id = tic.date_of_bk
WHERE tic.is_cancelled != 0
```

## NOTE:

* In `design.drawio` There will be tables named **DEFAULT_TRAINS**, **Def_Train_Dest**, **Def_Train_Name**, and **Def_Train_Train**.

* These used by the code to determain whether the train arrives daily or not.

* If a default train is removed all its values from bridge tables(Tables Starting with ***Def*** ) are to be deleted.

* These tables are not included in ***`design.db`***