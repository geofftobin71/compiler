#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vm.h"
#include "opcodes.h"

#define CODE_SIZE (256 * 1024)
#define DATA_SIZE (256 * 1024)
#define STRINGS_SIZE (256 * 1024)

void initVM(VM* vm)
{
  vm->code = malloc(CODE_SIZE);
  vm->lines = malloc(CODE_SIZE);
  vm->data = malloc(DATA_SIZE);
  vm->strings = malloc(STRINGS_SIZE);

  vm->code_count = 0;
  vm->data_count = 0;
  vm->strings_count = 0;
}

void freeVM(VM* vm)
{
  free(vm->code);
  free(vm->lines);
  free(vm->data);
  free(vm->strings);
}

bool run(VM* vm)
{
  vm->ip = vm->code;

#define READ_OPCODE() (*vm->ip++)

  for(;;)
  {
    uint32_t opcode;
    switch(opcode = READ_OPCODE())
    {
      case OP_FLOAT_LITERAL:
        {
          const uint32_t c = READ_OPCODE();
          const float v = *(float*)(&c);
          printf("%g\n", v);
          break;
        }
      case OP_RETURN:
        {
          return true;
        }
    }
  }

#undef READ_OPCODE
}

uint32_t writeCode(VM* vm, uint32_t opcode, uint32_t line)
{
  if(vm->code_count >= CODE_SIZE)
  {
    fprintf(stderr, "Out of code memory on line %d\n", line);
    exit(1);
  }

  uint32_t address = vm->code_count;

  vm->code[vm->code_count] = opcode;
  vm->lines[vm->code_count] = line;

  ++vm->code_count;

  return address;
}

uint32_t writeFloatLiteral(VM* vm, float v, uint32_t line)
{
  if(vm->code_count >= CODE_SIZE)
  {
    fprintf(stderr, "Out of code memory on line %d\n", line);
    exit(1);
  }

  uint32_t address = vm->code_count;

  vm->code[vm->code_count] = *(uint32_t*)&v;
  vm->lines[vm->code_count] = line;

  ++vm->code_count;

  return address;
}

uint32_t writeData(VM* vm, uint32_t data)
{
  if(vm->data_count >= DATA_SIZE)
  {
    fprintf(stderr, "Out of data memory\n");
    exit(1);
  }

  uint32_t address = vm->data_count;

  vm->data[vm->data_count++] = data;

  return address;
}

uint32_t writeString(VM* vm, const char* string, uint32_t length)
{
  if((vm->strings_count + length) >= STRINGS_SIZE)
  {
    fprintf(stderr, "Out of string memory\n");
    exit(1);
  }

  uint32_t address = vm->strings_count;

  memcpy(vm->strings + vm->strings_count, string, length);

  vm->strings[vm->strings_count + length] = '\0';

  vm->strings_count += (length + 1);

  return address;
}
