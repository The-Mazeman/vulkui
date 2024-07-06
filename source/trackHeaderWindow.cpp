#include "include.hpp"
#include "trackHeaderWindow.hpp"

START_SCOPE(trackHeaderWindow)

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->parentWindowTag = parentWindowTag;

    HWND parentWindowHandle = {};
    renderer::getWindowHandle(renderer, 0, &parentWindowHandle);

    HWND windowHandle = {};
    createWindowClass(L"trackHeaderWindowClass", windowCallback);
    createChildWindow(L"trackHeaderWindowClass", parentWindowHandle, state, &windowHandle);

    Window parent = {};
    renderer::getWindowDimension(renderer, parentWindowTag, &parent);

    float windowOffsetX = window->x;
    float windowOffsetY = window->y;

    window->x += parent.x;
    window->y += parent.y;

    uint windowTag = {};
    renderer::createWindowEntity(renderer, &windowTag);
    renderer::setWindowRelativePosition(renderer, windowTag, windowOffsetX, windowOffsetY);
    renderer::setWindowHandle(renderer, windowTag, windowHandle);
    renderer::setWindowDimension(renderer, windowTag, window);
    renderer::setWindowClipPlanes(renderer, windowTag, window);
    state->windowTag = windowTag;
    *selfWindowTag = windowTag;

    uint foregroundEntityTag = {};
    renderer::createEntity(renderer, &foregroundEntityTag);
    renderer::setEntityWindow(renderer, foregroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, foregroundEntityTag, 0.96f);
    renderer::setEntityColor(renderer, foregroundEntityTag, 0);
    state->foregroundEntityTag = foregroundEntityTag;

    uint backgroundEntityTag = {};
    renderer::createEntity(renderer, &backgroundEntityTag);
    renderer::setEntityWindow(renderer, backgroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, backgroundEntityTag, 0.96f);
    renderer::setEntityColor(renderer, backgroundEntityTag, 1);
    state->backgroundEntityTag = backgroundEntityTag;

    SendMessage(windowHandle, CWM_DRAW, (WPARAM)window, 0);
}
void draw(State* state, HWND windowHandle, WPARAM wParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint windowTag = state->windowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);

    float bottom = window.y + window.height; 
    window.height = 16.0f;
    uint backgroundEntityTag = state->backgroundEntityTag;
    renderer::addQuad(renderer, &window, backgroundEntityTag);

    uint foregroundEntityTag = state->foregroundEntityTag;
    WCHAR* string = (WCHAR*)L"Audio Track";
    float stringWidth = {};
    renderer::getStringWidth(renderer, (WCHAR*)string, 11, &stringWidth);

    float stringCenterX = stringWidth / 2.0f;
    float windowCenterX = window.width / 2.0f;
    float difference = windowCenterX - stringCenterX;
    renderer::addString(renderer, string, 11, window.x + difference, window.y, foregroundEntityTag);

    window.y = bottom - 1.0f;
    window.height = 1.0f;
    renderer::addQuad(renderer, &window, backgroundEntityTag);
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
    }
    return defaultWindowCallback(windowHandle, message, wParam, lParam);
}

END_SCOPE
