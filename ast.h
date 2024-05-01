#ifndef AST_H
#define AST_H

#include <iostream>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#define NO_ERR -2
#define END_OF_FILE -3

// Global variable declarations
extern std::vector<std::string> tokens;
extern int parsing_index;

// Type aliases (to make things easier to understand)
using StructId = std::string;
using FuncId = std::string;

// Forward declaration of structs
struct Type;
struct UnaryOp;
struct BinaryOp;
struct Exp;
struct Program;
struct Decl;
struct Struct;
struct Function;
struct Rhs;
struct Lval;
struct Stmt;

// Forward declaration of functions
int create_ast(Program *ast);
Program *initialize_ast();

int parse_global(Program *ast);
int parse_decls(Program *ast);
int parse_decl(Program *ast);
int parse_type(Program *ast, Type *typeField);
int parse_type_ad(Program *ast, Type *typeField);
int parse_type_op(Program *ast, std::vector<Type> *type_params,
                  Type *fn_ret_type);
int parse_type_ar(Program *ast, Type *prog_type);
int parse_rettyp(Program *ast, Type *ret_type);
int parse_type_fp(Program *ast, Type *ret_type);
int runFunction(Program *ast);
int runStruct(Program *ast);
int runExtern(Program *ast);
int checkValidDecls(std::vector<Decl> *globals);

bool checkValidIndex();

/*
 * struct: Type
 *
 * Represents a Type in the AST, which is either an int, Struct, Function, or
 * Pointer.
 *
 */
struct Type {
  enum class TypeKind { Int, Struct, Fn, Ptr };
  TypeKind kind;
  std::string struct_name;
  std::vector<Type> params;
  Type *ret_type;
  Type *pointed_to_type;

  // Int constructor
  Type() : ret_type(nullptr), pointed_to_type(nullptr) {}
  // Type() : kind(TypeKind::Int) {}

  // Struct constructor
  // Type(const StructId &name) : kind(TypeKind::Struct), struct_name(name) {}
  //
  // Fn constructor
  // Type(const std::vector<Type> &params, const Type &ret_type)
  //     : kind(TypeKind::Fn), params(params), ret_type(new Type(ret_type)) {}

  // Ptr constructor
  // Type(const Type *in_pointed_to_type)
  //     : kind(TypeKind::Ptr), pointed_to_type(in_pointed_to_type) {}

  // Destructor for memory cleanup from new calls
  // ~Type() {
  //   delete ret_type;
  //   delete pointed_to_type;
  // }
};

/*
 * struct: UnaryOp
 *
 * Represents a unary operation (negation, dereference)
 *
 */
struct UnaryOp {
  // Represents a unary operation (e.g., negation, dereference).
  enum class UnOpKind { Neg, Deref };
  UnOpKind kind;

  UnaryOp(const std::string &s) {
    if (s == "Neg") {
      kind = UnOpKind::Neg;
    } else if (s == "Deref") {
      kind = UnOpKind::Deref;
    }
  }

  UnaryOp(const UnaryOp &unary_operation) : kind(unary_operation.kind){};
};

/*
 * struct: BinaryOp
 *
 * Represents a binary operation (Add, Subtract, Multiply, Divide, Equals,
 * NotEquals, LessThan, LessThanEqual, GreaterThan, GreaterThanEqual)
 *
 */
struct BinaryOp {
  enum class BinOpKind { Add, Sub, Mul, Div, Equal, NotEq, Lt, Lte, Gt, Gte };
  BinOpKind kind;

  BinaryOp(std::string s) {
    if (s == "Add") {
      kind = BinOpKind::Add;
    } else if (s == "Sub") {
      kind = BinOpKind::Sub;
    } else if (s == "Mul") {
      kind = BinOpKind::Mul;
    } else if (s == "Div") {
      kind = BinOpKind::Div;
    } else if (s == "Equal") {
      kind = BinOpKind::Equal;
    } else if (s == "NotEq") {
      kind = BinOpKind::NotEq;
    } else if (s == "Lt") {
      kind = BinOpKind::Lt;
    } else if (s == "Lte") {
      kind = BinOpKind::Lte;
    } else if (s == "Gt") {
      kind = BinOpKind::Gt;
    } else if (s == "Gte") {
      kind = BinOpKind::Gte;
    }
  };
};

/*
 * struct: Exp
 *
 * Represents an expression in the program.
 * This could be a number, identifier, nul, unary operation, binary operation,
 * array access, field access, or function call.
 *
 */
struct Exp {
  enum class ExpKind {
    Num,
    Id,
    Nil,
    UnOp,
    BinOp,
    ArrayAccess,
    FieldAccess,
    Call
  };
  ExpKind kind;

  int32_t num_val;            // Num
  std::string id_name;        // Id
  UnaryOp *un_op = nullptr;   // UnOp
  Exp *un_operand = nullptr;  // UnOp
  BinaryOp *bin_op = nullptr; // BinOp
  Exp *left_exp = nullptr;    // BinOp
  Exp *right_exp = nullptr;   // BinOp
  Exp *array_ptr = nullptr;   // ArrayAccess
  Exp *array_exp = nullptr;   // ArrayAccess
  Exp *field_ptr = nullptr;   // FieldAccess
  std::string field_val;      // FieldAccess
  Exp *call_callee = nullptr; // Call
  std::vector<Exp> call_args; // Call

  // Num constructor
  Exp(const int32_t &n) : kind(ExpKind::Num), num_val(n){};

  // Id constructor
  Exp(const std::string &name) : kind(ExpKind::Id), id_name(name){};

  // Nil constructor
  Exp() : kind(ExpKind::Nil){};

