/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/

#ifndef READER_H
#define READER_H

#include <cstring>
#include "metadata_struct.h"

extern int IS_READ;
extern int  *DATA_TYPES;

int check_validity(FILE *fp);
void read_from_file(const char *filename);

#endif