#include "Automata.h"
//#include <iostream>//+_+


Next::Next() {
    for (int i = 0; i < Token::num_E; i++) {
        type_mark_[i] = false;
    }

    for (int i = 0; i < (-Rule::num_E); i++) {
        symble_mark_[i] = false;
    }
}

void Next::setNext(int mark, Item* nextItem) {
    if (!haveMark(mark)) {
        marklist_.push_back(mark);
    }

    if (mark > 0) {
        type_mark_[mark] = true;
        type_nextItem_[mark] = nextItem;
    }
    else {
        symble_mark_[-mark] = true;
        symble_nextItem_[-mark] = nextItem;
    }
}

void Next::resetNext(int mark, Item* nextItem) {
    if (mark > 0) {
        delete type_nextItem_[mark];
        type_nextItem_[mark] = nextItem;
    }
    else {
        delete symble_nextItem_[-mark];
        symble_nextItem_[-mark] = nextItem;
    }
}

bool Next::haveMark(int mark) {
    if (mark > 0) {
        return type_mark_[mark];
    }
    else {
        return symble_mark_[-mark];
    }
}

Item* Next::getNextItem(int mark) {
    if (mark > 0) {
        return type_nextItem_[mark];
    }
    else {
        return symble_nextItem_[-mark];
    }
}


/////////////////////////////////////////////////////////////
Item::Item(int num) {
    num_ = num;
    next_ = Next();
}

Item::~Item() {
    for (unsigned int i = 0; i < forwards_.size(); i++) {
        delete forwards_[i];
    }
    forwards_.clear();
}

Next* Item::getNext() {
    return &next_;
}

