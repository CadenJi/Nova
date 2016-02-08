#include "Parser.h"

Parser::Parser(Lexer* lexer) {
    lexer_ = lexer;

    Automata aut = Automata();
    aut.start();
    tab_ = new ParsingTable(aut.getFirstItem(), aut.getItemNum());

    while (parse_Start());
}

Parser::~Parser() {
    delete tab_;
}

bool Parser::parse_Start() {
    bool usedToken = false;
    bool isEnd = false;
    std::stack<int> s;
    s.push(0); //ѹ����ʼ״̬
    token_buf_ = lexer_->getToken();
    if (token_buf_->getType() == Token::endSymbol)
        isEnd = true;
    while (!isEnd) {
        state_buf_ = s.top();
        if (tab_->lookUpOperInAction(token_buf_->getType(), state_buf_) == OperationMark::acc) {
            std::cout << "����һ�����" << std::endl;
            return true;
        }
        else if (tab_->lookUpOperInAction(token_buf_->getType(), state_buf_) == OperationMark::shift) {
            s.push(token_buf_->getType());
            s.push(tab_->lookUpNumInAction(token_buf_->getType(), state_buf_));
            usedToken = true;
        }
        else if (tab_->lookUpOperInAction(token_buf_->getType(), state_buf_) == OperationMark::reduce) {
            int rule_cache = tab_->lookUpNumInAction(token_buf_->getType(), state_buf_);
            int times = (RuleGroup::ruleGroup_[rule_cache].getRuleLength()) * 2;
            for (int i = 0; i < times; i++)
                s.pop();
            state_buf_ = s.top();
            s.push(RuleGroup::ruleGroup_[rule_cache].getLeft());
            s.push(tab_->lookUpNumInGoto(RuleGroup::ruleGroup_[rule_cache].getLeft(), state_buf_));
        }
        else {
            std::cout << "Error::�﷨����" << std::endl;
            system("pause");
            exit(0); //�����﷨����
        }
        if (usedToken && token_buf_->getType() != Token::endSymbol) {
            token_buf_ = lexer_->getToken();
            usedToken = false;
        }
    }
    std::cout << "����һ�������" << std::endl;
    return false;
}