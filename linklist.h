#ifndef LINKLIST_H
#define LINKLIST_H
#include <bits/stdc++.h>
#include <QString>
#include <QObject>
#include <QTreeWidget>
#include <QDataStream>

struct LinkNode
{
    QString type, brand;
    int unitPrice, amount, childNum;
    bool nodeType;
    LinkNode* next;
    LinkNode(QString type = "", int childNum = 0, bool nodeType = true):
        type(type), childNum(childNum), nodeType(nodeType) {
        brand = "";
        unitPrice = -1, amount = -1, next = nullptr;
    }
    LinkNode(QString type = "", QString brand = "", int unitPrice = 0, int amount = 0, int childNum = 0, LinkNode* next = nullptr, bool nodeType = true):
        type(type), brand(brand), unitPrice(unitPrice), amount(amount), childNum(childNum), next(next), nodeType(nodeType) {}
    bool operator<(LinkNode &a) {
        return unitPrice < a.unitPrice;
    }
};

class LinkList
{
private:
    std::vector<LinkNode *> rootList;
    int listTypeSize;

public:
    LinkList(int num = 0);
    LinkList(std::ifstream &file);
    void addNewType(QString newType);
    void addChildNode(QTreeWidgetItem *curItem);
    bool deleteNode(QString keyName, QString keyBrand, bool type);
    LinkNode* searchNode(QString tmpType, int number);
    LinkNode* searchNode(QString tmpBrand);
    void sortAllItems();

    void setUpWidgetRoot(QTreeWidget *widget);
    void setUpWidgetItems(LinkNode *curNode, QTreeWidget *widget, QTreeWidgetItem *preItem);
    QString outputListData();
};

#endif // LINKLIST_H
