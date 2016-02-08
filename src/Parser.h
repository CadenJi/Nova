#ifndef PARSER_H_
#define PARSER_H_
#include <stack>
#include "Lexer.h"
#include "ParsingTable.h"

//�﷨������
class Parser {
public:
    Parser(Lexer* lexer);
    ~Parser();

private:
    ParsingTable* tab_; //LR������
    Lexer* lexer_;

    Token* token_buf_;
    int state_buf_;

    bool parse_Start(); //����false��ʾ�ı�������� ����true��ʾ����һ�����

};

#endif // !PARSER_H_
