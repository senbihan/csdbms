#ifndef METADATA_H
#define METADATA_H

#include "metadata_struct.h"
#include <cstdio>

int             NO_RECORDS;
int             NO_COLUMNS;
char*           TABLE_NAME;
int             RECORD_SIZE;
int             DATA_HEAD;
int             DATA_END;
struct Column   *col;

#endif // METADATA_H