#include "ast.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

std::vector<std::string> tokens;

// Forward declarations of functions
void read_file(string input_string);
void printAST(struct Program *ast);
void printGlobals(vector<Decl> prog_globs);
void printDecl(Decl declaration);
void printType(Type type);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "usage: " << argv[0] << " <source_file>\n";
    return 1;
  }

  std::ifstream source(argv[1]);
  if (!source) {
    std::cerr << "failed to open the file.\n";
    return 1;
  }

  std::string input_string((std::istreambuf_iterator<char>(source)),
                           std::istreambuf_iterator<char>());
  // make a vector of strings with all the tokens. (input as out param)
  read_file(input_string);

  // return parse error code
  int error = NO_ERR;
  struct Program *ast = initialize_ast();
  error = create_ast(ast);

  // Catches any parse errors
  if (error != NO_ERR) {
    std::cout << "parse error at token " << to_string(error);
    return 0;
  }

  printAST(ast);

  std::cout << "No parse error" << endl;

  return 0;
}

/*
 * function: read_file
 *
 * read file contents (which must be tokens). store contents into
 * out parameter.
 *
 * @param input out parameter, store all tokens into this vector
 * @param input_string file input as a string that holds the tokens
 */
void read_file(string input_string) {
  string currentString = "";
  for (auto curr_char : input_string) {
    if (curr_char == '\n') {
      tokens.push_back(currentString);
      currentString = "";
    } else {
      currentString += curr_char;
    }
  }
}

/*
 * function: printAST
 *
 * prints AST in the format for output
 *
 */
void printAST(struct Program *ast) {
  std::cout << "Program(" << std::endl;

  // Printing Globals
  std::cout << "globals = [";
  printGlobals(ast->globals);
  std::cout << "]," << std::endl;

  std::cout << ")" << std::endl; /* closes Program( */
}

/*
 * function: printGlobals
 *
 * prints out the global vector for grading
 *
 */
void printGlobals(vector<Decl> prog_globs) {
  if (prog_globs.size() > 0) {
    printDecl(prog_globs[0]);
  }
  for (int i = 1; i < prog_globs.size(); i++) {
    std::cout << std::endl;
    printDecl(prog_globs[i]);
  }
}

/*
 * function: printDecl
 *
 * prints a single decl object
 *
 */
void printDecl(Decl declaration) {
  std::cout << "Decl(";
  std::cout << declaration.name;
  std::cout << ",";

  printType(*declaration.type);

  std::cout << ")"; // Closes Decl(
}

/*
 * function: printDecl
 *
 * prints a single decl object
 *
 */
void printType(Type type) {
  if (type.kind == Type::TypeKind::Int) {
    std::cout << "Int";
  } else if (type.kind == Type::TypeKind::Ptr) {
    std::cout << "Ptr(";
    printType(*type.pointed_to_type);
    std::cout << ")"; // closes Ptr(
  }
}
