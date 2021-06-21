#include "mainwindow.h"
#include <globaldef.hpp>
#include <QApplication>
#include "maincontent/controlwidget/crashprocess.h"
#include "screen/mainobject.h"
#include <stdio.h>




int main(int argc, char *argv[])
{

    SetUnhandledExceptionFilter(errCallback);
    QApplication a(argc, argv);
    initMessageBox();
    MainObject mainObject;
    mainObject.setInit();
    LOADQSS(GlobalSpace::STYLE_QSS_FILE_PATH);
    int restartflag = a.exec();
    if(restartflag==-12345678){
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        return 0;
    }
    return restartflag;
}