  // UnOp constructor
  Exp(const UnaryOp &op, const Exp &operand)
      : kind(ExpKind::UnOp), un_op(new UnaryOp(op)),
        un_operand(new Exp(operand)){};

  // BinOp constructor
  Exp(const BinaryOp &op, const Exp &left, const Exp &right)
      : kind(ExpKind::BinOp), bin_op(new BinaryOp(op)), left_exp(new Exp(left)),
        right_exp(new Exp(right)){};

  // ArrayAccess constructor
  Exp(Exp ptr, Exp index)
      : kind(ExpKind::ArrayAccess), array_ptr(new Exp(ptr)),
        array_exp(new Exp(index)){};

  // FieldAccess constructor
  Exp(Exp ptr, std::string field)
      : kind(ExpKind::FieldAccess), field_ptr(new Exp(ptr)), field_val(field){};

  // Call constructor
  Exp(Exp callee, std::vector<Exp> args)
      : kind(ExpKind::Call), call_callee(new Exp(callee)), call_args(args){};
};

/*
 * struct: Program
 *
 * Represents the entire program.
 * Contains lists of global variables, struct definitions, extern declarations,
 * and function definitions.
 *
 */
struct Program {
  std::vector<Decl> globals;
  std::vector<Struct> structs;
  std::vector<Decl> externs;
  std::vector<Function> functions;
};

/*
 * struct: Decl
 *
 * Represents a declaration of a variable or field. Contains the name of the
 * variable/field and it's type.
 */
struct Decl {
  std::string name;
  Type *type;
};

/*
 * struct: Struct
 *
 * Represents a struct definition
 * contains the name of the struct and a lsit of its fields
 *
 */
struct Struct {
  StructId name;
  Type *type;
};

/*
 * struct: Function
 *
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
 * struct: Rhs
 *
 * Represents the right hand side of an assignment or new expression
 * It could be an expression or a new operation (for allocating memory)
 *
 */
struct Rhs {
  // Represents the right-hand side of an assignment or new expression.
  // It could be an expression or a new operation (for allocating memory).

  enum class RhsKind { RhsExp, New };
  RhsKind kind;
  Exp *rhs_exp;
  Type *new_type;
  Exp *new_amount;

  // RhsExp constructor
  Rhs(const Exp &exp) : kind(RhsKind::RhsExp), rhs_exp(new Exp(exp)) {}

  // New constructor
  Rhs(const Type &type, const Exp &amount)
      : kind(RhsKind::New), new_type(new Type(type)),
        new_amount(new Exp(amount)) {}
};

struct Lval {
  // Represents the left-hand side of an assignment or a function call.
  // It could be an identifier, a dereference operation, an array access, or a
  // field access.

  enum class LvalKind { Id, Deref, ArrayAccess, FieldAccess };
  LvalKind kind;
  std::string id_name;
  Lval *deref_lval;
  Lval *array_ptr;
  Exp *array_index;
  Lval *field_ptr;
  std::string acc_field;

  // Id constructor
  Lval(std::string name) : kind(LvalKind::Id), id_name(name) {}

  // Deref constructor
  Lval(const Lval &lval) : kind(LvalKind::Deref), deref_lval(new Lval(lval)) {}

  // ArrayAccess constructor
  Lval(const Lval &ptr, const Exp &index)
      : kind(LvalKind::ArrayAccess), array_ptr(new Lval(ptr)),
        array_index(new Exp(index)) {}

  // FieldAccess constructor
  Lval(const Lval &ptr, const std::string field)
      : kind(LvalKind::FieldAccess), field_ptr(new Lval(ptr)),
        acc_field(field) {}
};

/*
 * struct Stmt
 *
 * Represents a statement in the program
 * This could be a break, continue, return, assignment, function vall, if
 * statement, or while loop
 *
 */
struct Stmt {
  enum class StmtKind { Break, Continue, Return, Assign, Call, If, While };
  StmtKind kind;
  std::optional<Exp> *return_expression; // Return
  Lval *left_hand_side = nullptr;        // Assign
  Rhs *right_hand_side = nullptr;        // Assign
  Lval *callLval = nullptr;              // Call
  std::vector<Exp> call_arguments;       // Call
  Exp *exp_guard = nullptr;              // While, If
  std::vector<Stmt> true_vals;           // If
  std::vector<Stmt> false_vals;          // If
  std::vector<Stmt> while_body;          // While

  // Break & Continue constructor
  Stmt(std::string s) {
    if (s == "Break") {
      kind = StmtKind::Break;
    } else if (s == "Continue") {
      kind = StmtKind::Continue;
    } else {
      // shouldn't go here
    }
  }

  // Return constructor (TODO: Check if an extra parameter is needed since this
  // uses std::option)
  Stmt(const std::optional<Exp> &exp)
      : kind(StmtKind::Return), return_expression(new std::optional<Exp>(exp)) {
  }

  // Assign constructor
  Stmt(const Lval &lhs, const Rhs &rhs)
      : kind(StmtKind::Assign), left_hand_side(new Lval(lhs)),
        right_hand_side(new Rhs(rhs)) {}

  // Call constructor
  Stmt(Lval callee, std::vector<Exp> args)
      : kind(StmtKind::Call), callLval(new Lval(callee)), call_arguments(args) {
  }

  // If constructor
  Stmt(Exp guard, std::vector<Stmt> tt, std::vector<Stmt> ff)
      : kind(StmtKind::If), exp_guard(new Exp(guard)), true_vals(tt),
        false_vals(ff) {}

  // While constructor; While { guard: Exp, body: vector<Stmt> }
  Stmt(Exp guard, std::vector<Stmt> body)
      : kind(StmtKind::While), exp_guard(new Exp(guard)), while_body(body) {}
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
