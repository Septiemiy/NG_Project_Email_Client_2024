#include "emailclient.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EmailClient w;
    w.show();
    return a.exec();
}
