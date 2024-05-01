#include "ast.h"

int parsing_index;

/*
 * function: initialize_ast()
 *
 * Initializes all the fields for a Program block
 *
 */
struct Program *initialize_ast() {
  struct Program *our_prog = new struct Program;
  our_prog->globals = {};
  our_prog->structs = {};
  our_prog->externs = {};
  our_prog->functions = {};

  return our_prog;
}

/*
 * function: create_ast()
 *
 * Initializes all the fields for a Program block
 *
 */
int create_ast(Program *ast) {

  parsing_index = 0;
  int err = NO_ERR;
  std::string token;

  while (parsing_index < tokens.size()) {
    token = tokens[parsing_index];
    // handle a global

    if (token == "Let") {
      std::cout << "Handling a global" << std::endl;
      std::cout << "Index " << parsing_index << std::endl;
      if (!checkValidIndex()) {
        // Consume "Let"
        return -1;
      }
      err = parse_global(ast);
      if (err == END_OF_FILE) {
        err = NO_ERR;
        break;
      }
      if (err != NO_ERR) {
        return err;
      }
    }

    // handle a function
    else if (token == "Fn") {
      err = runFunction(ast);
      if (err != NO_ERR) {
        return err;
      }
    }

    // handle a struct
    else if (token == "Struct") {
      err = runStruct(ast);
      if (err != NO_ERR) {
        return err;
      }
    }

    // handle an extern
    else if (token == "Extern") {
      err = runExtern(ast);
      if (err != NO_ERR) {
        return err;
      }
    }

    else {
      // Not a Fn, Struct, Extern, or Global, return parse error at this token
      return parsing_index;
    }
  }

  return err;
}

/*
 * function: parse_global
 *
 * parses a single line of globals.
 *
 * glob ::= `let` decls `;`
 */
int parse_global(Program *ast) {
  std::cout << "funct: parse_global" << std::endl;
  // 'let' was handled previously
  int ret_val;
  std::string curr_token;

  ret_val = parse_decls(ast);
  if (ret_val != NO_ERR) {
    return ret_val;
  }

  curr_token = tokens[parsing_index];
  if (curr_token != "Semicolon") {
    return parsing_index;
  }
  if (!checkValidIndex()) {
    // Consume "Semicolon"
    return END_OF_FILE;
  }

  return NO_ERR;
}

/*
 * function: parse_decls
 *
 * parses a single decls.
 *
 * decls ::= decl (`,` decl)*
 */
int parse_decls(Program *ast) {
  std::cout << "funct: parse_decls" << std::endl;
  int ret_val;
  std::string curr_token;

  // decl
  ret_val = parse_decl(ast);
  if (ret_val != NO_ERR) {
    return ret_val;
  }

  // (',' decl)*
  while (1) {
    curr_token = tokens[parsing_index];
    if (curr_token != "Comma") {
      // Success on (',' decl)*
      return NO_ERR;
    }

    if (!checkValidIndex()) {
      // Consume "Comma"
      return -1;
    }

    ret_val = parse_decl(ast);
    if (ret_val != NO_ERR) {
      return ret_val;
    }
  }
}

/*
 * function: parse_decl
 *
 * parse one decl
 * appends it to the globals list here
 *
 */
int parse_decl(Program *ast) {
  std::cout << "funct: parse_decl" << std::endl;
  // id
  std::string token = tokens[parsing_index];

  if (token.length() < 3 || token.substr(0, 3) != "Id(") {
    return parsing_index;
  }
  std::string functionName = token.substr(3, token.length() - 4);

  // Creating the Decl object that will get stored in the Decl array for globals
  struct Decl *currentDecl = new Decl();
  // Assigning the string for the name
  currentDecl->name = functionName;

  if (!checkValidIndex()) {
    // Consume "Id([...])"
    return -1;
  }

  // ':'
  token = tokens[parsing_index];
  if (token != "Colon") {
    return parsing_index;
  }
  if (!checkValidIndex()) {
    // Consume "Colon"
    return -1;
  }

  // type
  struct Type *typeCreated = new Type();
  int ret_val = parse_type(ast, typeCreated);

  // Set the final field of Decl
  currentDecl->type = typeCreated;

  std::cout << "addr of TypeCreated" << typeCreated << std::endl;

  if (ret_val != NO_ERR) {
    return ret_val;
  }

  // Place created vector onto the AST
  ast->globals.push_back(*currentDecl);
  return NO_ERR;
}

/*
 * function: parse_type
 *
 * parses a single type.
 *
 *  type ::= `&`* type_ad
 */
