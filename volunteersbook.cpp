#include "volunteersbook.h"
#include "ui_volunteersbook.h"

VolunteersBook::VolunteersBook(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::VolunteersBook)
{
    ui->setupUi(this);

    //=================[Creating or connecting to database]=====================

    database = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    database->setDatabaseName("volunteerbook.db");

    if(!database->open())
    {
        qDebug() << "Opening error:" << database->lastError();
    }

    //===============[Creating new table if it's not exists]====================

    if(database->tables().isEmpty())
    {
        QSqlQuery query;
        QString str = "CREATE TABLE Volunteers (ID         INTEGER        NOT NULL ON CONFLICT ABORT"
                      "                                                   PRIMARY KEY AUTOINCREMENT,"
                      "                         Secondname VARCHAR (255)  NOT NULL ON CONFLICT ABORT,"
                      "                         Firstname  VARCHAR (255)  NOT NULL ON CONFLICT ABORT,"
                      "                         Thirdname  VARCHAR (255)  NOT NULL ON CONFLICT ABORT,"
                      "                         Phone      VARCHAR (12)   NOT NULL ON CONFLICT ABORT,"
                      "                         Address    VARCHAR (1024) NOT NULL ON CONFLICT ABORT,"
                      "                         CardID     VARCHAR (6)    NOT NULL ON CONFLICT ABORT"
                      "                                                   UNIQUE                   );";

        if(!query.exec(str))
        {
            QMessageBox::warning(this, "Помилка при створенні бази!",
                                       "Неможливо створити таблицю в базі!");
        }
    }

    //===============[Initialization of database TableView]=====================

    tablemodel = new QSqlTableModel(nullptr, *database);
    tablemodel->setTable("Volunteers");
    tablemodel->select();
    tablemodel->setHeaderData(1,Qt::Horizontal, tr("Прізвище"));
    tablemodel->setHeaderData(2,Qt::Horizontal, tr("Ім'я"));
    tablemodel->setHeaderData(3,Qt::Horizontal, tr("По-батькові"));
    tablemodel->setHeaderData(4,Qt::Horizontal, tr("Телефон"));
    tablemodel->setHeaderData(5,Qt::Horizontal, tr("Адреса"));
    tablemodel->setHeaderData(6,Qt::Horizontal, tr("Картка №"));

    ui->bookView->setModel(tablemodel);

    loadSettings(); // load font settings on starting

    ui->bookView->setContextMenuPolicy(Qt::CustomContextMenu); // initialize custom context menu for bookView widget

    ui->bookView->hideColumn(0);
    ui->bookView->resizeColumnToContents(0);
    ui->bookView->resizeColumnToContents(1);
    ui->bookView->resizeColumnToContents(2);
    ui->bookView->resizeColumnToContents(3);
    ui->bookView->resizeColumnToContents(4);
    ui->bookView->resizeColumnToContents(5);
    ui->bookView->resizeColumnToContents(6);

    ui->secondNameEdit->setFocus();
    ui->secondNameEdit->setTabOrder(ui->secondNameEdit, ui->firstnameEdit);
    ui->firstnameEdit->setTabOrder(ui->firstnameEdit, ui->thirdNameEdit);
    ui->thirdNameEdit->setTabOrder(ui->thirdNameEdit, ui->phoneEdit);
    ui->phoneEdit->setTabOrder(ui->phoneEdit, ui->cardIDEdit);

    ui->phoneEdit->setInputMask("+38 (999) 999-99-99");

    ui->bookView->show();

    //=======================[Connecting actions]===============================

    connect(ui->addItem, &QAction::triggered, this, &VolunteersBook::addItem);
    connect(ui->updateItem, &QAction::triggered, this, &VolunteersBook::updateItem);
    connect(ui->deleteItem, &QAction::triggered, this, &VolunteersBook::deleteItem);
    connect(ui->searchBtn, &QPushButton::clicked, this, &VolunteersBook::findItem);
    connect(ui->fontTuning, &QAction::triggered, this, &VolunteersBook::setFontModyfy);
    connect(ui->bookView, &QTableView::customContextMenuRequested, this, &VolunteersBook::itemContextMenu); // connect right button click with context menu
}

