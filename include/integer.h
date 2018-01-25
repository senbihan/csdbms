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

#ifndef INTEGER_H
#define INTEGER_H

#include<struct_info.h>
#include<string.h>
using namespace std;

struct __c_int{
    char *num;
    char *byte_arr;

    __c_int();
    __c_int(char *number);
    char* get_equivalent_number(char *);
    __c_int operator +(__c_int op);
    __c_int operator -(__c_int op);
    __c_int operator *(__c_int op);
};


typedef struct __c_int cint;

#endif  // INTEGER_H 