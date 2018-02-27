//
// Created by caffedrine on 27.02.18.
//
#include <ctype.h>
#include <cstring>
#include <stdlib.h>
#include <stddef.h>
#include "util.h"

/**
 * Note: This function returns a pointer to a substring of the original string.
 * If the given string was allocated dynamically, the caller must not overwrite
 * that pointer with the returned value, since the original pointer must be
 * deallocated using the same allocator with which it was allocated.  The return
 * value must NOT be deallocated using free() etc.
 *
 * @param str
 * @return
 */
char* util::trim(char *str)
{
    size_t len = 0;
    char *frontp = str;
    char *endp = NULL;
    
    if( str == NULL )
    {
        return NULL;
    }
    
    if( str[0] == '\0' )
    {
        return str;
    }
    
    len = strlen(str);
    endp = str + len;
    
    /* Move the front and back pointers to address the first non-whitespace
     * characters from each end.
     */
    while( isspace((unsigned char) *frontp))
    {
        ++frontp;
    }
    
    if( endp != frontp )
    {
        while( isspace((unsigned char) *(--endp)) && endp != frontp )
        {}
    }
    
    if( str + len - 1 != endp )
        *(endp + 1) = '\0';
    else if( frontp != str && endp == frontp )
        *str = '\0';
    
    /* Shift the string so that it starts at str so that if it's dynamically
     * allocated, we can still free it on the returned pointer.  Note the reuse
     * of endp to mean the front of the string buffer now.
     */
    endp = str;
    if( frontp != str )
    {
        while( *frontp )
        {
            *endp++ = *frontp++;
        }
        *endp = '\0';
    }
    
    return str;
}

size_t util::trim_const(char *out, size_t len, const char *str)
{
    if( len == 0 )
        return 0;
    
    const char *end;
    size_t out_size;
    
    // Trim leading space
    while( isspace((unsigned char) *str)) str++;
    
    if( *str == 0 )  // All spaces?
    {
        *out = 0;
        return 1;
    }
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while( end > str && isspace((unsigned char) *end)) end--;
    end++;
    
    // Set output size to minimum of trimmed string length and buffer size minus 1
    out_size = (end - str) < len - 1 ? (end - str) : len - 1;
    
    // Copy trimmed string and add null terminator
    memcpy(out, str, out_size);
    out[out_size] = 0;
    
    return out_size;
}