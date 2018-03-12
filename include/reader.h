#ifndef READER_H
#define READER_H

#include <cstring>
#include "metadata_struct.h"


void open_file(char *filename);
int check_validity();
void read_column_no();
void read_record_no();
void read_from_file();
void read_table_name();

#endif