#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>

#include "token_type.h"

class Literal {
public:
  virtual std::string to_string() = 0;
};

class LiteralString : public Literal {
public:
  LiteralString(std::string s) : value(s) {}

  std::string to_string() { return value; }

public:
  std::string value;
};

class LiteralNumber : public Literal {
public:
  LiteralNumber(double d) : value(d) {}

  std::string to_string() { return std::to_string(value); }

public:
  double value;
};

class Token {
public:
  Token(TokenType type, std::string lexeme, std::shared_ptr<Literal> literal,
        int line)
      : type(type), lexeme(lexeme), literal(literal), line(line){};

  std::string to_string() {
    std::string literal_str;
    if (literal)
      literal_str = literal->to_string();

    return std::string(token_type_to_string(type)) + " " + lexeme + " " +
           literal_str;
  }

  TokenType type;
  std::string lexeme;
  std::shared_ptr<Literal> literal;
  int line;
};

#endif // TOKEN_H_
