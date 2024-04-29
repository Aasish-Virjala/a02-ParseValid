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

  int i = 0;
  int err = NO_ERR;

  while (i < tokens.size()) {
    std::string token = tokens[i];

    // handle a global
    if (token == "Let") {
      err = runGlobal(ast, i + 1);
      if (err != NO_ERR) {
        return err;
      }
    }

    // handle a function
    else if (token == "Fn") {
      err = runFunction(ast, i + 1);
      if (err != NO_ERR) {
        return err;
      }

    }

    // handle a struct
    else if (token == "Struct") {
      err = runStruct(ast, i + 1);
      if (err != NO_ERR) {
        return err;
      }
    }

    // handle an extern
    else if (token == "Extern") {
      err = runExtern(ast, i + 1);
      if (err != NO_ERR) {
        return err;
      }
    }

    else {
      // Not a Fn, Struct, Extern, or Global, return parse error at this token
      return i;
    }

    i++;
  }

  return 1;
}

int runGlobal(Program &ast, int index) {
  // Figure out if global is okay

  return NO_ERR;
}

int runFunction(Program &ast, int index) {
  // Figure out if function is okay

  return NO_ERR;
}

int runStruct(Program &ast, int index) {
  // Figure out if struct is okay

  return NO_ERR;
}

int runExtern(Program &ast, int index) {
  // Figure out if extern is okay

  return NO_ERR;
}
