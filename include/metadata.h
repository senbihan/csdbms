#ifndef METADATA_H
#define METADATA_H

#include "metadata_struct.h"
#include <cstdio>
#include <iostream>

int             NO_RECORDS;
int             NO_COLUMNS;
char*           TABLE_NAME;
int             RECORD_SIZE;
int             DATA_HEAD;
int             DATA_END;
struct Column   *col;
int             *DATA_TYPES;
std::string     types[6] = {"", "INTEGER", "DOUBLE", "STRING", "DATE", "TIME"};
int             TYPE_SIZE[6]   = {0, sizeof(int), sizeof(double), sizeof(char*), 0 , 0};
#endif // METADATA_H