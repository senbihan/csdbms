/**
 * Auxiliary functions 
 * 
 * 
 * Author(s) : Bihan Sen (senbihan@gmail.com)
 * 
 * Last Modified:  29/01/2018
 * 
 **/

#include <bits/stdc++.h>
#include "aux_func.h"

using namespace std;

/**
 * Converts a digit (0-9) to a byte
 * where first nibble contains 0 and the next nibble contains
 * the binary of the digit
 * 
 * @params : digit in char type
 * 
 * returns : a byte
 **/
char dig_to_byte(char dig)
{
    return dig - '0';
}