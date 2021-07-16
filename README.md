Check out **Releases** for executables. The Linux binary is tested ony in ubuntu 20.04LTS

This is an Railway Reservation application that lets you to emulate the **Booking**, **Downloading** and **Cancelling** your tickets

# Features
As mentioned above this app lets you to book, download and cancel your tickets
- All the data in here are for in app experience and does not translate to real world information
- The app extensively dependent on a database which it manages to keep track of all the data
- Although the database contains all the data only necessary information will be shown in a screen
- Various input integrity checks have been implemented

# Limitations
As Everything out there this also has it's Limitations
- The app is current a station dependent app, i.e the user cannot change the boarding station(Defaults to Chennai Central) but can choose only their destination from the available ones
- The details of the trains is static but it can be altered by altering the db tables(DEST, NAME, TIMES and TRAIN, DEFAULT TRAINS Along with their connection tables)

# Installation
## Windows
Download the Railway_Reservation_System.zip
## Linux
- Download the Railway_Reservation_System.tar.gx and extract it 
- Install wkhtmltopdf using terminal
    ```bash
    # for Debian or Ubuntu machines
    sudo apt install wkhtmltopdf
    ```
- Run the binary in terminal
> **Note :** The binary was tested Ubuntu 20.04 LTS

# DEV
- Those who are willing to know more about the app's working refer `resources` and `Design` directory
