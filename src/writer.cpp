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
    //ofile.open(fname,ios::out|ios::binary);
    //if(!ofile)      ERR_MESG("writer.cpp : file cannot be opened!")
    
}

void write_file_header()
{
    fwrite("CS",sizeof(char),FILE_HEADER_SIZE,fptr);
    //ofile.write("CS",sizeof(char)*FILE_HEADER_SIZE);
}

void write_timestamp()
{

}

void write_no_records()
{
    fwrite(&NO_RECORDS,RECORD_NO_SIZE,1,fptr);
}

void write_no_columns()
{
    fwrite(&NO_COLUMNS,COLUMN_NO_SIZE,1,fptr);
}

void write_table_name()
{
    fwrite(TABLE_NAME,sizeof(char),TABLE_NAME_SIZE,fptr);
}

void write_data_head()
{
    //printf("writing at %d\n",ftell(fptr));
    DATA_HEAD = ftell(fptr) + DATA_HEAD_SIZE + DATA_END_SIZE ;
    DATA_HEAD += sizeof(int);   // storing last recordNO
    DATA_HEAD += NO_COLUMNS * sizeof(struct Column);
    fwrite(&DATA_HEAD, DATA_HEAD_SIZE, 1, fptr);
}

void write_data_end()
{
    DATA_END = D_END;
    fwrite(&DATA_END, DATA_END_SIZE, 1, fptr);
}

void write_size_of_records()
{
    fwrite(&RECORD_SIZE,RECORD_SIZE_SIZE,1,fptr);
}

void write_first_rec_no()
{
    fwrite(&FIRST_REC_NO,FIRST_REC_NO_SIZE,1,fptr);
}


void write_last_rec_no()
{
    fwrite(&LAST_REC_NO,LAST_REC_NO_SIZE,1,fptr);
}

void write_total_rec()
{
    fwrite(&TOTAL_RECORD,TOTAL_REC_SIZE,1,fptr);
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
    write_no_records();
    write_no_columns();
    write_table_name();
    write_size_of_records();
    write_data_head();
    write_data_end();
    write_first_rec_no();
    write_last_rec_no();
    write_total_rec();
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
    int type, cons;
    PRIMARY_KEY_COL_NO = -1;
    LAST_REC_NO = 0;
    FIRST_REC_NO = 1;
    TOTAL_RECORD = 0;
    for(int i = 0 ; i < NO_COLUMNS; i++)
    {
        //printf("Enter Column Name: ");
        scanf("%s",col[i].col_name);
        col[i].index = i + 1;
        //printf("Enter Value Type: \n");
        //printf("\n1. Number \n2. Double \n3. String \n4. Date \n5. Time \n");
        scanf("%d",&type);
        data_types[i] = type;
        //printf("Constraint : \n1. Primary Key \n2. Not Null \n3. Auto INCR \n");
        scanf("%d",&cons);
        if(cons == 1){
            if(PRIMARY_KEY_COL_NO == -1)
                PRIMARY_KEY_COL_NO = i;
            else
                ERR_MESG("Only 1 Primary Key can be assigned");
        }   
        // make 1 Byte rep of type and constraint;
        char data_type = get_type(type, cons);
        col[i].data_type = data_type;
        scanf("%d",&col[i].size);
        if(type == 1)   assert(col[i].size <= 9); // +1e9

        //string s(col[i].col_name);
        //COL_NT[s] = type;
        RECORD_SIZE += TYPE_SIZE[data_types[i]];
        CUM_POS[i] = i == 0 ? 0 : CUM_POS[i-1] + TYPE_SIZE[data_types[i]];
    }

    // seek upto RECORD_SIZE
    // get the next pointer
    //printf("RECORD SIZE: %d\n",RECORD_SIZE);
    TOTAL_RECORD_SIZE = RECORD_SIZE + 2 * PTR_SIZE;
    //printf("RECORD SIZE with pointer: %d\n",TOTAL_RECORD_SIZE);
    //printf("EXTRA SIZE : %d\n",BLOCK_SIZE - TOTAL_RECORD_SIZE);
    
    write_to_file();
    fclose(fptr);
    printf("Database Created Successfully....\n");
    return TABLE_NAME;
}

