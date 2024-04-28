#ifndef AST_H
#define AST_H

#include <optional>
#include <string>
#include <variant>
#include <vector>

// Type aliases (to make things easier to understand)
using StructId = std::string;
using FuncId = std::string;

// Forward declaration of structs
struct Type;
struct Program;
struct Decl;
struct Struct;
struct Function;
struct Stmt;
struct Rhs;
struct Lval;
struct Exp;
struct UnaryOp;
struct BinaryOp;

/*
 * struct: Type
 * represents a Type in the AST, which is either an int, Struct, Function, or
 * Pointer.
 * TODO: Figure out how to create these structs
 */
struct Type {
  enum class TypeKind { Int, Struct, Fn, Ptr };
  TypeKind kind;
  std::optional<std::string> struct_name;
  std::vector<Type> params;
  Type *ret_type;
  Type *pointed_to_type;

  // Int constructor
  Type() : kind(TypeKind::Int), ret_type(nullptr), pointed_to_type(nullptr) {}

  // Struct constructor
  Type(const StructId &name)
      : kind(TypeKind::Struct), struct_name(name), ret_type(nullptr),
        pointed_to_type(nullptr) {}

  // Fn constructor
  Type(const std::vector<Type> &params, const Type &ret_type)
      : kind(TypeKind::Fn), params(params), ret_type(new Type(ret_type)),
        pointed_to_type(nullptr) {}

  // Ptr constructor
  Type(const Type &pointed_to_type)
      : kind(TypeKind::Ptr), pointed_to_type(new Type(pointed_to_type)),
        ret_type(nullptr) {}

  // Destructor for memory cleanup from new calls
  ~Type() {
    delete ret_type;
    delete pointed_to_type;
  }
};

/*
 * struct: Program
 * Represents the entire program.
 * Contains lists of global variables, struct definitions, extern declarations,
 * and function definitions.
 */
struct Program {
  std::vector<Decl> globals;
  std::vector<Struct> structs;
  std::vector<Decl> externs;
  std::vector<Function> functions;
};

/*
 * struct: Decl
 * Represents a declaration of a variable or field. Contains the name of the
 * variable/field and it's type.
 */
struct Decl {
  std::string name;
  Type *type;
};

/*
 * struct: Struct
 * Represents a struct definition
 * contains the name of the struct and a lsit of its fields
 */
struct Struct {
  StructId name;
  Type *type;
};

/*
 * struct: Function
 * Represents a function definition
 * Contains the name of the function, parameters, return type, locals, and
 * statements
 */
struct Function {
  FuncId name;
  std::vector<Decl> params;
  std::optional<Type> rettyp;
  std::vector<std::pair<Decl, std::optional<Exp>>> locals;
  std::vector<Stmt> stmts;
};

/*
 * struct Stmt
 * Represents a statement in the program
 * This could be a break, continue, return, assignment, function vall, if
 * statement, or while loop
 * TODO: Figure out how to create these structs
 */
struct Stmt {
  enum class StmtKind { Break, Continue, Return, Assign, Call, If, While };
  StmtKind kind;
  Exp *return_expression;
  Lval *left_hand_side;
  Rhs *right_hand_side;
  Lval *callLval;
  std::vector<Exp> call_arguments;
  // Other contents Idk how do do
};

struct Rhs {
  // Represents the right-hand side of an assignment or new expression.
  // It could be an expression or a new operation (for allocating memory).
};

struct Lval {
  // Represents the left-hand side of an assignment or a function call.
  // It could be an identifier, a dereference operation, an array access, or a
  // field access.
};

struct Exp {
  // Represents an expression in the program.
  // This could be a number, identifier, nil, unary operation, binary operation,
  // array access, field access, or function call.
};

struct UnaryOp {
  // Represents a unary operation (e.g., negation, dereference).
};

struct BinaryOp {
  // Represents a binary operation (e.g., addition, subtraction, comparison).
};

// Used in struct definitions
enum class TokenType {
  Num,
  Id,
  Int,
  Struct,
  Nil,
  Break,
  Continue,
  Return,
  If,
  Else,
  While,
  New,
  Let,
  Extern,
  Fn,
  Address,
  Colon,
  Semicolon,
  Comma,
  Underscore,
  Arrow,
  Plus,
  Dash,
  Star,
  Slash,
  Equal,
  NotEq,
  Lt,
  Lte,
  Gt,
  Gte,
  Dot,
  Gets,
  OpenParen,
  CloseParen,
  OpenBracket,
  CloseBracket,
  OpenBrace,
  CloseBrace
};

// Token struct
struct Token {
  TokenType type;
  std::string value; // For Id and Num tokens
};

enum class ASTNodeType {
  // cflat types, '&' is right-associative.
  type,
  type_ad,
  type_op,
  type_fp,
  type_ar,

  // function type (used in extern declarations
  funtype,
  rettyp,

  // cflat program
  program,
  toplevel,

  // global variable declaration
  glob,

  // struct type definition
  ASTtypedef,

  // variable/field declaration(s)
  decl,
  decls,

  // external function declaration
  ASTextern,

  // function definition
  fundef,

  // local variable declaration / initialization
  let,

  // statement
  stmt,

  // A conditional with optional else clause
  cond,

  // loops : 'while' loop
  loop,

  // a block of statements
  block,

  // assignment or function call
  assign_or_call,
  gets_or_args,
  rhs,

  // lvalues for assignments and call statements
  lval,
  access,

  // call arguments
  args,

  // expression
  exp,
  exp_p4,
  exp_p3,
  exp_p2,
  exp_p1,
  exp_ac,

  // unary operators
  unop,

  // binary operators
  binoo_p1,
  binop_p2,
  binop_p3,

};

struct ASTNode {
  ASTNodeType type;
  std::variant<std::string, int> value;
  std::vector<ASTNode> children;
};

#endif /* AST_H */
