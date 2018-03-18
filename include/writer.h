/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/

#ifndef WRITER_H
#define WRITER_H

#include "metadata_struct.h"
#include <map>
#include <string.h>

extern int             NO_RECORDS;
extern int             NO_COLUMNS;
extern char            *TABLE_NAME;
extern int             RECORD_SIZE;
extern int             TOTAL_RECORD_SIZE;
extern int             DATA_HEAD;
extern int             DATA_END;
extern struct Column   *col;
extern int             *DATA_TYPES;
extern int             TYPE_SIZE[6]  ; //= {0, sizeof(int), sizeof(double), sizeof(char), 0 , 0};
extern int             PRIMARY_KEY_COL_NO;
extern int             *CUM_POS;
extern int             LAST_REC_NO;
extern std::map<const char*,int, cmp_str> COL_NT;

void write_to_file();
char* create_db();


#endif //