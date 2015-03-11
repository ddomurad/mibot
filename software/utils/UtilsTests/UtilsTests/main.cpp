#include <QCoreApplication>
#include <cstdio>

#include <mibLogger.h>
#include <mibStandardLoggerBuilder.h>
#include <mibUnitTest.h>

#include "TestHeader.h"

void printList(QList<QPair<QString, QString> > list)
{
    for( QPair<QString, QString> p : list)
    {
        qDebug() << p.first << p.second;
    }
}

int main()
{   
    mibot::UnitTestManager::RegTest("MyTest",new MyTest());
    QList<QPair<QString, QString> > list = mibot::UnitTestManager::Flush();
    printList(list);
}
