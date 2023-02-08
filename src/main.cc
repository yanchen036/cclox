#include <iostream>

#include "ast_printer.h"
#include "expr.h"
#include "lox.h"

int main(int argc, char *argv[]) {
  Lox lox;
  Lox::had_error = false;
  Lox::had_runtime_error = false;

  if (argc > 2) {
    std::cout << "Usage: cclox [script]" << std::endl;
    exit(64);
  } else if (argc == 2) {
    lox.run_file(argv[1]);
  } else {
    lox.run_prompt();
  }
  return 0;
}
