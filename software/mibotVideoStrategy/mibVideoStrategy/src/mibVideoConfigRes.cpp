#include "inc/mibVideoConfigRes.h"

using namespace mibot;

VideoConfigRes::VideoConfigRes()
    : AbstractResource("video_strategy_cfg")
{
    AddField("alias",QVariant::String);
    AddField("video_device",QVariant::String);
    AddField("fps_prescaler",QVariant::Int);
    AddField("frame_width",QVariant::Int);
    AddField("frame_height",QVariant::Int);
}

VideoConfigRes::~VideoConfigRes()
{}

QString VideoConfigRes::Alias()
{
    return Get("alias")->toString();
}

QString VideoConfigRes::Device()
{
    return Get("video_device")->toString();
}

int VideoConfigRes::FpsPrescaler()
{
    return Get("fps_prescaler")->toInt();
}

int VideoConfigRes::FrameWidth()
{
    return Get("frame_width")->toInt();
}

int VideoConfigRes::FrameHeight()
{
    return Get("frame_height")->toInt();
}

