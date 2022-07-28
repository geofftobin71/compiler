#ifndef _compiler_h_
#define _compiler_h_

#include "vm.h"
#include "object.h"

ObjFunction* compile(const char* source);

#endif
