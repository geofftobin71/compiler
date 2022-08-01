#ifndef _debug_h_
#define _debug_h_

#include "vm.h"

void disassemble(VM* vm);
uint32_t disassembleInstruction(VM* vm, uint32_t offset);

#endif
