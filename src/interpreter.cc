#include "interpreter.h"
#include "lox.h"
#include "runtime_error.h"

#include <string>
#include <vector>

bool Lox::had_runtime_error;

ExprValue Interpreter::visit_BinaryExpr(Binary *binary) {
  ExprValue left_val = evaluate(binary->left);
  ExprValue right_val = evaluate(binary->right);
  ExprValue bool_val;
  bool_val.type = VALBOOL;

  switch (binary->op.type) {
  case GREATER:
    check_number_operands(binary->op, left_val, right_val);
    bool_val.boolean = left_val.number > right_val.number;
    return bool_val;
  case GREATER_EQUAL:
    check_number_operands(binary->op, left_val, right_val);
    bool_val.boolean = left_val.number >= right_val.number;
    return bool_val;
  case LESS:
    check_number_operands(binary->op, left_val, right_val);
    bool_val.boolean = left_val.number < right_val.number;
    return bool_val;
  case LESS_EQUAL:
    check_number_operands(binary->op, left_val, right_val);
    bool_val.boolean = left_val.number <= right_val.number;
    return bool_val;
  case BANG_EQUAL:
    bool_val.boolean = !(left_val == right_val);
    return bool_val;
  case EQUAL_EQUAL:
    bool_val.boolean = left_val == right_val;
    return bool_val;
  case MINUS:
    check_number_operands(binary->op, left_val, right_val);
    left_val.number = left_val.number - right_val.number;
    return left_val;
  case PLUS:
    if (left_val.type == VALNUMBER && right_val.type == VALNUMBER) {
      left_val.number = left_val.number + right_val.number;
      return left_val;
    } else if (left_val.type == VALSTRING && right_val.type == VALSTRING) {
      left_val.string = left_val.string + right_val.string;
      return left_val;
    }
    throw RuntimeError(binary->op,
                       "Operands must be two numbers or two strings.");
  case SLASH:
    check_number_operands(binary->op, left_val, right_val);
    if (right_val.number == 0)
      throw RuntimeError(binary->op, "Attempt to divide by zero.");
    left_val.number = left_val.number / right_val.number;
    return left_val;
  case STAR:
    check_number_operands(binary->op, left_val, right_val);
    left_val.number = left_val.number * right_val.number;
    return left_val;
  default:
    break;
  }

  return bool_val;
}

ExprValue Interpreter::visit_GroupingExpr(Grouping *grouping) {
  return evaluate(grouping->expression);
}

ExprValue Interpreter::visit_UnaryExpr(Unary *unary) {
  ExprValue r_val = evaluate(unary->right);
  ExprValue bool_val;

  switch (unary->op.type) {
  case MINUS:
    check_number_operand(unary->op, r_val);
    r_val.number = -r_val.number;
    return r_val;
  case BANG:
    bool_val = is_truthy(r_val);
    bool_val.boolean = !bool_val.boolean;
    return bool_val;
  default:
    break;
  }

  // Unreachable
  ExprValue val;
  return val;
}

ExprValue Interpreter::visit_PrimitiveStringExpr(PrimitiveString *ps) {
  ExprValue val;
  val.string = ps->value;
  val.type = VALSTRING;
  return val;
};

ExprValue Interpreter::visit_PrimitiveNumberExpr(PrimitiveNumber *pn) {
  ExprValue val;
  val.number = pn->value;
  val.type = VALNUMBER;
  return val;
};

ExprValue Interpreter::visit_PrimitiveBoolExpr(PrimitiveBool *pb) {
  ExprValue val;
  val.boolean = pb->value;
  val.type = VALBOOL;
  return val;
};

ExprValue Interpreter::visit_PrimitiveNilExpr(PrimitiveNil *pn) {
  ExprValue val;
  return val;
};

ExprValue Interpreter::visit_VariableExpr(Variable *var) {
  return environment->get(var->name);
}

ExprValue Interpreter::visit_AssignExpr(Assign *assign) {
  ExprValue value = evaluate(assign->value);
  environment->assign(assign->name, value);
  return value;
}

void Interpreter::visit_ExpressionStmt(Expression *expression) {
  ExprValue val = evaluate(expression->expression);
}

void Interpreter::visit_PrintStmt(Print *print) {
  ExprValue val = evaluate(print->expression);
  std::cout << stringify(val) << std::endl;
}

void Interpreter::visit_VarStmt(Var *var) {
  ExprValue value;
  if (var->initializer != nullptr) {
    value = evaluate(var->initializer);
  }

  environment->define(var->name.lexeme, value);
}

void Interpreter::visit_BlockStmt(Block *stmt) {
  execute_block(stmt->statements, new Environment(environment));
}

ExprValue Interpreter::evaluate(Expr *expr) { return expr->accept(this); }

void Interpreter::interpret(std::vector<Stmt *> statements) {
  try {
    for (auto stmt : statements) {
      execute(stmt);
    }
  } catch (RuntimeError e) {
    Lox::runtime_error(e);
  }
}

void Interpreter::execute(Stmt *stmt) { stmt->accept(this); }

void Interpreter::execute_block(std::vector<Stmt *> statements,
                                Environment *environment) {
  Environment *previsous = this->environment;
  try {
    this->environment = environment;

    for (Stmt *statement : statements) {
      execute(statement);
    }
  } catch (...) {
  }
  delete environment;
  this->environment = previsous;
}

ExprValue Interpreter::is_truthy(ExprValue val) {
  ExprValue v;
  v.type = VALBOOL;
  if (val.type == VALNIL) {
    v.boolean = false;
  } else if (val.type == VALBOOL) {
    v.boolean = val.boolean;
  } else {
    v.boolean = true;
  }
  return v;
};

void Interpreter::check_number_operand(Token op, ExprValue operand) {
  if (operand.type != VALNUMBER)
    throw RuntimeError(op, "Operand must be a number.");
};

void Interpreter::check_number_operands(Token op, ExprValue left,
                                        ExprValue right) {
  if (left.type == VALNUMBER && right.type == VALNUMBER)
    return;
  throw RuntimeError(op, "Operands must be numbers.");
};

std::string Interpreter::stringify(ExprValue val) {
  if (val.type == VALNIL) {
    return "nil";
  } else if (val.type == VALSTRING) {
    return val.string;
  } else if (val.type == VALNUMBER) {
    return std::to_string(val.number);
  } else {
    return std::to_string(val.boolean);
  }
}