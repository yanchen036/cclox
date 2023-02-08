#include "expr.h"
#include "parser.h"
#include "scanner.h"
#include "stmt.h"

#include "gtest/gtest.h"

namespace {

TEST(BinaryTest, add_expr) {
  Scanner scanner("1.0 + 2.0;");
  auto tokens = scanner.scanTokens();

  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Binary *b = dynamic_cast<Binary *>(expr->expression);
  PrimitiveNumber *left = dynamic_cast<PrimitiveNumber *>(b->left);
  PrimitiveNumber *right = dynamic_cast<PrimitiveNumber *>(b->right);

  EXPECT_DOUBLE_EQ(left->value, 1.0);
  EXPECT_EQ(b->op.type, PLUS);
  EXPECT_DOUBLE_EQ(right->value, 2.0);
}

TEST(UnaryTest, bang_expr) {
  Scanner scanner("!true;");
  auto tokens = scanner.scanTokens();

  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Unary *u = dynamic_cast<Unary *>(expr->expression);
  PrimitiveBool *right = dynamic_cast<PrimitiveBool *>(u->right);

  EXPECT_EQ(u->op.type, BANG);
  EXPECT_DOUBLE_EQ(right->value, true);
}

TEST(GroupingTest, group_expr) {
  Scanner scanner("(1.0);");
  auto tokens = scanner.scanTokens();

  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Grouping *g = dynamic_cast<Grouping *>(expr->expression);
  PrimitiveNumber *n = dynamic_cast<PrimitiveNumber *>(g->expression);

  EXPECT_DOUBLE_EQ(n->value, 1.0);
}

} // namespace