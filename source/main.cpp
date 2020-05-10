#include <QApplication>
#include "logic.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Logic l;
    l.show();

    return a.exec();
}
