#ifndef _compiler_h_
#define _compiler_h_

#include "vm.h"
#include "object.h"

bool compile(const char* source, Chunk* chunk);

#endif
