/**
 * This is the common header file for the metadata structure
 * All the constants and size of each field is declared as a macro
 * 
 * 
 * Author(s) : Bihan Sen    (senbihan@gmail.com)
 * 
 * 
 **/


#ifndef STRUCT_INFO_H
#define STRUCT_INFO_H

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#define DEBUG                   0

/*Helper Macros*/

#define Malloc(T,size)          (T*)malloc(size*sizeof(T))
#define Calloc(T,size)          (T*)calloc(size,sizeof(T))
#define WARN_MESG(m)            {fprintf(stderr,"\033[0;31m");fprintf(stderr,m); fprintf(stderr,"\033[0m");}
#define ERR_MESG(m)             {fprintf(stderr,"\033[1;31m");fprintf(stderr,m); fprintf(stderr,"\033[0m"); exit(EXIT_FAILURE);}
#define OK_MESG(m)              {fprintf(stderr,"\033[0;32m");fprintf(stderr,m); fprintf(stderr,"\033[0m");}
#define FAIL_MESG(m)            {fprintf(stderr,"\033[0;33m");fprintf(stderr,m); fprintf(stderr,"\033[0m");}

/* Constants */

#define INT_MAX_DIGIT           9
#define INT_BYTE_SIZE           5                
#define MAX_INTEGER             999999999
#define FRAC_MAX_DIG            8
#define MAX_RECORDS             ((1<<32)-1)
#define MAX_COLUMNS             32
#define STR_MAX_SIZE            255
#define BLOCK_SIZE              (1 << 13)
#define PTR_SIZE                4

/* Dataypes maps */
#define INTEGER                 1
#define STRING                  3
#define DOUBLE                  2
#define DATE                    4
#define TIME                    5
/**
 * Size of each fields in the metadata
 * written in Bytes
 **/

/* Static Portion of the File */

#define FILE_HEADER_SIZE        2
#define DATE_TIME_SIZE          8   // may change
#define RECORD_NO_SIZE          4   // revised as 4
#define COLUMN_NO_SIZE          1
#define TABLE_NAME_SIZE         32
#define RECORD_SIZE_SIZE        2
#define DATA_HEAD_SIZE          4
#define DATA_END_SIZE           4
#define LAST_REC_NO_SIZE        4 
#define FIRST_REC_NO_SIZE       4 
#define TOTAL_REC_SIZE          4
#define TOTAL_STATIC_SIZE       72
/* Variable Portion */

#define COLUMN_DETAILS_SIZE     32
#define COLUMN_NAME_SIZE        16
#define DATA_TYPE_SIZE          1   
// first 3-bits : data type, next 3-bits: Constraint info
/**
 *  Datatypes: 
 *  int     :   000
 *  double  :   001
 *  string  :   010
 *  date    :   011
 *  time    :   100
 * 
 * Constriants:
 *  primary :   000
 *  not null:   001
 *  auto increment: 010
 *  default: 011
 **/

#define DATA_LEN_SIZE           1  

// first 4-bits : integer part size, next 4-bit: decimal part size (Real Numbers)
// first 4-bits : int/date/time size
// 8 bits       : string   

#define INDEX_COL_SIZE          1   // 0 : logically absent, NZ : Index of column
#define AUTO_INC_SIZE           5
#define MISC_SIZE               8

#define TOTAL_DYN_SIZE          (32 * MAX_COLUMNS)

// derived
#define DATE_TIME_POSITION      FILE_HEADER_SIZE
#define NO_REC_POSITION         DATE_TIME_POSITION + DATE_TIME_SIZE
#define NO_COL_POSITION         NO_REC_POSITION + RECORD_NO_SIZE
#define TBL_NAME_POSITION       NO_COL_POSITION + COLUMN_NO_SIZE
#define REC_SIZE_POSITION       TBL_NAME_POSITION + TABLE_NAME_SIZE
#define DATA_HEAD_POSITION      REC_SIZE_POSITION + RECORD_SIZE_SIZE
#define DATA_END_POSITION       DATA_HEAD_POSITION + DATA_HEAD_SIZE
#define FIRST_REC_NO_POS        DATA_END_POSITION + DATA_END_SIZE
#define LAST_REC_NO_POS         FIRST_REC_NO_POS + FIRST_REC_NO_SIZE
#define TOTAL_REC_POS           LAST_REC_NO_POS + TOTAL_REC_SIZE

#define COL_DESC_POS            TOTAL_STATIC_SIZE 

#define D_END                   DATA_HEAD + (TOTAL_RECORD * BLOCK_SIZE)
#define BLOCK_START(i)          DATA_HEAD + (i - 1) * BLOCK_SIZE


typedef unsigned char           byte;

struct Column
{
    char    col_name[COLUMN_NAME_SIZE]; // columnname
    byte    data_type;                  // type and constraint
    byte    index;                      // present
    byte    size;                       // length of data
    char    auto_incr[INT_BYTE_SIZE];   // 5 byte integer
    byte    buff[8];                    // extra space
};


#endif // STRUCT_INFO_H