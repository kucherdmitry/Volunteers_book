#include "fontmodifydialog.h"
#include "ui_fontmodifydialog.h"

FontModifyDialog::FontModifyDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::FontModifyDialog)
{
    ui->setupUi(this);

    font = new QFont(ui->itemFontBox->currentFont());
    ui->itemFontSize->setValue(font->pointSize());

    ui->itemFontBox->setFocus();
    ui->itemFontBox->setTabOrder(ui->itemFontBox, ui->itemFontSize);
    ui->itemFontSize->setTabOrder(ui->itemFontSize, ui->OKBtn);

    connect(ui->OKBtn, &QPushButton::clicked, this, &FontModifyDialog::ifFontModified);
}

FontModifyDialog::~FontModifyDialog()
{
    delete ui;
}

void FontModifyDialog::setFontFields(const QFont &loadedFont)
{
    ui->itemFontBox->setCurrentFont(loadedFont);
    ui->itemFontSize->setValue(loadedFont.pointSize());
}

void FontModifyDialog::ifFontModified()
{
    *font = ui->itemFontBox->currentFont();
    font->setPointSize(ui->itemFontSize->value());
}
