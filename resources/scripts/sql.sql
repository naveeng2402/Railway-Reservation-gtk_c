/* Table Creations */

CREATE TABLE "DATES" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT,
	"dates_val"	TEXT UNIQUE,
	"unix_time"	INTEGER
);

CREATE TABLE "DEST" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT,
	"destination"	TEXT,
	"price"	REAL,
    UNIQUE(destination , price)
);

CREATE TABLE "NAME" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT,
	"train_names"	TEXT UNIQUE
);

CREATE TABLE "TIMES" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT,
	"time"	TEXT UNIQUE
);

CREATE TABLE "DEFAULT_TRAINS"
(
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

CREATE TABLE "TRAIN" (
	"id"	INTEGER PRIMARY KEY AUTOINCREMENT,
	"is_train_full"	INTEGER
);

CREATE TABLE "Train_Dates" (
	"train_id"	INTEGER,
	"dates_id"	INTEGER
);

CREATE TABLE "Train_Dest" (
	"train_id"	INTEGER,
	"dest_id"	INTEGER
);

CREATE TABLE "Train_Name" (
	"train_id"	INTEGER,
	"name_id"	INTEGER
);

CREATE TABLE "Train_Time" (
	"train_id"	INTEGER,
	"time_id"	INTEGER
);

CREATE TABLE "SEAT" (
    "id"	INTEGER PRIMARY KEY AUTOINCREMENT,
    "seat_no"	INTEGER,
    "is_booked"	INTEGER DEFAULT 0,
    "is_window"	INTEGER,
    "class_id"	INTEGER,
    "train_id"	INTEGER
);

CREATE TABLE "TICKET" (
    "ticket_number"	INTEGER PRIMARY KEY AUTOINCREMENT,
    "is_cancelled"	INTEGER DEFAULT 0,
    "no_of_passengers"	INTEGER,
    "contact_name"	TEXT,
    "mobile_no"	TEXT, /*To avoid removal of initial zeros*/
    "email_id"	TEXT,
    "time_of_bk"	TEXT,
    "date_of_bk"	INTEGER,
    "ticket_train_id"	INTEGER
);

CREATE TABLE "PASSENGERS" (
    "id"	INTEGER PRIMARY KEY AUTOINCREMENT,
    "passenger_name"	TEXT,
    "age"	INTEGER,
    "gender_id"	INTEGER,
    "passenger_seat"	INTEGER,
    "ticket_id"	INTEGER
);

CREATE TABLE "static_SEAT_CLASS" (
    "id"	INTEGER,
    "class"	TEXT,
    PRIMARY KEY("id")
);

CREATE TABLE "static_GENDER" (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    gender TEXT
);

/* Data Insertion */

INSERT INTO "DEST" ("destination", "price") VALUES ('Thiruvallur', '30.0');
INSERT INTO "DEST" ("destination", "price") VALUES ('Madurai', '300.0');
INSERT INTO "DEST" ("destination", "price") VALUES ('Kanchipuram', '160.0');

INSERT INTO "NAME" ("train_names") VALUES ('Train 1');
INSERT INTO "NAME" ("train_names") VALUES ('Train 2');
INSERT INTO "NAME" ("train_names") VALUES ('Train 3');

INSERT INTO "TIMES" ("time") VALUES ('10:00');
INSERT INTO "TIMES" ("time") VALUES ('13:00');
INSERT INTO "TIMES" ("time") VALUES ('18:00');

INSERT INTO "DEFAULT_TRAINS" ("id") VALUES ('1');
INSERT INTO "DEFAULT_TRAINS" ("id") VALUES ('2');
/* INSERT INTO "DEFAULT_TRAINS" ("id") VALUES ('3'); */

INSERT INTO "Def_Train_Dest" ("train_id", "dest_id") VALUES ('1', '1');
INSERT INTO "Def_Train_Dest" ("train_id", "dest_id") VALUES ('2', '2');
/* INSERT INTO "Def_Train_Dest" ("train_id", "dest_id") VALUES ('3', '3'); */

INSERT INTO "Def_Train_Name" ("train_id", "name_id") VALUES ('1', '1');
INSERT INTO "Def_Train_Name" ("train_id", "name_id") VALUES ('2', '2');
/* INSERT INTO "Def_Train_Name" ("train_id", "name_id") VALUES ('3', '3'); */

INSERT INTO "Def_Train_Time" ("train_id", "time_id") VALUES ('1', '1');
INSERT INTO "Def_Train_Time" ("train_id", "time_id") VALUES ('2', '2');
/* INSERT INTO "Def_Train_Time" ("train_id", "time_id") VALUES ('3', '3'); */

INSERT INTO "static_GENDER" ("gender") VALUES ('MALE');
INSERT INTO "static_GENDER" ("gender") VALUES ('FEMALE');
INSERT INTO "static_GENDER" ("gender") VALUES ('TRANSGENDER');

INSERT INTO "static_SEAT_CLASS" ("class") VALUES ('A/C');
INSERT INTO "static_SEAT_CLASS" ("class") VALUES ('Non-A/C');
INSERT INTO "static_SEAT_CLASS" ("class") VALUES ('A/C Sleeper');
INSERT INTO "static_SEAT_CLASS" ("class") VALUES ('Non-A/C Sleeper');