bool Item::isResetMark(int mark) {
    int times = resetNotes_.size();
    for (int i = 0; i < times; i++) {
        if (mark == resetNotes_.at(i)) {
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////

void Automata::start() {

    //������ʼ��Ŀ��
    std::vector<int>* forward = new std::vector<int>();
    forward->push_back(-1);
    pushRules(firstItem_, 0, 0, forward);

    //std::cout << "��ʼ��Ŀ�����ɣ���ʼ������Ŀ���淶��\n" << std::endl; //+_+

    //��ʼ������Ŀ���淶��
    growthNewItem(firstItem_);
}

bool Automata::equalItem(Item* item_1, Item* item_2) {
    if (item_1->getRule()->size() == item_2->getRule()->size()) {
        int times = item_1->getRule()->size();
        for (int i = 0; i < times; i++) {
            if (item_1->getRule()->at(i) == item_2->getRule()->at(i)) {
                if (item_1->getIndex()->at(i) == item_2->getIndex()->at(i)) {
                    return true;
                }
            }
        }
        return false;
    }
    else
        return false;
}

bool Automata::haveMark(int mark) {
    if (mark > 0) {
        if (type_itemlist_[mark].size() > 0)
            return true;
        else
            return false;
    }
    else {
        if (symble_itemlist_[-mark].size() > 0)
            return true;
        else
            return false;
    }
}

void Automata::growthNewItem(Item* item) {
    int item_num = item->getRule()->size();
    //std::cout << "��Ŀ��" << item->getNum() << "\n�������£�" << std::endl;//+_+
    //������Item
    for (int i = 0; i < item_num; i++) {
        int rule_cache = item->getRule()->at(i);
        int index_cache = item->getIndex()->at(i);
        std::vector<int>* forward_cache = item->getForwards()->at(i);
        int mark;
        //std::cout << "Rule:" << rule_cache << " Index:" << index_cache << " forward:";//+_+
        //for (unsigned int x = 0; x<item->getForwards()->at(i)->size(); x++){ //+_+
        //	std::cout << item->getForwards()->at(i)->at(x) << " " << std::endl;//+_+
        //}//+_+
        if (RuleGroup::ruleGroup_[rule_cache].getRuleLength() > index_cache) {
            mark = RuleGroup::ruleGroup_[rule_cache].getRight(index_cache);
            if (item->getNext()->haveMark(mark)) {
                Item* nextItem = item->getNext()->getNextItem(mark);
                pushRules(nextItem, rule_cache, index_cache + 1, forward_cache);
            }
            else {
                numCounter_++;
                Item* newItem = new Item(numCounter_);//!@#$%^&û�ж�Ӧ��delete�������ڴ�й¶ ��һ��й¶��Next::resetNext()�н��
                pushRules(newItem, rule_cache, index_cache + 1, forward_cache);
                item->getNext()->setNext(mark, newItem);
            }
        }

    }
    //������Ŀ����Next�е�marklist_��,�Ͳ������Զ����е��ظ���
    for (int i = 0; i < item->getNext()->getMarkList()->size(); i++) {
        int mark = item->getNext()->getMarkList()->at(i);
        Item* nextItem = item->getNext()->getNextItem(mark);
        if (haveMark(mark)) {
            int times;
            if (mark > 0) {
                times = type_itemlist_[mark].size();
                bool is_equal = false;
                Item* item_buf = nullptr;
                for (int i = 0; i<times; i++) {
                    if (equalItem(type_itemlist_[mark].at(i), nextItem)) {
                        is_equal = true;
                        item_buf = type_itemlist_[mark].at(i);
                        break;
                    }
                }
                if (is_equal) {
                    item->getNext()->resetNext(mark, item_buf);
                    numCounter_--;
                    item->resetNotes_.push_back(mark);
                }
            }
            else {
                times = symble_itemlist_[-mark].size();
                bool is_equal = false;
                Item* item_buf = nullptr;
                for (int i = 0; i < times; i++) {
                    if (equalItem(symble_itemlist_[-mark].at(i), nextItem)) {
                        is_equal = true;
                        item_buf = symble_itemlist_[-mark].at(i);
                        break;
                    }
                }
                if (is_equal) {
                    item->getNext()->resetNext(mark, item_buf);
                    numCounter_--;
                    item->resetNotes_.push_back(mark);
                }
            }
        }
        else {
            //���ظ�����Ҫѹ��type_itemlist_��symble_itemlist_��
            if (mark > 0) {
                type_itemlist_[mark].push_back(nextItem);
            }
            else {
                symble_itemlist_[-mark].push_back(nextItem);
            }
        }
    }


    traversal(item);
}

void Automata::traversal(Item* item) {
    std::vector<int>* marklist = item->getNext()->getMarkList();
    int times = marklist->size();
    for (int i = 0; i < times; i++) {
        if (item->isResetMark(marklist->at(i))) {
            //std::cout << "����Ŀ��" << item->getNum() << "����" << marklist->at(i) << "ʱ����һ����Ŀ���ǣ�";//+_+
            //Item* nextItem = item->getNext()->getNextItem(marklist->at(i));//+_+
            //std::cout << "��Ŀ��" << nextItem->getNum() << std::endl;//+_+
        }
        else {
            //std::cout << "����Ŀ��" << item->getNum() << "����" << marklist->at(i) << "ʱ����һ����Ŀ���ǣ�";//+_+
            Item* nextItem = item->getNext()->getNextItem(marklist->at(i));
            growthNewItem(nextItem);
        }
    }
}

void Automata::pushRules(Item *item, std::vector<int> &clo, std::vector<int>* forward) {
    for (unsigned int i = 0; i<clo.size(); i++) {
        item->getRule()->push_back(clo.at(i));
        item->getIndex()->push_back(0);
        if (i == clo.size() - 1)
            item->getForwards()->push_back(forward);
        else {
            std::vector<int>* newForward = new std::vector<int>();
            for (unsigned int j = 0; j < forward->size(); j++) {
                newForward->push_back(forward->at(j));
            }
            item->getForwards()->push_back(newForward);
        }
    }
    //for (unsigned int i = 0; i < item->getRule()->size(); i++){//+_+
    //	std::cout << "���" << item->getRule()->at(i) << " " << item->getIndex()->at(i) << std::endl;//+_+
    //}//+_+
    for (unsigned int i = 0; i<clo.size(); i++) {
        int symCheck = checkSymble(clo.at(i), 0);
        if (symCheck < 0) {
            std::vector<int> newClo = closure(symCheck);
            std::vector<int>* new_forward = first(clo.at(i), 1, forward);
            pushRules(item, newClo, new_forward);
        }
    }
}
void Automata::pushRules(Item *item, int grule, int gindex, std::vector<int>* forward) {
    item->getRule()->push_back(grule);
    item->getIndex()->push_back(gindex);
    item->getForwards()->push_back(forward);

    int symCheck = checkSymble(grule, gindex);
    //std::cout << "���" << item->getRule()->at(0) << " " << item->getIndex()->at(0) << std::endl;//+_+
    if (symCheck < 0) {
        std::vector<int> newClo = closure(symCheck);
        std::vector<int>* new_forward = first(grule, (gindex + 1), forward);
        pushRules(item, newClo, new_forward);
    }
}

int Automata::checkSymble(int rule, int index) {
    int right = 0;
    if (RuleGroup::ruleGroup_[rule].getRuleLength() > index) {
        right = RuleGroup::ruleGroup_[rule].getRight(index);
    }
    else {
        return 0;
    }

    if (right < 0) {
        return right;
    }
    else {
        return 0;
    }
}

std::vector<int> Automata::closure(int s) {
    bool find = false;
    int count = 0;
    std::vector<int> clo;

    int sym;
    do {
        sym = RuleGroup::ruleGroup_[count].getLeft();
        if (sym == s) {
            find = true;
            clo.push_back(count);
        }

        count++;
    } while (!(find && (sym != s)));

    return clo;
}

std::vector<int>* Automata::first(int rule, int at, std::vector<int>* forward) {
    if (RuleGroup::ruleGroup_[rule].getRuleLength() > at) {
        std::vector<int>* newForward = new std::vector<int>();
        if (RuleGroup::ruleGroup_[rule].getRight(at) > 0) {
            newForward->push_back(RuleGroup::ruleGroup_[rule].getRight(at));
            return newForward;
        }
        else if (RuleGroup::ruleGroup_[rule].getRight(at) < 0) {
            bool find = false;
            int count = 0;
            int sym;

            //����ȥ���ظ���
            bool notes[Token::num_E];
            for (int i = 0; i < Token::num_E; i++)
                notes[i] = false;

            do {
                sym = RuleGroup::ruleGroup_[count].getLeft();
                if (sym == RuleGroup::ruleGroup_[rule].getRight(at)) {
                    find = true;
                    std::vector<int>* forward_buf = first(count, 0, forward);
                    for (unsigned int i = 0; i < forward_buf->size(); i++) {
                        if (!notes[forward_buf->at(i)]) {
                            newForward->push_back(forward_buf->at(i));
                            notes[forward_buf->at(i)] = true;
                        }
                    }
                    delete forward_buf;
                }
                count++;
            } while (!(find && (sym != RuleGroup::ruleGroup_[rule].getRight(at))));
            return newForward;
        }
    }
    else if (RuleGroup::ruleGroup_[rule].getRuleLength() == at) {
        std::vector<int>* newForward = new std::vector<int>();
        //����forward�е�ֵ���µ�forward
        for (unsigned int i = 0; i < forward->size(); i++) {
            newForward->push_back(forward->at(i));
        }

        return newForward;
    }
    else {
        exit(0);
    }
}