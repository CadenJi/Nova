#ifndef AUTOMATA_H
#define AUTOMATA_H
#include <vector>
#include "Lexer.h"
#include "Rule.h"


class Item;

//��Item��ʹ��,����ָ����һ����Ŀ��
class Next {
public:
    Next();

    void setNext(int mark, Item* nextItem);

    //�޸Ķ�Ӧmark,��Item*ָ��ֵ
    void resetNext(int mark, Item* nextItem);

    bool haveMark(int mark);

    Item* getNextItem(int mark);

    std::vector<int>* getMarkList() {
        return &marklist_;
    }

private:
    bool type_mark_[Token::num_E];
    Item* type_nextItem_[Token::num_E];

    bool symble_mark_[-Rule::num_E];
    Item* symble_nextItem_[-Rule::num_E];

    std::vector<int> marklist_;
};

//��Ŀ�����ݽṹ
class Item {
public:
    //���ϲ�����item��mark��¼����
    std::vector<int> resetNotes_;

    Item(int num);

    ~Item();

    Next* getNext();

    std::vector<int>* getRule() {
        return &rules_;
    }

    std::vector<int>* getIndex() {
        return &ruleIndex_;
    }

    std::vector<std::vector<int>*>* getForwards() {
        return &forwards_;
    }

    int getNum() {
        return num_;
    }

    //����Ը�mark��nextItem�Ƿ�ϲ��˵�
    bool isResetMark(int mark);

private:
    int num_;//��Ŀ�����(�� 1 ��ʼ)

    Next next_;//ָ����һ����Ŀ��

    std::vector<int> rules_;
    std::vector<int> ruleIndex_;

    //����ʽ��Լ��������������������-1��ʾ
    std::vector<std::vector<int>*> forwards_;
};

//////////////////////////////////////////////////
//��������RuleGroup�е��﷨������һ���Զ�������Ŀ���淶�飬�����Զ�������LR������
class Automata {
public:
    Automata() {};

    void start();

    Item* getFirstItem() {
        return firstItem_;
    }

    int getItemNum() {
        return numCounter_;
    }

private:
    int numCounter_ = 1; //��Ŀ����ż�����

                         //��mark��Ϊ��������¼������������ɵ�ȫ��item
    std::vector<Item*> type_itemlist_[Token::num_E];
    std::vector<Item*> symble_itemlist_[-Rule::num_E];

    //!@#$%^&û�ж�Ӧ��delete�������ڴ�й¶
    Item* firstItem_ = new Item(numCounter_);//�Զ�������㣬��ʼ��Ŀ��


                                             ////////////////////////////////////////////////////////////////////
                                             //�Ƚ�������Ŀ���Ƿ�ȼ�
    bool equalItem(Item* item_1, Item* item_2);

    //������ض�mark��Ϊ������item�Ƿ�����������һ��
    bool haveMark(int mark);

    //��������Item������Item
    void growthNewItem(Item* item);

    //����item��next�����marklist�����б���
    void traversal(Item* item);

    //����Ŀ��������µ���Ŀ
    //���裺
    //1.����clo���У�����Ŀ���м�������Ŀ
    //2.���ȫ������Ӻ� �����Ŀ�Ƿ���Ҫ���бհ����㡣���û��
    //  ����������������е�3��
    //3.���бհ����㣬�ݹ���ô˷�����ֱ��������Ŀ��������
    void pushRules(Item *item, std::vector<int> &clo, std::vector<int>* forward);
    void pushRules(Item *item, int grule, int gindex, std::vector<int>* forward);

    //�����Ŀ���е�ĳһ���Ƿ���Ҫ���бհ�����
    //�����Ҫ�����ط�����Ҫ��������ıհ���ֵ
    //�������Ҫ������ 0
    int checkSymble(int rule, int index);

    //�հ�����!@#$%^&Ч�ʵ����д��Ľ�
    std::vector<int> closure(int s);

    //���׷��ż�
    std::vector<int>* first(int rule, int at, std::vector<int>* forward);
};

#endif // !AUTOMATA_H
