#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "scanner.h"

typedef struct
{
  const char* start;
  const char* current;
  int         line;
} Scanner;

Scanner scanner;

void initScanner(const char* source)
{
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static bool isAlpha(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isDigit(char c)
{
  return c >= '0' && c <= '9';
}

static bool isAtEnd()
{
  return *scanner.current == '\0';
}

static char nextChar()
{
  scanner.current++;
  return scanner.current[-1];
}

static char peekChar()
{
  return *scanner.current;
}

static char peekNextChar()
{
  if(isAtEnd()) return '\0';
  return scanner.current[1];
}

static bool matchChar(char expected)
{
  if(isAtEnd()) return false;
  if(*scanner.current != expected) return false;
  scanner.current++;
  return true;
}

static Token makeToken(TokenType type)
{
  Token token;

  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  token.line = scanner.line;

  return token;
}

static Token errorToken(const char* message)
{
  Token token;

  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;

  return token;
}

static void skipWhitespace()
{
  for(;;)
  {
    char c = peekChar();
    switch(c)
    {
      case ' ':
      case '\r':
      case '\t':
        nextChar();
        break;
      case '\n':
        scanner.line++;
        nextChar();
        break;
      case '/':
        if(peekNextChar() == '/')
        {
          // A comment goes until the end of the line.
          while(peekChar() != '\n' && !isAtEnd()) nextChar();
        }
        else
        {
          return;
        }
        break;
      default:
        return;
    }
  }
}

static TokenType checkKeyword(int start, int length, const char* rest, TokenType type)
{
  if(scanner.current - scanner.start == start + length && memcmp(scanner.start + start, rest, length) == 0)
  {
    return type;
  }

  return TOKEN_IDENTIFIER;
}

static TokenType identifierType()
{
  switch(scanner.start[0])
  {
    case 'a':
      return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'e':
      return checkKeyword(1, 3, "lse", TOKEN_ELSE);
    case 'f':
      if(scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'a':
            return checkKeyword(2, 3, "lse", TOKEN_FALSE);
          case 'o':
            return checkKeyword(2, 1, "r", TOKEN_FOR);
          case 'u':
            return checkKeyword(2, 6, "nction", TOKEN_FUNCTION);
        }
      }
      break;
    case 'i':
      return checkKeyword(1, 1, "f", TOKEN_IF);
    case 'n':
      if(scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'i':
            return checkKeyword(2, 1, "l", TOKEN_NIL);
          case 'u':
            return checkKeyword(2, 1, "m", TOKEN_NUM);
        }
      }
      break;
    case 'o':
      return checkKeyword(1, 1, "r", TOKEN_OR);
    case 'p':
      return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r':
      return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's':
      if(scanner.current - scanner.start > 3) {
        if((scanner.start[1] == 't') && (scanner.start[2] == 'r')) {
          switch (scanner.start[3]) {
            case 'i':
              return checkKeyword(4, 2, "ng", TOKEN_STRING);
            case 'u':
              return checkKeyword(4, 2, "ct", TOKEN_STRUCT);
          }
        }
      }
      break;
    case 't':
      return checkKeyword(1, 3, "rue", TOKEN_TRUE);
    case 'w':
      return checkKeyword(1, 4, "hile", TOKEN_WHILE);
  }

  return TOKEN_IDENTIFIER;
}

static Token identifier()
{
  while(isAlpha(peekChar()) || isDigit(peekChar())) nextChar();
  return makeToken(identifierType());
}

static Token number()
{
  while(isDigit(peekChar())) nextChar();

  // Look for a fractional part.
  if(peekChar() == '.' && isDigit(peekNextChar()))
  {
    // Consume the ".".
    nextChar();

    while(isDigit(peekChar())) nextChar();
  }

  return makeToken(TOKEN_NUMBER_LIT);
}

static Token string()
{
  while(peekChar() != '"' && !isAtEnd())
  {
    if(peekChar() == '\n') scanner.line++;
    nextChar();
  }

  if(isAtEnd()) return errorToken("Unterminated string");

  // The closing quote.
  nextChar();
  return makeToken(TOKEN_STRING_LIT);
}

Token scanToken()
{
  skipWhitespace();

  scanner.start = scanner.current;

  if(isAtEnd()) return makeToken(TOKEN_EOF);

  char c = nextChar();

  if(isAlpha(c)) return identifier();
  if(isDigit(c)) return number();

  switch(c)
  {
    case '(':
      return makeToken(TOKEN_LEFT_PAREN);
    case ')':
      return makeToken(TOKEN_RIGHT_PAREN);
    case '{':
      return makeToken(TOKEN_LEFT_BRACE);
    case '}':
      return makeToken(TOKEN_RIGHT_BRACE);
    case '[':
      return makeToken(TOKEN_LEFT_BRACKET);
    case ']':
      return makeToken(TOKEN_RIGHT_BRACKET);
    case ';':
      return makeToken(TOKEN_SEMICOLON);
    case ',':
      return makeToken(TOKEN_COMMA);
    case '.':
      return makeToken(TOKEN_DOT);
    case '-':
      return makeToken(TOKEN_MINUS);
    case '+':
      return makeToken(TOKEN_PLUS);
    case '/':
      return makeToken(TOKEN_SLASH);
    case '*':
      return makeToken(TOKEN_STAR);
    case '!':
      return makeToken(matchChar('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return makeToken(matchChar('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
      return makeToken(matchChar('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
      return makeToken(matchChar('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '"': return string();
  }

  return errorToken("Unexpected character");
}
