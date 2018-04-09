/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/

#ifndef WRITER_H
#define WRITER_H

#include "metadata_struct.h"
#include <map>
#include <string.h>

extern int32_t             NO_RECORDS;         /* No of Records logically present */
extern int32_t             NO_COLUMNS;         /* No of columns */
extern char*               TABLE_NAME;
extern int32_t             RECORD_SIZE;
extern int32_t             DATA_HEAD;
extern int32_t             DATA_END;
extern struct Column       *col;
extern int                 *DATA_TYPES;
extern std::string         types[6];
extern uint                TYPE_SIZE[6];
extern int32_t             PRIMARY_KEY_COL_NO;
extern int32_t             *CUM_POS;
extern int32_t             FIRST_REC_NO;
extern int32_t             LAST_REC_NO;
extern int32_t             TOTAL_RECORD;
extern int                 IS_READ;
extern char*               OPEN_FILE;
extern int64_t             LAST_MOD_TIME;


char *table_to_file_name(char *);
void write_timestamp(FILE *);
void write_no_records(FILE *);
void write_no_columns(FILE *);
void write_data_end(FILE *);
void write_first_rec_no(FILE *);
void write_last_rec_no(FILE *);
void write_total_rec(FILE *);    
void write_to_file();
void close_file(FILE *);
char* create_db(int, char **);


#endif //