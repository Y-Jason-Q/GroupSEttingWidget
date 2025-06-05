#ifndef WINDOWEFFECT_H
#define WINDOWEFFECT_H

#include <Windows.h>

namespace WindowEffect
{
void moveWindow(HWND hWnd);
void addShadowEffect(HWND hWnd);
void removeShadowEffect(HWND hWnd);
void removeMenuShadowEffect(HWND hWnd);
void addWindowAnimation(HWND hWnd);
};

#endif // WINDOWEFFECT_H
