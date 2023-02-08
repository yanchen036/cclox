#ifndef AST_PRINTER_H_
#define AST_PRINTER_H_

#include <string>

#include "expr.h"

class AstPrinter : public ExprVisitor {
public:
  virtual ExprValue visit_BinaryExpr(Binary *binary);
  virtual ExprValue visit_GroupingExpr(Grouping *grouping);
  virtual ExprValue visit_UnaryExpr(Unary *unary);
  virtual ExprValue visit_PrimitiveStringExpr(PrimitiveString *ps);
  virtual ExprValue visit_PrimitiveNumberExpr(PrimitiveNumber *pn);
  virtual ExprValue visit_PrimitiveBoolExpr(PrimitiveBool *pb);
  virtual ExprValue visit_PrimitiveNilExpr(PrimitiveNil *pn);

  std::string print(Expr *expr);

private:
  ExprValue parenthesize(std::string name, std::vector<Expr *> exprs);
};

#endif // AST_PRINT_H_
