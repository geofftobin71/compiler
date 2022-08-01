#include <stdio.h>

#include "debug.h"
#include "opcodes.h"

static uint32_t floatLiteralInstruction(const char* name, VM* vm, uint32_t offset)
{
  float constant = *(float*)(&vm->code[offset + 1]);
  printf("%-16s %g\n", name, constant);
  return offset + 2;
}

static uint32_t simpleInstruction(const char* name, uint32_t offset)
{
  printf("%s\n", name);
  return offset + 1;
}

uint32_t disassembleInstruction(VM* vm, uint32_t offset)
{
  if(offset > 0 && vm->lines[offset] == vm->lines[offset - 1])
  {
    printf("   | ");
  }
  else
  {
    printf("%4d ", vm->lines[offset]);
  }

  uint32_t instruction = vm->code[offset];
  switch(instruction)
  {
    case OP_FLOAT_LITERAL:
      return floatLiteralInstruction("OP_FLOAT_LITERAL", vm, offset);
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}

void disassemble(VM* vm)
{
  for(uint32_t offset = 0; offset < vm->code_count;)
  {
    offset = disassembleInstruction(vm, offset);
  }
}
