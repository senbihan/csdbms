#include <bits/stdc++.h>
#include "reader.h"
#include "insert.h"

using namespace std;

void show_schema(const char *filename)
{
    printf("\nSHOW_SCHEMA : Opening file.... %s\n",filename);
    read_from_file(filename);

    printf("Relation Schema: \n");
    printf("----------------------------------------\n");
    for(int i = 0 ; i < NO_COLUMNS; i++){
        cout << col[i].col_name << "\t";
        cout << types[DATA_TYPES[i]] << "\t";
        cout << "(" << col[i].size << ")\t";
        printf("\n");
    }
    printf("\nNO of Records : %d\n",NO_RECORDS);
    printf("----------------------------------------\n");
}

void insert_data(const char *filename, int num_ins = 1)
{
    printf("\nINSERTION : Opening file.... %s\n",filename);
    read_from_file(filename);
    FILE *fp = fopen(filename, "r+");
    fseek(fp,DATA_END,SEEK_SET);

    int times = num_ins;
    while(times--){
        char *dest = (char*)malloc(255);
        assert(dest != NULL);
        int int_data;
        for(int i = 0 ; i < NO_COLUMNS; i++)
        {
            //fread(dest,TYPE_SIZE[DATA_TYPES[i]],1,stdin);
            if(DATA_TYPES[i] == 1){          
                scanf("%d",&int_data);
                assert(int_data <= MAX_INTEGER);
                fwrite(&int_data,int(sizeof(int)),1,fp);
            }
            else if(DATA_TYPES[i] == 3){
                scanf("%s",dest);
                assert(strlen(dest) <= 255);
                fwrite(dest,sizeof(char),255,fp);
            }
        }
    }
    NO_RECORDS += num_ins;
    fseek(fp,NO_REC_POSITION,SEEK_SET);
    fwrite(&NO_RECORDS,RECORD_NO_SIZE,1,fp);
    DATA_END = D_END;
    fseek(fp,DATA_END_POSITION,SEEK_SET);
    fwrite(&DATA_END,DATA_END_SIZE,1,fp);
    printf("%d row inserted.\n",num_ins);
    fclose(fp);
}

void show_data(const char *filename)
{
    printf("\nSHOW_DATA : Opening file.... %s\n",filename);
    read_from_file(filename);

    FILE *fp = fopen(filename, "r");
    fseek(fp,DATA_HEAD,SEEK_SET);
    for(int i = 0 ; i < NO_COLUMNS; i++)
        printf("%-12s",col[i].col_name);
    printf("\n");
    printf("----------------------------------------------------------\n");
    int int_data;
    char *dest = Malloc(char,255);
    for(int i = 0 ; i < NO_RECORDS; i++){
        for(int j = 0 ; j < NO_COLUMNS; j++){
             if(DATA_TYPES[j] == 1){          
                fread(&int_data,sizeof(int),1,fp);
                printf("%-12d",int_data);
            }
            else if(DATA_TYPES[j] == 3){
                fread(dest,sizeof(char),255,fp);
                printf("%-12s",dest);
           }
        }
        printf("\n");
    }

    fclose(fp);
}