int parse_type(Program *ast, Type *typeField) {
  std::cout << "funct: parse_type" << std::endl;
  int ret_val;
  std::string curr_token;

  // '&'*
  struct Type *origAddr = typeField;
  curr_token = tokens[parsing_index];
  while (1) {
    if (curr_token == "Address") {
      if (!checkValidIndex()) {
        // Consume "Address"

        std::cout << "ERR236, ind: " << parsing_index << std::endl;
        return -1;
      }

      // Constructing pointer Type struct
      typeField->kind = Type::TypeKind::Ptr;
      struct Type *typePointed = new Type();
      typeField->pointed_to_type = typePointed;
      typeField = typePointed;
      curr_token = tokens[parsing_index];

    } else {
      break;
    }
  }

  // type_ad
  ret_val = parse_type_ad(ast, typeField);
  if (ret_val != NO_ERR) {
    std::cout << "ERR253" << parsing_index << std::endl;
    return ret_val;
  }
  typeField = origAddr;

  return NO_ERR;
}

// let sai:&&int;
// parse_type(nextIndex) &int
// parse_type(nextIndex) int
//
/*
 * function: parse_type_ad
 *
 * parses a single type_ad.
 *
 * type_ad ::= `int`
 *         | id
 *         | `(` type_op
 */
int parse_type_ad(Program *ast, Type *typeField) {
  std::cout << "funct: parse_type_ad" << std::endl;
  int ret_val;
  std::string curr_token;

  curr_token = tokens[parsing_index];
  // 'int'
  if (curr_token == "Int") {
    if (!checkValidIndex()) {
      // Consume "Int"
      return -1;
    }

    // Assigning the Type struct for an int
    // end of recursive call
    typeField->kind = Type::TypeKind::Int;

    return NO_ERR;
  }

  // id
  else if (curr_token.length() > 3 && curr_token.substr(0, 3) == "Id(") {
    std::string functionName = curr_token.substr(3, curr_token.length() - 4);
    // TODO: Implement the id field
    if (!checkValidIndex()) {
      // Consume "Id([...])"
      return -1;
    }

    // Initialise the typefield struct for id (Struct)
    typeField->kind = Type::TypeKind::Struct;
    typeField->struct_name = functionName;
    return NO_ERR;
  }

  // '(' type_op
  else if (curr_token == "OpenParen") {
    if (!checkValidIndex()) {
      // Consume "OpenParen"
      return -1;
    }

    // type_op
    typeField->kind = Type::TypeKind::Fn;
    std::vector<Type> type_params;
    Type *type_fn_ret_type = new Type();
    // This function will return the type_params and the ret_type that are
    // stored
    ret_val = parse_type_op(ast, &type_params, type_fn_ret_type);

    if (ret_val != NO_ERR) {
      std::cout << "ERR318" << std::endl;
      return ret_val;
    }

    // Return success
    typeField->params = type_params;        // Get params from recursive call
    typeField->ret_type = type_fn_ret_type; // Get ret_type from recursive call
    return NO_ERR;
  }

  std::cout << "ERR325" << parsing_index << std::endl;
  return parsing_index;
}

/*
 * function: parse_type_op
 *
 * parses a single type_op.
 *
 * type_op ::= `)` type_ar
 *          | type type_fp
 */
int parse_type_op(Program *ast, std::vector<Type> *type_params,
                  Type *fn_ret_type) {
  std::cout << "funct: parse_type_op" << std::endl;
  int ret_val;
  std::string curr_token;

  curr_token = tokens[parsing_index];
  // ')' type_ar
  if (curr_token == "CloseParen") {
    if (!checkValidIndex()) {
      // Consume "CloseParen"
      return -1;
    }

    // type_ar
    ret_val = parse_type_ar(ast, fn_ret_type);
    if (ret_val != NO_ERR) {
      std::cout << "ERR352" << std::endl;
      return ret_val;
    }

    // End of recursive call
    return NO_ERR;
  }

  // type type_fp
  else {
    struct Type *newestType = new Type();
    ret_val = parse_type(ast, newestType);
    if (ret_val != NO_ERR) {
      std::cout << "ERR364" << std::endl;
      return ret_val;
    }
    std::cout << "YEA pushed one int" << std::endl;
    type_params->push_back(*newestType);

    ret_val = parse_type_fp(ast, fn_ret_type, type_params);
    if (ret_val != NO_ERR) {
      std::cout << "ERR370" << std::endl;
      return ret_val;
    }

    return NO_ERR;
  }
}

/*
 * function: parse_type_ar
 *
 * parses a single type_ar.
 *
 * type_ar ::= `->` rettyp
 */
