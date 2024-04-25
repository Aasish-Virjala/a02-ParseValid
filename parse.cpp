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
  vector<Token> tokenizedTokens;
  // make a vector of strings with all the tokens. (input as out param)
  read_file(tokens, input_string);

  // Take the tokens and turn them into Token structs.
  create_tokens(tokenizedTokens, tokens);
  for (auto val : tokenizedTokens) {
    if (val.type == Token::Type::NUM || val.type == Token::Type::ID) {
      std::cout << val.value << std::endl;
    } else {
      std::cout << "Token" << std::endl;
    }
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

void create_tokens(vector<Token> &tokenizedTokens, vector<string> tokens) {
  for (auto value : tokens) {
    if (value.substr(0, 3) == "Num") {
      int closeIndex = value.find(")");
      tokenizedTokens.push_back(
          Token(Token::Type::NUM, value.substr(4, closeIndex - 4)));
    } else if (value.substr(0, 2) == "Id") {
      int closeIndex = value.find(")");
      tokenizedTokens.push_back(
          Token(Token::Type::ID, value.substr(3, closeIndex - 3)));
    } else if (value == "Int") {
      tokenizedTokens.push_back(Token::Type::INT);
    } else if (value == "Struct") {
      tokenizedTokens.push_back(Token::Type::STRUCT);
    } else if (value == "Nil") {
      tokenizedTokens.push_back(Token::Type::NIL);
    } else if (value == "Break") {
      tokenizedTokens.push_back(Token::Type::BREAK);
    } else if (value == "Continue") {
      tokenizedTokens.push_back(Token::Type::CONTINUE);
    } else if (value == "Return") {
      tokenizedTokens.push_back(Token::Type::RETURN);
    } else if (value == "If") {
      tokenizedTokens.push_back(Token::Type::IF);
    } else if (value == "Else") {
      tokenizedTokens.push_back(Token::Type::ELSE);
    } else if (value == "While") {
      tokenizedTokens.push_back(Token::Type::WHILE);
    } else if (value == "New") {
      tokenizedTokens.push_back(Token::Type::NEW);
    } else if (value == "Let") {
      tokenizedTokens.push_back(Token::Type::LET);
    } else if (value == "Extern") {
      tokenizedTokens.push_back(Token::Type::EXTERN);
    } else if (value == "Fn") {
      tokenizedTokens.push_back(Token::Type::FN);
    } else if (value == "Address") {
      tokenizedTokens.push_back(Token::Type::ADDRESS);
    } else if (value == "Colon") {
      tokenizedTokens.push_back(Token::Type::COLON);
    } else if (value == "Semicolon") {
      tokenizedTokens.push_back(Token::Type::SEMICOLON);
    } else if (value == "Comma") {
      tokenizedTokens.push_back(Token::Type::COMMA);
    } else if (value == "Underscore") {
      tokenizedTokens.push_back(Token::Type::UNDERSCORE);
    } else if (value == "Arrow") {
      tokenizedTokens.push_back(Token::Type::ARROW);
    } else if (value == "Plus") {
      tokenizedTokens.push_back(Token::Type::PLUS);
    } else if (value == "Dash") {
      tokenizedTokens.push_back(Token::Type::DASH);
    } else if (value == "Star") {
      tokenizedTokens.push_back(Token::Type::STAR);
    } else if (value == "Slash") {
      tokenizedTokens.push_back(Token::Type::SLASH);
    } else if (value == "Equal") {
      tokenizedTokens.push_back(Token::Type::EQUAL);
    } else if (value == "NotEq") {
      tokenizedTokens.push_back(Token::Type::NOTEQ);
    } else if (value == "Lt") {
      tokenizedTokens.push_back(Token::Type::LT);
    } else if (value == "Lte") {
      tokenizedTokens.push_back(Token::Type::LTE);
    } else if (value == "Gt") {
      tokenizedTokens.push_back(Token::Type::GT);
    } else if (value == "Gte") {
      tokenizedTokens.push_back(Token::Type::GTE);
    } else if (value == "Dot") {
      tokenizedTokens.push_back(Token::Type::DOT);
    } else if (value == "Gets") {
      tokenizedTokens.push_back(Token::Type::GETS);
    } else if (value == "OpenParen") {
      tokenizedTokens.push_back(Token::Type::OPENPAREN);
    } else if (value == "CloseParen") {
      tokenizedTokens.push_back(Token::Type::CLOSEPAREN);
    } else if (value == "OpenBracket") {
      tokenizedTokens.push_back(Token::Type::OPENBRACKET);
    } else if (value == "CloseBracket") {
      tokenizedTokens.push_back(Token::Type::CLOSEBRACKET);
    } else if (value == "OpenBrace") {
      tokenizedTokens.push_back(Token::Type::OPENBRACE);
    } else if (value == "CloseBrace") {
      tokenizedTokens.push_back(Token::Type::CLOSEBRACE);
    }

    std::cout << value << std::endl;
  }
}
