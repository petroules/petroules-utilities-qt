#include <QtGui>
#include "hookmonitorwindow.h"
#include "integratedapplication.h"

int main(int argc, char *argv[])
{
    // Necessary because of static linking
    Q_INIT_RESOURCE(petroules_utilities_resources);

    IntegratedApplication a("{E75ACF2F-9AAF-43EB-B4C4-6FB8F1E58F56}", argc, argv);
    HookMonitorWindow w;
    w.show();

    return a.exec();
}
