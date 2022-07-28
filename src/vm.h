#ifndef _vm_h_
#define _vm_h_

#include <stdint.h>

typedef struct
{
  uint16_t*  code;
} VM;

void initVM(VM* vm);
void freeVM(VM* vm);

#endif
