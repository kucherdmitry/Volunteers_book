#include "volunteersbook.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VolunteersBook w;
    w.show();

    return a.exec();
}
