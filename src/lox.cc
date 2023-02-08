#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "ast_printer.h"
#include "lox.h"
#include "parser.h"
#include "scanner.h"

void Lox::run(const std::string &source) {
  Scanner scanner(source);
  auto tokens = scanner.scanTokens();
  // for (auto token : *tokens) {
  //   std::cout << token.to_string() << std::endl;
  // }

  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();

  // Stop if there was a syntax error.
  if (had_error)
    return;

  // AstPrinter ap;
  // std::string ppt = ap.print(expression);
  // std::cout << ppt << std::endl;

  interpreter.interpret(statements);
}

void Lox::run_file(char *file) {
  std::ifstream fin(file);
  std::stringstream buffer;
  if (fin.good()) {
    buffer << fin.rdbuf();
  }
  fin.close();
  Lox::run(buffer.str());

  if (Lox::had_error)
    exit(65);
  if (Lox::had_runtime_error)
    exit(70);
}

void Lox::run_prompt() {
  std::string line;
  while (std::cin) {
    getline(std::cin, line);
    Lox::run(line);
    Lox::had_error = false;
  }
}
