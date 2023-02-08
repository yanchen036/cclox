#include "scanner.h"

#include <iostream>
#include <vector>

#include "lox.h"
#include "token.h"
#include "token_type.h"

bool Lox::had_error;

bool Scanner::is_at_end() { return current >= source.size(); }

char Scanner::advance() { return source[current++]; }

char Scanner::peek() {
  if (is_at_end())
    return '\0';
  return source[current];
}

char Scanner::peek_next() {
  if (current + 1 >= source.size())
    return '\0';
  return source[current + 1];
}

bool Scanner::is_digit(char c) { return c >= '0' && c <= '9'; }

bool Scanner::is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::is_alpha_numeric(char c) { return is_alpha(c) || is_digit(c); }

void Scanner::identifier() {
  while (is_alpha_numeric(peek()))
    advance();

  std::string text = source.substr(start, current - start);
  auto mit = keywords.find(text);
  TokenType type = IDENTIFIER;
  if (mit != keywords.end())
    type = mit->second;
  add_token(type);
}

void Scanner::add_token(TokenType type) { add_token(type, nullptr); }

void Scanner::add_token(TokenType type, std::shared_ptr<Literal> literal) {
  std::string text = source.substr(start, current - start);
  tokens->push_back(Token(type, text, literal, line));
}

bool Scanner::match(char expected) {
  if (is_at_end())
    return false;
  if (source[current] != expected)
    return false;

  current++;
  return true;
}

void Scanner::string() {
  while (peek() != '"' && !is_at_end()) {
    if (peek() == '\n')
      line++;
    advance();
  }
  if (is_at_end()) {
    Lox::error(line, "Unterminated string");
    return;
  }

  // The closing ".
  advance();

  // Trim the surrounding quotes.
  std::string value = source.substr(start + 1, current - 1 - (start + 1));
  add_token(STRING, std::shared_ptr<LiteralString>(new LiteralString(value)));
}

void Scanner::number() {
  while (is_digit(peek()))
    advance();

  // Look for a fractional part.
  if (peek() == '.' && is_digit(peek_next())) {
    // Consume the "."
    advance();

    while (is_digit(peek()))
      advance();
  }

  double value = std::stod(source.substr(start, current));
  add_token(NUMBER, std::shared_ptr<LiteralNumber>(new LiteralNumber(value)));
}

void Scanner::scan_token() {
  char c = advance();
  switch (c) {
  case '(':
    add_token(LEFT_PAREN);
    break;
  case ')':
    add_token(RIGHT_PAREN);
    break;
  case '{':
    add_token(LEFT_BRACE);
    break;
  case '}':
    add_token(RIGHT_BRACE);
    break;
  case ',':
    add_token(COMMA);
    break;
  case '.':
    add_token(DOT);
    break;
  case '-':
    add_token(MINUS);
    break;
  case '+':
    add_token(PLUS);
    break;
  case ';':
    add_token(SEMICOLON);
    break;
  case '*':
    add_token(STAR);
    break;

  case '!':
    add_token(match('=') ? BANG_EQUAL : BANG);
    break;
  case '=':
    add_token(match('=') ? EQUAL_EQUAL : EQUAL);
    break;
  case '<':
    add_token(match('=') ? LESS_EQUAL : LESS);
    break;
  case '>':
    add_token(match('=') ? GREATER_EQUAL : GREATER);
    break;
  case '/':
    if (match('/')) {
      // A comment goes until the end of the line
      while (peek() != '\n' && !is_at_end())
        advance();
    } else {
      add_token(SLASH);
    }
    break;

  case ' ':
  case '\r':
  case '\t':
    // Ignore whitespace.
    break;

  case '\n':
    line++;
    break;

  case '"':
    string();
    break;

  default:
    if (is_digit(c)) {
      number();
    } else if (is_alpha(c)) {
      identifier();
    } else {
      Lox::error(line, "Unexpected character.");
    }
    break;
  }
}

std::shared_ptr<std::vector<Token>> Scanner::scanTokens() {
  while (!is_at_end()) {
    start = current;
    scan_token();
  }

  tokens->push_back(Token(EOFL, "", nullptr, line));
  return tokens;
}
