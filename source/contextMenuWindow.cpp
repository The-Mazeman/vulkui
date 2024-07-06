#include "include.hpp"
#include "contextMenuWindow.hpp"

START_SCOPE(contextMenuWindow)

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void create(void* renderer, String* menuOptions, uint optionCount, uint parentWindowTag, uint* selfWindowTag)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->parentWindowTag = parentWindowTag;
    state->visibility = 0;
    state->menuOptions = menuOptions;
    state->optionCount = optionCount;

    HWND parentWindowHandle = {};
    renderer::getWindowHandle(renderer, 0, &parentWindowHandle);

    HWND windowHandle = {};
    createWindowClass(L"contextMenuWindowClass", windowCallback);
    createChildWindow(L"contextMenuWindowClass", parentWindowHandle, state, &windowHandle);

    uint windowTag = {};
    renderer::createWindowEntity(renderer, &windowTag);
    renderer::setWindowHandle(renderer, windowTag, windowHandle);
    state->windowTag = windowTag;
    *selfWindowTag = windowTag;

    uint foregroundEntityTag = {};
    renderer::createEntity(renderer, &foregroundEntityTag);
    renderer::setEntityWindow(renderer, foregroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, foregroundEntityTag, 0.95f);
    renderer::setEntityColor(renderer, foregroundEntityTag, 1);
    state->foregroundEntityTag = foregroundEntityTag;

    uint backgroundEntityTag = {};
    renderer::createEntity(renderer, &backgroundEntityTag);
    renderer::setEntityWindow(renderer, backgroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, backgroundEntityTag, 0.95f);
    renderer::setEntityColor(renderer, backgroundEntityTag, 0);
    state->backgroundEntityTag = backgroundEntityTag;
}
void draw(State* state, HWND windowHandle, WPARAM wParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint windowTag = state->windowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);

    uint backgroundEntityTag = state->backgroundEntityTag;
    renderer::addQuad(renderer, &window, backgroundEntityTag);

    uint foregroundEntityTag = state->foregroundEntityTag;
    renderer::addBorder(renderer, &window, 1.0f, foregroundEntityTag);

    uint optionCount = state->optionCount;
    for(uint i = 0; i != optionCount; ++i)
    {
        String* menuOptions = state->menuOptions;
        WCHAR* string = menuOptions[i].string;
        uint length = menuOptions[i].length;
        float x = window.x + 4;
        float y = window.y + ((float)i * 16.0f);
        renderer::addString(renderer, string, length, x, y, foregroundEntityTag);
        Window line = {x, y + 16.0f, window.width - 8.0f, 1.0f};
        renderer::addQuad(renderer, &line, foregroundEntityTag);
    }
}
void show(State* state, HWND windowHandle, WPARAM wParam)
{
    Window* window = (Window*)wParam;

    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint parentWindowTag = state->parentWindowTag;
    Window parent = {};
    renderer::getWindowDimension(renderer, parentWindowTag, &parent);

    float windowOffsetX = window->x;
    float windowOffsetY = window->y;

    window->x += parent.x;
    window->y += parent.y;

    uint windowTag = state->windowTag;
    renderer::setWindowRelativePosition(renderer, windowTag, windowOffsetX, windowOffsetY);
    renderer::setWindowClipPlanes(renderer, windowTag, window);
    renderer::setWindowDimension(renderer, windowTag, window);
    renderer::getEntitiesUnderArea(renderer, windowTag, window);

    SendMessage(windowHandle, CWM_DRAW, (WPARAM)window, 0);
}
void hide(State* state, HWND windowHandle)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint windowTag = state->windowTag;
    renderer::drawOccludedEntites(renderer, windowTag);
}
void handleMouseLeftClick(State* state, HWND windowHandle, LPARAM lParam)
{
    int x = GET_X_LPARAM(lParam);
    int y = GET_Y_LPARAM(lParam);

    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint windowTag = state->windowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);

    x -= (int)window.x;
    y -= (int)window.y;

    uint optionIndex = (uint)(y / 16);
    uint parentWindowTag = state->parentWindowTag;
    renderer::sendMessage(renderer, parentWindowTag, CWM_CONTEXTMENU, (void*)&optionIndex, 0); 
    hide(state, windowHandle);
}
LRESULT CALLBACK windowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    State* state = (State*)GetProp(windowHandle, L"state");
    switch(message)
    {
        case CWM_DRAW:
        {
            draw(state, windowHandle, wParam);
            break;
        }
        case CWM_SHOW:
        {
            show(state, windowHandle, wParam);
            break;
        }
        case CWM_HIDE:
        {
            hide(state, windowHandle);
            break;
        }
        case CWM_SCROLLUPDATE:
        {
            //handleScrollUpdate(state, windowHandle, wParam);
            break;
        }
        case CWM_MOUSELEFTCLICKDOWN:
        {
            handleMouseLeftClick(state, windowHandle, lParam);
            break;
        }
        case WM_MOUSEMOVE:
        {
            //handleMouseMove(state);
            break;
        }
    }
    return defaultWindowCallback(windowHandle, message, wParam, lParam);
}

END_SCOPE
