#ifndef STANDARDLOGGERBUILDER_H
#define STANDARDLOGGERBUILDER_H

#include "mibUtilsGlobal.h"
#include "mibLoggerBuilder.h"

namespace mibot
{

class MIBUTILSSHARED_EXPORT StandardLoggerBuilder final : public LoggerBuilder
{
public:
    ~StandardLoggerBuilder();

    // LoggerBuilder interface
public:
    void BuildLogger(QJsonObject &json);

private:
    void BuildSink(QJsonObject &json);
    LoggerFormater *CreateFormater(QJsonObject &json);
};

}

#endif // STANDARDLOGGERBUILDER_H
