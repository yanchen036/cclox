#ifndef TOKEN_TYPE_H_
#define TOKEN_TYPE_H_

enum TokenType {
  // clang-format off
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN ,SUPER, THIS, TRUE, VAR, WHILE,

  EOFL
  // clang-format on
};

static const char *TokenTypeString[] = {
    "LEFT_PAREN", "RIGHT_PAREN",   "LEFT_BRACE", "RIGHT_BRACE",
    "COMMA",      "DOT",           "MINUS",      "PLUS",
    "SEMICOLON",  "SLASH",         "STAR",

    "BANG",       "BANG_EQUAL",    "EQUAL",      "EQUAL_EQUAL",
    "GREATER",    "GREATER_EQUAL", "LESS",       "LESS_EQUAL",

    "IDENTIFIER", "STRING",        "NUMBER",

    "AND",        "CLASS",         "ELSE",       "FALSE",
    "FUN",        "FOR",           "IF",         "NIL",
    "OR",         "PRINT",         "RETURN",     "SUPER",
    "THIS",       "TRUE",          "VAR",        "WHILE",

    "EOFL"};

static const char *token_type_to_string(TokenType type) {
  return TokenTypeString[type];
}

#endif // TOKEN_TYPE_H_
