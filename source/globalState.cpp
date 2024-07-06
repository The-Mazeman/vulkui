#include "include.hpp"
#include "globalState.hpp"

START_SCOPE(globalState)

void get(HWND windowHandle, GlobalState** globalState)
{
    HWND parentHandle = GetAncestor(windowHandle, GA_PARENT);
    *globalState = (GlobalState*)GetProp(parentHandle, L"globalState");
}
END_SCOPE
