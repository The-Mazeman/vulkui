#include "include.hpp"
#include "trackHeaderAreaWindow.hpp"

START_SCOPE(trackHeaderAreaWindow)

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void create(void* renderer, Window* window, uint parentWindowTag, uint* selfWindowTag)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->parentWindowTag = parentWindowTag;
    state->offsetY = 0;
    state->contextMenuVisible = 0;

    HWND parentWindowHandle = {};
    renderer::getWindowHandle(renderer, 0, &parentWindowHandle);

    HWND windowHandle = {};
    createWindowClass(L"trackHeaderAreaWindowClass", windowCallback);
    createChildWindow(L"trackHeaderAreaWindowClass", parentWindowHandle, state, &windowHandle);

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
    renderer::setWindowClipPlanes(renderer, windowTag, window);
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

    String* menuOptions = {};
    allocateMemory(sizeof(String) * 2, (void**)&menuOptions);
    menuOptions[0] = {(WCHAR*)L"Add Track", 9};
    menuOptions[1] = {(WCHAR*)L"Remove Track", 12};

    uint contextMenuWindowTag = {};
    contextMenuWindow::create(renderer, menuOptions, 2, windowTag, &contextMenuWindowTag);
    state->contextMenuWindowTag = contextMenuWindowTag;

    SendMessage(windowHandle, CWM_DRAW, (WPARAM)window, 0);
}
void toggleContextMenu(State* state, HWND windowHandle, LPARAM lParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint contextMenuWindowTag = state->contextMenuWindowTag;
    uint contextMenuVisible = state->contextMenuVisible;
    if(contextMenuVisible)
    {
        renderer::sendMessage(renderer, contextMenuWindowTag, CWM_HIDE, 0, 0);
        state->contextMenuVisible = 0;
    }
    else
    {
        float x = (float)GET_X_LPARAM(lParam);
        float y = (float)GET_Y_LPARAM(lParam);
        uint windowTag = state->windowTag;
        Window window = {};
        renderer::getWindowDimension(renderer, windowTag, &window);
        Window contextMenu = {x - window.x, y - window.y, 200.0f, 100.0f};
        renderer::sendMessage(renderer, contextMenuWindowTag, CWM_SHOW, &contextMenu, 0);
        state->contextMenuVisible = 1;
    }
}
void handleMouseVerticalScroll(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint parentWindowTag = state->parentWindowTag;
    renderer::sendMessage(renderer, parentWindowTag, CWM_SCROLLYAXIS, (void*)wParam, (void*)lParam); 
}
void draw(State* state, HWND windowHandle, WPARAM wParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    Window* update = (Window*)wParam;

    uint backgroundEntityTag = state->backgroundEntityTag;
    renderer::addQuad(renderer, update, backgroundEntityTag);

}
void createAudioTrack(State* state, HWND windowHandle, uint trackNumber, uint trackCount)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);

    uint windowTag = state->windowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);

    GlobalState* globalState = {};
    globalState::get(windowHandle, &globalState);

    Window previousTrackHeader = {};
    float startOffsetY = {};
    if(trackNumber)
    {
        uint previousTrack = trackNumber - 1;
        uint previousTrackWindowTag = globalState->trackHeaderWindowTags[previousTrack];
        renderer::getWindowDimension(renderer, previousTrackWindowTag, &previousTrackHeader);
        startOffsetY = previousTrackHeader.y + previousTrackHeader.height - window.y;
    }
    for(uint i = 0; i != trackCount; ++i)
    {
        uint trackHeaderWindowTag = {};
        float clipY = startOffsetY + 1.0f + ((float)i * 0.0f);
        Window trackHeader = {1.0f, clipY, window.width - 2.0f, 128.0f};
        trackHeaderWindow::create(renderer, &trackHeader, windowTag, &trackHeaderWindowTag);
        globalState->trackHeaderWindowTags[trackNumber + i] = trackHeaderWindowTag;

    }
}
void handleAddTrackOption(State* state, HWND windowHandle, uint trackCount)
{
    GlobalState* globalState = {};
    globalState::get(windowHandle, &globalState);

    uint trackNumber = globalState->trackCount;
    createAudioTrack(state, windowHandle, trackNumber, trackCount);

    globalState->trackCount += trackCount;
}
void contextMenuCallback(State* state, HWND windowHandle, WPARAM wParam)
{
    uint* menuOption = (uint*)wParam;
    switch(*menuOption)
    {
        case 0:
        {
            handleAddTrackOption(state, windowHandle, 1);
            break;
        }
    }
    state->contextMenuVisible = 0;
}
void handleCreateTrack(State* state, HWND windowHandle, WPARAM wParam)
{
    uint* tuple = (uint*)wParam;
    uint trackNumber = tuple[0];
    uint trackCount = tuple[1];
    createAudioTrack(state, windowHandle, trackNumber, trackCount);
}
void scrollYAxis(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);
    uint windowTag = state->windowTag;

    int wheelDelta = (int)wParam;
    Window window = {};
    renderer::getWindowDimension(renderer, windowTag, &window);
    renderer::scroll(renderer, windowTag, &window, 0.0f, (float)wheelDelta);
}
void handleScrollUpdate(State* state, HWND windowHandle, WPARAM wParam)
{
#if 0
    void* renderer = {};
    renderer::getRenderer(windowHandle, &renderer);
    uint windowTag = state->windowTag;

    Window* update = (Window*)wParam;

    GlobalState* globalState = {};
    globalState::get(windowHandle, &globalState);

    uint firstTrackHeader = state->firstTrackHeader;
    uint firstTrackHeaderWindowTag = globalState->trackHeaderWindowTags[firstTrackHeader];

    Window window = {};
    renderer::getWindowDimension(renderer, firstTrackHeaderWindowTag, &window);
    if(update->x < window.x)
    {
    }
#endif
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
        case CWM_SCROLLYAXIS:
        {
            scrollYAxis(state, windowHandle, wParam, lParam);
            break;
        }
        case CWM_CONTEXTMENU:
        {
            contextMenuCallback(state, windowHandle, wParam);
            break;
        }
        case CWM_CREATEAUDIOTRACK:
        {
            handleCreateTrack(state, windowHandle, wParam);
            break;
        }
        case CWM_MOUSERIGHTCLICKDOWN:
        {
            toggleContextMenu(state, windowHandle, lParam);
            break;
        }
        case CWM_MOUSESCROLLVERTICAL:
        {
            handleMouseVerticalScroll(state, windowHandle, wParam, lParam);
            break;
        }
        case CWM_SCROLLUPDATE:
        {
            handleScrollUpdate(state, windowHandle, wParam);
            break;
        }
    }
    return defaultWindowCallback(windowHandle, message, wParam, lParam);
}

END_SCOPE
