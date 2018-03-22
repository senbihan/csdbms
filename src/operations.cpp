/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/

#include <bits/stdc++.h>
#include <variant>
#include "reader.h"
#include "writer.h"
#include "operations.h"
using namespace std;

void show_schema(const char *filename)
{
    if(!IS_READ){
        cout << "\nSHOW_SCHEMA : Opening file.... " << filename << "\n";
        read_from_file(filename);
    }
    printf("Relation Schema: \n");
    printf("----------------------------------------\n");
    printf("----------------------------------------\n");
    for(int i = 0 ; i < NO_COLUMNS; i++){
        cout << col[i].col_name << "\t";
        cout << types[DATA_TYPES[i]] << "\t";
        cout << "(" << int(col[i].size) << ")\t";
        printf("\n");
    }
    printf("\nNumber of Records present: %d\n",NO_RECORDS);
    printf("Primary Key : %s\n",col[PRIMARY_KEY_COL_NO].col_name);
    printf("----------------------------------------\n");
    printf("----------------------------------------\n");
    
}






/********************************************* INSERT FUNCTION ***********************************************/

bool is_valid_int(string s)
{
    unsigned i = 0;
    if(s[i] == '-') i++;
    for( ; i < s.length() ; i++)
        if(s[i] < '0' || s[i] > '9')
            return false;
    return true;
}

