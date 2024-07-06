#include "include.hpp"
#include "clipAreaWindow.hpp"

START_SCOPE(clipAreaWindow)

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->parentWindowTag = parentWindowTag;

    HWND parentWindowHandle = {};
    renderer::getWindowHandle(renderer, 0, &parentWindowHandle);

    HWND windowHandle = {};
    createWindowClass(L"clipAreaWindowClass", windowCallback);
    createChildWindow(L"clipAreaWindowClass", parentWindowHandle, state, &windowHandle);

    Window parent = {};
    renderer::getWindowDimension(renderer, parentWindowTag, &parent);

    float windowOffsetX = window->x;
    float windowOffsetY = window->y;

    window->x += parent.x;
    window->y += parent.y;

    uint windowTag = {};
    renderer::createWindowEntity(renderer, &windowTag);
    renderer::setWindowDimension(renderer, windowTag, window);
    renderer::setWindowRelativePosition(renderer, windowTag, windowOffsetX, windowOffsetY);
    renderer::setWindowHandle(renderer, windowTag, windowHandle);
    renderer::setWindowClipPlanes(renderer, windowTag, window);
    state->windowTag = windowTag;
    *selfWindowTag = windowTag;

    uint foregroundEntityTag = {};
    renderer::createEntity(renderer, &foregroundEntityTag);
    renderer::setEntityWindow(renderer, foregroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, foregroundEntityTag, 0.96f);
    renderer::setEntityColor(renderer, foregroundEntityTag, 5);
    state->foregroundEntityTag = foregroundEntityTag;

    uint backgroundEntityTag = {};
    renderer::createEntity(renderer, &backgroundEntityTag);
    renderer::setEntityWindow(renderer, backgroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, backgroundEntityTag, 0.97f);
    renderer::setEntityColor(renderer, backgroundEntityTag, 0);
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

    float y = window.y;
    float x = window.x;

    float updateX = update->x - window.x;
    uint left = (uint)((updateX + (float)offsetX) / spacing);
    uint right = (uint)((updateX + update->width + (float)offsetX) / spacing);

    for(uint i = left; i != right; ++i)
    {
        float markingOffset = (float)(i + 1) * spacing;
        Window line = {x + markingOffset - (float)offsetX, update->y, 1.0f, update->height};
        renderer::addQuad(renderer, &line, foregroundEntityTag);
    }
    float markingOffset = (float)(right) * spacing;
    Window line = {x + markingOffset - (float)offsetX, update->y, 1.0f, update->height};
    renderer::addQuad(renderer, &line, foregroundEntityTag);
}
void handleMouseVerticalScroll(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint parentWindowTag = state->parentWindowTag;
    int keyState = GET_KEYSTATE_WPARAM(wParam);
    switch(keyState)
    {
        case 0:
        {
            renderer::sendMessage(renderer, parentWindowTag, CWM_SCROLLYAXIS, (void*)wParam, (void*)lParam); 
            break;
        }
        case MK_CONTROL:
        {
            renderer::sendMessage(renderer, parentWindowTag, CWM_HORIZONTALZOOM, (void*)wParam, (void*)lParam); 
            break;
        }
    }
}
void handleMouseHorizontalScroll(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint parentWindowTag = state->parentWindowTag;
    renderer::sendMessage(renderer, parentWindowTag, CWM_SCROLLXAXIS, (void*)wParam, (void*)lParam); 
}
void createAudioClip(State* state, HWND windowHandle, WPARAM wParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    GlobalState* globalState = {};
    globalState::get(windowHandle, &globalState);

    uint windowTag = state->windowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);

    uint64* tuple = (uint64*)wParam;
    uint trackNumber = (uint)tuple[1];
    uint trackCount = (uint)tuple[2];
    AudioClip* audioClip = (AudioClip*)tuple[3];
    uint width = (uint)(audioClip->waveformFrameCount / (globalState->framesPerPixel / 16));
    float clipX = (float)tuple[0];
    float clipY = 0.0f;
    Window audioClipWindow = {clipX, clipY, (float)width, 130.0f};

    uint audioClipWindowTag = {};
    audioClipWindow::create(renderer, &audioClipWindow, audioClip, windowTag, &audioClipWindowTag);
    state->audioClipWindowTag = audioClipWindowTag;
}
void handleHorizontalZoom(State* state, HWND windowHandle, WPARAM wParam)
{
#if 0
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint audioClipWindowTag = state->audioClipWindowTag;
    Window audioClipWindow = {};
    renderer::getWindowDimension(renderer, audioClipWindowTag, &audioClipWindow);

    float relativeX = {};
    float relativeY = {};
    renderer::getWindowRelativePosition(renderer, audioClipWindowTag, &relativeX, &relativeY);

    uint windowTag = state->windowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);

    int* tuple = (int*)wParam;
    int framesPerPixel = tuple[0];
    int offsetX = tuple[2];
    float positionInSamples = relativeX * (float)framesPerPixel;

    int newFramesPerPixel = tuple[1];
    float newX = positionInSamples / (float)newFramesPerPixel;

    int newOffsetX = tuple[3];
    audioClipWindow.x = newX + window.x - (float)newOffsetX;

    relativeX = newX; 
    renderer::setWindowRelativePosition(renderer, audioClipWindowTag, relativeX, relativeY);

    Window intersection = {};
    calculateIntersection(&window, &audioClipWindow, &intersection);
    renderer::setWindowDimension(renderer, audioClipWindowTag, &intersection);
    renderer::setWindowClipPlanes(renderer, audioClipWindowTag, &intersection);

    renderer::sendMessage(renderer, audioClipWindowTag, CWM_DRAW, &intersection, 0);
#endif
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

#if 0
    uint audioClipWindowTag = state->audioClipWindowTag;
    Window audioClipWindow = {};
    renderer::getWindowDimension(renderer, audioClipWindowTag, &audioClipWindow);
    audioClipWindow.x += (float)wheelDelta;

    Window intersection = {};
    calculateIntersection(&window, &audioClipWindow, &intersection);
    renderer::setWindowDimension(renderer, audioClipWindowTag, &intersection);
    renderer::setWindowClipPlanes(renderer, audioClipWindowTag, &intersection);
#endif
}
void scrollYAxis(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
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
        case CWM_CREATEAUDIOTRACK:
        {
            break;
        }
        case CWM_CREATEAUDIOCLIP:
        {
            createAudioClip(state, windowHandle, wParam);
            break;
        }
        case CWM_SCROLLYAXIS:
        {
            scrollYAxis(state, windowHandle, wParam, lParam);
            break;
        }
        case CWM_SCROLLXAXIS:
        {
            scrollXAxis(state, windowHandle, wParam, lParam);
            break;
        }
        case CWM_HORIZONTALZOOM:
        {
            handleHorizontalZoom(state, windowHandle, wParam);
            break;
        }
        case CWM_MOUSESCROLLVERTICAL:
        {
            handleMouseVerticalScroll(state, windowHandle, wParam, lParam);
            break;
        }
        case CWM_MOUSESCROLLHORIZONTAL:
        {
            handleMouseHorizontalScroll(state, windowHandle, wParam, lParam);
            break;
        }
        case CWM_SCROLLUPDATE:
        {
            draw(state, windowHandle, wParam);
            break;
        }
    }
    return defaultWindowCallback(windowHandle, message, wParam, lParam);
}

END_SCOPE
