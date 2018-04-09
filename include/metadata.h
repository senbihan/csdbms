/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/

#ifndef METADATA_H
#define METADATA_H

#include "metadata_struct.h"
#include <unordered_map>
#include <map>
#include <cstring>  
#include <utility>
#include <variant>
#include <cstdio>
#include <iostream>

int32_t             NO_RECORDS;         /* No of Records logically present */
int32_t             NO_COLUMNS;         /* No of columns */
char*               TABLE_NAME;
int32_t             RECORD_SIZE;
int32_t             DATA_HEAD;
int32_t             DATA_END;
struct Column       *col;
int                 *DATA_TYPES;
std::string         types[6] = {"", "INTEGER", "DOUBLE", "STRING", "DATE", "TIME"};
uint                TYPE_SIZE[6]   = {0, sizeof(int), sizeof(long), sizeof(char), 0 , 0};
int32_t             PRIMARY_KEY_COL_NO;
int32_t             *CUM_POS;
int32_t             FIRST_REC_NO;
int32_t             LAST_REC_NO;
int32_t             TOTAL_RECORD;
int                 IS_READ;
char*               OPEN_FILE;
int64_t             LAST_MOD_TIME;

std::map<std::string,int> COL_NT;


/* Constraint */




#endif // METADATA_H