#ifndef _compiler_h_
#define _compiler_h_

#include <stdbool.h>

#include "vm.h"

bool compile(VM* vm, const char* source);

#endif
