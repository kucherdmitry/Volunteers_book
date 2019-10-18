#include "itemdialog.h"
#include "ui_itemdialog.h"

ItemDialog::ItemDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::ItemDialog)
{
    ui->setupUi(this);

    connect(ui->itemAddBtn, &QPushButton::clicked, this, &ItemDialog::setAddItemFields);
    connect(ui->itemUpdateBtn, &QPushButton::clicked, this, &ItemDialog::setUpdateItemFields);

    ui->itemPhoneEdit->setInputMask("+38 (999) 999-99-99");
}

ItemDialog::~ItemDialog()
{
    delete ui;
}

void ItemDialog::setAddItemFields()
{
    if(!ui->itemCardIDEdit->text().isEmpty())
    {
        CardID = ui->itemCardIDEdit->text();

        while (CardID.length() != 5)
        {
            CardID = CardID.insert(0, '0');
        }
    }

    Firstname = ui->itemFirstNameEdit->text().remove(1, ui->itemFirstNameEdit->text().length()).toUpper()
              + ui->itemFirstNameEdit->text().remove(0, 1).toLower();

    Secondname = ui->itemSecondnameEdit->text().remove(1, ui->itemSecondnameEdit->text().length()).toUpper()
               + ui->itemSecondnameEdit->text().remove(0, 1).toLower();

    Thirdname = ui->itemThirdNameEdit->text().remove(1, ui->itemThirdNameEdit->text().length()).toUpper()
              + ui->itemThirdNameEdit->text().remove(0, 1).toLower();

    if(!ui->itemPhoneEdit->text().isEmpty())
    {
        Phone = ui->itemPhoneEdit->text();
    }

    if(!ui->itemStreetNameEdit->text().isEmpty())
    {
        Address = "вул. " + ui->itemStreetNameEdit->text().remove(1, ui->itemStreetNameEdit->text().length()-1).toUpper()
                          + ui->itemStreetNameEdit->text().remove(0, 1).toLower() + ", " + ui->itemBuildingNumberEdit->text();

        if(!ui->itemAptNumberEdit->text().isEmpty())
        {
            Address += ", кв. " + ui->itemAptNumberEdit->text();
        }
    }
}

void ItemDialog::setUpdateItemFields()
{
    CardID = ui->itemCardIDEdit->text();

    Firstname = ui->itemFirstNameEdit->text().remove(1, ui->itemFirstNameEdit->text().length()).toUpper()
              + ui->itemFirstNameEdit->text().remove(0, 1).toLower();

    Secondname = ui->itemSecondnameEdit->text().remove(1, ui->itemSecondnameEdit->text().length()).toUpper()
               + ui->itemSecondnameEdit->text().remove(0, 1).toLower();

    Thirdname = ui->itemThirdNameEdit->text().remove(1, ui->itemThirdNameEdit->text().length()).toUpper()
              + ui->itemThirdNameEdit->text().remove(0, 1).toLower();

    Phone = ui->itemPhoneEdit->text();

    Address = "вул. " + ui->itemStreetNameEdit->text().remove(1, ui->itemStreetNameEdit->text().length()-1).toUpper() +
                        ui->itemStreetNameEdit->text().remove(0, 1).toLower() + ", " + ui->itemBuildingNumberEdit->text();

    if(!ui->itemAptNumberEdit->text().isEmpty())
    {
        Address += ", кв. " + ui->itemAptNumberEdit->text();
    }
}

void ItemDialog::addItemMode()
{
    setWindowIcon(QIcon(":/icons/addItemIcon.ico"));
    ui->itemUpdateBtn->setDisabled(true);
    ui->itemAddBtn->setDefault(true);

    ui->itemCardIDEdit->setFocus();

    ui->itemCardIDEdit->setTabOrder(ui->itemCardIDEdit,ui->itemSecondnameEdit);
    ui->itemSecondnameEdit->setTabOrder(ui->itemSecondnameEdit,ui->itemFirstNameEdit);
    ui->itemFirstNameEdit->setTabOrder(ui->itemFirstNameEdit,ui->itemThirdNameEdit);
    ui->itemThirdNameEdit->setTabOrder(ui->itemThirdNameEdit,ui->itemPhoneEdit);
    ui->itemPhoneEdit->setTabOrder(ui->itemPhoneEdit,ui->itemStreetNameEdit);
    ui->itemStreetNameEdit->setTabOrder(ui->itemStreetNameEdit,ui->itemBuildingNumberEdit);
    ui->itemBuildingNumberEdit->setTabOrder(ui->itemBuildingNumberEdit,ui->itemAptNumberEdit);
}

void ItemDialog::updateItemMode(int &index, QSqlTableModel &model)
{
    QString address = model.index(index, 5).data().toString();

    setWindowIcon(QIcon(":/icons/updateItemIcon.ico"));

    ID = model.index(index, 0).data().toString();
    ui->itemCardIDEdit->setText(model.index(index, 6).data().toString());
    ui->itemFirstNameEdit->setText(model.index(index, 2).data().toString());
    ui->itemSecondnameEdit->setText(model.index(index, 1).data().toString());
    ui->itemThirdNameEdit->setText(model.index(index, 3).data().toString());
    ui->itemPhoneEdit->setText(model.index(index, 4).data().toString());

    ui->itemStreetNameEdit->setText(address.section(',', 0, 0).remove(0, 5));
    ui->itemBuildingNumberEdit->setText(address.section(',', 1, 1).remove(0, 1));
    ui->itemAptNumberEdit->setText(address.section(',', 2).remove(0, 5));

    ui->itemCardIDEdit->setDisabled(true);
    ui->itemAddBtn->setDisabled(true);
    ui->itemUpdateBtn->setDefault(true);

    ui->itemSecondnameEdit->setFocus();
    ui->itemSecondnameEdit->setTabOrder(ui->itemSecondnameEdit,ui->itemFirstNameEdit);
    ui->itemFirstNameEdit->setTabOrder(ui->itemFirstNameEdit,ui->itemThirdNameEdit);
    ui->itemThirdNameEdit->setTabOrder(ui->itemThirdNameEdit,ui->itemPhoneEdit);
    ui->itemPhoneEdit->setTabOrder(ui->itemPhoneEdit,ui->itemStreetNameEdit);
    ui->itemStreetNameEdit->setTabOrder(ui->itemStreetNameEdit,ui->itemBuildingNumberEdit);
    ui->itemBuildingNumberEdit->setTabOrder(ui->itemBuildingNumberEdit,ui->itemAptNumberEdit);
}

bool ItemDialog::isFieldsIsNull()
{
    if(CardID.isNull()
    || Secondname.isNull()
    || Firstname.isNull()
    || Thirdname.isNull()
    || Phone.isNull()
    || Address.isNull())
    {
        return true;
    }
    return false;
}
