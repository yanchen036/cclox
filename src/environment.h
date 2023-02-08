#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include <map>

#include "expr.h"

class Environment {
public:
  Environment() : enclosing(nullptr) {}
  Environment(Environment *enclosing) : enclosing(enclosing) {}

  void define(std::string name, ExprValue value);
  void assign(Token name, ExprValue value);
  ExprValue get(Token name);
  // enclosing is the envirionment which is outside of "this" environment.
  Environment *enclosing;

  void list();

private:
  std::map<std::string, ExprValue> values;
};

#endif // ENVIRONMENT_H_