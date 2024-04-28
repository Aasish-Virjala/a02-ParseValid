#include "ast.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void read_file(vector<string> &tokensOut, string input_string);
void create_tokens(vector<Token> &tokenizedTokens, vector<string> tokens);

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
  vector<string> tokens;
  // make a vector of strings with all the tokens. (input as out param)
  read_file(tokens, input_string);

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
void read_file(vector<string> &tokensOut, string input_string) {
  string currentString = "";
  for (auto curr_char : input_string) {
    if (curr_char == '\n') {
      tokensOut.push_back(currentString);
      currentString = "";
    } else {
      currentString += curr_char;
    }
  }
}
