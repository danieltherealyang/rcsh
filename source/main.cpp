#include <string>
#include <iostream>
#include <vector>
#include <assert.h>
#include <queue>
#include "lexer.h"

using namespace std;
int main() {
    queue<Token> token_stream;
    Lexer lexer(&token_stream);
    
    string line;
    string prompt = "$ ";
    while (true) {
        cout << prompt;
        getline(cin, line);
        if (line.empty())
            continue;
        lexer.tokenize(line);
        lexer.print_tokens();
    }
}