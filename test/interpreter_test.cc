#include "expr.h"
#include "interpreter.h"
#include "parser.h"
#include "scanner.h"

#include "gtest/gtest.h"

namespace {

TEST(NumberTest, add) {
  Scanner scanner("1.2 + 3.4;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALNUMBER);
  EXPECT_DOUBLE_EQ(val.number, 4.6);
}

TEST(NumberTest, sub) {
  Scanner scanner("1.2 - 3.5;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALNUMBER);
  EXPECT_DOUBLE_EQ(val.number, -2.3);
}

TEST(NumberTest, mult) {
  Scanner scanner("1.5 * 30;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALNUMBER);
  EXPECT_DOUBLE_EQ(val.number, 45.0);
}

TEST(NumberTest, div) {
  Scanner scanner("30 / 1.5;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALNUMBER);
  EXPECT_DOUBLE_EQ(val.number, 20.);
}

TEST(NumberTest, mix1) {
  Scanner scanner("1 + 2 * 3;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALNUMBER);
  EXPECT_DOUBLE_EQ(val.number, 7);
}

TEST(NumberTest, mix2) {
  Scanner scanner("(1 + 2) * 3;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALNUMBER);
  EXPECT_DOUBLE_EQ(val.number, 9);
}

TEST(NumberTest, mix3) {
  Scanner scanner("1 + 2 * 3 + 4;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALNUMBER);
  EXPECT_DOUBLE_EQ(val.number, 11);
}

TEST(NumberTest, mix4) {
  Scanner scanner("1 + 2 * 3 / 4;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALNUMBER);
  EXPECT_DOUBLE_EQ(val.number, 2.5);
}

TEST(StringTest, add) {
  Scanner scanner("\"foo\" + \"bar\";");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALSTRING);
  EXPECT_EQ(val.string, "foobar");
}

TEST(BooleanTest, bang) {
  Scanner scanner("!false;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALBOOL);
  EXPECT_TRUE(val.boolean);
}

TEST(BooleanTest, num_equal) {
  Scanner scanner("42 == 42;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALBOOL);
  EXPECT_TRUE(val.boolean);
}

TEST(BooleanTest, str_equal) {
  Scanner scanner("\"foo\" == \"foo\";");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALBOOL);
  EXPECT_TRUE(val.boolean);
}

TEST(BooleanTest, num_not_equal) {
  Scanner scanner("42 != 43;");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALBOOL);
  EXPECT_TRUE(val.boolean);
}

TEST(BooleanTest, str_not_equal) {
  Scanner scanner("\"foo\" != \"bar\";");
  auto tokens = scanner.scanTokens();
  Parser parser(tokens);
  std::vector<Stmt *> statements = parser.parse();
  EXPECT_EQ(statements.size(), 1);
  Expression *expr = dynamic_cast<Expression *>(statements[0]);
  Interpreter interpreter;
  ExprValue val = interpreter.evaluate(expr->expression);

  EXPECT_EQ(val.type, VALBOOL);
  EXPECT_TRUE(val.boolean);
}

} // namespace
