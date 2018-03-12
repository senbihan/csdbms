#ifndef WRITER_H
#define WRITER_H

#include "metadata_struct.h"

extern int             NO_RECORDS;
extern int             NO_COLUMNS;
extern char            *TABLE_NAME;
extern int             RECORD_SIZE;
extern int              DATA_HEAD;
extern struct Column   *col;

void create_db_file();
/*void write_file_header();
void write_timestamp();
void write_no_records();
void write_no_columns();
void write_table_name();
void write_size_of_records();
void write_data_head_offset();
void write_data_end_offset();
void write_columns();
*/
void insert_data();
void write_to_file();
void create_db();


#endif //