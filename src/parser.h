#ifndef PARSER_H_
#define PARSER_H_

#include <memory>
#include <stdexcept>
#include <vector>

#include "expr.h"
#include "stmt.h"
#include "token.h"

class Parser {
public:
  Parser(std::shared_ptr<std::vector<Token>> tokens)
      : tokens(tokens), current(0) {}

  std::vector<Stmt *> parse();

private:
  class ParserError : public std::runtime_error {
  public:
    ParserError(const std::string &what_arg) : std::runtime_error(what_arg){};
  };

  std::shared_ptr<std::vector<Token>> tokens;
  int current;

  Expr *expression();
  Expr *assignment();
  Expr *equality();
  Expr *comparison();
  Expr *term();
  Expr *factor();
  Expr *unary();
  Expr *primary();

  Stmt *statement();
  Stmt *print_statement();
  Stmt *expression_statement();
  Stmt *declaration();
  Stmt *var_declaration();
  std::vector<Stmt *> block();

  bool match(std::vector<TokenType> types);
  bool check(TokenType type);
  Token advance();
  bool is_at_end();
  Token peek();
  Token previous();
  Token consume(TokenType type, std::string message);
  ParserError error(Token token, std::string message);
  void synchronize();
};

#endif // PARSER_H_