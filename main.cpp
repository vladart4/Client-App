#include "mainwindow.h"
#include "client.h"
#include "authorizationdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Client *w = new Client();
    w->ShowAut();

    return a.exec();
}
