#ifndef RUNTIME_ERROR_H_
#define RUNTIME_ERROR_H_

#include "token_type.h"
#include <stdexcept>

class RuntimeError : public std::runtime_error {
public:
  RuntimeError(Token op, const std::string &what_arg)
      : op(op), std::runtime_error(what_arg){};

  Token op;
};

#endif // RUNTIME_ERROR_H_