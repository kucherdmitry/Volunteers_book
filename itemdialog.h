#ifndef ITEMDIALOG_H
#define ITEMDIALOG_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class ItemDialog;
}

class ItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ItemDialog(QWidget *parent = nullptr);
    ~ItemDialog();

    QString getID()
    {
        return ID;
    }
    QString getFirstname()
    {
        return Firstname;
    }
    QString getSecondname()
    {
        return Secondname;
    }
    QString getThirdname()
    {
        return  Thirdname;
    }
    QString getPhone()
    {
        return Phone;
    }
    QString getAddress()
    {
        return Address;
    }
    QString getCardID()
    {
        return CardID;
    }

    void setAddItemFields(); // set private class fields in AddMode
    void setUpdateItemFields(); // set private class fields in EditMode

    void addItemMode(); // modifying access to fields in dialog
    void updateItemMode(int &index, QSqlTableModel &model); // modifying access to fields in dialog

    bool isFieldsIsEmpty(); // checking for empty input strings

private:
    Ui::ItemDialog *ui;

    QString ID = "";
    QString Firstname = "";
    QString Secondname = "";
    QString Thirdname = "";
    QString Phone = "";
    QString Address = "";
    QString CardID = "";
};

#endif // ITEMDIALOG_H
