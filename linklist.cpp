#include "linklist.h"

LinkList::LinkList(int num):listTypeSize(num) {
    rootList.resize(num);
}

LinkList::LinkList(std::ifstream &file) {
    rootList.clear();
    file >> listTypeSize;
    for (int i = 1; i <= listTypeSize; i++) {
        std::string tmpType, tmpBrand;
        int tmpNum, tmpUnitPrice, tmpAmount;
        file >> tmpType >> tmpNum;
        LinkNode *headNode = new LinkNode(QString::fromStdString(tmpType), tmpNum, false);
        rootList.push_back(headNode);
        LinkNode *pre = headNode;
        for (int j = 1; j <= tmpNum; j++) {
            file >> tmpBrand >> tmpUnitPrice >> tmpAmount;
            LinkNode *tmp = new LinkNode(QString::fromStdString(tmpType), QString::fromStdString(tmpBrand), tmpUnitPrice, tmpAmount, tmpNum - j, nullptr);
            pre->next = tmp;
            pre = tmp;
        }
    }
}

void LinkList::addNewType(QString newType)
{
    LinkNode *newNode = new LinkNode(newType, 0, false);
    rootList.push_back(newNode);
    listTypeSize++;
}

void LinkList::addChildNode(QTreeWidgetItem *curItem)
{
    for (auto &it : rootList) {
        if (it->type == curItem->text(0)) {
            LinkNode *lastNode = it;
            it->childNum++;
            LinkNode *newNode = new LinkNode(curItem->text(0), QString("[No Brand]"), 0, 0, 0, nullptr, true);
            while (lastNode->next != nullptr) {
                lastNode = lastNode->next;
            }
            lastNode->next = newNode;
            break;
        }
    }
}

bool LinkList::deleteNode(QString keyName, QString keyBrand, bool type)
{
    if (!type) {
        for (int i = 0; i < rootList.size(); i++) {
            if (rootList[i]->type == keyName) {
                rootList.erase(rootList.begin() + i);
                listTypeSize--;
                break;
            }
        }
        return true;
    }
    for (auto &it : rootList) {
        if (it->type == keyName) {
            LinkNode *curCurse = it->next, *preCurse = it;
            while (curCurse->brand != keyBrand && curCurse != nullptr) {
                preCurse = curCurse;
                curCurse = curCurse->next;
            }
            if (!curCurse) {
                preCurse->next = curCurse->next;
                delete curCurse;
                return true;
            }
            else
                return false;
        }
    }
    return false;
}

LinkNode *LinkList::searchNode(QString tmpType, int number)
{
    for (auto &it : rootList) {
        if (it->type == tmpType) {
            LinkNode *ansNode = it;
            while (number--) {
                ansNode = ansNode->next;
            }
            return ansNode;
        }
    }
    return nullptr;
}

LinkNode *LinkList::searchNode(QString tmpBrand)
{
    for (auto &it : rootList) {
        LinkNode *tmpNode = it->next;
        while (tmpNode != nullptr) {
            if (tmpNode->brand == tmpBrand)
                return tmpNode;
            tmpNode = tmpNode->next;
        }
    }
    return nullptr;
}

void LinkList::sortAllItems()
{
    for (auto &it : rootList) {
        std::vector<LinkNode *> tmpArray;
        LinkNode *tmpNode = it->next;
        it->next = nullptr;
        while (tmpNode != nullptr) {
            LinkNode *preNode = tmpNode;
            tmpArray.push_back(preNode);
            tmpNode = tmpNode->next;
            preNode->next = nullptr;
        }
        sort(tmpArray.begin(), tmpArray.end(), [](LinkNode *a, LinkNode *b) { return a->unitPrice < b->unitPrice; });
        tmpNode = it;
        for (auto &it2 : tmpArray) {
            tmpNode->next = it2;
            tmpNode = it2;
        }
    }
}

void LinkList::setUpWidgetRoot(QTreeWidget *widget)
{
    widget->clear();
    for (auto &it : rootList) {
        QStringList rootInfo;
        rootInfo << it->type;
        QTreeWidgetItem *item = new QTreeWidgetItem(rootInfo, Qt::EditRole);
        widget->addTopLevelItem(item);
        setUpWidgetItems(it->next, widget, item);
    }
}

void LinkList::setUpWidgetItems(LinkNode *curNode, QTreeWidget *widget, QTreeWidgetItem *preItem)
{
    int i = 1;
    while (curNode != nullptr) {
        QStringList itemInfo;
        itemInfo << QString::number(i) << curNode->brand;
        itemInfo << QString::number(curNode->unitPrice) << QString::number(curNode->amount);
        QTreeWidgetItem *item = new QTreeWidgetItem(itemInfo, Qt::EditRole);
        // item->setFlags(item->flags() | Qt::ItemIsEditable);
        preItem->addChild(item);
        curNode = curNode->next;
        i++;
    }
}

QString LinkList::outputListData() {
    QString fileContent = "";
    fileContent.append(QString::number(listTypeSize));
    fileContent.append('\n');
    for (auto &it : rootList) {
        QString tmpData = it->type + QObject::tr(" ") + QString::number(it->childNum);
        fileContent.append(tmpData);
        fileContent.append('\n');
        LinkNode *tmpCurse = it->next;
        while (tmpCurse != nullptr) {
            QString lineData = tmpCurse->brand + QObject::tr(" ") + QString::number(tmpCurse->unitPrice) +
                               QObject::tr(" ") + QString::number(tmpCurse->amount) + QObject::tr("\n");
            fileContent.append(lineData);
            tmpCurse = tmpCurse->next;
        }
    }
    // qDebug() << fileContent;
    return fileContent;
}
