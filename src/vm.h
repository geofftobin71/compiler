#ifndef _vm_h_
#define _vm_h_

#include <stdbool.h>
#include <stdint.h>

#define STACK_MAX 256

typedef union
{
  uint32_t  as_uint;
  float     as_float;
} Value;

#define AS_UINT(value)    ((value).as_uint)
#define AS_FLOAT(value)  ((value).as_float)

typedef struct
{
  uint32_t* ip;

  Value     stack[STACK_MAX];
  Value*    stack_top;

  uint32_t* code;
  uint32_t* lines;
  Value*    data;
  char*     strings;

  uint32_t  code_count;
  uint32_t  data_count;
  uint32_t  strings_count;
} VM;

void initVM(VM* vm);
void freeVM(VM* vm);
bool run(VM* vm);

uint32_t writeCode(VM* vm, uint32_t opcode, uint32_t line);
uint32_t writeValue(VM* vm, Value v, uint32_t line);

uint32_t writeData(VM* vm, Value data);
uint32_t writeString(VM* vm, const char* string, uint32_t length);

#endif
