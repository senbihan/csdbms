/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/

#include <bits/stdc++.h>
#include <variant>
#include "reader.h"
#include "writer.h"
#include "operations.h"
#include "index_operations.h"
#include "datatypes.h"
using namespace std;

void show_schema(char *filename)
{
    char *fname = table_to_file_name(filename);
    #if DEBUG
        printf("%s\n",fname);
    #endif
    if(!IS_READ || strcmp(OPEN_FILE,fname) != 0){
        //cout << "\nSHOW_SCHEMA : Opening file.... " << filename << "\n";
        if(read_from_file(fname) == -1)
            return ;
    }
    
    printf("\n\nRelation Schema: %14s\n",TABLE_NAME);
    printf("----------------------------------------\n");
    printf("----------------------------------------\n");
    for(int i = 0 ; i < NO_COLUMNS; i++){
        printf("%-12s",col[i].col_name);
        printf("%-12s",types[DATA_TYPES[i]].c_str());
        if(DATA_TYPES[i] == DOUBLE)
        {
            int sz          = int(col[i].size);
            int int_part    = (sz & 240) >> 4;
            int frac_part   = (sz & 15);
            printf("(%d,%d)",int_part,frac_part);
        }
        else if(DATA_TYPES[i] == INTEGER){
            int sz  = int(col[i].size);
            sz      >>= 4;
            printf("(%d)",sz);
        }
        else
            printf("(%d)",int(col[i].size));
        
        switch(CONST[i])
        {
            case 0:
                printf("\tPRIMARY KEY");
                break;
            case 1:
                printf("\tNOT NULL");
                break;
            case 2:
                printf("\tAUTO INCR");
                break;
            case 3:
                printf("\tDEFAULT - No constraint");
                break;
            default:
                printf("\tDEFAULT - No constraint");
                break;
        }
        
        printf("\n");
    }
    printf("\nNumber of Records present: %d\n",NO_RECORDS);
    printf("Primary Key : %s\n",col[PRIMARY_KEY_COL_NO].col_name);
    struct datetime *dtm = get_date_time();
    printf("Last Modified : %02d/%02d/%d %02d:%02d:%02d\n",dtm->day,dtm->month,dtm->year,dtm->hh,dtm->mm,dtm->ss);
    printf("----------------------------------------\n");
    printf("----------------------------------------\n");
    
}




/********************************************* INSERT FUNCTION ***********************************************/

void insert_data(char *filename, char **values)
{
    if(!IS_READ || strcmp(OPEN_FILE,filename) != 0){
        #if DEBUG
            printf("%s\n",OPEN_FILE);
            printf("INSERT DATA : Opening file.... %s\n", filename);
        #endif
        if(read_from_file(filename) == -1)
            return ;
    }
    FILE *fp = fopen(filename, "rb+");
    assert(fp != NULL);
    char *blank = (char*)calloc(BLOCK_SIZE - (RECORD_SIZE + 2*PTR_SIZE), sizeof(char));
    assert(blank != NULL);
    // build index if not already built


    int record_no_ins;
    int last_next;
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
            // already a blank node is there, go to next and write there
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
                #if DEBUG
                    printf("Writing : %s\n",values[i]);
                #endif
                // convert to 5 byte representation
                
                fwrite(values[i],(col[i].size >> 4)+1,1,fp);
                // update index call
                if(i == PRIMARY_KEY_COL_NO)
                {
                    temp = atoi(values[i]);
                	bool ret = indexInsert(temp,(unsigned long long)record_no_ins,table_name_to_index(TABLE_NAME));
            		if(ret == false)    WARN_MESG("Index File not updated!");
                }
                break;
            case STRING :
                fwrite(values[i],sizeof(char),(unsigned int)col[i].size,fp);
                break;
            case DOUBLE:

                #if DEBUG
                    printf("Writing : %s and length = %d\n",values[i], strlen(values[i]));
                #endif
                // convert to 9 byte representation
                
                fwrite(values[i],strlen(values[i]),1,fp);
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
    //free(blank);
    close_file(fp);
}

