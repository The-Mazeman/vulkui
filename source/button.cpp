#include "include.hpp"
#include "button.hpp"

START_SCOPE(button)

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->parentWindowTag = parentWindowTag;

    HWND parentWindowHandle = {};
    renderer::getWindowHandle(renderer, parentWindowTag, &parentWindowHandle);

    HWND windowHandle = {};
    createWindowClass(L"buttonWindowClass", windowCallback);
    createChildWindow(L"buttonWindowClass", parentWindowHandle, state, &windowHandle);

    uint windowTag = {};
    renderer::createWindowEntity(renderer, &windowTag);
    renderer::setWindowHandle(renderer, windowTag, windowHandle);
    renderer::setWindowDimension(renderer, windowTag, window);
    state->windowTag = windowTag;
    *selfWindowTag = windowTag;

    uint foregroundEntityTag = {};
    renderer::createEntity(renderer, &foregroundEntityTag);
    renderer::setEntityWindow(renderer, foregroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, foregroundEntityTag, 0.97f);
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

    uint foregroundEntityTag = state->foregroundEntityTag;
    renderer::addBorder(renderer, &window, 1.0f, foregroundEntityTag);

    WCHAR string[] = L"Button";
    renderer::addString(renderer, (WCHAR*)string, 6, window.x, window.y + 1.0f, foregroundEntityTag);

    uint backgroundEntityTag = state->backgroundEntityTag;
    renderer::addQuad(renderer, &window, backgroundEntityTag);

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
