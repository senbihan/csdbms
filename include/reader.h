/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/

#ifndef READER_H
#define READER_H

#include <cstring>
#include "metadata_struct.h"

extern int      IS_READ;
extern int      *DATA_TYPES;
extern char     *OPEN_FILE;

struct datetime{
    int32_t     year;
    int32_t     month;
    int32_t     day;
    int32_t     hh;
    int32_t     mm;
    int32_t     ss;
};

int check_validity();
void read_timestamp(FILE *);
int read_from_file(const char *filename);
struct datetime *get_date_time();

#endif