bool insert_data(int argc, char **argv)
{
    char *filename = table_to_file_name(argv[2]);
    if(!IS_READ || strcmp(OPEN_FILE,filename) != 0){
        #if DEBUG
            printf("%s\n",OPEN_FILE);
            //printf("%d %d %d",strlen(OPEN_FILE),strlen(filename),strcmp(OPEN_FILE,filename));
            printf("INSERT DATA : Opening file.... %s\n", filename);
        #endif
    
        if(read_from_file(filename) == -1)
            return false;
    }
    if(strcmp(argv[3],"values") != 0)
    {
        WARN_MESG("Invalid Syntax. Type 'help' to see all Syntax.");
        return false;
    }
    char **values = (char**)malloc(NO_COLUMNS*sizeof(char*));
    string stemp;
    int i, k = 4;
    bool inserted = true;
    unsigned int sz, int_part_size, frac_part_size, size;
    bool seen_dot = false; 
    string d_str;
    for(i = 0 ; i < NO_COLUMNS ; i++,k++)
    {
        stemp = string(argv[k]);
        switch(DATA_TYPES[i])
        {         
            case INTEGER :
                if(!is_valid_int(argv[k])){
                    WARN_MESG("Error : Type mismatch \n");
                    inserted = false;
                }
                size = (unsigned int)(col[i].size >> 4) + 1;
                if(!(strlen(argv[k])-1 <= size)){
                    WARN_MESG("Error : NUMBER length is too big\n");
                    inserted = false;
                }
                values[i] = Malloc(char,size);
                assert(values[i] != NULL);
                strncpy(values[i],argv[k],size);
                
                if(inserted && PRIMARY_KEY_COL_NO == i) // check if already exist in b+ tree
                {
                	if(indexFind(atoi(values[i]), table_name_to_index(TABLE_NAME)) > 0){
                		WARN_MESG("PRIMARY CONSTRAINT : This key is already present");
                		inserted = false;
                	}
                }
                #if DEBUG
                    printf("%s %u %u\n",values[i],sizeof(values[i]),strlen(values[i]));
                    printf("%d\n",(col[i].size >> 4));
                #endif

                break;

            case STRING :
                if(stemp[0] != '\'') { WARN_MESG("Strings must be quoted.\n"); inserted = false; }
                while(stemp[0] == '\'' && stemp[stemp.length()-1] != '\''){
                    if(k == argc){   
                        WARN_MESG("Invalid Syntax. Type 'help' to see all Syntax.");
                        inserted = false;
                        break;
                    }
                    stemp += " " + string(argv[++k]);
                }
                stemp = stemp.substr(1,stemp.length()-2);
                if(!(stemp.length() <= (unsigned int)(col[i].size))){
                    WARN_MESG("Error : NUMBER length is too big\n");
                    inserted = false;
                }
                values[i] = Malloc(char,(unsigned int)(col[i].size));
                strncpy(values[i],stemp.c_str(),(unsigned int)(col[i].size));
                break;
            case DOUBLE :
                sz                  = int(col[i].size);
                int_part_size       = (sz & 240) >> 4;
                frac_part_size      = (sz & 15);

                #if DEBUG
                    printf("size of real: %d\n",int_part_size + frac_part_size);
                #endif
                values[i] = Malloc(char,int_part_size + frac_part_size);
                
                for(unsigned j = 0 ; j < stemp.length() ; j++){
                    if(stemp[j] == '.') {seen_dot = true; continue;}
                    if(!seen_dot)   int_part_size--;
                    else            frac_part_size--;
                }
                if(int_part_size < 0 || frac_part_size < 0)
                {
                    WARN_MESG("Error : Not valid Real number as specified\n");
                    inserted = false;
                }
                if(stemp.find('.') != string::npos) stemp.erase(stemp.find('.'),1);
                while(frac_part_size > 0){  // add extra 0s
                    stemp += "0";
                    frac_part_size--;
                }
                strcpy(values[i],stemp.c_str());
                #if DEBUG
                    printf("copying %s to values : %s %d\n", stemp.c_str(),values[i], strlen(values[i]));
                #endif
                
                seen_dot = false;
                break;
            default:
                break;
        }
    }
    if(inserted) insert_data(filename,values);
    free(values);
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
vector<int> select_data(char* filename, FILE *fp, map<string,variant<int,string,long> >cond)
{
    #if DEBUG
        printf("%s\n",OPEN_FILE);
    #endif
    if(!(IS_READ && strcmp(OPEN_FILE,filename) == 0)){
        #if DEBUG
            printf("\nSELECT_DATA : Opening file.... %s\n",filename);
        #endif

        read_from_file(filename);
        fp = fopen(filename, "rb+");
    }
    assert(fp != NULL);
    vector<int>record_numbers;

    int int_data; //i_part, f_part;
    //long real_data;
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
            string s(col[j].col_name);
            if(DATA_TYPES[j] == INTEGER)
            {          
                fread(dest,int(col[j].size >> 4) + 1,1,fp);
                int_data = atoi(dest);
                if(cond.find(s) != cond.end())
                {
                    int ival = get<int>(cond[s]);
                    if( ival == int_data)  flag &= 1;
                    else    flag &= 0;
                }
            }
            else if(DATA_TYPES[j] == STRING)
            {
                fread(dest,sizeof(char),(unsigned int)col[j].size,fp);
                if(cond.find(s) != cond.end())
                {
                    string temp = get<string>(cond[s]);
                    if(strcmp(temp.c_str(),dest) == 0)    flag &= 1;
                    else    flag &= 0;
                }
            }
            
            /*else if (DATA_TYPES[j] == DOUBLE)
            {
                i_part = (int(col[j].size) & 240) >> 4;
                f_part = int(col[j].size) & 15;
                fread(dest,i_part + f_part,1,fp);
                real_data = atol(dest);
                if(cond.find(s) != cond.end())
                {
                    long lval = get<long>(cond[s]);
                    if( lval == real_data)  flag &= 1;
                    else    flag &= 0;
                }
            }*/
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

void delete_data_from_rec(char *filename, FILE *fp, int recNo)
{
    if(!(IS_READ && strcmp(OPEN_FILE,filename) == 0)){
        //printf("\nDELETE_DATA : Opening file.... %s\n",filename);
        if(read_from_file(filename) == -1)
            return ;
        fp = fopen(filename, "rb+");
    }
    if(NO_RECORDS == 0) {
        printf("No Data In this Table\n");
        return ;
    }
    if(NO_RECORDS == 1){
        NO_RECORDS = 0;
        write_no_records(fp);
        reset_all(fp);
        return;
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
}

void delete_data_from_db(char *filename, map<string,variant<int,string,long> >cond)
{
    FILE *fp = fopen(filename, "rb+");
    if(fp == NULL){
        WARN_MESG("Database does not exist."); return;
    }
    vector<int>record_no = select_data(filename, fp, cond);
    for(int r_no : record_no)
        delete_data_from_rec(filename,fp, r_no);
    close_file(fp);
    printf("%d records deleted\n",(int)record_no.size());    
}


void delete_data(int argc, char **argv)
{
    char *filename = table_to_file_name(argv[2]);
    if(!(IS_READ && strcmp(OPEN_FILE,filename) == 0)){
        //printf("\nSHOW_DATA : Opening file.... %s\n",filename);
        if(read_from_file(filename) == -1)
            return ;
    }
    map<string,variant<int,string,long> > cond;
    
    // delete from <table>
    if(argc == 3)
        delete_data_from_db(filename,cond);
    // only equality
    // delete from <table> where
    // col1 val1 col2 val2
    else
    {
        if(argc == 4){
            WARN_MESG("Invalid Syntax, Expecting COLUMN_NAME, VALUE pairs\n");
            return;
        }
            
        for(int i = 4 ; i < argc; i++)
        {
            string s(argv[i]);
            if(COL_NT.find(s) == COL_NT.end()){
                FAIL_MESG("No column of the name");
                return ;
            }
            if(COL_NT[s] == INTEGER){
                int int_data = atoi(argv[++i]);
                cond.insert(make_pair(s,int_data));
            }
            else if(COL_NT[s] == STRING)
            {
                string stemp(argv[++i]);
                #if DEBUG
                    cout << "key =  " << s <<  " value = " << stemp <<  "\n";
                #endif
                if(stemp[0] != '\'') { WARN_MESG("Strings must be quoted.\n");}
                while(stemp[0] == '\'' && stemp[stemp.length()-1] != '\''){
                    stemp += " " + string(argv[++i]);
                }
                stemp = stemp.substr(1,stemp.length()-2);
                #if DEBUG
                    cout << "key =  " << s <<  " value = " << stemp <<  "\n";
                #endif
                
                 cond.insert(make_pair(s,stemp));
                
            }
                // double cannot be compared
        }    
        delete_data_from_db(filename,cond);
        cond.clear();
    }
    free(filename);
}



/******************************* DISPLAY ********************************/

/**
 * Show data s from the database
 * @param : filename
 *          conditions (Hashmap) : if empty equivalent to SELECT *
 * 
 */
void show_data_from_db(char *filename, map<string,variant<int,string, long> >cond)
{
    #if DEBUG
        printf("%s\n",OPEN_FILE);
    #endif
    if(!(IS_READ && strcmp(OPEN_FILE,filename) == 0)){
        #if DEBUG
            printf("\nSHOW_DATA : Opening file.... %s\n",filename);
        #endif
        if(read_from_file(filename) == -1)
            return ;
    }
    if(NO_RECORDS == 0) {
        printf("No Data In this Table\n");
        return ;
    }
    FILE *fp = fopen(filename, "rb+");
    assert(fp != NULL);
    int i_part, f_part;
    double r_data;
    char *dest = Malloc(char,255);
    vector<int>rec_no = select_data(filename,fp,cond);
    if(rec_no.size() == 0)
    {
        OK_MESG("No records found!\n");
        return ;
    }
        
    for(int i = 0 ; i < NO_COLUMNS; i++)
        printf("%-20s",col[i].col_name);
    printf("\n");
    for(int i = 0 ; i < NO_COLUMNS; i++)
        for(int j = 0 ; j < 10; j++)
            printf("--");
    printf("\n");

    for(int r : rec_no)
    {
        fseek(fp,BLOCK_START(r) + 2*PTR_SIZE,SEEK_SET);
        for(int j = 0 ; j < NO_COLUMNS; j++)
        {
            switch(DATA_TYPES[j])
            {          
                case INTEGER:
                    fread(dest,int(col[j].size >> 4) + 1,1,fp);
                    printf("%-20s",dest);
                    break;
                case STRING :
                    fread(dest,sizeof(char),(int)col[j].size,fp);
                    printf("%-20s",dest);
                    break;
                case DOUBLE:
                    i_part = (int(col[j].size) & 240) >> 4;
                    f_part = int(col[j].size) & 15;
                    fread(dest,i_part + f_part,1,fp);
                    r_data = atol(dest) / ((double)pow(10,f_part));
                    printf("%-20.*f",f_part,r_data);
                    break;
                default:
                     break;
            }
        }
        printf("\n");
    }
    //free(dest);
    fclose(fp);
}

void show_data(int argc, char **argv)
{
    char *filename = table_to_file_name(argv[3]);
    if(!(IS_READ && strcmp(OPEN_FILE,filename) == 0)){
        //printf("\nSHOW_DATA : Opening file.... %s\n",filename);
        if(read_from_file(filename) == -1)
            return ;
    }
    map<string,variant<int,string,long> > cond;
    
    // select * from <table>
    if(strcmp(argv[1],"*") == 0)
    {
        if(argc == 4)
            show_data_from_db(filename,cond);
        // only equality
        // select * from <table> where
        // col1 val1 col2 val2
        else
        {
            if(argc == 5){
                WARN_MESG("Invalid Syntax, Expecting COLUMN_NAME, VALUE pairs\n");
                return;
            }
            
            for(int i = 5 ; i < argc; i++)
            {
                string s(argv[i]);
                if(COL_NT.find(s) == COL_NT.end()){
                    cout << s << "\n";
                    FAIL_MESG("No column of this name\n");
                    return ;
                }
                if(COL_NT[s] == INTEGER){
                    int int_data = atoi(argv[++i]);
                    cond.insert(make_pair(s,int_data));
                }
                else if(COL_NT[s] == STRING)
                {
                    string stemp(argv[++i]);
                    #if DEBUG
                        cout << "key =  " << s <<  " value = " << stemp <<  "\n";
                    #endif
                    if(stemp[0] != '\'') { WARN_MESG("Strings must be quoted.\n");}
                    while(stemp[0] == '\'' && stemp[stemp.length()-1] != '\''){
                        stemp += " " + string(argv[++i]);
                    }
                    stemp = stemp.substr(1,stemp.length()-2);
                    #if DEBUG
                        cout << "key =  " << s <<  " value = " << stemp <<  "\n";
                    #endif
                    
                    cond.insert(make_pair(s,stemp));
                    
                }
                // double cannot be compared
            }
            show_data_from_db(filename,cond);
            cond.clear();
        }
    }
    free(filename);
}