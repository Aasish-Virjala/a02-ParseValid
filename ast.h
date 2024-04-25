#ifndef AST_H
#define AST_H

#include <iostream>

/* Struct: Token
 *
 * @field Type holds the token
 * @field Value holds what the token holds in the case of NUM or ID
 */
struct Token {
  enum class Type {
    NUM,
    ID,
    INT,
    STRUCT,
    NIL,
    BREAK,
    CONTINUE,
    RETURN,
    IF,
    ELSE,
    WHILE,
    NEW,
    LET,
    EXTERN,
    FN,
    ADDRESS,
    COLON,
    SEMICOLON,
    COMMA,
    UNDERSCORE,
    ARROW,
    PLUS,
    DASH,
    STAR,
    SLASH,
    EQUAL,
    NOTEQ,
    LT,
    LTE,
    GT,
    GTE,
    DOT,
    GETS,
    OPENPAREN,
    CLOSEPAREN,
    OPENBRACKET,
    CLOSEBRACKET,
    OPENBRACE,
    CLOSEBRACE,
  } type;
  std::string value;

  // Constructor to initialize token
  Token(Type t, const std::string &v = "") : type(t), value(v) {}
};

struct Ast {
  Token token;
  Ast *leftChild;
  Ast *rightChild;
};

#endif /* AST_H */
