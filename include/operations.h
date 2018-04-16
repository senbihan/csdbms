#ifndef INSERT_H
#define INSERT_H

#include "metadata_struct.h"
#include <variant>
#include <map>
#include <string.h>
#include <unordered_map>

extern int32_t             NO_RECORDS;         /* No of Records logically present */
extern int32_t             NO_COLUMNS;         /* No of columns */
extern char*               TABLE_NAME;
extern int32_t             RECORD_SIZE;
extern int32_t             DATA_HEAD;
extern int32_t             DATA_END;
extern struct Column       *col;
extern int                 *DATA_TYPES;
extern int                 *CONST;
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

extern std::map<std::string,int>   COL_NT;

void show_schema(char *);
void build_hash_table(const char *);
bool insert_data(int, char **);
void delete_data(int, char **);
void show_data(int , char** );

#endif // INSERT_H