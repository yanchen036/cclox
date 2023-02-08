#include "ast_printer.h"

#include <vector>

ExprValue AstPrinter::visit_BinaryExpr(Binary *binary) {
  std::vector<Expr *> exprs;
  exprs.push_back(binary->left);
  exprs.push_back(binary->right);
  return parenthesize(binary->op.lexeme, exprs);
};

ExprValue AstPrinter::visit_GroupingExpr(Grouping *grouping) {
  std::vector<Expr *> exprs(1, grouping->expression);
  return parenthesize("group", exprs);
}

ExprValue AstPrinter::visit_UnaryExpr(Unary *unary) {
  std::vector<Expr *> exprs(1, unary->right);
  return parenthesize(unary->op.lexeme, exprs);
}

ExprValue AstPrinter::visit_PrimitiveStringExpr(PrimitiveString *ps) {
  ExprValue val;
  val.string = ps->value;
  val.type = VALSTRING;
  return val;
}

ExprValue AstPrinter::visit_PrimitiveNumberExpr(PrimitiveNumber *pn) {
  ExprValue val;
  val.string = std::to_string(pn->value);
  val.type = VALNUMBER;
  return val;
}

ExprValue AstPrinter::visit_PrimitiveBoolExpr(PrimitiveBool *pb) {
  ExprValue val;
  val.string = std::to_string(pb->value);
  val.type = VALBOOL;
  return val;
}

ExprValue AstPrinter::visit_PrimitiveNilExpr(PrimitiveNil *pn) {
  ExprValue val;
  val.string = "nil";
  val.type = VALNIL;
  return val;
}

ExprValue AstPrinter::parenthesize(std::string name,
                                   std::vector<Expr *> exprs) {
  ExprValue val;
  val.type = VALSTRING;
  std::string &ppt = val.string;
  ppt = std::string("(");
  ppt += name;
  for (Expr *expr : exprs) {
    ExprValue v = expr->accept(this);
    ppt += " " + v.string;
  }
  ppt.append(")");
  return val;
};

std::string AstPrinter::print(Expr *expr) {
  ExprValue val = expr->accept(this);
  return val.string;
}