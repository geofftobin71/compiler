#ifndef _vm_h_
#define _vm_h_

#include <stdint.h>

typedef struct
{
  uint32_t* code;
  uint32_t* lines;
  uint32_t* data;
  char*     strings;

  uint32_t  code_count;
  uint32_t  data_count;
  uint32_t  strings_count;
} VM;

void initVM(VM* vm);
void freeVM(VM* vm);

uint32_t writeCode(VM* vm, uint32_t opcode, uint32_t line);
uint32_t writeData(VM* vm, uint32_t data);
uint32_t writeString(VM* vm, const char* string, uint32_t length);

#endif
