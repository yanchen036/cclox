#include "environment.h"
#include "runtime_error.h"

#include <iostream>

void Environment::define(std::string name, ExprValue value) {
  values.emplace(std::make_pair(name, value));
}

void Environment::assign(Token name, ExprValue value) {
  if (auto search = values.find(name.lexeme); search != values.end()) {
    search->second = value;
    return;
  }

  if (enclosing != nullptr) {
    enclosing->assign(name, value);
    return;
  }

  throw RuntimeError(name, "Undefined variable \'" + name.lexeme + "\'.");
}

ExprValue Environment::get(Token name) {
  if (auto search = values.find(name.lexeme); search != values.end()) {
    return search->second;
  }

  if (enclosing != nullptr)
    return enclosing->get(name);

  throw RuntimeError(name, "Undefined variable \'" + name.lexeme + "\'.");
}

void Environment::list() {
  std::cout << "ENV contains:" << std::endl;
  for (auto &v : values) {
    std::cout << "[" << v.first << "]" << std::endl;
  }
}