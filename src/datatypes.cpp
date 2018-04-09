#include <cstdlib>
#include <string.h>
#include "datatypes.h"
#include "metadata_struct.h"


char dig_to_byte(char dig)
{
    return dig - '0';
}

bool is_valid_int(char *s)
{
    unsigned i = 0, size = strlen(s);
    if(s[i] == '-') i++;
    for( ; i < size ; i++)
        if(s[i] < '0' || s[i] > '9')
            return false;
    return true;
}

char *int_to_byte(char *number)
{
    char *temp_num = Calloc(char, INT_BYTE_SIZE);
    char f_byte = 0, cur_byte = 0, last_byte = 0; 
    
    // if negative integer
    if(number[0] == '-'){
        f_byte |= 1;
        f_byte <<= 4;
    }

    unsigned j = 0;
    unsigned k = 4;
    int n = strlen(number);     // length
    for(int i = n-1; i >= 0; i--){
        if(number[i] == '-') break;
        ++j;
        if(k == 0){
            f_byte |= dig_to_byte(number[i]);
        }
        if(j & 1){
            last_byte = dig_to_byte(number[i]);
        }
        else{
            cur_byte = dig_to_byte(number[i]);
            cur_byte <<= 4;
            cur_byte |= last_byte;
            temp_num[k--] = cur_byte;
            cur_byte = 0;
        }   
    }
    temp_num[0] = f_byte;
    return temp_num;
}