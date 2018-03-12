#include<bits/stdc++.h>
#include "writer.h"
#include "reader.h"
using namespace std;

char* readInt_t(){   
    char ch;
    char *s = (char*)malloc(10*sizeof(char));
    int i = 0;
    while((ch  = getchar())!= '\n'){
        assert((ch == '-' && i==0) || (ch >= '0' && ch <= '9'));
        s[i++] = ch;
    }
    return s;
}   

int main(void)
{
    /*char *s;
    s = readInt_t();
    //printf("%s\n",s);
    //char *num = get_num(s);

    cint i(s);
    // 125:  0000 0000   0000 0000   0000 0000   0000 0001  0010 0101
    //-125:  0001 0000   0000 0000   0000 0000   0000 0001  0010 0101 

    // 4256:                                     0100 0010  0101 0110
    // print the bytes of integer number
    i.print();
    */

    create_db();
    insert_data("student");
    //printf("\n\nREADING.....\n\n");
    //read_from_file();


    return 0;
}