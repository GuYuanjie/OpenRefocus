#ifndef CRASHPROCESS_H
#define CRASHPROCESS_H


#include <QSystemSemaphore>
#include <QDir>
#include <QDateTime>
#include <QThread>
#include <QMessageBox>
#include <QProcess>
#include <QTimer>
#include <QDebug>
#include <QAbstractButton>
#include <Windows.h>
#include <DbgHelp.h>
#include <QApplication>


QMessageBox *msgbPtr;//在子线程内创建的messageBox会有问题，所以只能在主线程创建并传递过过来
void initMessageBox(){
    msgbPtr = new QMessageBox();
    msgbPtr->setWindowFlag(Qt::WindowStaysOnTopHint);
    msgbPtr->setIcon(QMessageBox::Critical);
    msgbPtr->setWindowTitle("Software crash");
    msgbPtr->setText("Software crash unknown cause crash, the program will reboot in 60 seconds");
    msgbPtr->setStyleSheet("QMessageBox { background-color:#1A1A1A; color:#1A1A1A; }");
    QPushButton *rebootButton;
    QPushButton *rejectButton ;

    rebootButton = msgbPtr->addButton("restart", QMessageBox::AcceptRole);
    rejectButton = msgbPtr->addButton("exit", QMessageBox::RejectRole);

    QObject::connect(msgbPtr, &QMessageBox::buttonClicked, [=](QAbstractButton *button){
        if(msgbPtr->clickedButton() == (QAbstractButton*)rebootButton)
        {
            qDebug() << "start application:" << QProcess::startDetached(qApp->applicationFilePath(), QStringList());//重启
            //            qApp->quit();
        }

        msgbPtr->close();
    });
}

//异常捕获回调函数
long __stdcall   errCallback(_EXCEPTION_POINTERS*  pException)
{
    /******这里最好还是加个防止反复进入该回调函数的机制。防止在倒计时的过程中，程序那边又再次触发异常。*********/
    // 信号量的意义，把操作共享内存的代码锁住。因为有可能同时启动, 防止并发
    QSystemSemaphore sema("my exception", 1, QSystemSemaphore::Open);
    sema.acquire();

    QDir dir;
    dir.mkdir("../dumps");
    dir.cd("../dumps");

    /*
          ***保存数据代码***
        */

    QString fileName = dir.path() + "/" +
            QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss.zzz") + ".dmp";

    LPCWSTR pFileName = (LPCWSTR)fileName.unicode();

    //创建 Dump 文件
    HANDLE hDumpFile = CreateFile(pFileName,
                                  GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    qDebug() << "create dumpFile:" << hDumpFile << INVALID_HANDLE_VALUE;
    if(hDumpFile != INVALID_HANDLE_VALUE)
    {
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }

    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number((uint)record->ExceptionAddress,16)),errMod;


    qDebug() << "main thread:" << qApp->thread() << QThread::currentThread();

    int lastTime = 60;
    QTimer *timer = new QTimer();
    timer->setInterval(1000);
    QObject::connect(timer, &QTimer::timeout, [&lastTime, &errCode](){
        lastTime--;

        if(lastTime < 0)
        {
            //            qApp->quit();
            qDebug() << "start application:" << QProcess::startDetached(qApp->applicationFilePath(), QStringList());//重启
            msgbPtr->close();
            return;
        }

        QString info = QString("Software crash unknown cause crash, the program will restart in %1s seconds\r\n%2").arg(lastTime).arg(errCode);
        msgbPtr->setText(info);
    });
    timer->start();

    msgbPtr->exec();


    return EXCEPTION_EXECUTE_HANDLER;
}





#endif // CRASHPROCESS_H
