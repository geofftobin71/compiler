#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "compiler.h"
#include "debug.h"
#include "opcodes.h"
#include "vm.h"

static char* readFile(const char* path)
{
  FILE* file = fopen(path, "rb");

  if(file == NULL)
  {
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    exit(1);
  }

  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char* buffer = (char*)malloc(fileSize + 1);

  if(buffer == NULL)
  {
    fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    exit(1);
  }

  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);

  if(bytesRead < fileSize)
  {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    free(buffer);
    exit(1);
  }

  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    fprintf(stderr, "\nusage: gt <source>\n\n");
    return 1;
  }

  VM vm;

  initVM(&vm);

  writeCode(&vm, OP_FLOAT_LITERAL, 122);
  writeFloatLiteral(&vm, 123.456f, 122);
  writeCode(&vm, OP_RETURN, 123);

  run(&vm);

  /*
  char* source = readFile(argv[1]);
  bool result = compile(&vm, source);
  free(source);

  if(result)
  {
    // run();
  }
  */

  freeVM(&vm);

  return 0;
}
