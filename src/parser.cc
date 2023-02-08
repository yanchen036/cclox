#include "parser.h"
#include "lox.h"

bool Parser::match(std::vector<TokenType> types) {
  for (TokenType type : types) {
    if (check(type)) {
      advance();
      return true;
    }
  }

  return false;
}

bool Parser::check(TokenType type) {
  if (is_at_end())
    return false;
  return peek().type == type;
}

Token Parser::advance() {
  if (!is_at_end())
    current++;
  return previous();
}

bool Parser::is_at_end() { return peek().type == EOFL; }

Token Parser::peek() { return tokens->at(current); }

Token Parser::previous() { return tokens->at(current - 1); }

Token Parser::consume(TokenType type, std::string message) {
  if (check(type))
    return advance();

  throw error(peek(), message);
}

Parser::ParserError Parser::error(Token token, std::string message) {
  Lox::error(token, message);
  return ParserError(token.to_string() + message);
}

std::vector<Stmt *> Parser::parse() {
  std::vector<Stmt *> statements;
  while (!is_at_end()) {
    statements.push_back(declaration());
  }

  return statements;
}

void Parser::synchronize() {
  advance();

  while (!is_at_end()) {
    if (previous().type == SEMICOLON)
      return;

    switch (peek().type) {
    case CLASS:
    case FUN:
    case VAR:
    case FOR:
    case IF:
    case WHILE:
    case PRINT:
    case RETURN:
      return;
    default:
      break;
    }

    advance();
  }
}

// program -> declaration* EOF ;
// declaration -> varDecl | statement ;
Stmt *Parser::declaration() {
  try {
    if (match({VAR}))
      return var_declaration();
    return statement();
  } catch (ParserError error) {
    synchronize();
    return nullptr;
  }
}

// varDecl -> "var" IDENTIFIER ( "=" expression )? ";" ;
Stmt *Parser::var_declaration() {
  Token name = consume(IDENTIFIER, "Expect variable name");

  Expr *initializer = nullptr;
  if (match({EQUAL})) {
    initializer = expression();
  }

  consume(SEMICOLON, "Expect \';\' after variable declaration.");
  return new Var(name, initializer);
}

// statement -> exprStmt | printStmt | block;
Stmt *Parser::statement() {
  if (match({PRINT}))
    return print_statement();
  if (match({LEFT_BRACE}))
    return new Block(block());

  return expression_statement();
}

// printStmt -> "print" expression ";" ;
Stmt *Parser::print_statement() {
  Expr *expr = expression();
  consume(SEMICOLON, "Expect \';\' after value.");
  return new Print(expr);
}

// exprStmt -> expression ";" ;
Stmt *Parser::expression_statement() {
  Expr *expr = expression();
  consume(SEMICOLON, "Expect \';\' after expression.");
  return new Expression(expr);
}

// block -> "{" declaration* "}" ;
std::vector<Stmt *> Parser::block() {
  std::vector<Stmt *> statements;

  while (!check(RIGHT_BRACE) && !is_at_end()) {
    statements.push_back(declaration());
  }

  consume(RIGHT_BRACE, "Expect '}' after block.");
  return statements;
}

// expression -> assignment ;
Expr *Parser::expression() { return assignment(); }

// assignment -> IDENTIFIER "=" assignment | equality ;
Expr *Parser::assignment() {
  Expr *expr = equality();

  if (match({EQUAL})) {
    Token equals = previous();
    Expr *value = assignment();

    if (expr->get_type() == VARIABLE) {
      Variable *var = dynamic_cast<Variable *>(expr);
      Token name = var->name;
      return new Assign(name, value);
    }

    throw error(equals, "Invalid assignment target");
  }

  return expr;
}

// equality -> comparison ( ( "!=" | "==" ) comparison )* ;
Expr *Parser::equality() {
  Expr *expr = comparison();

  while (match({BANG_EQUAL, EQUAL_EQUAL})) {
    Token op = previous();
    Expr *right = comparison();
    expr = new Binary(expr, op, right);
  }
  return expr;
}

// comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
Expr *Parser::comparison() {
  Expr *expr = term();

  while (match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
    Token op = previous();
    Expr *right = term();
    expr = new Binary(expr, op, right);
  }

  return expr;
}

// term -> factor ( ( "-" | "+" ) factor )* ;
Expr *Parser::term() {
  Expr *expr = factor();

  while (match({MINUS, PLUS})) {
    Token op = previous();
    Expr *right = factor();
    expr = new Binary(expr, op, right);
  }

  return expr;
}

// factor -> unary ( ( "/" | "*" ) unary )* ;
Expr *Parser::factor() {
  Expr *expr = unary();

  while (match({SLASH, STAR})) {
    Token op = previous();
    Expr *right = unary();
    expr = new Binary(expr, op, right);
  }

  return expr;
}

// unary -> ( "!" | "-" ) unary | primary ;
Expr *Parser::unary() {
  if (match({BANG, MINUS})) {
    Token op = previous();
    Expr *right = unary();
    return new Unary(op, right);
  }

  return primary();
}

// primary -> NUMBER | STRING
//         | "true" | "false" | "nil"
//         | "(" expression ")"
//         | IDENTIFIER;
Expr *Parser::primary() {
  if (match({FALSE}))
    return new PrimitiveBool(false);
  if (match({TRUE}))
    return new PrimitiveBool(true);
  if (match({NIL}))
    return new PrimitiveNil(nullptr);
  if (match({NUMBER})) {
    std::shared_ptr<LiteralNumber> ln =
        std::dynamic_pointer_cast<LiteralNumber>(previous().literal);
    return new PrimitiveNumber(ln->value);
  }
  if (match({STRING})) {
    std::shared_ptr<LiteralString> ls =
        std::dynamic_pointer_cast<LiteralString>(previous().literal);
    return new PrimitiveString(ls->value);
  }
  if (match({LEFT_PAREN})) {
    Expr *expr = expression();
    consume(RIGHT_PAREN, "Expect \')\' after expression.");
    return new Grouping(expr);
  }
  if (match({IDENTIFIER})) {
    return new Variable(previous());
  }

  throw error(peek(), "Expect expression.");
}