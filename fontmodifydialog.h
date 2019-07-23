#ifndef FONTMODIFYDIALOG_H
#define FONTMODIFYDIALOG_H

#include <QDialog>
#include <QtWidgets>

namespace Ui
{
class FontModifyDialog;
}

class FontModifyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FontModifyDialog(QWidget *parent = nullptr);
    ~FontModifyDialog();

    QFont getFont()
    {
        return *font;
    }
    void setFont(QFont newfont)
    {
        *font = newfont;
    }
    void setPointSize(int pointsize)
    {
        font->setPointSize(pointsize);
    }

    void setFontFields(const QFont &font);
    void ifFontModified();

private:
    Ui::FontModifyDialog *ui;

    QFont *font;
};

#endif // FONTMODIFYDIALOG_H
