#ifndef SCANNER_H_
#define SCANNER_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "token.h"

class Scanner {
public:
  Scanner(const std::string &source)
      : source(source), start(0), current(0), line(1) {
    tokens = std::make_shared<std::vector<Token>>();
  }

  std::shared_ptr<std::vector<Token>> scanTokens();

private:
  bool is_at_end();
  char advance();
  char peek();
  char peek_next();
  bool match(char expected);
  bool is_digit(char c);
  bool is_alpha(char c);
  bool is_alpha_numeric(char c);
  void string();
  void number();
  void identifier();

  void scan_token();
  void add_token(TokenType type);
  void add_token(TokenType type, std::shared_ptr<Literal> literal);

private:
  std::string source;
  std::shared_ptr<std::vector<Token>> tokens;
  int start;
  int current;
  int line;

private:
  inline static std::map<std::string, TokenType> keywords = {
      {"and", AND},   {"class", CLASS}, {"else", ELSE},     {"false", FALSE},
      {"for", FOR},   {"fun", FUN},     {"if", IF},         {"nil", NIL},
      {"or", OR},     {"print", PRINT}, {"return", RETURN}, {"super", SUPER},
      {"this", THIS}, {"true", TRUE},   {"var", VAR},       {"while", WHILE},
  };
};

#endif // SCANNER_H_
