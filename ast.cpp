#include "ast.h"

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

  int parsing_index = 0;
  int err = NO_ERR;

  while (parsing_index < tokens.size()) {
    std::string token = tokens[parsing_index];

    // handle a global

    if (token == "Let") {
      parsing_index++;
      err = runGlobal(ast);
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

    // parsing_index++;
  }

  return 1;
}

int runGlobal(Program *ast) {
  // Figure out if global is okay

  if (parsing_index > tokens.size() - 1){
    // TODO: Check which index it should return
    return parsing_index - 1; 
  }

  int valid_declaration = checkValidDecls(&ast->globals);
  if(valid_declaration != NO_ERR){
    return valid_declaration;
  }



  // if(token == 'Semicolon'){
    
  // }

  return NO_ERR;
}

int runFunction(Program *ast, int index) {
  // Figure out if function is okay
  if (tokens[index].substr(0, 3) != "Id(") return index;
    std::string functionName = tokens[index].substr(3, tokens[index].length() - 4);
    index++; // Move past function name
    if (tokens[index] != "OpenParen") return index;
    index++;
    
    // HANDLE PARAMETERS HERE
    std::vector<Decl> parameters;
    while (tokens[index] != "CloseParen") {
        if (tokens[index].substr(0, 3) == "Id(") {
            std::string paramName = tokens[index].substr(3, tokens[index].length() - 4);
            index++; // Move past parameter name
            if (tokens[index] != "Colon") return index;
            index++; // Move past ':'
            if (tokens[index] != "Int" && tokens[index] != "Address") return index; // Assume only Int and Address types for simplicity
            // struct Type paramType = (tokens[index] == "Int") ? Type(Type::TypeKind::Int) : Type(Type::TypeKind::Ptr);
            // parameters.push_back(Decl(paramName, &paramType));
            index++; // Move past type
            if (tokens[index] == "Comma") index++; // Move past ',' if more parameters follow
        }
        else {
            return index; // Unexpected token
        }
    }
    index++; // Move past ')'


    if (tokens[index] != "CloseParen") return index;
    index++; // Move past ')'

    
    // Check for arrow indicating return type
    if (tokens[index] != "Arrow") return index;
    index++; // Move past '->'
    if(tokens[index] != "Int" && tokens[index] != "Struct" && tokens[index] != "Fn" && tokens[index] != "Ptr") return index;
    index++;
    if(tokens[index] != "OpenBrace") return index;
    index++;
    int openBraceCount = 1;
    while(tokens[index] != "CloseBrace") {
      // Fetch next token and figure out what to do with it
      
      index++;
    }
    // Eventually, add the function to the AST
    Function newFunction;
    newFunction.name = functionName;
    ast->functions.push_back(newFunction);


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


int checkValidDecls(std::vector<Decl>* globals){
  // Check if it's a decl
  // std::string token = tokens[parsing_index];
  // decls ::= decl (`,` decl)*
  // let sai:struct;

  /* decl */
  // decl ::= id `:` type
  //ID(A)
  //COLON
  //2

  // token is an identifier
  
  // let a:int, b:int,c,d : int
  // First fetch all names
  // vector<strings> "a","b","c","d"
  // Once you get to :
  // Figure out token type
  // assign that to each parameter
  if (tokens[parsing_index].length() >= 3 && tokens[parsing_index].substr(0, 3) == "Id("){
    struct Decl decl_token;

    size_t end_index = tokens[parsing_index].find(')');
    decl_token.name = tokens[parsing_index].substr(3, tokens[parsing_index].length() - 4);
    

    if(!checkValidIndex()){
      // Consumed Id() token
      return parsing_index;
    }
    if(tokens[parsing_index] != "Colon") return parsing_index;

    if(!checkValidIndex()){
      // Consumed Colon token
      return parsing_index;
    }
    
    int checkValidType();
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
    /* validate type */


    /* */

  //   if(tokens[parsing_index] != "Int" && tokens[parsing_index] != "Struct" && tokens[parsing_index] != "Fn" && tokens[parsing_index] != "Ptr") return parsing_index;
  //   struct Type new_type;
  //   if(tokens[parsing_index] == "Int") {
  //     new_type = Type();
  //   }
  //   else if (tokens[parsing_index] == "Struct"){
  //     if (checkValidIndex()) {
  //       std::string new_name = tokens[]
  //       new_type = Type(tokens[])
  //     }
  //   }
  //   else if (tokens[parsing_index] == "Fn"){
  //     new_type = 
  //   }
  //   else if (tokens[parsing_index] == "Ptr"){
  //     new_type = 
  //   }
  
  //   decl_token.type = Type(tokens[parsing_index]);
  //   globals->push_back(decl_token);
  }

  /* end_decl */
  if(!checkValidIndex()){
    // Consumed "int", "struct", "Fn",  token
    return parsing_index;
  }

  if()


  // Check if it's a (comma followed by decl)*


}

bool checkValidIndex() {
  parsing_index++;
  if(parsing_index >= tokens.size()) {
    return false;
  }
  return true;
}

/*

*/

let a:int, b: int, c: int, d: &&&&int;

