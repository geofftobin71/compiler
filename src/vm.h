#ifndef _vm_h_
#define _vm_h_

#include <stdint.h>

typedef struct
{
  uint8_t*  code;
} VM;

extern VM vm;

void initVM();
void freeVM();
int compile(const char* source);

#endif
