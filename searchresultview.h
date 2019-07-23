#ifndef SEARCHRESULTVIEW_H
#define SEARCHRESULTVIEW_H

#include <QDialog>
#include <QtSql>
#include "fontmodifydialog.h"

namespace Ui
{
class SearchResultView;
}

class SearchResultView : public QDialog
{
    Q_OBJECT

public:
    explicit SearchResultView(QWidget *parent = nullptr);
    ~SearchResultView();

    void displaySearchResult(FontModifyDialog &font, QString &filter, QSqlDatabase *database);

private:
    Ui::SearchResultView *ui;

    QSqlTableModel *model;
};

#endif // SEARCHRESULTVIEW_H
