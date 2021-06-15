# Set the name and the supported language of the project
project(Railway_reservation C)

# Set the minimum version of cmake required to build this project
cmake_minimum_required(VERSION 2.6)

# Use the package PkgConfig to detect GTK+ headers/library files and others
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package (SQLite3)

# Setup CMake to use GTK+, tell the compiler where to look for headers and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
include_directories(${SQLITE3_INCLUDE_DIRS})

# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})

# Add an executable compiled from hello.c
add_executable(${PROJECT_NAME} main.c resources/rsc.c)

# Link the target to the GTK+ and other libraries
target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
target_link_libraries (${PROJECT_NAME} LINK_PUBLIC sqlite3)

# Adding custom files
FILE(COPY resources/UI/UI.glade DESTINATION "${CMAKE_BINARY_DIR}/resources/UI/")
FILE(COPY rsc/data DESTINATION "${CMAKE_BINARY_DIR}/rsc/")
