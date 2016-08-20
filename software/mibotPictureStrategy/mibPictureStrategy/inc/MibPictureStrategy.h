#ifndef MIBPICTURESTRATEGY_H
#define MIBPICTURESTRATEGY_H

#include <QObject>
#include <mibJSONProtocol.h>
#include <mibAbstractSocketStrategy.h>

#include "mibPictureStrategyGlobal.h"

namespace mibot
{
class MIBPICTURESTRATEGYSHARED_EXPORT PictureStrategy : public AbstractSocketStrategy
{
    Q_OBJECT
public:
    explicit PictureStrategy(mibot::Connection *connection);
    ~PictureStrategy();

signals:
private slots:
protected:
    void processNewData(QByteArray arr);
    bool init();

private:
    JSONProtocol   _json_protocol;
    void fixIfJsonIsCorrupted();
    void processCommand(QJsonObject & obj);

    void sendError(QString str);
    void sendPicture(QString path);
    bool takePicture(int w, int h, int q, QString path);
};

}

extern "C" mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection);
#endif // MIBPICTURESTRATEGY_H
