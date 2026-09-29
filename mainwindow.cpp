#include "mainwindow.h"
#include "./ui_mainwindow.h"

using namespace Qt::StringLiterals;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/pic/icon.png"));

    initVariant();
    createActions();
    createMenus();

    QString message = tr("Welcome to Inventory Management System !");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Inventory Model"));
    setMinimumSize(320, 320);
}

bool MainWindow::isInt(QString str)
{
    bool isInt;
    str.toInt(&isInt);
    return isInt;
}

MainWindow::~MainWindow()
{
    delete dialogWindow;
    delete list;
    delete widget;
    delete ui;
}

void MainWindow::newFile()
{
    QString dictionary_name = QFileDialog::getExistingDirectory(this, tr("Caption"), "./");
    QString newfileName = dictionary_name + "/" +
                          QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".txt";
    // qDebug() << dictionary_name << "\n";
    // qDebug() << newfileName;
    if (QFile::exists(newfileName)) {
        int ret = QMessageBox::warning(this, tr("Warning"),
                                       tr("File already exists !\n"
                                          "Do you want to open the file or overwrite it with an empty file?"),
                                       QMessageBox::Open | QMessageBox::Discard | QMessageBox::Cancel);
        switch (ret) {
        case QMessageBox::Open:
            openfile(newfileName);
            break;
        case QMessageBox::Discard:
            QFile::remove(newfileName);
            openfile(newfileName);
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
        }
    }
    else {
        openfile(newfileName);
    }
    fileName = newfileName;
}

void MainWindow::open()
{
    if (widget == nullptr) {
        widget = new QTreeWidget();
        setCentralWidget(widget);
    }
    else
        widget->clear();
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "./", tr("TXT files(*.txt);;All files (*.*)"));
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("File Open Failed !"), QMessageBox::Ok);
        return;
    }
    openfile(fileName);
}

void MainWindow::save()
{
    QString fileContent = list->outputListData();
    QFile file(fileName);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.write(fileContent.toUtf8());
    file.close();
}

