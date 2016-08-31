#ifndef JSONPROTOCOL_H
#define JSONPROTOCOL_H

#include <QtCore>

namespace mibot
{

class JSONProtocol
{
public:
    JSONProtocol();
    ~JSONProtocol();

    void SetMaxJsonObjectLength(int length);
    void PushData(QByteArray &arr);
    bool ContainsValidJSONObject();
    bool IsDataCorupted();

    QJsonObject GetPendingObject();

    void RemoveCoruptedData();
    void RemoveAllData();
private:
    QByteArray _arr;
    int last_bracket;
    int _max_length;
};

}

#endif // JSONPROTOCOL_H
