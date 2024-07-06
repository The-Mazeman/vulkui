#include "include.hpp"
#include "audioClipWindow.hpp"

START_SCOPE(audioClipWindow)

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void create(void* renderer, Window* window, AudioClip* audioClip, uint parentWindowTag, uint* selfWindowTag)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->userIsDragging = 0;
    state->parentWindowTag = parentWindowTag;
    state->audioClip = *audioClip;
    state->mouseRelativeX = 0;

    HWND parentWindowHandle = {};
    renderer::getWindowHandle(renderer, 0, &parentWindowHandle);

    HWND windowHandle = {};
    createWindowClass(L"audioClipWindowClass", windowCallback);
    createChildWindow(L"audioClipWindowClass", parentWindowHandle, state, &windowHandle);
     
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
    renderer::setEntityDepth(renderer, foregroundEntityTag, 0.94f);
    renderer::setEntityColor(renderer, foregroundEntityTag, 1);
    state->foregroundEntityTag = foregroundEntityTag;

    uint backgroundEntityTag = {};
    renderer::createEntity(renderer, &backgroundEntityTag);
    renderer::setEntityWindow(renderer, backgroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, backgroundEntityTag, 0.95f);
    renderer::setEntityColor(renderer, backgroundEntityTag, 5);
    state->backgroundEntityTag = backgroundEntityTag;

    renderer::getEntitiesUnderArea(renderer, windowTag, window);
    SendMessage(windowHandle, CWM_DRAW, (WPARAM)window, 0);
}
void draw(State* state, HWND windowHandle, WPARAM wParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint windowTag = state->windowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);

    GlobalState* globalState = {};
    globalState::get(windowHandle, &globalState);

    Window* update = (Window*)wParam;
    uint backgroundEntityTag = state->backgroundEntityTag;
    renderer::addQuad(renderer, update, backgroundEntityTag);

    uint foregroundEntityTag = state->foregroundEntityTag;
    sint8* waveformData = state->audioClip.waveformData;
    uint framesPerPixel = globalState->framesPerPixel / 16;
    uint mipLevel = (uint)log2((float)framesPerPixel);

    uint waveformFrameCount = (uint)state->audioClip.waveformFrameCount;
    uint mipLevelOffset = {};
    for(uint i = 0; i != mipLevel; ++i)
    {
        mipLevelOffset += (waveformFrameCount / (uint)powf(2.0f, (float)i));
    }
    waveformData += (mipLevelOffset * 2);
#if 0

    uint left = (uint)(window.x - update->x);
    uint right = (uint)(window.x - update->x + update->width);
    for(uint i = left; i != right; ++i)
    {
        float centerY = window.y + (window.height / 2);
        float x = window.x;
        float sampleY = centerY - (float)waveformData[(i * 2) + 1];
        float sampleHeight = (float)abs((int)(waveformData[(i * 2) + 1] - waveformData[i * 2]));
        Window line = {window.x + (float)i, sampleY, 1.0f, sampleHeight};
        renderer::addQuad(renderer, &line, foregroundEntityTag);

    }
#endif
}
void handleMouseLeftClickUp(State* state)
{
    state->userIsDragging = 0;
}
void handleMouseLeftClickDown(State* state, HWND windowHandle, LPARAM lParam)
{
    int mouseX = GET_X_LPARAM(lParam);
    state->userIsDragging = 1;

    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint windowTag = state->windowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);
    mouseX -= (int)window.x;
    state->mouseRelativeX = (uint)mouseX;
}
void handleMouseHover(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    uint userIsDragging = state->userIsDragging; 
    if(userIsDragging)
    {
        void* renderer = {};
        renderer::getRenderer(windowHandle, &renderer);

        int mouseX = GET_X_LPARAM(lParam);
        int mouseRelativeX = (int)state->mouseRelativeX;
        int clipX = mouseX - mouseRelativeX;

        uint windowTag = state->windowTag;
        Window window = {};
        renderer::getWindowDimension(renderer, windowTag, &window);
        int deltaX = clipX - (int)window.x;
        renderer::moveWindow(renderer, windowTag, (float)deltaX, 0.0f);
        int a  = 0;
    }
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
        case CWM_MOUSELEFTCLICKDOWN:
        {
            handleMouseLeftClickDown(state, windowHandle, lParam);
            break;
        }
        case CWM_MOUSELEFTCLICKUP:
        {
            handleMouseLeftClickUp(state);
            break;
        }
        case CWM_MOUSEHOVER:
        {
            handleMouseHover(state, windowHandle, wParam, lParam);
            break;
        }
    }
    return defaultWindowCallback(windowHandle, message, wParam, lParam);
}

END_SCOPE
