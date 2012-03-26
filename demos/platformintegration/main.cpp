#include <QtGui>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
#ifdef Q_WS_MAC
    // Necessary because of static linking
    Q_INIT_RESOURCE(petroules_utilities_resources);
#endif

    IntegratedApplication a("{8CA9A45E-12BC-49B5-8811-B466192C459F}", argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}