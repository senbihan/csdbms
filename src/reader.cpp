#include "metadata.h"
#include "reader.h"
#include <cstdio>
using namespace std;

FILE *frptr;
int *DATA_TYPES;


void open_file(char *filename)
{
    frptr = fopen(filename,"rb");
    if(frptr == NULL)   ERR_MESG("reader : File cannot be opened or doesnot exist!");
}

//check if the file is actually in the correct format
int check_validity()
{
    assert(frptr != NULL);
    char *dest = Malloc(char,FILE_HEADER_SIZE);
    memset(dest,0,sizeof(dest));
    fread(dest,sizeof(char),FILE_HEADER_SIZE,frptr);
    return (strcmp("CS",dest) == 0);
}

void read_record_no()
{
    assert(frptr != NULL);
    fread(&NO_RECORDS,RECORD_NO_SIZE,1,frptr);
}

void read_column_no()
{
    assert(frptr != NULL);
    fread(&NO_COLUMNS,COLUMN_NO_SIZE,1,frptr);
}

void read_table_name()
{
    assert(frptr != NULL);
    TABLE_NAME = Malloc(char, TABLE_NAME_SIZE);
    assert(TABLE_NAME != NULL);
    fread(TABLE_NAME,sizeof(char),TABLE_NAME_SIZE,frptr);
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

int get_type_int(char type)
{
    return ((112 & type) >> 5) + 1;
}

void read_from_file()
{
    char file_name[TABLE_NAME_SIZE];
    //printf("Enter filename to be read: \n");
    scanf("%s",file_name);
    open_file(file_name);

    if(!check_validity())
        ERR_MESG("reader : file format is not valid!");
    printf("File format is valid. Read successfully!\n");

    read_record_no();
    printf("File Details: \n\n");
    printf("Records : %d\n",NO_RECORDS);
    read_column_no();
    printf("Columns : %d\n",NO_COLUMNS);
    read_table_name();
    printf("Table Name : %s\n",TABLE_NAME);
    read_record_size();
    printf("Each Record Size: %d\n",RECORD_SIZE);
    read_data_head();
    printf("Data Head : %d\n",DATA_HEAD);


    col = Malloc(struct Column, NO_COLUMNS);
    assert(col != NULL);
    DATA_TYPES = Malloc(int,NO_COLUMNS);
    assert(DATA_TYPES != NULL);

    // read all column details to col[]
    fread(col,sizeof(struct Column),NO_COLUMNS,frptr);

    for(int i = 0 ; i < NO_COLUMNS ; i++){
        DATA_TYPES[i] = get_type_int(col[i].data_type);
        col[i].print_col();
    }

    fclose(frptr);
}