int parse_type_ar(Program *ast, Type *ret_type) {
  std::cout << "funct: parse_type_ar" << std::endl;
  int ret_val;
  std::string curr_token;

  // '->' rettyp
  curr_token = tokens[parsing_index];
  if (curr_token == "Arrow") {
    if (!checkValidIndex()) {
      // Consume "Arrow"
      return -1;
    }

    // RETURN 01
    // rettyp
    ret_val = parse_rettyp(ast, ret_type);
    if (ret_val != NO_ERR) {
      std::cout << "ERR401" << std::endl;
      return ret_val;
    }

    return NO_ERR;
  }
  std::cout << "ERR407" << std::endl;

  return parsing_index;
}

/*
 * function: parse_rettyp
 *
 * parses a single rettyp
 *
 * rettyp ::= type
 *          | `_`
 */
int parse_rettyp(Program *ast, Type *ret_type) {
  std::cout << "funct: parse_rettyp" << std::endl;
  int ret_val;
  std::string curr_token;
  curr_token = tokens[parsing_index];

  // type
  int orig_index = parsing_index;
  ret_val = parse_type(ast, ret_type);
  if (ret_val == NO_ERR) {
    return NO_ERR;
  }

  // '_'
  else if (tokens[orig_index] == "Underscore") {
    // Set the value of ret_type to null, this signifies empty
    ret_type = nullptr;
    parsing_index = orig_index; // resetting the parsing_index global
    if (!checkValidIndex()) {
      // Consume "Underscore"
      return -1;
    }
    return NO_ERR;
  }

  parsing_index = orig_index;
  std::cout << "ERR446" << std::endl;
  return parsing_index;
}

/*
 * function: parse_type_fp
 *
 * parses a single type_fp
 *
 *
 * type_fp ::= `)` type_ar?
 *         | (`,` type)+ `)` type_ar
 */
int parse_type_fp(Program *ast, Type *func_ret_type,
                  std::vector<Type> *param_types) {
  std::cout << "funct: parse_type_fp" << std::endl;
  int ret_val;
  std::string curr_token;

  curr_token = tokens[parsing_index];
  Type old_ret_type = *func_ret_type;
  // ')' type_ar?
  if (curr_token == "CloseParen") {
    if (!checkValidIndex()) {
      // Consume "CloseParen"
      return -1;
    }

    // type_ar?
    int prev_index = parsing_index;
    ret_val = parse_type_ar(ast, func_ret_type); // TODO: FIX
    if (ret_val != NO_ERR) {
      // Reset parsing index in the case that type_ar fails
      // Not an error, ? allows for 1 or 0 instances
      // this is resetting the ret_type value to undo anything from the function
      // call
      parsing_index = prev_index;
      func_ret_type->kind = Type::TypeKind::NulPtr;
    }

    return NO_ERR;
  }

  // (',' type)+ ')' type_ar
  else {
    // ',' type   (ensuring at least one instance from + operator)
    if (curr_token != "Comma") {
      return parsing_index;
    }
    if (!checkValidIndex()) {
      // Consume "Comma"
      return -1;
    }

    struct Type *currentType = new Type();
    ret_val = parse_type(ast, currentType);
    if (ret_val != NO_ERR) {
      return ret_val;
    }

    std::cout << "YEA pushed one int" << std::endl;
    param_types->push_back(*currentType);
    delete currentType;

    // (',' type)+   (essentially star operator)
    while (1) {
      // No comma means ending the *
      if (curr_token != "Comma") {
        break;
      }
      if (!checkValidIndex()) {
        // Consume "Comma"
        return -1;
      }

      currentType = new Type();
      // Invalid type parse means error.
      ret_val = parse_type(ast, currentType);
      if (ret_val != NO_ERR) {
        return ret_val;
      }
      param_types->push_back(*currentType);
      delete currentType;
    }

    // ')'
    curr_token = tokens[parsing_index];
    if (curr_token != "CloseParen") {
      return parsing_index;
    }
    if (!checkValidIndex()) {
      // Consume "CloseParen"
      return -1;
    }

    // type_ar
    curr_token = tokens[parsing_index];
    ret_val = parse_type_ar(ast, func_ret_type);
    if (ret_val != NO_ERR) {
      return ret_val;
    }

    return NO_ERR;
  }
}

int runFunction(Program *ast) {
  // Figure out if function is okay

  return NO_ERR;
}

int runStruct(Program *ast) {
  // Figure out if struct is okay

  return NO_ERR;
}

int runExtern(Program *ast) {
  // Figure out if extern is okay

  return NO_ERR;
}

bool checkValidIndex() {
  parsing_index++;
  if (parsing_index >= tokens.size()) {
    return false;
  }
  std::cout << "Consumed token " << parsing_index - 1 << ", "
            << tokens[parsing_index - 1] << std::endl;
  return true;
}
