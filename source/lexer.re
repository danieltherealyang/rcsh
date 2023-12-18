/*!header:re2c:on*/
#pragma once
#include <string>
#include <queue>
enum Result {
    SUCCESS,
    FAILURE
};

enum Token_t {
    BOOL_T,
    TRUE_V,
    FALSE_V,
    INT_T,
    INT_V,
    STRING_T,
    STRING_V,
    CMD_T,
    CMD_V,
    
    ASSIGN_OP,
    ADD_OP,
    SUB_OP,
    MULT_OP,
    MOD_OP,
    NEG_OP,

    AND_OP,
    OR_OP,
    XOR_OP,

    EQ_CMP,
    NEQ_CMP,
    LT_CMP,
    GT_CMP,

    OPEN_BRACKET,
    CLOSE_BRACKET,
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_PAREN,
    CLOSE_PAREN,

    IF_KW,
    ELSE_KW,
    WHILE_KW,
    FOR_KW,
    BREAK_KW,
    CONTINUE_KW,

    END_STATEMENT,
    VARNAME,
    IGNORE
};

typedef struct Token {
    Token_t type;
    std::string value;
    Token(){};
    Token(Token_t t): type(t) {};
    Token(Token_t t, std::string v): type(t), value(v) {};
};

class Lexer {
    private:
        const char* m_cursor;
        const char* m_marker;
        const char* m_limit;
        Token m_token;
        std::queue<Token>* m_q;
        Result next_token();
    public:
        void tokenize(std::string s);
        void print_tokens();
        Lexer(std::queue<Token>* q): m_q(q){};
};
/*!header:re2c:off*/

#include "lexer.h"
#include <iostream>
#include <string>
using namespace std;

void Lexer::tokenize(string s) {
    const char* c_str = s.c_str();
    m_cursor = c_str;
    m_marker = m_cursor;
    m_limit = c_str + s.length();
    while (next_token() != FAILURE) {
        if (m_token.type == IGNORE)
            continue;
        m_q->push(m_token);
    }
}

void Lexer::print_tokens() {
    while (!m_q->empty()) {
        cout << m_q->front().type << m_q->front().value << endl;
        m_q->pop();
    }
}

Result Lexer::next_token() {
    if (m_cursor >= m_limit)
        return FAILURE;
    
    const char *s, *c, *v;
    /*!stags:re2c format = 'const char* @@;'; */
    /*!re2c
        re2c:define:YYCTYPE = char;
        re2c:define:YYCURSOR = m_cursor;
        re2c:define:YYMARKER = m_marker;
        re2c:define:YYLIMIT = m_limit;
        re2c:yyfill:enable = 0;
        re2c:tags = 1;

        end = "\x00";
        eol = "\n";
        eos = ";";
        sep = [ ]|eol;
        alpha = [a-zA-Z];
        digit = [0-9];
        delim = "\\" .;
        char = [^''""\\] | delim;

        bool_t = "bool";
        true_v = "true";
        false_v = "false";
        int_t = "int";
        int_v = digit+;
        string_t = "string";
        string_v = (["]char*["]) | ([']char*[']);
        cmd_t = "cmd";
        cmd_v = "$(" [^)]* ")";
        
        assign_op = "=";
        add_op = "+";
        sub_op = "-";
        mult_op = "*";
        mod_op = "%";
        neg_op = "-";
        
        and_op = "&&";
        or_op = "||";
        xor_op = "^";
        
        eq_cmp = "==";
        neq_cmp = "!=";
        lt_cmp = "<";
        gt_cmp = ">";
        
        open_bracket = "[";
        close_bracket = "]";
        open_brace = "{";
        close_brace = "}";
        open_paren = "(";
        close_paren = ")";
        
        varname = (alpha|[_]) (alpha|digit|[_])*;
        
        if_kw = "if";
        else_kw = "else";
        while_kw = "while";
        for_kw = "for";
        break_kw = "break";
        continue_kw = "continue";
        end_statement = ";";

        * {cout << "failed to match" << endl; return FAILURE;}
        sep | eol | end {cout << "ignored" << endl;m_token = Token(IGNORE); return SUCCESS;}
        bool_t {m_token = Token(BOOL_T); return SUCCESS;}
        true_v {m_token = Token(TRUE_V); return SUCCESS;}
        false_v {m_token = Token(FALSE_V); return SUCCESS;}
        int_t {m_token = Token(INT_T); return SUCCESS;}
        int_v {m_token = Token(INT_V); return SUCCESS;}
        string_t {m_token = Token(STRING_T); return SUCCESS;}
        @s string_v {m_token = Token(STRING_V, string(s+1, (int) (m_cursor - s) - 2)); return SUCCESS;}
        cmd_t {m_token = Token(CMD_T); return SUCCESS;}
        @c cmd_v {m_token = Token(CMD_V, string(c+2, (int) (m_cursor - c)-3)); return SUCCESS;}
        assign_op {m_token = Token(ASSIGN_OP); return SUCCESS;}
        add_op {m_token = Token(ADD_OP); return SUCCESS;}
        sub_op {m_token = Token(SUB_OP); return SUCCESS;}
        mult_op {m_token = Token(MULT_OP); return SUCCESS;}
        mod_op {m_token = Token(MOD_OP); return SUCCESS;}
        neg_op {m_token = Token(NEG_OP); return SUCCESS;}
        and_op {m_token = Token(AND_OP); return SUCCESS;}
        or_op {m_token = Token(OR_OP); return SUCCESS;}
        xor_op {m_token = Token(XOR_OP); return SUCCESS;}
        eq_cmp {m_token = Token(EQ_CMP); return SUCCESS;}
        neq_cmp {m_token = Token(NEQ_CMP); return SUCCESS;}
        lt_cmp {m_token = Token(LT_CMP); return SUCCESS;}
        gt_cmp {m_token = Token(GT_CMP); return SUCCESS;}
        open_bracket {m_token = Token(OPEN_BRACKET); return SUCCESS;}
        close_bracket {m_token = Token(CLOSE_BRACKET); return SUCCESS;}
        open_brace {m_token = Token(OPEN_BRACE); return SUCCESS;}
        close_brace {m_token = Token(CLOSE_BRACE); return SUCCESS;}
        open_paren {m_token = Token(OPEN_PAREN); return SUCCESS;}
        close_paren {m_token = Token(CLOSE_PAREN); return SUCCESS;}
        if_kw {m_token = Token(IF_KW); return SUCCESS;}
        else_kw {m_token = Token(ELSE_KW); return SUCCESS;}
        while_kw {m_token = Token(WHILE_KW); return SUCCESS;}
        for_kw {m_token = Token(FOR_KW); return SUCCESS;}
        break_kw {m_token = Token(BREAK_KW); return SUCCESS;}
        continue_kw {m_token = Token(CONTINUE_KW); return SUCCESS;}
        end_statement {m_token = Token(END_STATEMENT); return SUCCESS;}
        @v varname {m_token = Token(VARNAME, string(v, (int) (m_cursor - v))); return SUCCESS;}
    */
}