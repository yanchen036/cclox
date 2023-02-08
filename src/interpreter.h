#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <string>

#include "environment.h"
#include "expr.h"
#include "stmt.h"

class Interpreter : public ExprVisitor, public StmtVisitor {
public:
  Interpreter() { environment = new Environment(); }
  virtual ~Interpreter() {}

  virtual ExprValue visit_BinaryExpr(Binary *binary);
  virtual ExprValue visit_GroupingExpr(Grouping *grouping);
  virtual ExprValue visit_UnaryExpr(Unary *unary);
  virtual ExprValue visit_PrimitiveStringExpr(PrimitiveString *ps);
  virtual ExprValue visit_PrimitiveNumberExpr(PrimitiveNumber *pn);
  virtual ExprValue visit_PrimitiveBoolExpr(PrimitiveBool *pb);
  virtual ExprValue visit_PrimitiveNilExpr(PrimitiveNil *pn);
  virtual ExprValue visit_VariableExpr(Variable *var);
  virtual ExprValue visit_AssignExpr(Assign *assign);

  virtual void visit_ExpressionStmt(Expression *expression);
  virtual void visit_PrintStmt(Print *print);
  virtual void visit_VarStmt(Var *var);
  virtual void visit_BlockStmt(Block *stmt);

  void interpret(std::vector<Stmt *> statements);
  ExprValue evaluate(Expr *expr);

private:
  void execute(Stmt *stmt);
  void execute_block(std::vector<Stmt *> statements, Environment *environment);
  ExprValue is_truthy(ExprValue val);
  std::string stringify(ExprValue val);
  void check_number_operand(Token op, ExprValue operand);
  void check_number_operands(Token op, ExprValue left, ExprValue right);

  Environment *environment;
};

#endif // INTERPRETER_H_