VolunteersBook::~VolunteersBook()
{
    saveSettings();

    if(database->isOpen())
    {
        database->close();
    }
    delete ui;
}

bool VolunteersBook::addItem()
{
    itemDialog = new ItemDialog(this);
    itemDialog->addItemMode();
    itemDialog->exec();

    if(!itemDialog->isFieldsIsNull())
    {
        QSqlQuery query;
        QString str = "INSERT INTO Volunteers (Secondname, Firstname, Thirdname, Phone, Address, CardID) VALUES ('%1', '%2', '%3', '%4', '%5', '%6')";
        QString str_add = str.arg(itemDialog->getSecondname())
                             .arg(itemDialog->getFirstname())
                             .arg(itemDialog->getThirdname())
                             .arg(itemDialog->getPhone())
                             .arg(itemDialog->getAddress())
                             .arg(itemDialog->getCardID());

        if(!query.exec(str_add))
        { 
            QMessageBox::warning(this, "Помилка при створенні запису!",
                                       "Неможливо створити запис!"
                                       "\nОбліковий номер картки волонтера вже існує в базі!");
            return false;
        }

        tablemodel->select();
        ui->bookView->hideColumn(0);
        ui->bookView->resizeColumnToContents(0);
        ui->bookView->resizeColumnToContents(1);
        ui->bookView->resizeColumnToContents(2);
        ui->bookView->resizeColumnToContents(3);
        ui->bookView->resizeColumnToContents(4);
        ui->bookView->resizeColumnToContents(5);
        ui->bookView->resizeColumnToContents(6);
        ui->bookView->show();

        return true;
    }
    else
    {
        QMessageBox::warning(this, "Помилка при створенні запису!",
                                   "Неможливо створити запис!"
                                   "\nНе всі поля заповнені!");
        return false;
    }
}

bool VolunteersBook::updateItem()
{
    if(ui->bookView->selectionModel()->hasSelection())
    {
        int index = ui->bookView->selectionModel()->currentIndex().row();

        itemDialog = new ItemDialog;                     //
        itemDialog->updateItemMode(index, *tablemodel);  // Launch service dialog
        itemDialog->exec();                              //

        if(!itemDialog->isFieldsIsNull())
        {
            QSqlQuery query;
            QString str = "UPDATE Volunteers SET Secondname = '%1', "
                                                "Firstname = '%2', "
                                                "Thirdname = '%3', "
                                                "Phone = '%4', "
                                                "Address = '%5' "
                                                "WHERE ID = '%6';";

            QString str_edit = str.arg(itemDialog->getSecondname())
                                  .arg(itemDialog->getFirstname())
                                  .arg(itemDialog->getThirdname())
                                  .arg(itemDialog->getPhone())
                                  .arg(itemDialog->getAddress())
                                  .arg(itemDialog->getID());

            if(!query.exec(str_edit))
            {
                QMessageBox::warning(this, "Помилка при редагуванні запису!",
                                           "Неможливо відредагувати запис!");
                return false;
            }

            tablemodel->select();
            ui->bookView->hideColumn(0);
            ui->bookView->resizeColumnToContents(0);  //
            ui->bookView->resizeColumnToContents(1);  //
            ui->bookView->resizeColumnToContents(2);  //
            ui->bookView->resizeColumnToContents(3);  // Resize table columns to its contents
            ui->bookView->resizeColumnToContents(4);  //
            ui->bookView->resizeColumnToContents(5);  //
            ui->bookView->resizeColumnToContents(6);  //
            ui->bookView->show();

            return true;
        }
    }

    return false;
}

bool VolunteersBook::deleteItem()
{
    if(ui->bookView->selectionModel()->hasSelection())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Видалення записів",
                                      "Ви впевнені, що хочете видалити позначені записи?",
                                      QMessageBox::Yes | QMessageBox::No);

        if(reply == QMessageBox::Yes)
        {
            QModelIndexList selectedRows = ui->bookView->selectionModel()->selectedIndexes();

            for (QModelIndex item : selectedRows)
            {
                QSqlQuery query;
                QString str = "DELETE FROM Volunteers WHERE ID = '" + item.model()->index(item.row(), 0).data().toString() + "';";

                if(!query.exec(str))
                {
                    QMessageBox::warning(this, "Помилка!","Помилка при видаленні запису!");
                    return false;
                }
            }


            tablemodel->select();
            return true;
        }

        ui->bookView->hideColumn(0);
        ui->bookView->show();
        return false;
    }

    return false;
}

