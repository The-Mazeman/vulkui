#include "include.hpp"
#include "rulerWindow.hpp"

START_SCOPE(rulerWindow)

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->parentWindowTag = parentWindowTag;

    HWND parentWindowHandle = {};
    renderer::getWindowHandle(renderer, parentWindowTag, &parentWindowHandle);

    HWND windowHandle = {};
    createWindowClass(L"rulerWindowClass", windowCallback);
    createChildWindow(L"rulerWindowClass", parentWindowHandle, state, &windowHandle);

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
    renderer::setEntityDepth(renderer, foregroundEntityTag, 0.97f);
    renderer::setEntityColor(renderer, foregroundEntityTag, 1);
    state->foregroundEntityTag = foregroundEntityTag;

    uint backgroundEntityTag = {};
    renderer::createEntity(renderer, &backgroundEntityTag);
    renderer::setEntityWindow(renderer, backgroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, backgroundEntityTag, 0.98f);
    renderer::setEntityColor(renderer, backgroundEntityTag, 0);
    state->backgroundEntityTag = backgroundEntityTag;

    uint textEntityTag = {};
    renderer::createEntity(renderer, &textEntityTag);
    renderer::setEntityWindow(renderer, textEntityTag, windowTag);
    renderer::setEntityDepth(renderer, textEntityTag, 0.98f);
    renderer::setEntityColor(renderer, textEntityTag, 1);
    state->textEntityTag = textEntityTag;

    SendMessage(windowHandle, CWM_DRAW, (WPARAM)window, 0);
}
void draw(State* state, HWND windowHandle, WPARAM wParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint windowTag = state->windowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);

    Window* update = (Window*)wParam;
    uint backgroundEntityTag = state->backgroundEntityTag;
    renderer::addQuad(renderer, update, backgroundEntityTag);

    GlobalState* globalState = {};
    globalState::get(windowHandle, &globalState);

    float framesPerPixel = (float)globalState->framesPerPixel;
    float sampleRate = (float)globalState->sampleRate;
    float spacing = sampleRate / framesPerPixel;

    uint offsetX = globalState->offsetX;
    uint foregroundEntityTag = state->foregroundEntityTag;
    uint textEntityTag = state->textEntityTag;

    float y = window.y;
    float x = window.x;

    float updateX = update->x - window.x;
    uint left = (uint)((updateX + (float)offsetX) / spacing);
    uint right = (uint)((updateX + update->width + (float)offsetX) / spacing);

    WCHAR mark[4] = {};
    for(uint i = left; i != right; ++i)
    {
        float markingOffset = (float)(i + 1) * spacing;
        Window line = {x + markingOffset - (float)offsetX, y, 1.0f, 8.0f};
        uint digits = (uint)(log((float)i) / log(10.0f)) + 1;
        wsprintf(mark, L"%u", i);
        renderer::addString(renderer, mark, digits, line.x + 4.0f, line.y, textEntityTag);
        renderer::addQuad(renderer, &line, foregroundEntityTag);
    }
    if(right)
    {
        float markingOffset = (float)right * spacing;
        Window line = {x + markingOffset - (float)offsetX, y, 1.0f, 8.0f};
        right -= 1;
        uint digits = (uint)(log((float)right) / log(10.0f)) + 1;
        wsprintf(mark, L"%u", right);
        renderer::addString(renderer, mark, digits, line.x + 4.0f, line.y, textEntityTag);
        renderer::addQuad(renderer, &line, foregroundEntityTag);
    }
}
void handleMouseHorizontalScroll(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint parentWindowTag = state->parentWindowTag;
    renderer::sendMessage(renderer, parentWindowTag, CWM_SCROLLXAXIS, (void*)wParam, (void*)lParam); 
}
void handleMouseVerticalScroll(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint parentWindowTag = state->parentWindowTag;
    renderer::sendMessage(renderer, parentWindowTag, CWM_HORIZONTALZOOM, (void*)wParam, (void*)lParam); 
}
void scrollXAxis(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);
    uint windowTag = state->windowTag;

    int wheelDelta = (int)wParam;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);
    renderer::scroll(renderer, windowTag, &window, (float)wheelDelta, 0.0f);
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
        case CWM_MOUSESCROLLHORIZONTAL:
        {
            handleMouseHorizontalScroll(state, windowHandle, wParam, lParam);
            break;
        }
        case CWM_MOUSESCROLLVERTICAL:
        {
            handleMouseVerticalScroll(state, windowHandle, wParam, lParam);
            break;
        }
        case CWM_SCROLLXAXIS:
        {
            scrollXAxis(state, windowHandle, wParam, lParam);
            break;
        }
        case CWM_SCROLLUPDATE:
        {
            draw(state, windowHandle, wParam);
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
