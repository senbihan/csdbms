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
    printf("| 4. SHOW ALL DATA %17c\n",'|');
    printf("| 5. HELP %26c\n",'|');
    printf("| 6. QUIT %26c\n",'|');
    printf("------------------------------------\n");
    
}

void start_session()
{
    print_menu();
    int choice, rec;
    char *file = Malloc(char,255);
    bool show = true;
    while(show)
    {
        printf("Enter Operation: ");
        scanf("%d",&choice);
        switch(choice)
        {
            case 0 :
                printf("LOAD: Enter database name : ");
                scanf("%s",file);
                read_from_file(file);
                printf("Database Loaded successfully\n");
                break;
            case 1 : 
                printf("CREATE: ");
                printf("format : {'DB_NAME', 'Number of Columns', \n{'Column Name', 'data type', 'primary key', 'max length'} }\n");
                file = create_db();
                break;
            case 2:
                //printf("Database Name : ");
                //scanf("%s",file);
                if(!IS_READ) ERR_MESG("Database is not loaded\n");
                printf("INSERTION: Number of insertions: ");
                int c;
                scanf("%d",&c);
                while(c--)
                    insert_data(file,1);
                break;
            case 3:
                //printf("Database Name : ");
                //scanf("%s",file);
                if(!IS_READ) ERR_MESG("Database is not loaded\n");
                printf("DELETE : Record Number :");
                scanf("%d",&rec);
                delete_data_from_rec(file,rec);
                //printf("1 row deleted\n");
                break;
            case 4:
                //printf("Database Name : ");
                //scanf("%s",file);
                if(!IS_READ) ERR_MESG("Database is not loaded\n");
                printf("SHOWING ALL DATA: \n");
                show_data(file);
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

