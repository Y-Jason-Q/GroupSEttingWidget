#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <QString>

namespace bw {

#define MAX_DEVICE_COUNT 16

enum ScreenLayout
{
    SINGLE_SCREEN = 0,
    DOUBLE_SCREEN,
    TRIPLE_SCREEN,
    FOUR_SCREEN,
    SIX_SCREEN,
    EIGHT_SCREEN,
    NINE_SCREEN,
};

// 屏幕索引
enum ViewIndex
{
    VIEW_1 = 0,
    VIEW_2,
    VIEW_3,
    VIEW_4,
    VIEW_5,
    VIEW_6,
    VIEW_7,
    VIEW_8,
    VIEW_9,
};

struct FrameInfo
{
    uint8_t *yuvData;
    QString devID;
    int width;
    int height;
    ViewIndex index;  // 窗口索引
    int groupId;
};

}
#endif // COMMON_H
