#include "include.hpp"
#include "titleBarWindow.hpp"

START_SCOPE(titleBarWindow)

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->parentWindowTag = parentWindowTag;

    HWND parentWindowHandle = {};
    renderer::getWindowHandle(renderer, 0, &parentWindowHandle);

    HWND windowHandle = {};
    createWindowClass(L"tileBarWindowClass", windowCallback);
    createChildWindow(L"tileBarWindowClass", parentWindowHandle, state, &windowHandle);

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

    uint foregroundEntityTag = {};
    renderer::createEntity(renderer, &foregroundEntityTag);
    renderer::setEntityWindow(renderer, foregroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, foregroundEntityTag, 0.98f);
    renderer::setEntityColor(renderer, foregroundEntityTag, 1);
    state->foregroundEntityTag = foregroundEntityTag;

    uint backgroundEntityTag = {};
    renderer::createEntity(renderer, &backgroundEntityTag);
    renderer::setEntityWindow(renderer, backgroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, backgroundEntityTag, 0.98f);
    renderer::setEntityColor(renderer, backgroundEntityTag, 0);
    state->backgroundEntityTag = backgroundEntityTag;

    SendMessage(windowHandle, CWM_DRAW, 0, 0);
}
void draw(State* state, HWND windowHandle)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint windowTag = state->windowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);

    uint backgroundEntityTag = state->backgroundEntityTag;
    renderer::addQuad(renderer, &window, backgroundEntityTag);

    WCHAR string[] = L"Untitled Session";
    float stringWidth = {};
    renderer::getStringWidth(renderer, (WCHAR*)string, 16, &stringWidth);

    float stringCenterX = stringWidth / 2.0f;
    float windowCenterX = window.width / 2.0f;
    float difference = windowCenterX - stringCenterX;

    uint foregroundEntityTag = state->foregroundEntityTag;
    renderer::addString(renderer, (WCHAR*)string, 16, window.x + difference, window.y, foregroundEntityTag);
}
LRESULT CALLBACK windowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    State* state = (State*)GetProp(windowHandle, L"state");
    switch(message)
    {
        case CWM_DRAW:
        {
            draw(state, windowHandle);
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
