//
// Created by caffedrine on 27.02.18.
//

#ifndef SMARTBET_UTIL_H
#define SMARTBET_UTIL_H

#include <ctype.h>
#include <cstring>
#include <stdlib.h>
#include <stddef.h>

namespace util
{
    char* trim(char *str);
    size_t trim_const(char *out, size_t len, const char *str);
};
#endif //SMARTBET_UTIL_H
