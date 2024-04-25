## Naming Rules
- No duplicate names among globals, externs, and functions
- No duplicate names among struct types, and for a given struct type no duplicate names among it's fields.
- For a given function, no duplicate names amongst its parameters and no duplicate names amongst its locals.

## Miscellaneous Rules
- There exists a function called main and its type is () -> int.
- For every function, all paths from that functions entry reach a Return statement.

## Type Checking
### Types
Construct the following based on needs.
t: Type ::= int | struct_Id | &t | (vector(t)) -> t^?
A type is either: integer, struct (qualified by the name of the struct), pointer (qualified by the type being pointed to), function (qualified by the types of parameters and an optional return type).
i.e The only types are: int, struct_Id, pointer, function.
Examples:
- Function taking an int and a pointer to an int, returning an int has type: (int, &int) -> int.
- Function taking an int and a pointer to an int, returning nothing has type: (int, &int) -> _. 
