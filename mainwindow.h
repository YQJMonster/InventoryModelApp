#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include "dialog.h"
#include "linklist.h"
#include <QMainWindow>
#include <QtWidgets>
#include <QIcon>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QFile;
class QString;
class QTreeWidget;
class QDateTime;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool isInt(QString str);
    ~MainWindow();

private slots:
    void newFile();
    void open();
    void save();
    void close();
    void about();

    void openEditor(QTreeWidgetItem *item, int column);
    void closeEditor();

    void receiveData(QString data);

    void updateActions();

private:
    void initVariant();

    void openfile(QString filename);
    void initWidget(LinkList *list);

    void showAllItems();
    void insertItem();
    void removeItem();
    void insertChild();
    void searchItem();
    void sortItems();

    void createActions();
    void createMenus();

    int tmpColumn;
    Ui::MainWindow *ui;
    Dialog *dialogWindow;
    LinkList *list;
    QTreeWidget *widget;
    QTreeWidgetItem *tmpItem;
    QMenu *fileMenu;
    QMenu *actionMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *closeAct;
    QAction *exitAct;

    QAction *showAllItemsAction;
    QAction *insertItemAction;
    QAction *removeItemAction;
    QAction *insertChildAction;
    QAction *searchAction;
    QAction *sortItemsAction;

    QAction *aboutAct;
    QString fileName;
    QString tmpString;
};
#endif // MAINWINDOW_H
