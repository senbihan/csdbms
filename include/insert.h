#ifndef INSERT_H
#define INSERT_H

#include "metadata_struct.h"

extern int             NO_RECORDS;
extern int             NO_COLUMNS;
extern char            *TABLE_NAME;
extern int             RECORD_SIZE;
extern int              DATA_HEAD;
extern int              DATA_END;
extern struct Column   *col;
extern std::string      types[5] ;//= {"INTEGER", "DOUBLE", "STRING", "DATE", "TIME"};
extern int              TYPE_SIZE[5];

void show_schema(const char *);
void insert_data(const char *, int);
void show_data(const char *);

#endif // INSERT_H