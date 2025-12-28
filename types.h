#ifndef TYPES_H
#define TYPES_H

#include "common.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif
