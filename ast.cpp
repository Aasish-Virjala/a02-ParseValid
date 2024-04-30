#include "ast.h"

int parsing_index;

struct Program *initialize_ast() {
  struct Program *our_prog = new struct Program;
  our_prog->globals = {};
  our_prog->structs = {};
  our_prog->externs = {};
  our_prog->functions = {};

  return our_prog;
}

int create_ast() {
  struct Program *ast = initialize_ast();

  std::cout << "Globals: " << ast->globals.size()
            << "\nStructs: " << ast->structs.size()
            << "\nExterns: " << ast->externs.size()
            << "\nFunctions: " << ast->functions.size() << std::endl;

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

  std::cout << "returning err" << std::endl;

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
  std::cout << "In parse_global" << std::endl;
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
  std::cout << "Validation" << std::endl;
  if (!checkValidIndex()) {
    // Consume "Semicolon"
    return END_OF_FILE;
  }
  // TODO: Append this to the AST

  std::cout << "No ERR" << std::endl;
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
  std::cout << "In parse_decls" << std::endl;
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
 *
 */
int parse_decl(Program *ast) {
  std::cout << "In parse_decl" << std::endl;
  // id
  std::string token = tokens[parsing_index];
  if (token.length() < 3 || token.substr(0, 3) != "Id(") {
    return parsing_index;
  }
  std::string functionName = token.substr(3, token.length() - 4);

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
  int ret_val = parse_type(ast);
  if (ret_val != NO_ERR) {
    return ret_val;
  }

  return NO_ERR;
}

/*
 * function: parse_type
 *
 * parses a single type.
 *
 *  type ::= `&`* type_ad
 */
int parse_type(Program *ast) {
  std::cout << "In parse_type" << std::endl;
  int ret_val;
  std::string curr_token;

  // '&'*
  while (1) {
    curr_token = tokens[parsing_index];
    if (curr_token == "Address") {
      // TODO: Wrap the type in Ptr(type)
      if (!checkValidIndex()) {
        // Consume "Address"
        return -1;
      }
    } else {
      break;
    }
  }

  // type_ad
  ret_val = parse_type_ad(ast);
  if (ret_val != NO_ERR) {
    return ret_val;
  }

  return NO_ERR;
}

/*
 * function: parse_type_ad
 *
 * parses a single type_ad.
 *
 * type_ad ::= `int`
 *         | id
 *         | `(` type_op
 */
int parse_type_ad(Program *ast) {
  std::cout << "In parse_type_ad" << std::endl;
  int ret_val;
  std::string curr_token;

  curr_token = tokens[parsing_index];
  // 'int'
  if (curr_token == "Int") {
    // TODO: Implement the Int field, that may/may not be wrapped in ptr
    if (!checkValidIndex()) {
      // Consume "Int"
      return -1;
    }

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

    return NO_ERR;
  }

  // '(' type_op
  else if (curr_token == "OpenParen") {
    if (!checkValidIndex()) {
      // Consume "OpenParen"
      return -1;
    }

    // type_op
    ret_val = parse_type_op(ast);
    if (ret_val != NO_ERR) {
      return ret_val;
    }

    return NO_ERR;
  }

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
int parse_type_op(Program *ast) {
  std::cout << "In parse_type_op" << std::endl;
  int ret_val;
  std::string curr_token;

  // ')' type_ar
  if (curr_token == "CloseParen") {
    if (!checkValidIndex()) {
      // Consume "CloseParen"
      return -1;
    }

    // type_ar
    ret_val = parse_type_ar(ast);
    if (ret_val != NO_ERR) {
      return ret_val;
    }

    return NO_ERR;
  }

  // type type_fp
  else {
    ret_val = parse_type(ast);
    if (ret_val != NO_ERR) {
      return ret_val;
    }

    ret_val = parse_type_fp(ast);
    if (ret_val != NO_ERR) {
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
int parse_type_ar(Program *ast) {
  std::cout << "In parse_type_ar" << std::endl;
  int ret_val;
  std::string curr_token;

  // '->' rettyp
  curr_token = tokens[parsing_index];
  if (curr_token == "Arrow") {
    if (!checkValidIndex()) {
      // Consume "Arrow"
      return -1;
    }

    // rettyp
    ret_val = parse_rettyp(ast);
    if (ret_val != NO_ERR) {
      return ret_val;
    }

    return NO_ERR;
  }

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
int parse_rettyp(Program *ast) {
  std::cout << "In parse_rettyp" << std::endl;
  int ret_val;
  std::string curr_token;
  curr_token = tokens[parsing_index];

  // type
  int orig_index = parsing_index;
  ret_val = parse_type(ast);
  if (ret_val == NO_ERR) {
    return NO_ERR;
  }

  // '_'
  else if (tokens[orig_index] == "Underscore") {
    // TODO: Append the underscore
    parsing_index = orig_index; // resetting the parsing_index global
    if (!checkValidIndex()) {
      // Consume "Underscore"
      return -1;
    }
    return NO_ERR;
  }

  parsing_index = orig_index;
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
int parse_type_fp(Program *ast) {
  std::cout << "In parse_type_fp" << std::endl;
  int ret_val;
  std::string curr_token;

  curr_token = tokens[parsing_index];
  // ')' type_ar?
  if (curr_token == "CloseParen") {
    if (!checkValidIndex()) {
      // Consume "CloseParen"
      return -1;
    }

    // type_ar?
    int prev_index = parsing_index;
    ret_val = parse_type_ar(ast);
    if (ret_val != NO_ERR) {
      // Reset parsing index in the case that type_ar fails
      // Not an error, ? allows for 1 or 0 instances
      parsing_index = prev_index;
      // TODO: DISCARD EVERYTHING THAT WAS ADDED FROM parse_type_ar call
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

    ret_val = parse_type(ast);
    if (ret_val != NO_ERR) {
      return ret_val;
    }

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

      // Invalid type parse means error.
      ret_val = parse_type(ast);
      if (ret_val != NO_ERR) {
        return ret_val;
      }
    }

    // ')'
    curr_token = tokens[parsing_index];
    if (curr_token != "CloseParen") {
      return parsing_index;
    }
    if (!checkValidIndex()) {
      // Comeume "CloseParen"
      return -1;
    }

    // type_ar
    curr_token = tokens[parsing_index];
    ret_val = parse_type_ar(ast);
    if (ret_val != NO_ERR) {
      return ret_val;
    }

    return NO_ERR;
  }
}

int runFunction(Program *ast) {
  // Figure out if function is okay
  //
  // /* START handling decl */
  // std::string token = tokens[parsing_index];
  // if (token.substr(0, 3) != "Id(") {
  //   return parsing_index;
  // }
  // std::string functionName = token.substr(3, token.length() - 4);
  //
  // if (!checkValidIndex()) {
  //   // Consume "Id([...])"
  //   return -1;
  // }
  //
  // token = tokens[parsing_index];
  // if (token != "Colon") {
  //   return parsing_index;
  // }
  // if (!checkValidIndex()) {
  //   // Consume "Colon"
  //   return -1;
  // }
  //
  // int ret_val = check_type();
  // if (ret_val != NO_ERR) {
  //   return ret_val;
  // }
  //
  // // HANDLE PARAMETERS HERE
  // std::vector<Decl> parameters;
  // while (tokens[index] != "CloseParen") {
  //   if (tokens[index].substr(0, 3) == "Id(") {
  //     std::string paramName =
  //         tokens[index].substr(3, tokens[index].length() - 4);
  //     index++; // Move past parameter name
  //     if (tokens[index] != "Colon")
  //       return index;
  //     index++; // Move past ':'
  //     if (tokens[index] != "Int" && tokens[index] != "Address")
  //       return index; // Assume only Int and Address types for simplicity
  //     // struct Type paramType = (tokens[index] == "Int") ?
  //     // Type(Type::TypeKind::Int) : Type(Type::TypeKind::Ptr);
  //     // parameters.push_back(Decl(paramName, &paramType));
  //     index++; // Move past type
  //     if (tokens[index] == "Comma")
  //       index++; // Move past ',' if more parameters follow
  //   } else {
  //     return index; // Unexpected token
  //   }
  // }
  // index++; // Move past ')'
  //
  // if (tokens[index] != "CloseParen")
  //   return index;
  // index++; // Move past ')'
  //
  // // Check for arrow indicating return type
  // if (tokens[index] != "Arrow")
  //   return index;
  // index++; // Move past '->'
  // if (tokens[index] != "Int" && tokens[index] != "Struct" &&
  //     tokens[index] != "Fn" && tokens[index] != "Ptr")
  //   return index;
  // index++;
  // if (tokens[index] != "OpenBrace")
  //   return index;
  // index++;
  // int openBraceCount = 1;
  // while (tokens[index] != "CloseBrace") {
  //   // Fetch next token and figure out what to do with it
  //
  //   index++;
  // }
  // // Eventually, add the function to the AST
  // Function newFunction;
  // newFunction.name = functionName;
  // ast->functions.push_back(newFunction);

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

/*
checkValidType(){
  // check '&'*
  checkValidType_ad();
}

checkValidType_ad(){
  if(tokenInt == TOKEN_OK) return; // checking 'int'
  else if(token == '('){ // checking ()
    checkType_op(nextToken);
  }
  else{ // checking id
    if(itsId()){
      return good;
    }

  }
  return currenToken?
}

checkType_op(){
  if(token == CloseParen){
    checkType_ar(nextToken);
  }
  else{
    checkType(token)
    checkType_fp(nextToken)

  }
}

checkType_fp() {
  if(token == closeParen){
    check(type_ar)?;
  }
  else{
    // see if there is ',' followed by type
    // else error

    // then see if there is ',' followed by type star

    // check for ')' else error

    checkType_ar(nextToken);
  }
}

checkType_ar(){
  if(token == arrow){
    checkRettyp(nextToken);
  }
  return currenToken; // error
}

checkRetTyp(){
  if(checkType() == TOKEN_OK){
    return TOKEN_OK
  }
  else{
    if(token == "Underscore"){
      return TOKEN_OK
    }
  }
}


*/

bool checkValidIndex() {
  parsing_index++;
  if (parsing_index >= tokens.size()) {
    return false;
  }
  return true;
}
