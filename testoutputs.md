## Test 0
### Input
fn main()->int{
    x = 1 + 2 * 3;
    return 0;
}

### Tokens
Fn
Id(main)
OpenParen
CloseParen
Arrow
Int
OpenBrace
Id(x)
Gets
Num(1)
Plus
Num(2)
Star
Num(3)
Semicolon
Return
Num(0)
Semicolon
CloseBrace

### Output
Program(
  globals = [],
  structs = [],
  externs = [],
  functions = [
    Function(
      name = main,
      params = [],
      rettyp = Int,
      locals = [],
      stmts = [
        Assign(
          lhs = Id(x),
          rhs = BinOp(
            op = Add,
            left = Num(1),
            right = BinOp(
              op = Mul,
              left = Num(2),
              right = Num(3)
            )
          )
        ),
        Return(
          Num(0)
        )
      ]
    )
  ]
)

[ID] in function main: variable x undefined




## Test 2
### input
fn main(->int{
    int x;
    x = 1 + 2 * 3;
    return x;
}

### tokens
Fn
Id(main)
OpenParen
Arrow
Int
OpenBrace
Int
Id(x)
Semicolon
Id(x)
Gets
Num(1)
Plus
Num(2)
Star
Num(3)
Semicolon
Return
Id(x)
Semicolon
CloseBrace

### Output
parse error at token 3


## test 3
### input
fn test()->int{
    return 0;
}

### tokens
Fn
Id(test)
OpenParen
Int
Id(a)
Comma
Int
Id(b)
CloseParen
Arrow
Int
OpenBrace
Return
Num(0)
Semicolon
CloseBrace

### output
parse error at token 3


## test 4
fn test()->int{
    return 0;
}

### tokens
Fn
Id(test)
OpenParen
CloseParen
Arrow
Int
OpenBrace
Return
Num(0)
Semicolon
CloseBrace

### output
Program(
  globals = [],
  structs = [],
  externs = [],
  functions = [
    Function(
      name = test,
      params = [],
      rettyp = Int,
      locals = [],
      stmts = [
        Return(
          Num(0)
        )
      ]
    )
  ]
)

## test 4
fn main()->int{
    return 0;
}
fn test()->int{
    return 1;
}

### tokens
Fn
Id(main)
OpenParen
CloseParen
Arrow
Int
OpenBrace
Return
Num(0)
Semicolon
CloseBrace
Fn
Id(test)
OpenParen
CloseParen
Arrow
Int
OpenBrace
Return
Num(1)
Semicolon
CloseBrace


### output
Program(
  globals = [],
  structs = [],
  externs = [],
  functions = [
    Function(
      name = main,
      params = [],
      rettyp = Int,
      locals = [],
      stmts = [
        Return(
          Num(0)
        )
      ]
    ),
    Function(
      name = test,
      params = [],
      rettyp = Int,
      locals = [],
      stmts = [
        Return(
          Num(1)
        )
      ]
    )
  ]
)

## test 5
fn main()->int{
    return test();
}
fn test()->int{
    return 1;
}

### tokens
Fn
Id(main)
OpenParen
CloseParen
Arrow
Int
OpenBrace
Return
Id(test)
OpenParen
CloseParen
Semicolon
CloseBrace
Fn
Id(test)
OpenParen
CloseParen
Arrow
Int
OpenBrace
Return
Num(1)
Semicolon
CloseBrace

### output
Program(
  globals = [],
  structs = [],
  externs = [],
  functions = [
    Function(
      name = main,
      params = [],
      rettyp = Int,
      locals = [],
      stmts = [
        Return(
          Call(
            callee = Id(test),
            args = []
          )
        )
      ]
    ),
    Function(
      name = test,
      params = [],
      rettyp = Int,
      locals = [],
      stmts = [
        Return(
          Num(1)
        )
      ]
    )
  ]
)


# test 6
fn main()->int{
    return great();
}
fn test()->int{
    return 1;
}


### tokens
Fn
Id(main)
OpenParen
CloseParen
Arrow
Int
OpenBrace
Return
Id(great)
OpenParen
CloseParen
Semicolon
CloseBrace
Fn
Id(test)
OpenParen
CloseParen
Arrow
Int
OpenBrace
Return
Num(1)
Semicolon
CloseBrace

### output
Program(
  globals = [],
  structs = [],
  externs = [],
  functions = [
    Function(
      name = main,
      params = [],
      rettyp = Int,
      locals = [],
      stmts = [
        Return(
          Call(
            callee = Id(great),
            args = []
          )
        )
      ]
    ),
    Function(
      name = test,
      params = [],
      rettyp = Int,
      locals = [],
      stmts = [
        Return(
          Num(1)
        )
      ]
    )
  ]
)

[ID] in function main: variable great undefined


## Test 7
### program
fn main() -> int {
  let a: int = 10;
  if a == 10 {
    a = 11;
    return a;
  }

  else{
    a = a + 5;
    return a;
  }
  return 0;
}


### Tokens
Fn
Id(main)
OpenParen
CloseParen
Arrow
Int
OpenBrace
Let
Id(a)
Colon
Int
Gets
Num(10)
Semicolon
If
Id(a)
Equal
Num(10)
OpenBrace
Id(a)
Gets
Num(11)
Semicolon
Return
Id(a)
Semicolon
CloseBrace
Else
OpenBrace
Id(a)
Gets
Id(a)
Plus
Num(5)
Semicolon
Return
Id(a)
Semicolon
CloseBrace
Return
Num(0)
Semicolon
CloseBrace

### Output
Program(
  globals = [],
  structs = [],
  externs = [],
  functions = [
    Function(
      name = main,
      params = [],
      rettyp = Int,
      locals = [(Decl(a, Int), Num(10))],
      stmts = [
        If(
          guard = BinOp(
            op = Equal,
            left = Id(a),
            right = Num(10)
          ),
          tt = [
            Assign(
              lhs = Id(a),
              rhs = Num(11)
            ),
            Return(
              Id(a)
            )
          ],
          ff = [
            Assign(
              lhs = Id(a),
              rhs = BinOp(
                op = Add,
                left = Id(a),
                right = Num(5)
              )
            ),
            Return(
              Id(a)
            )
          ]
        ),
        Return(
          Num(0)
        )
      ]
    )
  ]
)


### With no else
Program(
  globals = [],
  structs = [],
  externs = [],
  functions = [
    Function(
      name = main,
      params = [],
      rettyp = Int,
      locals = [(Decl(a, Int), Num(10))],
      stmts = [
        If(
          guard = BinOp(
            op = Equal,
            left = Id(a),
            right = Num(10)
          ),
          tt = [
            Assign(
              lhs = Id(a),
              rhs = Num(11)
            ),
            Return(
              Id(a)
            )
          ],
          ff = []
        ),
        Return(
          Num(0)
        )
      ]
    )
  ]
)


## Test 8
### program
fn fn() -> int {
  return 0;
}

struct list {
  value: int,
  next: &list
}

let a: int = 10;


# general steps:
# 1. go through tokens and create AST while checking for parsing errors
# 2. create validation functions for globals, structs,  with AST as input; return 