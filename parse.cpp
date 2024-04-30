#include "ast.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

std::vector<std::string> tokens;

void read_file(string input_string);

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
  error = create_ast();

  // Catches any parse errors
  if (error != NO_ERR) {
    std::cout << "parse error at token " << to_string(error);
    return 0;
  }

  

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
