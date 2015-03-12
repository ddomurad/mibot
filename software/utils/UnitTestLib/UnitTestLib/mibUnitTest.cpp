#include "mibUnitTest.h"
#include <QFile>

using namespace mibot;

QList<QPair<QString, QString> > UnitTestManager::Flush()
{
    QList<QPair<QString, QString> > out;

    for( QPair<QString, UnitTest*> test : _utests )
    {
        out.append( ExecuteTest(test.first, test.second) );
        delete test.second;
    }


    return out;
}

void UnitTestManager::ResultsPtr(QList<QPair<QString,QString>> list)
{
    for( QPair<QString, QString> p : list)
    {
        qDebug() << p.first << p.second;
    }
}

void UnitTestManager::ResultsToTextFile(QString fileName, QList<QPair<QString,QString>> list)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly) )
    {
        qDebug() << QString("Can't open file '%1' to write.").arg(fileName);
        return;
    }

    for( QPair<QString, QString> p : list)
    {
        file.write( QString("%1: %2\n").arg(p.first, p.second).toLatin1() );
    }

    file.close();
}

void UnitTestManager::RegTest(QString name, UnitTest * test)
{
    _utests.append( QPair<QString, UnitTest*>(name, test) );
}

QList<QPair<QString,QString>> UnitTestManager::ExecuteTest(QString tn, UnitTest *t)
{
    QList<QPair<QString,QString>> out_list;
    QObject * obj = dynamic_cast<QObject*>(t);

    if(obj == nullptr)
    {
        out_list.append( QPair<QString, QString>(tn,"Execution Error - Casting failed") );
        return out_list;
    }

    try
    {
        t->ClassInit();
    }
    catch(QString e)
    {
        out_list.append( QPair<QString, QString>(tn + "::ClassInit","FAILED: " + e) );
        return out_list;
    }
    catch(...)
    {
        out_list.append( QPair<QString, QString>(tn + "::ClassInit","FAILED: Unknown error.") );
        return out_list;
    }

    const QMetaObject* metaObject = obj->metaObject();
    for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
    {
        if (metaObject->method(i).methodType() == QMetaMethod::Slot)
        {
            QString sig = QString::fromLatin1(metaObject->method(i).methodSignature());
            bool isInited = false;
            try
            {
                t->TestInit();
                isInited = true;
            }
            catch(QString e)
            {
                out_list.append( QPair<QString, QString>(tn + "::TestInit",e) );
            }
            catch(...)
            {
                out_list.append( QPair<QString, QString>(tn + "::TestInit","ERROR") );
            }

            if(isInited == false) continue;

            try
            {
                if(metaObject->method(i).invoke(obj) == false)
                {
                    out_list.append( QPair<QString, QString>(tn + "::" + sig,"INVOKE_ERROR") );
                }

                out_list.append( QPair<QString, QString>(tn + "::" + sig,"SUCCESS") );
            }
            catch(QString e)
            {
                out_list.append( QPair<QString, QString>(tn + "::" + sig, e) );
            }
            catch(...)
            {
                out_list.append( QPair<QString, QString>(tn + "::" + sig, "ERROR") );
            }

            try
            {
                t->TestClean();
            }
            catch(QString e)
            {
                out_list.append( QPair<QString, QString>(tn + "::TestClean",e) );
                return out_list;
            }
            catch(...)
            {
                out_list.append( QPair<QString, QString>(tn + "::TestClean","ERROR") );
                return out_list;
            }
        }
    }

    try
    {
        t->ClassClean();
    }
    catch(QString e)
    {
        out_list.append( QPair<QString, QString>(tn + "::ClassClean",e) );
        return out_list;
    }
    catch(...)
    {
        out_list.append( QPair<QString, QString>(tn + "::ClassClean","ERROR") );
        return out_list;
    }

    return out_list;
}

QList<QPair<QString, UnitTest*>> UnitTestManager::_utests = QList<QPair<QString, UnitTest*>>();

_Assert::_Assert(int line):
    _line(line)
{}

void _Assert::ThrowError(QString message)
{
    throw QString( message + " @ line " + QString::number(_line) );
}
