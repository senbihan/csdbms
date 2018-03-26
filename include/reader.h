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

int check_validity();
void read_from_file(const char *filename);

#endif