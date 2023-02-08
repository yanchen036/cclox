#include "scanner.h"

#include "gtest/gtest.h"
#include <iostream>

namespace {

TEST(ScannerTest, parenthesis) {
  Scanner scanner("()");
  auto tokens = scanner.scanTokens();

  EXPECT_EQ(tokens->size(), 3);
  EXPECT_EQ((*tokens)[0].type, LEFT_PAREN);
  EXPECT_EQ((*tokens)[0].lexeme, "(");
  EXPECT_EQ((*tokens)[0].line, 1);
  EXPECT_EQ((*tokens)[1].type, RIGHT_PAREN);
  EXPECT_EQ((*tokens)[1].lexeme, ")");
  EXPECT_EQ((*tokens)[1].line, 1);
}

TEST(ScannerTest, str) {
  std::cout << "foobar" << std::endl;
  Scanner scanner("\"foobar\"");
  auto tokens = scanner.scanTokens();

  EXPECT_EQ(tokens->size(), 2);
  EXPECT_EQ((*tokens)[0].type, STRING);
  EXPECT_EQ((*tokens)[0].lexeme, "\"foobar\"");
  std::shared_ptr<LiteralString> ls =
      std::dynamic_pointer_cast<LiteralString>((*tokens)[0].literal);
  EXPECT_EQ(ls->value, "foobar");
  EXPECT_EQ((*tokens)[0].line, 1);
}

TEST(ScannerTest, number) {
  Scanner scanner("12.34");
  auto tokens = scanner.scanTokens();

  EXPECT_EQ(tokens->size(), 2);
  EXPECT_EQ((*tokens)[0].type, NUMBER);
  EXPECT_EQ((*tokens)[0].lexeme, "12.34");
  std::shared_ptr<LiteralNumber> ln =
      std::dynamic_pointer_cast<LiteralNumber>((*tokens)[0].literal);
  EXPECT_EQ(ln->value, 12.34);
  EXPECT_EQ((*tokens)[0].line, 1);
}

TEST(ScannerTest, identifier) {
  Scanner scanner("orchid or and");
  auto tokens = scanner.scanTokens();

  EXPECT_EQ(tokens->size(), 4);
  EXPECT_EQ((*tokens)[0].type, IDENTIFIER);
  EXPECT_EQ((*tokens)[0].lexeme, "orchid");
  EXPECT_EQ((*tokens)[0].line, 1);

  EXPECT_EQ((*tokens)[1].type, OR);
  EXPECT_EQ((*tokens)[1].lexeme, "or");
  EXPECT_EQ((*tokens)[1].line, 1);

  EXPECT_EQ((*tokens)[2].type, AND);
  EXPECT_EQ((*tokens)[2].lexeme, "and");
  EXPECT_EQ((*tokens)[2].line, 1);
}

TEST(ScannerTest, unexpect_token) {
  Scanner scanner("@");
  auto tokens = scanner.scanTokens();

  EXPECT_EQ(tokens->size(), 1);
  EXPECT_EQ((*tokens)[0].type, EOFL);
}

TEST(ScannerTest, expression) {
  Scanner scanner("a>=b; c=123; d=\"hello\"");
  auto tokens = scanner.scanTokens();

  EXPECT_EQ(tokens->size(), 12);
  EXPECT_EQ((*tokens)[0].type, IDENTIFIER);
  EXPECT_EQ((*tokens)[1].type, GREATER_EQUAL);
  EXPECT_EQ((*tokens)[2].type, IDENTIFIER);
  EXPECT_EQ((*tokens)[3].type, SEMICOLON);
  EXPECT_EQ((*tokens)[4].type, IDENTIFIER);
  EXPECT_EQ((*tokens)[5].type, EQUAL);
  EXPECT_EQ((*tokens)[6].type, NUMBER);
  EXPECT_EQ((*tokens)[7].type, SEMICOLON);
  EXPECT_EQ((*tokens)[8].type, IDENTIFIER);
  EXPECT_EQ((*tokens)[9].type, EQUAL);
  EXPECT_EQ((*tokens)[10].type, STRING);
}

} // namespace
