/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/

#include "metadata.h"
#include "reader.h"
#include <cstdio>
using namespace std;

FILE *frptr;


int open_file(const char *filename)
{
    //if(frptr != NULL)   fclose(frptr);
    frptr = fopen(filename,"rb");
    if(frptr == NULL){   
        WARN_MESG("Table not present\n");
        return -1;
    }
    return 0;
}

//check if the file is actually in the correct format
int check_validity(FILE *fp)
{
    assert(fp != NULL);
    char *dest = Calloc(char,FILE_HEADER_SIZE+1);
    fread(dest,sizeof(char),FILE_HEADER_SIZE,fp);
    int ret = strcmp("CS",dest);
    //free(dest);
    return ret == 0;
}

void read_record_no()
{
    assert(frptr != NULL);
    fseek(frptr,NO_REC_POSITION,SEEK_SET);
    fread(&NO_RECORDS,RECORD_NO_SIZE,1,frptr);
}

void read_timestamp(FILE *fp)
{
    assert(fp!= NULL);
    fseek(fp,DATE_TIME_POSITION,SEEK_SET);
    fread(&LAST_MOD_TIME,DATE_TIME_SIZE,1,fp);
}

void read_column_no()
{
    assert(frptr != NULL);
    fread(&NO_COLUMNS,COLUMN_NO_SIZE,1,frptr);
}

void read_table_name()
{
    assert(frptr != NULL);
    TABLE_NAME = Malloc(char, TABLE_NAME_SIZE+1);
    assert(TABLE_NAME != NULL);
    fread(TABLE_NAME,sizeof(char),TABLE_NAME_SIZE,frptr);
    TABLE_NAME[TABLE_NAME_SIZE] = '\0';
}

void read_record_size()
{
    assert(frptr != NULL);
    fread(&RECORD_SIZE,RECORD_SIZE_SIZE,1,frptr);
}

void read_data_head()
{
    assert(frptr != NULL);
    fread(&DATA_HEAD,DATA_HEAD_SIZE,1,frptr);
}

void read_data_end()
{
    assert(frptr != NULL);
    fread(&DATA_END,DATA_END_SIZE,1,frptr);
}

void read_columns()
{
    fseek(frptr,COL_DESC_POS,SEEK_SET);
    fread(col, sizeof(struct Column), NO_COLUMNS , frptr); 
}

void read_first_rec_no()
{
    fread(&FIRST_REC_NO,FIRST_REC_NO_SIZE,1,frptr);
}

void read_last_rec_no()
{
    fread(&LAST_REC_NO,FIRST_REC_NO_SIZE,1,frptr);
}

void read_total_rec()
{
    fread(&TOTAL_RECORD,TOTAL_REC_SIZE,1,frptr);
}

struct datetime *get_date_time()
{
    long tm = LAST_MOD_TIME;
    struct datetime *dt = Malloc(struct datetime,1);
    dt->ss   = tm % 100;
    tm /= 100;
    dt->mm   = tm % 100;
    tm /= 100;
    dt->hh   = tm % 100;
    tm /= 100;
    dt->day   = tm % 100;
    tm /= 100;
    dt->month   = tm % 100;
    tm /= 100;
    dt->year   = tm % 100;
    return dt;
}

/*****
 * 
 * Auxiliary function for getting type and constraint
 **/

int get_type_int(char type)
{
    return ((112 & type) >> 5) + 1;
}

int get_const_int(char type)
{
    return ((28 & type) >> 2);
}


int read_from_file(const char *filename)
{
    //printf("Enter filename to be read: \n");
    //scanf("%s",file_name);
    if(open_file(filename) == -1)   return -1;

    if(!check_validity(frptr))
        ERR_MESG("reader : file format is not valid!");

    read_timestamp(frptr);
    read_record_no();
    read_column_no();
    read_table_name();
    read_record_size();
    read_data_head();
    read_data_end();
    read_first_rec_no();
    read_last_rec_no();
    read_total_rec();
    

    #if READ_DEBUG
        printf("File Details: \n\n");
        printf("Records : %d\n",NO_RECORDS);
        printf("Columns : %d\n",NO_COLUMNS);
        printf("Table Name : %s\n",TABLE_NAME);
        printf("Each Record Size: %d\n",RECORD_SIZE);
        printf("Data Head : %d\n",DATA_HEAD);
        printf("Data End : %d\n",DATA_END);
        printf("first record no: %d\n",FIRST_REC_NO);
        printf("Last record no: %d\n",LAST_REC_NO);
    #endif

    col = Malloc(struct Column, NO_COLUMNS);
    assert(col != NULL);
    DATA_TYPES = Malloc(int,NO_COLUMNS);
    assert(DATA_TYPES != NULL);
    CONST = Malloc(int,NO_COLUMNS);
    assert(CONST != NULL);
    
    read_columns();
    PRIMARY_KEY_COL_NO = -1;

    for(int i = 0 ; i < NO_COLUMNS ; i++){
        DATA_TYPES[i] = get_type_int(col[i].data_type);
        #if READ_DEBUG
            printf("%d ",col[i].data_type);
            printf("%s \t %d ",col[i].col_name,DATA_TYPES[i]);
            printf("%d \n",int(col[i].size));
        #endif
        string s(col[i].col_name);
        COL_NT[s] = DATA_TYPES[i];
        CONST[i] = get_const_int(col[i].data_type);    // 0 : primary key
        if(CONST[i] == 0)
            PRIMARY_KEY_COL_NO = i;
    }
    IS_READ = 1;
    if(OPEN_FILE == NULL)  
        OPEN_FILE = Malloc(char,strlen(filename)+1);
    else{                    
        free(OPEN_FILE);
        OPEN_FILE = Malloc(char,strlen(filename)+1);
    }
    strncpy(OPEN_FILE,filename,strlen(filename));
    fclose(frptr);
    
    
    #if READ_DEBUG
        printf("Reading Complete..... Closing..\n");
    #endif

    return 0;
}