/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/


#include "writer.h"
#include <cstdio>
#include <fstream>
using namespace std;

FILE *fptr;
ofstream ofile;
int *data_types;
void create_db_file(char *fname)
{
    fptr = fopen(fname,"wb+");
    if(NULL == fptr) ERR_MESG("writer.cpp : file cannot be opened!")
}

void write_file_header()
{
    fwrite("CS",sizeof(char),FILE_HEADER_SIZE,fptr);
}

void write_timestamp(FILE *fp)
{
    assert(fp != NULL);
    fseek(fp,DATE_TIME_POSITION,SEEK_SET);
    long tm = 0;
    fwrite(&tm,DATE_TIME_SIZE,1,fp);
}

void write_no_records(FILE *fp)
{
    assert(fp != NULL);
    fseek(fp,NO_REC_POSITION,SEEK_SET);
    fwrite(&NO_RECORDS,RECORD_NO_SIZE,1,fp);
}

void write_no_columns(FILE *fp)
{
    assert(fp != NULL);
    fseek(fp,NO_COL_POSITION,SEEK_SET);
    fwrite(&NO_COLUMNS,COLUMN_NO_SIZE,1,fp);
}

void write_table_name()
{
    fwrite(TABLE_NAME,sizeof(char),TABLE_NAME_SIZE,fptr);
}

void write_data_head()
{
    DATA_HEAD = ftell(fptr) + DATA_HEAD_SIZE + DATA_END_SIZE ;
    DATA_HEAD += FIRST_REC_NO_SIZE + LAST_REC_NO_SIZE + TOTAL_REC_SIZE;
    DATA_HEAD += NO_COLUMNS * sizeof(struct Column);
    DATA_HEAD += BUFFER_SIZE;
    fwrite(&DATA_HEAD, DATA_HEAD_SIZE, 1, fptr);
}

void write_data_end(FILE *fp)
{
    assert(fp != NULL);
    fseek(fp,DATA_END_POSITION,SEEK_SET);
    DATA_END = D_END;
    fwrite(&DATA_END, DATA_END_SIZE, 1, fp);
}

void write_size_of_records()
{
    fwrite(&RECORD_SIZE,RECORD_SIZE_SIZE,1,fptr);
}

void write_first_rec_no(FILE *fp)
{
    assert(fp != NULL);
    fseek(fp,FIRST_REC_NO_POS,SEEK_SET);
    fwrite(&FIRST_REC_NO,FIRST_REC_NO_SIZE,1,fp);
}


void write_last_rec_no(FILE *fp)
{
    assert(fp != NULL);
    fseek(fp,LAST_REC_NO_POS,SEEK_SET);
    fwrite(&LAST_REC_NO,LAST_REC_NO_SIZE,1,fp);
}

void write_total_rec(FILE *fp)
{
    assert(fp != NULL);
    fseek(fp,TOTAL_REC_POS,SEEK_SET);
    fwrite(&TOTAL_RECORD,TOTAL_REC_SIZE,1,fp);
}

/**
 * write all columns to the file
 */
void write_columns()
{
    fwrite(col, sizeof(struct Column), NO_COLUMNS , fptr);
}

void write_to_file()
{
    assert(fptr != NULL);
    write_file_header();
    write_timestamp(fptr);
    write_no_records(fptr);
    write_no_columns(fptr);
    write_table_name();
    write_size_of_records();
    write_data_head();
    write_data_end(fptr);
    write_first_rec_no(fptr);
    write_last_rec_no(fptr);
    write_total_rec(fptr);
    write_columns();
}

char get_type(int type, int cons)
{
    char data_type = char(0);
    assert(type >= 1 && type <= 5);
    assert(cons >= 0 && cons <= 3);
    data_type = char(type - 1);
    data_type <<= 3;
    data_type |= char(cons);
    data_type <<= 2;
    return data_type;        
}

/**
 * first creation of database
 **/
char *create_db()
{
    TABLE_NAME = Malloc(char, TABLE_NAME_SIZE);
    //printf("Enter database name: \n");
    scanf("%s",TABLE_NAME);
    create_db_file(TABLE_NAME);
    //printf("Enter Number of Column: \n");
    scanf("%d",&NO_COLUMNS);
    NO_RECORDS = 0; // initially 0

    col = Malloc(struct Column, NO_COLUMNS);
    data_types = Malloc(int, NO_COLUMNS);
    CUM_POS = Malloc(int,NO_COLUMNS);
    assert(col != NULL && data_types != NULL);
    char ctype;
    int sz, type, cons;
    PRIMARY_KEY_COL_NO = -1;
    LAST_REC_NO = 0;
    FIRST_REC_NO = 1;
    TOTAL_RECORD = 0;
    for(int i = 0 ; i < NO_COLUMNS; i++)
    {
        scanf("%s",col[i].col_name);
        col[i].index = 1;   // present
        scanf(" %c",&ctype);
        switch(ctype){
            case 'i' :      type = INTEGER; break;
            case 'r' :      type = DOUBLE; break;
            case 's' :      type = STRING; break;
            case 'd' :      type = DATE; break;
            case 't' :      type = TIME; break;
            default  :      ERR_MESG("No proper data type found\n");
        }
        //printf("Constraint : \n1. Primary Key \n2. Not Null \n3. Auto INCR \n0.Default");
        scanf("%d",&cons);
        if(cons == 1){
            if(PRIMARY_KEY_COL_NO == -1)    PRIMARY_KEY_COL_NO = i;
            else                            ERR_MESG("Only 1 Primary Key can be assigned");
        }   
        
        // make 1 Byte rep of type and constraint;
        col[i].data_type = get_type(type, cons);
        scanf("%d",&sz);
        col[i].size = char(sz);
        
        switch(type)
        {
            case INTEGER:   assert(sz <= INT_MAX_DIGIT); break;
            case STRING :   assert(sz <= STR_MAX_SIZE); break;
            default :  assert(0 == 0);
        }

        // length calculation
        RECORD_SIZE += type == STRING ? sz * TYPE_SIZE[type] : TYPE_SIZE[type];
        CUM_POS[i] = i == 0 ? 0 : CUM_POS[i-1] + (type == STRING ? sz * TYPE_SIZE[type] : TYPE_SIZE[type]);
    }

    write_to_file();
    fclose(fptr);
    printf("Database Created Successfully....\n");
    return TABLE_NAME;
}

