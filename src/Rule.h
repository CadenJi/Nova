#ifndef RULE_H_
#define RULE_H_

#include <vector>

//�����﷨����Ĳ���ʽ���ݽṹ
class Rule {
public:
    //�﷨�����еķ��ս������
    static enum Symble {
        symble_1 = -1,
        symble_2 = -2,
        symble_3 = -3,
        symble_4 = -4,

        //��¼ö��������
        num_E = -5,
    };

    Rule(int length, int left, int* right);
    int getRuleLength();
    int getLeft();
    int getRight(int index); //��ȡ����right_���±�Ϊnum������

private:
    int ruleLength_; //����ʽ�Ҳ��ĳ���
    int left_; //����ʽ����
    int* right_; //����ʽ���Ҳ�
};

class RuleGroup {
public:
    RuleGroup();
    static Rule ruleGroup_[9];

};

#endif // !RULE_H_
