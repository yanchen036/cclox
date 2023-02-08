#ifndef LOX_H_
#define LOX_H_

#include <iostream>
#include <string>

#include "interpreter.h"
#include "runtime_error.h"

class Lox {
public:
  void run(const std::string &source);
  void run_file(char *file);
  void run_prompt();

  Interpreter interpreter;

  static bool had_error;
  static bool had_runtime_error;

  static void report(int line, const std::string &where,
                     const std::string &message) {
    std::cout << "[line " << line << "] Error" + where + ": " + message
              << std::endl;
    had_error = true;
  }

  static void error(int line, const std::string &message) {
    report(line, "", message);
  }

  static void error(Token token, const std::string &message) {
    if (token.type == EOFL) {
      report(token.line, " at end", message);
    } else {
      report(token.line, " at \'" + token.lexeme + "\'", message);
    }
  }

  static void runtime_error(RuntimeError e) {
    std::cerr << "[line " << e.op.line << "] RuntimeError: " << e.what()
              << std::endl;
    had_runtime_error = true;
  }
};

#endif // LOX_H_