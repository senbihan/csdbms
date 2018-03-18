/**
 * Author : Bihan Sen (senbihan@gmail.com)
 *
 **/

#include <bits/stdc++.h>
#include <unistd.h>
#include "reader.h"
#include "user_interface.h"
#include "writer.h"
#include "operations.h"

using namespace std;


void print_menu()
{
    printf("\n------------------------------------\n");
    printf("| 0. LOAD Existing Database %8c\n",'|');
    printf("| 1. CREATE Database %15c\n",'|');
    printf("| 2. INSERT into Database %10c\n",'|');
    printf("| 3. DELETE from Database %10c\n",'|');
    printf("| 4. SHOW DATA %21c\n",'|');
    printf("| 5. HELP %26c\n",'|');
    printf("| 6. QUIT %26c\n",'|');
    printf("------------------------------------\n");
    
}

void start_session()
{
    print_menu();
    map<const char*,variant<int,char*>, cmp_str >cond;
    int choice, i, c, int_data;
    //char *name = Malloc(char,255);
    char *dest = Malloc(char,255);
    char *file = Malloc(char,255);
    bool show = true;
    while(show)
    {
        printf("\nEnter Operation: ");
        scanf("%d",&choice);
        switch(choice)
        {
            case 0 :
                printf("\nLOAD: Enter database name : ");
                scanf("%s",file);
                read_from_file(file);
                printf("Database Loaded successfully\n");
                break;
            case 1 : 
                printf("\nCREATE: ");
                printf("format : {'DB_NAME', 'Number of Columns', \n{'Column Name', 'data type', 'primary key', 'max length'} }\n");
                file = create_db();
                read_from_file(file);
                printf("Database Loaded successfully\n");
                break;
            case 2:
                //printf("Database Name : ");
                //scanf("%s",file);
                if(!IS_READ) ERR_MESG("Database is not loaded\n");
                printf("\nINSERTION: Number of insertions: ");
                i = 0;
                scanf("%d",&c);
                while(i++ < c)  insert_data(file,1);
                printf("%d row inserted\n",c);
                break;
            case 3:
                //printf("Database Name : ");
                //scanf("%s",file);
                if(!IS_READ) ERR_MESG("Database is not loaded\n");
                
                //delete_data_from_rec(file,rec);
                //printf("1 row deleted\n");
                break;
            case 4:
                //printf("Database Name : ");
                //scanf("%s",file);
                if(!IS_READ) ERR_MESG("Database is not loaded\n");
                printf("\nSHOWING DATA: \n");
                printf("Number of conditions: ");
                scanf("%d",&c);
                while(c--)
                {
                    printf("Enter {COLUMN NAME, VALUE}\n");
                    string s;
                    cin >> s;
                    assert(!COL_NT.empty());
                    if(COL_NT.find(s.c_str()) == COL_NT.end())
                        ERR_MESG("NO column of this name\n");
                    if(COL_NT[s.c_str()] == INTEGER){
                        scanf("%d",&int_data);
                        cond.insert(make_pair(s.c_str(),int_data));
                    }
                    else if(COL_NT[s.c_str()] == STRING){
                        scanf("%s",dest);
                        cond.insert(make_pair(s.c_str(),dest));
                    }   
                }
                show_data(file,cond);
                cond.clear();
                break;
            case 5:
                print_menu();
                break;
            case 6:
                show = false;
                break;
            default:
                print_menu();
                break;
        }
    }
}

