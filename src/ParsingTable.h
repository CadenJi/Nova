#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H
#include <stack>
#include "Automata.h"

class OperationMark {
public:
    static enum Oper {
        shift,
        reduce,
        gotoo, //goto
        acc,
        nil, //��ʾ������

             //��¼ö��������
        num_E,
    };
    OperationMark();
    OperationMark(int operation, int number);

    void setOperation(int oper) {
        operation_ = oper;
    }

    int getOperation() {
        return operation_;
    }

    void setNumber(int num) {
        number_ = num;
    }

    int getNumber() {
        return number_;
    }
private:
    int operation_;
    int number_;
};

//LR�ķ�������
class ParsingTable {
public:
    ParsingTable(Item* firstItem, int item_num);
    ~ParsingTable();

    int lookUpOperInAction(int token, int state);
    int lookUpNumInAction(int token, int state);
    int lookUpNumInGoto(int token, int state);
private:
    int item_num_;
    //��һά��ʾ������ڶ�ά��ʾ״̬
    OperationMark** action_;//ACTION�� Token::num_E�У�item_num��,
                            //action_[0][..]��ʾ�޽�����ʵ������action[Token::endSymble][..]�ȼۣ�
    OperationMark** gotoo_;//GOTO�� ��-Rule::num_E�� - 1�У�item_num��

    void travel(Item* item);//���Զ������б���
};


#endif // !PARSING_TABLE_H
