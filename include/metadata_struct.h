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

/*Helper Macros*/

#define Malloc(T,size)  (T*)malloc(size*sizeof(T))
#define Calloc(T,size)  (T*)calloc(size,sizeof(T))
#define ERR_MESG(m)     {perror(m); exit(0);}

/* Constants */

#define INT_MAX_DIGIT           9
#define INT_BYTE_SIZE           5                
#define MAX_INTEGER             (int(1e10)-1)
#define MAX_RECORDS             2048
#define MAX_COLUMNS             10
#define STR_MAX_SIZE            255
#define BLOCK_SIZE              (1 << 11)
#define PTR_SIZE                4

/* Dataypes maps */
#define INTEGER                 1
#define STRING                  2
#define DOUBLE                  3
#define DATE                    4
#define TIME                    5
/**
 * Size of each fields in the metadata
 * written in Bytes
 **/

/* Static Portion of the File */

#define FILE_HEADER_SIZE        2
#define DATE_TIME_SIZE          8   // may change
#define RECORD_NO_SIZE          8
#define COLUMN_NO_SIZE          1
#define TABLE_NAME_SIZE         32
#define RECORD_SIZE_SIZE        2
#define DATA_HEAD_SIZE          4
#define DATA_END_SIZE           4
#define LAST_REC_NO_SIZE        4 
/* Variable Portion */

#define COLUMN_DETAILS_SIZE     32
#define COLUMN_NAME_SIZE        16
#define DATA_TYPE_SIZE          1   

// first 3-bits : data type, next 3-bits: Constraint info
/**
 *  Datatypes: 
 *  int     :   001
 *  double  :   010
 *  string  :   011
 *  date    :   100
 *  time    :   101
 * 
 * Constriants:
 *  primary :   001
 *  not null:   010
 *  auto increment: 011
 *  default: 000
 **/

#define DATA_LEN_SIZE           1  

// first 4-bits : integer part size, next 4-bit: decimal part size (Real Numbers)
// first 4-bits : int/date/time size
// 8 bits       : string   

#define INDEX_COL_SIZE          1   // 0 : logically absent, NZ : Index of column
#define AUTO_INC_SIZE           5
#define MISC_SIZE               8

// derived
#define NO_REC_POSITION     FILE_HEADER_SIZE
#define NO_COL_POSITION     NO_REC_POSITION + RECORD_NO_SIZE
#define TBL_NAME_POSITION   NO_COL_POSITION + COLUMN_NO_SIZE
#define REC_SIZE_POSITION   TBL_NAME_POSITION + TABLE_NAME_SIZE
#define DATA_HEAD_POSITION  REC_SIZE_POSITION + RECORD_SIZE_SIZE
#define DATA_END_POSITION   DATA_HEAD_POSITION + DATA_HEAD_SIZE
#define LAST_REC_NO_POS     DATA_END_POSITION + DATA_END_SIZE
#define COL_DESC_POR        LAST_REC_NO_POS + LAST_REC_NO_SIZE 

#define D_END               DATA_HEAD + (NO_RECORDS * BLOCK_SIZE)
#define BLOCK_START(i)      DATA_HEAD + (i - 1) * BLOCK_SIZE
#define BLOCK_NO
struct Column
{
    char    col_name[COLUMN_NAME_SIZE];
    char    index;
    char    data_type;
    int     size; // 1 byte rep will be added later


    /*void print_col(){
        printf("\n");
        printf("Col Name : %s\n",col_name);
        printf("Col Index: %d\n",index);
        printf("Data Type: %d\n",data_type);
        printf("Size : %d\n",size);
    }*/
};

struct cmp_str
{
   bool operator()(char const *a, char const *b)
   {
      return std::strcmp(a, b) < 0;
   }
};

#endif // STRUCT_INFO_H