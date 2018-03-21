/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/

#include <bits/stdc++.h>
#include "reader.h"
#include "operations.h"
using namespace std;

void show_schema(const char *filename)
{
    if(!IS_READ){
        printf("\nSHOW_SCHEMA : Opening file.... %s\n",filename);
        read_from_file(filename);
    }
    printf("Relation Schema: \n");
    printf("----------------------------------------\n");
    for(int i = 0 ; i < NO_COLUMNS; i++){
        cout << col[i].col_name << "\t";
        cout << types[DATA_TYPES[i]] << "\t";
        cout << "(" << int(col[i].size) << ")\t";
        printf("\n");
    }
    printf("\nNO of Records : %d\n",NO_RECORDS);
    printf("Primary Key : %s\n",col[PRIMARY_KEY_COL_NO].col_name);
    printf("----------------------------------------\n");
}

/********************************************* INSERT FUNCTION ***********************************************/

void insert_data(const char *filename, int num_ins = 1)
{
    if(!IS_READ){
        printf("\nINSERTION : Opening file.... %s\n",filename);
        read_from_file(filename);
    }
    FILE *fp = fopen(filename, "rb+");
    assert(fp != NULL);
    int int_data;
    char *dest = Malloc(char,255);
    char *blank = (char*)calloc(BLOCK_SIZE - (RECORD_SIZE + 2*PTR_SIZE), sizeof(char));
    assert(dest != NULL && blank != NULL);
    int last_next;
    // build index if not already built


    int record_no_ins;
    int prev = -1, next = -1;
    int iden = 0;
    if(LAST_REC_NO == 0 && TOTAL_RECORD == 0)
    {
        // No records in the database and no slot is also there 
        // after update prev = 0, next = 0
        prev = next = 0;
        LAST_REC_NO++;
        TOTAL_RECORD++;
        record_no_ins = 1;
        // update total records
        fseek(fp,TOTAL_REC_POS,SEEK_SET);
        fwrite(&TOTAL_RECORD,TOTAL_REC_SIZE,1,fp);
        // update last
        fseek(fp,LAST_REC_NO_POS,SEEK_SET);
        fwrite(&LAST_REC_NO,LAST_REC_NO_SIZE,1,fp);

        // update will be at this position
        fseek(fp,BLOCK_START(LAST_REC_NO),SEEK_SET);
        // write previous and next
        fwrite(&prev,PTR_SIZE,1,fp);
        fwrite(&next,PTR_SIZE,1,fp);

    }
    else if(LAST_REC_NO == 0 && TOTAL_RECORD > 0)
    {
        // a linked list is already built, so follow that. go to last
        LAST_REC_NO++;
        // update last
        fseek(fp,LAST_REC_NO_POS,SEEK_SET);
        fwrite(&LAST_REC_NO,LAST_REC_NO_SIZE,1,fp);

        fseek(fp,BLOCK_START(LAST_REC_NO) + 2* PTR_SIZE ,SEEK_SET);
        record_no_ins = LAST_REC_NO;
        // no need to update prev and next
    }
    else
    {
        // go to the last and read its next
        fseek(fp,BLOCK_START(LAST_REC_NO) + PTR_SIZE ,SEEK_SET);
        fread(&last_next,PTR_SIZE,1,fp);
        //printf("last next = %d\n",last_next);
        if( last_next != 0 )
        {
            // already a blank node is there.
            // go to next and write there
            LAST_REC_NO = last_next;
            // update last
            fseek(fp,LAST_REC_NO_POS,SEEK_SET);
            fwrite(&LAST_REC_NO,LAST_REC_NO_SIZE,1,fp);

            fseek(fp,BLOCK_START(last_next) + 2*PTR_SIZE,SEEK_SET);
            record_no_ins = last_next;
            // no need to update prev and next
        }
        else
        {
            // free list is empty
            TOTAL_RECORD = TOTAL_RECORD + 1;
            // write at last->next = this
            fseek(fp,BLOCK_START(LAST_REC_NO) + PTR_SIZE ,SEEK_SET);
            last_next = TOTAL_RECORD;
            fwrite(&last_next,PTR_SIZE,1,fp);
            
            fseek(fp,BLOCK_START(TOTAL_RECORD) ,SEEK_SET);
            // prev = last next = 0
            // last->next = TOTAL_RECORD
            // last = this
            prev = LAST_REC_NO;
            next = 0;
            // write previous and next
            fwrite(&prev,PTR_SIZE,1,fp);
            fwrite(&next,PTR_SIZE,1,fp);
            iden = 1;
            record_no_ins = TOTAL_RECORD;
        }
    }
    
    // key : primary key, val = record_no_ins

    // Update Index call

    // write the data
    for(int i = 0 ; i < NO_COLUMNS ; i++){
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

    if(prev != -1 && next != -1){
        // write blank after data
        fwrite(blank,sizeof(char),BLOCK_SIZE - (RECORD_SIZE + 2*PTR_SIZE),fp);    
    }
    if(iden)
    {
        // update TOTAL_RECORD
        fseek(fp,TOTAL_REC_POS,SEEK_SET);
        fwrite(&TOTAL_RECORD,TOTAL_REC_SIZE,1,fp);
        // update last
        LAST_REC_NO = TOTAL_RECORD;
        fseek(fp,LAST_REC_NO_POS,SEEK_SET);
        fwrite(&LAST_REC_NO,LAST_REC_NO_SIZE,1,fp);
    }

    ++NO_RECORDS;
    fseek(fp,NO_REC_POSITION,SEEK_SET);
    fwrite(&NO_RECORDS,RECORD_NO_SIZE,1,fp);

    //printf("Last record no: %d\n",LAST_REC_NO);
    //printf("Total Records: %d\n",TOTAL_RECORD);

    free(dest);
    free(blank);
    fclose(fp);
}

/*********************************** SELECTION ****************************************/

/**
 * Returns record numbers based on a conditions
 * @param : filename
 *          conditions (Hashmap) : if empty equivalent to SELECT *
 * 
 */
vector<int> select_data(const char* filename, FILE *fp, map<string,variant<int,string> >cond)
{
    if(!IS_READ){
        printf("\nSELECT_DATA : Opening file.... %s\n",filename);
        read_from_file(filename);
        fp = fopen(filename, "rb+");
    }
    assert(fp != NULL);
    vector<int>record_numbers;

    int int_data;
    char *dest = Malloc(char,255);
    int prev, next;
    int blockNo = FIRST_REC_NO;
    fseek(fp,BLOCK_START(blockNo),SEEK_SET);
    
    // revise
    do{
        int flag = 1;
        fread(&prev,PTR_SIZE,1,fp);
        fread(&next,PTR_SIZE,1,fp);
        for(int j = 0 ; j < NO_COLUMNS; j++)
        {
            if(DATA_TYPES[j] == INTEGER){          
                fread(&int_data,sizeof(int),1,fp);
                string s(col[j].col_name);
                if(cond.find(s) != cond.end()){
                    int ival = get<int>(cond[s]);
                    if( ival == int_data)  flag &= 1;
                    else    flag &= 0;
                }
            }
            else if(DATA_TYPES[j] == STRING){
                fread(dest,sizeof(char),255,fp);
                string s(col[j].col_name);
                if(cond.find(s) != cond.end()){
                    string temp = get<string>(cond[s]);
                    if(strcmp(temp.c_str(),dest) == 0)    flag &= 1;
                    else    flag &= 0;
                }
            }
        }
        if(flag)    record_numbers.push_back(blockNo);

        if(blockNo == LAST_REC_NO || next == 0)   break;  // end of data
        blockNo = next;
        fseek(fp,BLOCK_START(next),SEEK_SET);
    }while(true);

    free(dest);
    return record_numbers;
}

/********************************* DELETE ********************************/
/**
 * Deleted data on specified record Number
 * 
 * @param: filename, File pointer, record no *
 * */
void delete_data_from_rec(const char *filename, FILE *fp, int recNo)
{
    if(!IS_READ){
        printf("\nDELETE_DATA : Opening file.... %s\n",filename);
        read_from_file(filename);
        fp = fopen(filename, "rb+");
    }
    if(NO_RECORDS == 0) {
        printf("No Data In this Table\n");
        return ;
    }
    int prev, next;
    assert(fp != NULL);
    fseek(fp,BLOCK_START(recNo) , SEEK_SET); 
    fread(&prev,PTR_SIZE,1,fp); // prev = cur->prev
    fread(&next,PTR_SIZE,1,fp); // next = cur->next

    if(recNo == LAST_REC_NO)
    {
        int temp;
        // read its previous
        fseek(fp,BLOCK_START(LAST_REC_NO) , SEEK_SET);
        fread(&temp,PTR_SIZE,1,fp);
        // update last record no
        LAST_REC_NO = temp;
        fseek(fp,LAST_REC_NO_POS,SEEK_SET);
        fwrite(&LAST_REC_NO,LAST_REC_NO_SIZE,1,fp);
        --NO_RECORDS;
        fseek(fp,NO_REC_POSITION,SEEK_SET);
        fwrite(&NO_RECORDS,RECORD_NO_SIZE,1,fp);
        if(NO_RECORDS == 0)
        {
            // reset all
            LAST_REC_NO = 0;
            FIRST_REC_NO = 1;
            TOTAL_RECORD = 0;
            fseek(fp,FIRST_REC_NO_POS,SEEK_SET);
            fwrite(&FIRST_REC_NO,FIRST_REC_NO_SIZE,1,fp);
            fseek(fp,LAST_REC_NO_POS,SEEK_SET);
            fwrite(&LAST_REC_NO,LAST_REC_NO_SIZE,1,fp);
            fseek(fp,TOTAL_REC_POS,SEEK_SET);
            fwrite(&TOTAL_RECORD,TOTAL_REC_SIZE,1,fp);
        }
        return ;
    }

    if(recNo == FIRST_REC_NO)
    {
        FIRST_REC_NO = next;
        fseek(fp,FIRST_REC_NO_POS,SEEK_SET);
        fwrite(&FIRST_REC_NO,FIRST_REC_NO_SIZE,1,fp);
        // update its previous = 0
        int temp = 0;
        fseek(fp,BLOCK_START(FIRST_REC_NO), SEEK_SET);
        fread(&temp,PTR_SIZE,1,fp);
    }
    else{
        // go to next block and update its previous to prev
        fseek(fp,BLOCK_START(next), SEEK_SET);
        fwrite(&prev,PTR_SIZE,1,fp);

        // go to 'prev' block and change its next to 'next'
        fseek(fp,BLOCK_START(prev) + PTR_SIZE , SEEK_SET);
        fwrite(&next,PTR_SIZE,1,fp);
    }

    // go to last and read its next
    int temp;
    fseek(fp,BLOCK_START(LAST_REC_NO) + PTR_SIZE ,SEEK_SET);
    fread(&temp,PTR_SIZE,1,fp); // temp = last->next
    fseek(fp,BLOCK_START(LAST_REC_NO) + PTR_SIZE ,SEEK_SET);
    fwrite(&recNo,PTR_SIZE,1,fp); // last->next = curr
    prev = LAST_REC_NO; // curr->prev = last
    fseek(fp,BLOCK_START(recNo), SEEK_SET);
    fwrite(&prev,PTR_SIZE,1,fp);
    fwrite(&temp,PTR_SIZE,1,fp);  //curr->next = temp
    if(temp != 0){
        fseek(fp,BLOCK_START(temp), SEEK_SET);
        fwrite(&recNo,PTR_SIZE,1,fp);// temp->prev = curr
    }
    --NO_RECORDS;
    fseek(fp,NO_REC_POSITION,SEEK_SET);
    fwrite(&NO_RECORDS,RECORD_NO_SIZE,1,fp);
    if(NO_RECORDS == 0)
    {
        // reset all
        LAST_REC_NO = 0;
        FIRST_REC_NO = 1;
        TOTAL_RECORD = 0;
        fseek(fp,FIRST_REC_NO_POS,SEEK_SET);
        fwrite(&FIRST_REC_NO,FIRST_REC_NO_SIZE,1,fp);
        fseek(fp,LAST_REC_NO_POS,SEEK_SET);
        fwrite(&LAST_REC_NO,LAST_REC_NO_SIZE,1,fp);
        fseek(fp,TOTAL_REC_POS,SEEK_SET);
        fwrite(&TOTAL_RECORD,TOTAL_REC_SIZE,1,fp);
    }
}

void delete_data(const char *filename, map<string,variant<int,string> >cond)
{
    FILE *fp = fopen(filename, "rb+");
    vector<int>record_no = select_data(filename, fp, cond);
    for(int r_no : record_no)
        delete_data_from_rec(filename,fp, r_no);
    fclose(fp);
    printf("%d records deleted\n",(int)record_no.size());    
}

/******************************* DISPLAY ********************************/

/**
 * Show data s from the database
 * @param : filename
 *          conditions (Hashmap) : if empty equivalent to SELECT *
 * 
 */
void show_data(const char *filename, map<string,variant<int,string> >cond)
{
    if(!IS_READ){
        printf("\nSHOW_DATA : Opening file.... %s\n",filename);
        read_from_file(filename);
    }
    if(NO_RECORDS == 0) {
        printf("No Data In this Table\n");
        return ;
    }
    FILE *fp = fopen(filename, "rb+");
    assert(fp != NULL);
    int int_data;
    char *dest = Malloc(char,255);
    if(cond.empty()){
        // No condition show all
        for(int i = 0 ; i < NO_COLUMNS; i++)
            printf("%-12s",col[i].col_name);
        printf("\n");
        for(int i = 0 ; i < NO_COLUMNS; i++)
            printf("----------");
        printf("\n");
        //printf("FIRST REC NO %d\n",FIRST_REC_NO);
        //printf("Last rec no: %d\n",LAST_REC_NO);
        int current = FIRST_REC_NO;
        fseek(fp,BLOCK_START(current),SEEK_SET);
        int prev, next;
        do{
            //printf("current : %d ",current);
            fread(&prev,PTR_SIZE,1,fp);
            //printf("previous : %-12d",prev);
            fread(&next,PTR_SIZE,1,fp);
            //printf("next : %-12d",next);
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
            //system("read n");
            if(current == LAST_REC_NO || next == 0)   break;  // end of data
            fseek(fp,BLOCK_START(next),SEEK_SET);
            current = next;
            printf("\n");
        }while(true);
        printf("\n");
        //free(dest);
    }
    else{
        vector<int>rec_no = select_data(filename,fp,cond);
        //printf("got returned from selection\n");
        //system("read n");
        if(rec_no.size() == 0){
            printf("No records found!\n");
            return ;
        }
        for(int i = 0 ; i < NO_COLUMNS; i++)
            printf("%-12s",col[i].col_name);
        printf("\n");
        for(int i = 0 ; i < NO_COLUMNS; i++)
            printf("----------");
        printf("\n");
        for(int r : rec_no){
            fseek(fp,BLOCK_START(r),SEEK_SET);
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
    }
    free(dest);
    fclose(fp);
}