void insert_data(const char *filename, int num_ins, char **values)
{
    if(!IS_READ){
        cout << "\nINSERT DATA : Opening file.... " << filename << "\n";
        read_from_file(filename);
    }
    FILE *fp = fopen(filename, "rb+");
    assert(fp != NULL);
    char *blank = (char*)calloc(BLOCK_SIZE - (RECORD_SIZE + 2*PTR_SIZE), sizeof(char));
    assert(blank != NULL);
    // build index if not already built


    int record_no_ins, last_next;
    int prev = -1, next = -1, iden = 0;
    if(LAST_REC_NO == 0 && TOTAL_RECORD == 0)
    {
        // No records in the database and no slot is also there 
        // after update prev = 0, next = 0
        prev = next = 0;
        LAST_REC_NO++;
        TOTAL_RECORD++;
        record_no_ins = 1;
        // update total records
        write_total_rec(fp);
        // update last
        write_last_rec_no(fp);
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
        write_last_rec_no(fp);
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
            write_last_rec_no(fp);

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
    int temp;
    for(int i = 0 ; i < NO_COLUMNS ; i++)
    {
        switch(DATA_TYPES[i])
        {
            case INTEGER :      
                temp = atoi(values[i]);
                fwrite(&temp,sizeof(int),1,fp);
                break;
            case STRING :
                fwrite(values[i],sizeof(char),(unsigned int)col[i].size,fp);
                break;
            default :
                break;
        }
    }

    if(prev != -1 && next != -1){
        // write blank after data
        fwrite(blank,sizeof(char),BLOCK_SIZE - (RECORD_SIZE + 2*PTR_SIZE),fp);    
    }
    if(iden)
    {
        // update TOTAL_RECORD
        write_total_rec(fp);
        // update last
        LAST_REC_NO = TOTAL_RECORD;
        write_last_rec_no(fp);
    }

    ++NO_RECORDS;
    write_no_records(fp);
    write_data_end(fp);
    free(blank);
    fclose(fp);
}

bool insert_data(const char *filename)
{
    if(!IS_READ){
        cout << "\nINSERT DATA : Opening file.... " << filename << "\n";
         read_from_file(filename);
    }

    char *values[NO_COLUMNS];
    string stemp;
    int i;
    bool inserted = true;
    for(i = 0 ; i < NO_COLUMNS ; i++)
    {
        cin >> stemp;
        switch(DATA_TYPES[i])
        {         
            case INTEGER :
                if(!is_valid_int(stemp)){
                    cerr << "DATATYPE MISMATCH ERROR\n";
                    inserted = false;
                    break;
                }
                assert(stemp.length() <= (unsigned int)(col[i].size));
                values[i] = Malloc(char,4);
                strncpy(values[i],stemp.c_str(),4);
                break;

            case STRING :
                assert(stemp.length() <= (unsigned int)(col[i].size));
                values[i] = Malloc(char,stemp.length());
                strncpy(values[i],stemp.c_str(),(unsigned int)(col[i].size));
                break;
            
            default:
                break;
        }
    }
    if(inserted) insert_data(filename,1,values);
    //for(int j = 0 ; j < i ; i++)   free(values[j]);
    return inserted;
}

/*********************************** Insert Ends Here **********************************/












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

inline void reset_all(FILE *fp)
{
    LAST_REC_NO = 0;
    FIRST_REC_NO = 1;
    TOTAL_RECORD = 0;
    write_first_rec_no(fp);
    write_last_rec_no(fp);
    write_total_rec(fp);
}

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
        write_last_rec_no(fp);
        --NO_RECORDS;
        write_no_records(fp);
        if(NO_RECORDS == 0) reset_all(fp);      // reset
        return ;
    }

    if(recNo == FIRST_REC_NO)
    {
        // if the first record is deleted
        // change the first to the next of first
        FIRST_REC_NO = next;
        write_first_rec_no(fp);
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
    prev = LAST_REC_NO; 
    fseek(fp,BLOCK_START(recNo), SEEK_SET);
    fwrite(&prev,PTR_SIZE,1,fp);  // curr->prev = last
    fwrite(&temp,PTR_SIZE,1,fp);  //curr->next = temp
    if(temp != 0){
        fseek(fp,BLOCK_START(temp), SEEK_SET);
        fwrite(&recNo,PTR_SIZE,1,fp);// temp->prev = curr
    }
    --NO_RECORDS;
    write_no_records(fp);
    if(NO_RECORDS == 0) reset_all(fp);      // reset
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
        // print Column Head
        for(int i = 0 ; i < NO_COLUMNS; i++)
            printf("%-20s",col[i].col_name);
        printf("\n");
        for(int i = 0 ; i < NO_COLUMNS; i++)
            for(int j = 0 ; j < int(col[i].size); j++)
                printf("--");
        printf("\n");
        
        int current = FIRST_REC_NO;
        fseek(fp,BLOCK_START(current),SEEK_SET);
        int prev, next;
        do{
            fread(&prev,PTR_SIZE,1,fp);
            fread(&next,PTR_SIZE,1,fp);
            for(int j = 0 ; j < NO_COLUMNS; j++){
                if(DATA_TYPES[j] == 1){          
                    fread(&int_data,sizeof(int),1,fp);
                    printf("%-20d",int_data);
                }
                else if(DATA_TYPES[j] == 3){
                    fread(dest,sizeof(char),255,fp);
                    printf("%-20s",dest);
                }
            }
            if(current == LAST_REC_NO || next == 0)   break;  // end of data
            fseek(fp,BLOCK_START(next),SEEK_SET);
            current = next;
            printf("\n");
        }while(true);
        printf("\n");
    }
    else{
        vector<int>rec_no = select_data(filename,fp,cond);
        if(rec_no.size() == 0){
            printf("No records found!\n");
            return ;
        }
        
        for(int i = 0 ; i < NO_COLUMNS; i++)
            printf("%-20s",col[i].col_name);
        printf("\n");
        for(int i = 0 ; i < NO_COLUMNS; i++)
            for(int j = 0 ; j < int(col[i].size); j++)
                printf("--");
        printf("\n");

        for(int r : rec_no){
            fseek(fp,BLOCK_START(r) + 2*PTR_SIZE,SEEK_SET);
            for(int j = 0 ; j < NO_COLUMNS; j++){
                if(DATA_TYPES[j] == 1){          
                    fread(&int_data,sizeof(int),1,fp);
                    printf("%-20d",int_data);
                }
                else if(DATA_TYPES[j] == 3){
                    fread(dest,sizeof(char),255,fp);
                    printf("%-20s",dest);
                }
            }
            printf("\n");
        }
    }
    free(dest);
    fclose(fp);
}

