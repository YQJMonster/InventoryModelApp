#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/pic/icon.png"));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setUpInsert()
{
    setWindowTitle("Insert New Item");
    ui->label->setText("请输入要添加的家电种类名：");
}

void Dialog::setUpSearch()
{
    setWindowTitle("Search Certain Item");
    ui->label->setText("请输入要查询的家电品牌名：");
}

void Dialog::on_buttonBox_accepted()
{
    emit sendData(ui->lineEdit->text());
    this->close();
}


void Dialog::on_buttonBox_rejected()
{
    QMessageBox::information(this, tr("Information"), tr("Type Add Canceled."));
    this->close();
}

