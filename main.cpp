#include "basewnd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    baseWnd w;
    //w.show();
    w.showFullScreen();

    return a.exec();
}
