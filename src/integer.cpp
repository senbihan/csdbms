#include <integer.h>
#include<cstdlib>
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