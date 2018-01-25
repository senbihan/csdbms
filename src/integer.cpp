/**
 * Integer Equivalent datatype which takes 5 bytes
 * Maximum integer can be stored 999999999
 * 
 * 
 * Author(s) : Bihan Sen (senbihan@gmail.com)
 * 
 * Last Modified:   15/01/2018
 * 
 **/

#include <integer.h>
#include <cstdlib>
using namespace std;

__c_int :: __c_int()
{
    num = Calloc(char,INT_MAX_DIGIT);
    byte_arr = Calloc(char, INT_BYTE_SIZE);
}

__c_int :: __c_int(char *number)
{
    num = Calloc(char,INT_MAX_DIGIT);
    strcpy(num,number);
    byte_arr = get_equivalent_number(num);
}

char* __c_int :: get_equivalent_number(char *number)
{
    char *temp_num = Calloc(char, INT_BYTE_SIZE);
    char f_byte = 0, cur_byte = 0, pur_byte = 0; 
    
    // if negative integer
    if(number[0] == '-'){
        f_byte |= 1;
        f_byte <<= 4;
    }

    unsigned j = 0;
    unsigned k = 4;
    int n = strlen(number);
    for(unsigned i = n-1; i >= 0; i--){
        if(number[i] == '-') break;
        ++j;
        if(k == 0){
            f_byte |= int(number[i]-'0');
        }
        if(j & 1){
            pur_byte = int(number[i] - '0');
        }
        else{
            cur_byte = int(number[i] - '0');
            cur_byte <<= 4;
            cur_byte |= pur_byte;
            temp_num[k--] = cur_byte;
            cur_byte = 0;
        }   
    }
    temp_num[0] = f_byte;
    return temp_num;
}