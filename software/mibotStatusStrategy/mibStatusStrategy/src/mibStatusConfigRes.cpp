#include "inc/mibStatusConfigRes.h"

using namespace mibot;

StatusConfigRes::StatusConfigRes() :
    AbstractResource("status_strategy_cfg")
{
    AddField("alias",QVariant::String);
    AddField("use_mcp3008",QVariant::Bool);
    AddField("mcp3008_base",QVariant::Int);
    AddField("mcp3008_spi_channel",QVariant::Int);
    AddField("read_engines_accu",QVariant::Bool);
    AddField("engines_accu_mcp3008_channel",QVariant::Int);
    AddField("engines_accu_v_scale",QVariant::Double);
    AddField("read_cpu_temp",QVariant::Bool);
    AddField("cpu_temp_scale",QVariant::Double);
    AddField("values_update_ratio",QVariant::Int);
    AddField("internal_delay_value",QVariant::Int);
}

StatusConfigRes::~StatusConfigRes()
{}

QString StatusConfigRes::Alias()
{
    return Get("alias")->toString();
}

bool StatusConfigRes::UseMcp3008()
{
    return Get("use_mcp3008")->toBool();
}

int StatusConfigRes::Mcp3008Base()
{
    return Get("mcp3008_base")->toInt();
}

int StatusConfigRes::Mcp3008SpiChannel()
{
    return Get("mcp3008_spi_channel")->toInt();
}

bool StatusConfigRes::ReadEnginesAccu()
{
    return Get("read_engines_accu")->toBool();
}

int StatusConfigRes::EnginesAccuMcp3008Channel()
{
    return Get("engines_accu_mcp3008_channel")->toInt();
}

double StatusConfigRes::EnginesAccuVScale()
{
    return Get("engines_accu_v_scale")->toDouble();
}

bool StatusConfigRes::ReadCpuTemp()
{
    return Get("read_cpu_temp")->toBool();
}

double StatusConfigRes::CpuTempScale()
{
    return Get("cpu_temp_scale")->toDouble();
}

int StatusConfigRes::UpdateRatio()
{
    return Get("values_update_ratio")->toInt();
}

int StatusConfigRes::InternalDelay()
{
    return Get("internal_delay_value")->toInt();
}

