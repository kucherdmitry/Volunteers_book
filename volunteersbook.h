#ifndef VOLUNTEERSBOOK_H
#define VOLUNTEERSBOOK_H

#include <QMainWindow>
#include <QtDebug>
#include <QtSql>
#include <QShortcut>
#include <QMessageBox>
#include <QSettings>
#include <QMouseEvent>
#include "itemdialog.h"
#include "searchresultview.h"
#include "fontmodifydialog.h"

namespace Ui
{
class VolunteersBook;
}

class VolunteersBook : public QMainWindow
{
    Q_OBJECT

public:
    explicit VolunteersBook(QWidget *parent = nullptr);
    ~VolunteersBook();

    bool addItem(); //add new item to database
    bool updateItem(); //update current item in database
    bool deleteItem(); //delete current item from database
    bool findItem(); //search item in database with criterias
    bool isSearchFieldsIsEmpty(); //checking search fields is empty
    void clearSearchFields(); //clear search fields after searching
    void setFontModyfy();
    void saveSettings();
    void loadSettings();

    void itemContextMenu(QPoint pos);

private:
    Ui::VolunteersBook *ui;

    QSqlDatabase *database = nullptr;
    QSqlTableModel *tablemodel = nullptr;
    QShortcut *deleteFeatureShortcut = nullptr;
    QKeySequence *deleteFeatureSequence = nullptr;
    ItemDialog *itemDialog = nullptr;
    SearchResultView *searchResult = nullptr;
    FontModifyDialog fontModify;

    QMenu *menu = nullptr;
    QAction *ptr_addItem = nullptr;
    QAction *ptr_updateItem = nullptr;
    QAction *ptr_deleteItem = nullptr;
};

#endif // VOLUNTEERSBOOK_H