bool VolunteersBook::findItem()
{
    if(!isSearchFieldsIsEmpty())
    {
        searchResult = new SearchResultView(this); // Search result window object

        QString cardID = ui->cardIDEdit->text();

        while (cardID.length() != 5)
        {
            cardID = cardID.insert(0, '0');
        }

        QString str = "Secondname = '%1' OR Firstname = '%2' OR Thirdname = '%3' OR Phone = '%4' OR CardID = '%5';";
        QString filter = str.arg(ui->secondNameEdit->text())
                            .arg(ui->firstnameEdit->text())
                            .arg(ui->thirdNameEdit->text())
                            .arg(ui->phoneEdit->text())
                            .arg(cardID);

        // Method thats display result of search using filter stringc
        searchResult->displaySearchResult(fontModify, filter, database);
        searchResult->exec();

        clearSearchFields();

        return true;
    }
    return false;
}

bool VolunteersBook::isSearchFieldsIsEmpty()
{
    if(ui->firstnameEdit->text().isEmpty()
    && ui->secondNameEdit->text().isEmpty()
    && ui->thirdNameEdit->text().isEmpty()
    && ui->phoneEdit->text().isEmpty()
    && ui->cardIDEdit->text().isEmpty())
    {
        return true;
    }
    return false;
}

void VolunteersBook::clearSearchFields()
{
    ui->firstnameEdit->clear();
    ui->secondNameEdit->clear();
    ui->thirdNameEdit->clear();
    ui->phoneEdit->clear();
    ui->cardIDEdit->clear();
}

void VolunteersBook::setFontModyfy()
{
    fontModify.exec();

    ui->bookView->setFont(fontModify.getFont());

    ui->bookView->resizeColumnToContents(0);  //
    ui->bookView->resizeColumnToContents(1);  //
    ui->bookView->resizeColumnToContents(2);  //
    ui->bookView->resizeColumnToContents(3);  // Resize table columns to its contents
    ui->bookView->resizeColumnToContents(4);  //
    ui->bookView->resizeColumnToContents(5);  //
    ui->bookView->resizeColumnToContents(6);  //
    ui->bookView->show();
}

void VolunteersBook::saveSettings()
{
    QSettings settings("fontSettings.ini", QSettings::IniFormat);

    settings.beginGroup("fontSettings");
    settings.setValue("fontType", ui->bookView->font());
    settings.setValue("fontPointSize", ui->bookView->font().pointSize());
    settings.endGroup();
}

void VolunteersBook::loadSettings()
{
    QSettings settings("fontSettings.ini", QSettings::IniFormat);

    settings.beginGroup("fontSettings");
    fontModify.setFont(qvariant_cast<QFont>(settings.value("fontType", ui->bookView->font())));
    fontModify.setPointSize(qvariant_cast<int>(settings.value("fontPointSize", ui->bookView->font().pointSize())));
    ui->bookView->setFont(fontModify.getFont());
    fontModify.setFontFields(fontModify.getFont());
    settings.endGroup();


}

void VolunteersBook::itemContextMenu(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QAction *addItem = new QAction(tr("Створити запис"), this);
    QAction *updateItem = new QAction(tr("Змінити запис"), this);
    QAction *deleteItem = new QAction(tr("Видалити запис"), this);

    addItem->setIcon(QIcon(":/icons/addItemIcon.ico"));
    updateItem->setIcon(QIcon(":/icons/updateItemIcon.ico"));
    deleteItem->setIcon(QIcon(":/icons/deleteItemIcon.ico"));

    connect(addItem, &QAction::triggered, this, &VolunteersBook::addItem);
    connect(updateItem, &QAction::triggered, this, &VolunteersBook::updateItem);
    connect(deleteItem, &QAction::triggered, this, &VolunteersBook::deleteItem);

    menu->addAction(addItem);
    menu->addAction(updateItem);
    menu->addAction(deleteItem);

    menu->popup(ui->bookView->viewport()->mapToGlobal(pos));
}
