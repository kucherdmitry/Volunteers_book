#include "searchresultview.h"
#include "ui_searchresultview.h"

SearchResultView::SearchResultView(QWidget *parent) : QDialog(parent),
    ui(new Ui::SearchResultView)
{
    ui->setupUi(this);
}

SearchResultView::~SearchResultView()
{
    delete ui;
}

void SearchResultView::displaySearchResult(FontModifyDialog &font, QString &filter, QSqlDatabase *database)
{
    model = new QSqlTableModel(nullptr, *database);
    model->setTable("Volunteers");
    model->setFilter(filter);
    model->select();
    model->setHeaderData(1,Qt::Horizontal, tr("Прізвище"));
    model->setHeaderData(2,Qt::Horizontal, tr("Ім'я"));
    model->setHeaderData(3,Qt::Horizontal, tr("По-батькові"));
    model->setHeaderData(4,Qt::Horizontal, tr("Телефон"));
    model->setHeaderData(5,Qt::Horizontal, tr("Адреса"));
    model->setHeaderData(6,Qt::Horizontal, tr("Картка №"));

    ui->searchResultTableView->setModel(model);
    ui->searchResultTableView->hideColumn(0);
    ui->searchResultTableView->setFont(font.getFont());

    ui->searchResultTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->searchResultTableView->resizeColumnToContents(0);
    ui->searchResultTableView->resizeColumnToContents(1);
    ui->searchResultTableView->resizeColumnToContents(2);
    ui->searchResultTableView->resizeColumnToContents(3);
    ui->searchResultTableView->resizeColumnToContents(4);
    ui->searchResultTableView->resizeColumnToContents(5);
    ui->searchResultTableView->resizeColumnToContents(6);

    ui->searchResultTableView->show();
}