void MainWindow::close()
{
    int ret = QMessageBox::question(this, tr("Question"),
                                    tr("File will be closed !\n"
                                       "Do you want to save the file first ?"),
                                    QMessageBox::Yes | QMessageBox::No);
    switch (ret) {
    case QMessageBox::Yes:
        save();
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
    if (list != nullptr)
        delete list;
    if (widget != nullptr)
        delete widget;
    initVariant();
    // 禁用文件操作
    saveAct->setEnabled(false);
    closeAct->setEnabled(false);
    actionMenu->menuAction()->setEnabled(false);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Storage Model"), tr("Developer: <b>YQJMonster</b>"));
}

void MainWindow::openEditor(QTreeWidgetItem *item, int column)
{
    if ((column == 1 || column == 2 || column == 3) && isInt(item->text(0))) {
        widget->openPersistentEditor(item, column);
        tmpItem = item;
        tmpColumn = column;
    }
}

void MainWindow::closeEditor()
{
    if (tmpItem != nullptr) {
        widget->closePersistentEditor(tmpItem, tmpColumn);
        int number = tmpItem->text(0).toInt();
        LinkNode *findNode = list->searchNode(tmpItem->parent()->text(0), number);
        switch (tmpColumn) {
        case 1:
            findNode->brand = tmpItem->text(tmpColumn);
            break;
        case 2:
            findNode->unitPrice = tmpItem->text(tmpColumn).toInt();
            break;
        case 3:
            findNode->amount = tmpItem->text(tmpColumn).toInt();
            break;
        default:
            break;
        }
    }
    tmpItem = nullptr;
    tmpColumn = 0;
}

void MainWindow::receiveData(QString data)
{
    tmpString = data;
}

void MainWindow::openfile(QString filename)
{
    std::ifstream stream(filename.toStdString());

    list = new LinkList(stream);
    stream.close();

    initWidget(list);
}

void MainWindow::initWidget(LinkList *list)
{
    // 启用文件操作
    saveAct->setEnabled(true);
    closeAct->setEnabled(true);
    actionMenu->menuAction()->setEnabled(true);
    // 绑定双击编辑特定参数
    connect(widget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(openEditor(QTreeWidgetItem*,int)));
    connect(widget, SIGNAL(itemSelectionChanged()), this, SLOT(closeEditor()));

    const QStringList headers({tr("Type & Number"), tr("Brand"), tr("Unit Price"), tr("Amount")});
    widget->setHeaderLabels(headers);
    widget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    list->setUpWidgetRoot(widget);
    widget->expandAll();
}

void MainWindow::initVariant()
{
    fileName = nullptr;
    tmpString = nullptr;
    list = nullptr;
    widget = nullptr;
    tmpItem = nullptr;
    tmpColumn = 0;
    dialogWindow = nullptr;
}

void MainWindow::createActions()
{
    // File栏内的动作实现
    // 创建新文件
    newAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew),
                         tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    // 打开已有文件
    openAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentOpen),
                          tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    // 保存当前文件
    saveAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave),
                          tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    // 关闭当前文件
    closeAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties),
                           tr("&Close"), this);
    closeAct->setShortcuts(QKeySequence::Close);
    closeAct->setStatusTip(tr("Close the document"));
    connect(closeAct, &QAction::triggered, this, &MainWindow::close);

    // 退出应用
    exitAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::ApplicationExit),
                          tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    // Actions栏内的动作实现
    // 显示所有项目
    showAllItemsAction = new QAction(tr("&Show All Items"), this);
    showAllItemsAction->setStatusTip(tr("Show All Existing Items"));
    connect(showAllItemsAction, &QAction::triggered, this, &MainWindow::showAllItems);

    // 插入行
    insertItemAction = new QAction(tr("&Insert Item"), this);
    insertItemAction->setStatusTip(tr("Insert A New Item"));
    connect(insertItemAction, &QAction::triggered, this, &MainWindow::insertItem);

    // 删除行
    removeItemAction = new QAction(tr("&Remove Item"), this);
    removeItemAction->setStatusTip(tr("Remove Current Item"));
    connect(removeItemAction, &QAction::triggered, this, &MainWindow::removeItem);

    // 添加子项目
    insertChildAction = new QAction(tr("&Insert Child"), this);
    insertChildAction->setStatusTip(tr("Insert A New Child"));
    connect(insertChildAction, &QAction::triggered, this, &MainWindow::insertChild);

    // 查询子项目
    searchAction = new QAction(tr("&Search Item"), this);
    searchAction->setStatusTip(tr("Search Certain Item"));
    connect(searchAction, &QAction::triggered, this, &MainWindow::searchItem);

    // 排序所有子项目
    sortItemsAction = new QAction(tr("&Sort Items"), this);
    sortItemsAction->setStatusTip(tr("Sort All Items"));
    connect(sortItemsAction, &QAction::triggered, this, &MainWindow::sortItems);

    // 关于此应用
    aboutAct = new QAction(QIcon::fromTheme(QIcon::ThemeIcon::HelpAbout),
                           tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(closeAct);
    fileMenu->addAction(exitAct);

    actionMenu = menuBar()->addMenu(tr("&Action"));
    actionMenu->addAction(showAllItemsAction);
    actionMenu->addAction(insertItemAction);
    actionMenu->addAction(removeItemAction);
    actionMenu->addAction(insertChildAction);
    actionMenu->addAction(searchAction);
    actionMenu->addAction(sortItemsAction);

    saveAct->setEnabled(false);
    closeAct->setEnabled(false);
    actionMenu->menuAction()->setEnabled(false);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::showAllItems()
{
    QTreeWidgetItemIterator it(widget);
    while (*it)
    {
        (*it)->setHidden(false);
        ++it;
    }
}

void MainWindow::insertItem()
{
    if (dialogWindow == nullptr) {
        dialogWindow = new Dialog(this);
        dialogWindow->setUpInsert();
        connect(dialogWindow, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
        dialogWindow->show();
        if (dialogWindow->exec() == QDialog::Accepted) {
            list->addNewType(tmpString);
        }
        tmpString = nullptr;
        initWidget(list);
        delete dialogWindow;
        dialogWindow = nullptr;
    }
}

void MainWindow::removeItem()
{
    QList<QTreeWidgetItem *> itemList =  widget->selectedItems();
    for (int i = 0; i < itemList.size(); i++) {
        QTreeWidgetItem *curItem = itemList.at(i);
        if (isInt(curItem->text(0))) {
            list->deleteNode(curItem->text(0), curItem->text(1), true);
            delete curItem;
            continue;
        }
        int cnt = curItem->childCount();
        for (int i = 0; i < cnt; i++) {
            QTreeWidgetItem *childItem = curItem->child(0);
            list->deleteNode(childItem->text(0), childItem->text(1), true);
            delete childItem;
        }
        list->deleteNode(curItem->text(0), "", false);
        delete curItem;
    }
}

void MainWindow::insertChild()
{
    QTreeWidgetItem *curItem = widget->currentItem();
    if (isInt(curItem->text(0))) {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot add child node to the current item !"), QMessageBox::Ok);
        return;
    }
    list->addChildNode(curItem);
    initWidget(list);
}

void MainWindow::searchItem()
{
    if (dialogWindow == nullptr) {
        dialogWindow = new Dialog(this);
        dialogWindow->setUpSearch();
        connect(dialogWindow, SIGNAL(sendData(QString)), this, SLOT(receiveData(QString)));
        dialogWindow->show();
        if (dialogWindow->exec() == QDialog::Accepted) {}
        QTreeWidgetItemIterator it(widget);
        while (*it)
        {
            if((*it)->text(1).contains(tmpString))
            {
                (*it)->setHidden(false);
                QTreeWidgetItem *item = *it;
                while (item->parent())
                {
                    item->parent()->setHidden(false);
                    item = item->parent();
                }
            }
            else
            {
                (*it)->setHidden(true);
            }
            ++it;
        }
        tmpString = nullptr;
        delete dialogWindow;
        dialogWindow = nullptr;
    }
}

void MainWindow::sortItems()
{
    list->sortAllItems();
    initWidget(list);
}

void MainWindow::updateActions()
{

}

