#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

//�ʷ�����������ĵ��ʵ����ݽṹ����
class Token {
public:
    //���嵥�ʵ�����
    static enum Type {
        intNum = 1, //�޷���������
        floatNum, //������
        signAssign, //��ֵ��'='
        signAdd, //�Ӻ�'+'
        signSub, //����'-'
        signTimes, //�˺�'*'
        signDivision, //����'/'
        signMod, //ȡ���'%'
        signEqual, //���ں�'=='
        signUnequal, //�����ں�'!='
        signLess, // С�ں�'<'
        signMore, //���ں�'>'
        signLessOrEqual, //С�ڵ��ں�'<='
        signMoreOrEqual, //���ڵ��ں�'>='
        signAnd, //��'&&'
        signOr, //��'||'
        signNot, //��'!'
        keyword, //�ؼ���
        identifier, //��ʶ��
        space, //�ո�
        newLine, //����
        endSymbol, //��������ֹ��־

                   //��¼ö��������
        num_E,
    };

    //Token��Ĺ��캯��
    Token(Type type, std::string str) {
        type_ = type;
        str_ = str;
    }

    int getType() {
        return type_;
    }

    std::string getStr() {
        return str_;
    }
private:
    Type type_;
    std::string str_;
};


//���ڴ����޷���������
//����ʽ��[1-9][0-9]*|0
class IntNum : public Token {
public:
    IntNum(std::string str);

    int getValue() {
        return value_;
    }
private:
    int value_;
};


//���ڴ�������
class FloatNum : public Token {
public:
    FloatNum(std::string str);

    float getValue() {
        return value_;
    }
private:
    float value_;
};


//���ڴ��������������������������� + - * / %
//                  ��ϵ����������� == <= >= != > <
//                  �߼������������ && || !
//�Լ�              ��ֵ����� =
class Sign : public Token {
public:
    Sign(Type type, std::string str);

    std::string getValue() {
        return value_;
    }
private:
    std::string value_;
};


//���ڴ����ʶ��
//class identifier :public Token {
//public:
//    identifier(std::string value):Token(identifier)
//
//};

//���ڴ���ؼ���
//class Keyword : public Token{
//public:
//	Keyword(std::string value) : Token(keyword){
//		value_ = value;
//	}
//
//	std::string getValue(){
//		return value_;
//	}
//private:
//	std::string value_;
//};


//////////////////////////////////////////////
//�ʷ�������
class Lexer {
public:
    //����״̬��״̬
    //�޷�������������������״̬int_zero��int_num
    static enum State {
        start = 0,
        int_zero,
        int_num,
        sign_0,
        sign_1,
        sign_2,
        sign_3, //�ǽ���״̬
        sign_4,
        sign_5, //�ǽ���״̬
        sign_6,
        float_0, //�ǽ���״̬
        float_1,
        back, //����״�壬״̬��ѭ�������ı�־
    };

    Lexer();


    //�ɴʷ�����������Ҫ��ʱ����ã�ÿ�η���һ������
    //�ڲ�ʵ����һ���ֹ�����������Զ���
    //ѭ������readChar()��Ĭ��ת�������ַ�
    //����ָ��һ����һ�����ϵĿո��ַ�' '��һ����һ�����ϵĻ����ַ�'\n'
    //����������ڵ��ô˺���ʱֻ����һ���ո�' '����'\n'
    //һ����һ�����ϵ��Ʊ��'\t'ֻ����һ���ո�' '
    //����'\n'��Ŀո�' '��ȫ������
    Token *getToken();

private:
    void lexerConfig(); //�ɹ��캯��Lexer()���ã�������������ʾ�û��������ڳ�ʼ��Lexer����
                        //Ϊ��Ա����file_name_��ֵ

    void readChar(); //��getToken()���ã�ÿ�η���һ���ַ�	 

    void setState(); //�Զ�����״̬ת������,����state_��old_state_
    void setState(State state);

    std::string file_name_; //��ŵ�ǰ��ȡ���ļ���
    std::fstream file_in_;
    char c_; //�洢��ǰ���ļ��ж�����ַ�

    State state_; //��¼״̬����ǰ״̬
    State old_state_;
    std::string str_;
};

#endif // !LEXER_H_