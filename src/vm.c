#include <stdlib.h>

#include "vm.h"

void initVM(VM* vm)
{
  vm->code = malloc(256 * 1024);
}

void freeVM(VM* vm)
{
  free(vm->code);
}

