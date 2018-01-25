/**
 * This is the common header file for the metadata structure
 * All the constants and size of each field is declared as a macro
 * 
 * 
 * Author(s) : Bihan Sen    (senbihan@gmail.com)
 * 
 * Last Modified Date : 25/01/2018
 * 
 **/


#ifndef STRUCT_INFO_H
#define STRUCT_INFO_H


/*Helper Macros*/

#define Malloc(T,size)  (T*)malloc(size*sizeof(T))
#define Calloc(T,size)  (T*)calloc(size,sizeof(T))

/* Constants */

#define FILE_HEADER             "CS"
#define INT_MAX_DIGIT           9
#define INT_BYTE_SIZE           5                

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
 *  no-auto-increment: 011
 **/

#define DATA_TYPE_SIZE_SIZE     1  

// first 4-bits : integer part size, next 4-bit: decimal part size (Real Numbers)
// first 4-bits : int/date/time size
// 8 bits       : string   

#define INDEX_COL_SIZE          1   // 0 : logically absent, NZ : Index of column
#define AUTO_INC_SIZE           5
#define MISC_SIZE               8


#endif // STRUCT_INFO_H