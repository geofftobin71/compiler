#ifndef _chunk_h_
#define _chunk_h_

#include "common.h"
#include "value.h"

typedef enum
{
  OP_RETURN,
  OP_CONSTANT,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NEGATE,
} OpCode;

typedef struct
{
  int         count;
  int         capacity;
  uint8_t*    code;
  int*        lines;
  ValueArray  constants;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);

#endif
