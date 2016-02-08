#include "ParsingTable.h"

OperationMark::OperationMark() {
    operation_ = nil;
    number_ = -1;
}

OperationMark::OperationMark(int operation, int number) {
    operation_ = operation;
    number_ = number;
}

///////////////////////////////////////////////////////////
ParsingTable::ParsingTable(Item* firstItem, int item_num) {
    item_num_ = item_num;
    //��ʼ��action��
    action_ = new OperationMark*[Token::num_E];
    for (int i = 0; i < Token::num_E; i++) {
        action_[i] = new OperationMark[item_num];
        for (int j = 0; j < item_num; j++)
            action_[i][j] = OperationMark();
    }
    //��ʼ��goto��
    gotoo_ = new OperationMark*[(-Rule::num_E) - 1];
    for (int i = 0; i < (-Rule::num_E) - 1; i++) {
        gotoo_[i] = new OperationMark[item_num];
        for (int j = 0; j < item_num; j++)
            gotoo_[i][j] = OperationMark();
    }

    //ͨ�������Զ���������action��goto��
    travel(firstItem);

    //std::cout << "---------------action����---------------" << std::endl;//+_+
    //for (int i = 0; i < item_num; i++){ //+_+
    //	for (int j = 0; j < Token::num_E; j++) //+_+
    //		std::cout << " " << action_[j][i].getOperation() << action_[j][i].getNumber(); //+_+
    //	std::cout << " " << std::endl; //+_+
    //}//+_+
    //std::cout << "---------------goto����---------------" << std::endl;//+_+
    //for (int i = 0; i < item_num; i++){//+_+
    //	for (int j = 0; j < (-Rule::num_E) - 1; j++)//+_+
    //		std::cout << " " << gotoo_[j][i].getOperation() << gotoo_[j][i].getNumber();//+_+
    //	std::cout << " " << std::endl;//+_+
    //}//+_+
}

ParsingTable::~ParsingTable() {
    for (int i = 0; i < Token::num_E; i++)
        delete[] action_[i];
    delete[] action_;

    for (int i = 0; i < (-Rule::num_E) - 1; i++)
        delete[] gotoo_[i];
    delete[] gotoo_;
}

int ParsingTable::lookUpOperInAction(int token, int state) {
    if (state == 1) {
        return OperationMark::acc;
    }
    return action_[token][state].getOperation();
}

int ParsingTable::lookUpNumInAction(int token, int state) {
    return action_[token][state].getNumber();
}

int ParsingTable::lookUpNumInGoto(int token, int state) {
    return gotoo_[(-token) - 1][state].getNumber();
}

void ParsingTable::travel(Item* item) {
    bool* travel_log = new bool[item_num_]; //��¼�Ѿ�������������Ŀ��
    for (int i = 0; i < item_num_; i++)
        travel_log[i] = false;

    std::stack<Item *> s;
    s.push(item);
    while (!s.empty()) {
        Item* item_buf = s.top();
        s.pop();
        travel_log[item_buf->getNum() - 1] = true;
        //�鿴�Ƿ���ڹ�Լ,���������ACTION��GOTO������ӹ�Լ����
        for (unsigned int i = 0; i < item_buf->getRule()->size(); i++) {
            int rule_cache = item_buf->getRule()->at(i);
            int index_cache = item_buf->getIndex()->at(i);
            std::vector<int>* forward_cache = item_buf->getForwards()->at(i);
            if (RuleGroup::ruleGroup_[rule_cache].getRuleLength() == index_cache) {
                for (unsigned int j = 0; j < forward_cache->size(); j++) {
                    if (forward_cache->at(j)>0) {
                        action_[forward_cache->at(j)][item_buf->getNum() - 1].setOperation(OperationMark::reduce);
                        action_[forward_cache->at(j)][item_buf->getNum() - 1].setNumber(rule_cache);
                    }
                    else {
                        if (rule_cache == 0) {
                            action_[0][item_buf->getNum() - 1].setOperation(OperationMark::acc);
                            action_[0][item_buf->getNum() - 1].setNumber(rule_cache);

                            action_[Token::endSymbol][item_buf->getNum() - 1].setOperation(OperationMark::acc);
                            action_[Token::endSymbol][item_buf->getNum() - 1].setNumber(rule_cache);
                        }
                        else {
                            action_[0][item_buf->getNum() - 1].setOperation(OperationMark::reduce);
                            action_[0][item_buf->getNum() - 1].setNumber(rule_cache);

                            action_[Token::endSymbol][item_buf->getNum() - 1].setOperation(OperationMark::reduce);
                            action_[Token::endSymbol][item_buf->getNum() - 1].setNumber(rule_cache);
                        }
                    }
                }
            }
        }

        for (unsigned int i = 0; i < item_buf->getNext()->getMarkList()->size(); i++) {
            int next_mark = item_buf->getNext()->getMarkList()->at(i);
            //����item_buf���ṩ�����ݣ���ACTION��GOTO�����������
            if (next_mark>0) {
                action_[next_mark][item_buf->getNum() - 1].setOperation(OperationMark::shift);
                action_[next_mark][item_buf->getNum() - 1].setNumber(
                    item_buf->getNext()->getNextItem(next_mark)->getNum() - 1);
            }
            else {
                gotoo_[(-next_mark) - 1][item_buf->getNum() - 1].setOperation(OperationMark::gotoo);
                gotoo_[(-next_mark) - 1][item_buf->getNum() - 1].setNumber(
                    item_buf->getNext()->getNextItem(next_mark)->getNum() - 1);
            }
            //����Ŀ��û�б�������ʱ�����б���
            if (!travel_log[item_buf->getNext()->getNextItem(next_mark)->getNum() - 1])
                s.push(item_buf->getNext()->getNextItem(next_mark));
        }
    }

    delete[] travel_log;
}