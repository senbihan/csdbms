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
    DATA_HEAD = ftell(fptr) + DATA_END_SIZE + NO_COLUMNS * sizeof(struct Column);
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
void create_db()
{
    TABLE_NAME = Malloc(char, TABLE_NAME_SIZE);
    //printf("Enter database name: \n");
    scanf("%s",TABLE_NAME);
    create_db_file(TABLE_NAME);
    //printf("Enter Number of Column: \n");
    scanf("%d",&NO_COLUMNS);
    NO_RECORDS = 0; // initially no data

    col = Malloc(struct Column, NO_COLUMNS);
    data_types = Malloc(int, NO_COLUMNS);
    assert(col != NULL && data_types != NULL);
    int type, cons;

    for(int i = 0 ; i < NO_COLUMNS; i++)
    {
        //printf("Enter Column Name: ");
        scanf("%s",col[i].col_name);
        col[i].index = i + 1;
        //printf("Enter Value Type: \n");
        //printf("\n1. Number \n2. Double \n3. String \n4. Date \n5. Time \n");
        scanf("%d",&type);
        //printf("Constraint : \n1. Primary Key \n2. Not Null \n3. Auto INCR \n");
        data_types[i] = type;
        scanf("%d",&cons);
        // make 1 Byte rep of type and constraint;
        char data_type = get_type(type, cons);
        col[i].data_type = data_type;
        //only one primary key check to be implemented
        //printf("Size of data \n");
        scanf("%d",&col[i].size);
        if(type == 1)   assert(col[i].size <= 9); // +1e9

        if(type == 1)   RECORD_SIZE += sizeof(int);
        //if(type == 2)   REC_SIZE += 0;        
        if(type == 3)   RECORD_SIZE += 255; // char of size 255
        //if(type == 4)   REC_SIZE
    }

    
    write_to_file();
    /*printf("Writing...\n");
    printf("Data Head : %d\n",DATA_HEAD);
    printf("No of Records : %d\n",NO_RECORDS);
    printf("Record Size: %d\n",RECORD_SIZE);
    printf("Data End: %d\n",DATA_END);
    */
    fclose(fptr);
    printf("File written Successfully....\n");
}



/**
 * Insert Data
 * On existing file
 **/
void insert_data(const char *filename)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
{
    fstream fp (filename, fstream::in | fstream::binary);
    fp.seekg(NO_REC_POSITION);
    fp.read((char*)&NO_RECORDS,RECORD_NO_SIZE);
    fp.seekg(REC_SIZE_POSITIOn);
    fp.read((char*)&RECORD_SIZE,RECORD_SIZE_SIZE);
    //fp.seekg(DATA_HEAD_POSITION);
    fp.read((char*)&DATA_HEAD,DATA_END_SIZE);
    //fp.seekg(DATA_END_POSITION);
    fp.read((char*)&DATA_END,DATA_END_SIZE);
    fp.close();

    printf("Before Insertion: \n");
    printf("Data Head : %d\n",DATA_HEAD);
    printf("No of Records : %d\n",NO_RECORDS);
    printf("Record Size: %d\n",RECORD_SIZE);
    printf("Data End: %d\n",DATA_END);
    fp.open(filename, fstream::out | fstream::binary);
    fp.seekp(DATA_END);

    {
        // insert rows
        
        NO_RECORDS++;
    }

    fp.seekg(DATA_END_POSITION);
    DATA_END = D_END;
    
    printf("After Insertion: \n");
    printf("Data Head : %d\n",DATA_HEAD);
    printf("No of Records : %d\n",NO_RECORDS);
    printf("Record Size: %d\n",RECORD_SIZE);
    printf("Data End: %d\n",DATA_END);
    
    fp.write((char*)&DATA_END,DATA_END_SIZE);
    fp.close();
}
