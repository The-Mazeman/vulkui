#include "include.hpp"
#include "mainWindow.hpp"

START_SCOPE(mainWindow)

LRESULT CALLBACK windowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void setAttribute(HWND window)
{
    SetWindowTheme(window, L"", L"");
    DragAcceptFiles(window, 1);
}
void create(Window* window)
{
    State* state = {};
    allocateMemory(sizeof(State), (void**)&state);
    state->focusedWindowTag = 0;
    state->globalState.offsetX = 0;
    state->globalState.offsetY = 0;
    state->globalState.trackCount = 0;
    state->globalState.framesPerPixel = 512;
    state->globalState.sampleRate = 48000;

    HWND windowHandle = {};
    createWindowClass(L"mainWindowClass", windowCallback);
    createWindow(L"mainWindowClass", state, &windowHandle);
    SetProp(windowHandle, L"globalState", &state->globalState);

    setAttribute(windowHandle);
    placeWindow(windowHandle, 0, 0, (int)window->width, (int)window->height);

    HINSTANCE windowInstance = {};
    getModuleHandle(&windowInstance);

    void* renderer = {};
    renderer::create(windowHandle, windowInstance, &renderer);
    SetProp(windowHandle, L"renderer", renderer);

    uint windowTag = {};
    renderer::createWindowEntity(renderer, &windowTag);
    renderer::setWindowHandle(renderer, windowTag, windowHandle);
    renderer::setWindowClipPlanes(renderer, windowTag, window);
    renderer::setWindowDimension(renderer, windowTag, window);
    renderer::setWindowRelativePosition(renderer, windowTag, 0.0f, 0.0f);

    uint backgroundEntityTag = {};
    renderer::createEntity(renderer, &backgroundEntityTag);
    renderer::setEntityWindow(renderer, backgroundEntityTag, windowTag);
    renderer::setEntityDepth(renderer, backgroundEntityTag, 0.99f);
    renderer::setEntityColor(renderer, backgroundEntityTag, 1);
    state->backgroundEntityTag = backgroundEntityTag;

    Window titleBar = {1.0f, 1.0f, window->width - 2.0f, 16.0f};
    uint titleBarWindowTag = {};
    titleBarWindow::create(renderer, &titleBar, windowTag, &titleBarWindowTag);
    state->titleBarWindowTag = titleBarWindowTag;

    Window ruler = {130.0f, 18.0f, window->width - 131.0f, 16.0f};
    uint rulerWindowTag = {};
    rulerWindow::create(renderer, &ruler, windowTag, &rulerWindowTag);
    state->rulerWindowTag = rulerWindowTag;

    Window trackHeaderArea = {1.0f, 35.0f, 128.0f, window->height - 36.0f};
    uint trackHeaderAreaWindowTag = {};
    trackHeaderAreaWindow::create(renderer, &trackHeaderArea, windowTag, &trackHeaderAreaWindowTag);
    state->trackHeaderAreaWindowTag = trackHeaderAreaWindowTag;

    Window clipAreaWindow = {130.0f, 35.0f, window->width - 131.0f, window->height - 36.0f};
    uint clipAreaWindowTag = {};
    clipAreaWindow::create(renderer, &clipAreaWindow, windowTag, &clipAreaWindowTag);
    state->clipAreaWindowTag = clipAreaWindowTag;

    SendMessage(windowHandle, CWM_DRAW, (WPARAM)window, 0);
    renderer::draw(renderer);
}
LRESULT handleHitTesting(HWND window, LPARAM lParam)
{
	int cursorX = GET_X_LPARAM(lParam);
	int cursorY = GET_Y_LPARAM(lParam);

	int a, b, c, d;
	getWindowRectangle(window, &a, &b, &c, &d);

	if(cursorY < b + 16)
	{
		return HTCAPTION;
	}
	if(cursorX < a + 8)
	{
		return HTLEFT;
	}
	if(cursorX > c - 8)
	{
		return HTRIGHT;
	}
	if(cursorY > d - 8)
	{
		return HTBOTTOM;
	}
	return HTCLIENT;
}
void handleMouseMove(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    uint x = (uint)GET_X_LPARAM(lParam);
    uint y = (uint)GET_Y_LPARAM(lParam);
    void* renderer = GetProp(windowHandle, L"renderer");

    uint currentWindowTag = {};
    renderer::getEntityOnScreen(renderer, x, y, &currentWindowTag);
    uint focusedWindowTag = state->focusedWindowTag;
    if(currentWindowTag != focusedWindowTag)
    {
       renderer::sendMessage(renderer, focusedWindowTag, CWM_MOUSEEXIT, 0, 0);
       renderer::sendMessage(renderer, currentWindowTag, CWM_MOUSEENTER, 0, 0);
       state->focusedWindowTag = currentWindowTag;
    }
    renderer::sendMessage(renderer, currentWindowTag, CWM_MOUSEHOVER, (void*)wParam, (void*)lParam);
    renderer::draw(renderer);
}
void draw(State* state, HWND windowHandle, WPARAM wParam)
{
    void* renderer = GetProp(windowHandle, L"renderer");

    Window* update = (Window*)wParam;

    uint backgroundEntityTag = state->backgroundEntityTag;
    renderer::addQuad(renderer, update, backgroundEntityTag);
}
void handleVerticalScroll(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = GetProp(windowHandle, L"renderer");
    uint focusedWindowTag = state->focusedWindowTag;     
    renderer::sendMessage(renderer, focusedWindowTag, CWM_MOUSESCROLLVERTICAL, (void*)wParam, (void*)lParam);
    renderer::draw(renderer);
}
void handleHorizontalScroll(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = GetProp(windowHandle, L"renderer");
    uint focusedWindowTag = state->focusedWindowTag;     
    renderer::sendMessage(renderer, focusedWindowTag, CWM_MOUSESCROLLHORIZONTAL, (void*)wParam, (void*)lParam);
    renderer::draw(renderer);
}
void handleMouseLeftClickUp(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = GetProp(windowHandle, L"renderer");
    uint focusedWindowTag = state->focusedWindowTag;     
    renderer::sendMessage(renderer, focusedWindowTag, CWM_MOUSELEFTCLICKUP, (void*)wParam, (void*)lParam);
    renderer::draw(renderer);
}
void handleMouseLeftClickDown(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = GetProp(windowHandle, L"renderer");
    uint focusedWindowTag = state->focusedWindowTag;     
    renderer::sendMessage(renderer, focusedWindowTag, CWM_MOUSELEFTCLICKDOWN, (void*)wParam, (void*)lParam);
    renderer::draw(renderer);
}
void handleMouseRightClickDown(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = GetProp(windowHandle, L"renderer");
    uint focusedWindowTag = state->focusedWindowTag;     
    renderer::sendMessage(renderer, focusedWindowTag, CWM_MOUSERIGHTCLICKDOWN, (void*)wParam, (void*)lParam);
    renderer::draw(renderer);
}
void scrollYAxis(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    int offsetY = (int)state->globalState.offsetY;

    offsetY -= wheelDelta;
    if(offsetY < 0)
    {
        return;
    }

    state->globalState.offsetY = (uint)offsetY;

    void* renderer = GetProp(windowHandle, L"renderer");
    Window window = {};

    uint trackHeaderAreaWindowTag = state->trackHeaderAreaWindowTag;
    renderer::sendMessage(renderer, trackHeaderAreaWindowTag, CWM_SCROLLYAXIS, (void*)(uint64)wheelDelta, 0);

    uint clipAreaWindowTag = state->clipAreaWindowTag;
    //renderer::sendMessage(renderer, clipAreaWindowTag, CWM_SCROLLYAXIS, (void*)(uint64)wheelDelta, 0);
}
void scrollXAxis(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    wheelDelta *= -1;
    int offsetX = (int)state->globalState.offsetX;
    offsetX -= wheelDelta;
    if(offsetX < 0)
    {
        return;
    }

    state->globalState.offsetX = (uint)offsetX;

    void* renderer = GetProp(windowHandle, L"renderer");
    Window window = {};

    uint rulerWindowTag = state->rulerWindowTag;
    renderer::sendMessage(renderer, rulerWindowTag, CWM_SCROLLXAXIS, (void*)(uint64)wheelDelta, 0);

    uint clipAreaWindowTag = state->clipAreaWindowTag;
    renderer::sendMessage(renderer, clipAreaWindowTag, CWM_SCROLLXAXIS, (void*)(uint64)wheelDelta, 0);
}
void checkExtension(String* filePath, String* extension)
{
	WCHAR* pathString = filePath->string;
	uint64 characterCount = filePath->length;

	pathString += characterCount - 1;
	const WCHAR* dot = L".";
	uint extensionCharacterCount = {};

	for(uint64 i = 0; i != characterCount; --i)
	{
		if(*pathString == *dot)
		{
			break;
		}
		--pathString;
		++extensionCharacterCount;
	}

	++pathString;
	WCHAR* extensionString = extension->string;

	for(uint i = 0; i != extensionCharacterCount; ++i)
	{
		if(i > extension->length)
		{
			break;
		}
		if(pathString[i] != extensionString[i])
		{
			filePath->length = 0;
			break;
		}
	}
}
void getFilePathArray(WPARAM wParam, String** filePathArray, uint* fileCount)
{
	uint filePathCount;
	getFileCount(wParam, &filePathCount);

	String* filePath;
	allocateMemory(sizeof(String) * filePathCount, (void**)&filePath);
	*filePathArray = filePath;
	const WCHAR* extensionString = L"wav";

	String extension = {};
	extension.string = (WCHAR*)extensionString;
	extension.length = 3;
	for(uint i = 0; i != filePathCount; ++i)
	{
		WCHAR* pathString = {};
		allocateMemory(sizeof(WCHAR) * 256, (void**)&pathString);

        uint characterCount = {};
		getFilePath(wParam, pathString, &characterCount, i);

		filePath->string = pathString;
        filePath->length = characterCount; 

		checkExtension(filePath, &extension);
		if(filePath->length == 0)
		{
            freeMemory(pathString);
			continue;
		}

		++filePath;
		++(*fileCount);
	}
}
void createWaveformData(float* sampleChunk, uint waveformFrameCount, uint mipLevelCount, sint8* waveformData)
{
    for(uint i = 0; i != waveformFrameCount; ++i)
    {
        float minSample = {};
        float maxSample = {};
        for(uint j = 0; j != 16; ++j)
        {
            maxSample = max(sampleChunk[(i * 16) + j], maxSample);
            minSample = min(sampleChunk[(i * 16) + j], minSample);
        }
        minSample *= 127.0f;
        maxSample *= 127.0f;
        waveformData[i * 2] = (sint8)minSample;
        waveformData[(i * 2) + 1] = (sint8)maxSample;
    }
    uint mipLevelFrameCount = waveformFrameCount / 2;
    sint8* inputSamples = waveformData;
    sint8* outputSamples = waveformData + waveformFrameCount * 2;
    for(uint i = 0; i != mipLevelCount; ++i)
    {
        for(uint j = 0; j != mipLevelFrameCount; ++j)
        {
            outputSamples[2 * j] = 0;
            outputSamples[(2 * j) + 1] = 0;
            for(uint k = 0; k != 4; ++k)
            {
                outputSamples[2 * j] = min(inputSamples[(4 * j) + k], outputSamples[2 * j]);
                outputSamples[(2 * j) + 1] = max(inputSamples[(4 * j) + k], outputSamples[(2 * j) + 1]);
            }
        }
        inputSamples = outputSamples;
        outputSamples = outputSamples + (mipLevelFrameCount * 2);
        mipLevelFrameCount /= 2;
    }
}
void calculateMouseInput(State* state, void* renderer, POINT* mouse, uint* trackNumber)
{
    int mouseY = mouse->y;

    uint trackHeaderAreaWindowTag = state->trackHeaderAreaWindowTag;
    Window window = {};
    renderer::getWindowDimension(renderer, trackHeaderAreaWindowTag, &window);
    float relativeY = (float)mouseY - window.y;
    mouse->x -= (int)window.x;

    GlobalState* globalState = &state->globalState;
    uint trackCount = globalState->trackCount;
    float clipBottom = {};
    uint trackUnderMouse = {};
    for(uint i = 0; i != trackCount; ++i)
    {
        uint trackHeaderWindowTag = globalState->trackHeaderWindowTags[i];
        Window trackHeaderWindow = {};
        renderer::getWindowDimension(renderer, trackHeaderWindowTag, &trackHeaderWindow);
        clipBottom += trackHeaderWindow.height;
        if(relativeY < clipBottom)
        {
            trackUnderMouse = i;
            break;
        }
    }
    if(!trackUnderMouse && trackCount)
    {
        uint trackHeaderWindowTag = globalState->trackHeaderWindowTags[trackCount-1];
        Window trackHeaderWindow = {};
        renderer::getWindowDimension(renderer, trackHeaderWindowTag, &trackHeaderWindow);
        clipBottom = trackHeaderWindow.y + trackHeaderWindow.height;
        if(relativeY > clipBottom)
        {
            trackUnderMouse = trackCount;
        }
    }
    *trackNumber = trackUnderMouse;
}
void handleFileDrop(State* state, HWND windowHandle, WPARAM wParam)
{
	uint fileCount = {};
    String* filePathArray = {};
	getFilePathArray(wParam, &filePathArray, &fileCount);
#if 0

    void* file = {};
    uint64 fileSize = {};
    loadFile(filePathArray[0].string, &file, &fileSize); 

    WavFile wavFile = {};
    parserWav::parse(file, &wavFile);


    uint blockSize = 16;
    uint waveformFrameCount = (uint)wavFile.frameCount / blockSize;
    uint mipLevelCount = (uint)log2((float)waveformFrameCount);

    sint8* waveformData = {};
    uint waveformDataSize = waveformFrameCount * wavFile.header.channelCount * sizeof(sint8) * 2 * 2;
    allocateMemory(waveformDataSize, (void**)&waveformData);
    createWaveformData((float*)wavFile.sampleChunk, (uint)waveformFrameCount, mipLevelCount, waveformData);

    AudioClip audioClip = {};
    //allocateMemory(sizeof(AudioClip), (void**)&audioClip);
    audioClip.filePath = filePathArray[0];
    audioClip.wavFile = wavFile;
    audioClip.waveformFrameCount = waveformFrameCount;
    audioClip.waveformData = waveformData;

#endif
    void* renderer = GetProp(windowHandle, L"renderer");

    POINT mouse;
    GetCursorPos(&mouse);
    uint trackNumber = {};
    calculateMouseInput(state, renderer, &mouse, &trackNumber);

    uint trackCount = state->globalState.trackCount;
    uint trackHeaderAreaWindowTag = state->trackHeaderAreaWindowTag;
    uint newTrackCount = {};
    if(trackNumber + fileCount > trackCount)
    {
        newTrackCount = trackNumber + fileCount - trackCount;

        GlobalState* globalState = &state->globalState;

        float startOffsetY = {};
        float trackHeaderWidth = 128.0f - 2.0f;
        if(trackCount)
        {
            uint lastTrackNumber = trackCount - 1;
            uint lastTrackWindowTag = globalState->trackHeaderWindowTags[lastTrackNumber];

            Window lastTrackHeader = {};
            renderer::getWindowDimension(renderer, lastTrackWindowTag, &lastTrackHeader);

            Window trackHeaderWindowArea = {};
            renderer::getWindowDimension(renderer, trackHeaderAreaWindowTag, &trackHeaderWindowArea);
            trackHeaderWidth = trackHeaderWindowArea.width;

            startOffsetY = lastTrackHeader.y + lastTrackHeader.height - trackHeaderWindowArea.y;
        }
        float defaultTrackHeight = 128.0f - 2.0f;
        for(uint i = 0; i != newTrackCount; ++i)
        {
            float trackHeaderY = startOffsetY + 1.0f + ((float)i * (defaultTrackHeight + 1.0f));
            Window trackHeader = {1.0f, trackHeaderY, trackHeaderWidth, defaultTrackHeight};

            uint trackHeaderWindowTag = {};
            trackHeaderWindow::create(renderer, &trackHeader, trackHeaderAreaWindowTag, &trackHeaderWindowTag);
            globalState->trackHeaderWindowTags[trackCount + i] = trackHeaderWindowTag;
        }
    }
#if 0
    uint trackTuple[2] = {trackNumber, trackCount};
    renderer::sendMessage(renderer, trackHeaderAreaWindowTag, CWM_CREATEAUDIOTRACK, trackTuple, 0);

    uint64 clipTuple[4] = {(uint64)mouse.x, (uint64)trackNumber, (uint64)trackCount, (uint64)&audioClip};
    uint clipAreaWindowTag = state->clipAreaWindowTag;
    renderer::sendMessage(renderer, clipAreaWindowTag, CWM_CREATEAUDIOCLIP, clipTuple, 0);
    
#endif
    renderer::draw(renderer);
    state->globalState.trackCount += newTrackCount;
}
void horizontalZoom(State* state, HWND windowHandle, WPARAM wParam, LPARAM lParam)
{
    void* renderer = GetProp(windowHandle, L"renderer");
    Window window = {};

    uint rulerWindowTag = state->rulerWindowTag;
    renderer::getWindowDimension(renderer, rulerWindowTag, &window);
    int offsetX = (int)state->globalState.offsetX;

    float mouseX = (float)GET_X_LPARAM(lParam);
    RECT mainWindow = {};
    GetWindowRect(windowHandle, &mainWindow);
    mouseX -= (float)mainWindow.left;
    mouseX -= window.x;
    mouseX += (float)offsetX;

    //int mouseY = GET_Y_LPARAM(lParam);

    int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    wheelDelta *= -1;
    float framesPerPixel = (float)state->globalState.framesPerPixel;
    float mouseSamplePosition = framesPerPixel * mouseX;
    float newFramesPerPixel = framesPerPixel + (float)wheelDelta;
    if(newFramesPerPixel < 0.0f)
    {
        return;
    }

    float newMouseX = mouseSamplePosition / newFramesPerPixel;
    float difference = newMouseX - mouseX;

    int newOffsetX = offsetX + (int)difference;
    if(newOffsetX < 0)
    {
        offsetX = 0;
        newOffsetX = 0;
    }

    state->globalState.offsetX = (uint)newOffsetX;
    state->globalState.framesPerPixel = (uint)newFramesPerPixel;

    renderer::sendMessage(renderer, rulerWindowTag, CWM_DRAW, &window, 0);

    uint clipAreaWindowTag = state->clipAreaWindowTag;
    int tuple[4] = {(int)framesPerPixel, (int)newFramesPerPixel, offsetX, newOffsetX};
    renderer::sendMessage(renderer, clipAreaWindowTag, CWM_HORIZONTALZOOM, tuple, 0);

    renderer::getWindowDimension(renderer, clipAreaWindowTag, &window);
    renderer::sendMessage(renderer, clipAreaWindowTag, CWM_DRAW, &window, 0);
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
        case WM_RBUTTONDOWN:
        {
            handleMouseRightClickDown(state, windowHandle, wParam, lParam);
            break;
        }
        case WM_LBUTTONUP:
        {
            handleMouseLeftClickUp(state, windowHandle, wParam, lParam);
            break;
        }
        case WM_LBUTTONDOWN:
        {
            handleMouseLeftClickDown(state, windowHandle, wParam, lParam);
            break;
        }
        case CWM_HORIZONTALZOOM:
        {
            horizontalZoom(state, windowHandle, wParam, lParam);
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
        case WM_MOUSEWHEEL:
        {
            handleVerticalScroll(state, windowHandle, wParam, lParam);
            break;
        }
        case WM_MOUSEHWHEEL:
        {
            handleHorizontalScroll(state, windowHandle, wParam, lParam);
            break;
        }
        case WM_MOUSEMOVE:
        {
            handleMouseMove(state, windowHandle, wParam, lParam);
            break;
        }
        case WM_DROPFILES:
        {
            handleFileDrop(state, windowHandle, wParam);
            break;
        }
        case WM_NCCALCSIZE:
		{
			return 0;
		}
		case WM_NCACTIVATE:
		{
			lParam = -1;
            return 0;
		}
		case WM_NCPAINT:
		case WM_SETICON:
		case WM_SETTEXT:
		case WM_NCUAHDRAWCAPTION:
		case WM_NCUAHDRAWFRAME:
		{
			return 0;
		}
		case WM_NCHITTEST:
		{
			return handleHitTesting(windowHandle, lParam);
		}
    }
    return defaultWindowCallback(windowHandle, message, wParam, lParam);
}
END_SCOPE
