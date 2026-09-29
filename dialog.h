#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

signals:
    void sendData(QString data);

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void setUpInsert();
    void setUpSearch();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
