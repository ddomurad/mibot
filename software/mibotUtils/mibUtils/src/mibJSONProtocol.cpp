#include "inc/mibJSONProtocol.h"

using namespace mibot;

JSONProtocol::JSONProtocol() :
    last_bracket(0),
    _max_length(0)
{}

JSONProtocol::~JSONProtocol()
{}

void JSONProtocol::SetMaxJsonObjectLength(int length)
{
    _max_length = length;
}

void JSONProtocol::PushData(QByteArray &arr)
{
    _arr.append(arr);
}

bool JSONProtocol::ContainsValidJSONObject()
{
    if(IsDataCorupted()) return false;
    if(_arr.length() == 0) return false;

    int tmp_bracket_cnt = 1;
    bool tmp_is_quote = false;

    for(int i=1; i<_arr.length();i++)
    {
        if(_arr.at(i) == '\"' && tmp_is_quote == false)
        {
            tmp_is_quote = true;
            continue;
        }

        if(_arr.at(i) == '\"' && tmp_is_quote == true)
        {
            if(_arr.at(i -1) == '\\') continue;
            tmp_is_quote = false;
            continue;
        }
        if(!tmp_is_quote)
        {
            if(_arr.at(i) == '{')
                tmp_bracket_cnt++;

            if(_arr.at(i) == '}')
                tmp_bracket_cnt--;
        }

        if(tmp_bracket_cnt == 0)
        {
            i++;
            last_bracket = i;
            return true;
        }
    }

    return false;
}

bool JSONProtocol::IsDataCorupted()
{
    if(_arr.length() == 0)
        return false;

    if(_max_length > 0)
        if(_arr.length() >= _max_length)
            return true;

    if(_arr.at(0) != '{')
        return true;

    return false;
}

QJsonObject JSONProtocol::GetPendingObject()
{
    if(IsDataCorupted()) return QJsonObject();
    if(last_bracket == 0) return QJsonObject();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(_arr.left(last_bracket), &error);
    _arr = _arr.right( _arr.length() - last_bracket );
    last_bracket = 0;
    return doc.object();
}

void JSONProtocol::RemoveCoruptedData()
{
    if(_max_length > 0)
    {
        if(_arr.length() >= _max_length)
        {
            _arr.clear();
            return;
        }
    }

    for(int i=0;i<_arr.length(); i++)
    {
        if(_arr.at(i) == '{')
        {
            _arr = _arr.right( _arr.length() - i );
            return;
        }
    }

    _arr.clear();
}
