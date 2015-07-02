#include "inc/mibStatusSettings.h"

using namespace mibot;

StatusSettings::StatusSettings(QString res) :
    SettingsObject(res,true)
{
    alias               = new StringSettingsValue("alias");

    useMcp3008          = new BoolSettingsValue("use_mcp3008");
    mcp3008Base         = new IntSettingsValue("mcp3008_base");
    mcp3008SpiChannel   = new IntSettingsValue("mcp3008_spi_channel");
    readEnginesAccu     = new BoolSettingsValue("read_engines_accu");
    enginesAccuMcp3008Channel = new IntSettingsValue("engines_accu_mcp3008_channel");
    enginesAccuVScale   = new DoubleSettingsValue("engines_accu_v_scale");
    readCpuTemp         = new BoolSettingsValue("read_cpu_temp");
    cpuTempScale        = new DoubleSettingsValue("cpu_temp_scale");

    readCpuUsage        = new BoolSettingsValue("read_cpu_usage");
    readMemory          = new BoolSettingsValue("read_memory");

    updateRatio         = new IntSettingsValue("values_update_ratio");
    internalDelay       = new IntSettingsValue("internal_delay_value");


    AddValue(alias);
    AddValue(useMcp3008);
    AddValue(mcp3008Base);
    AddValue(mcp3008SpiChannel);
    AddValue(readEnginesAccu);
    AddValue(enginesAccuMcp3008Channel);
    AddValue(enginesAccuVScale);
    AddValue(readCpuTemp);
    AddValue(cpuTempScale);
    AddValue(readCpuUsage);
    AddValue(readMemory);
    AddValue(updateRatio);
    AddValue(internalDelay);
}

StatusSettings::~StatusSettings()
{}
