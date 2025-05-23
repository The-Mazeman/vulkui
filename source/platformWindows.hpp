#pragma once
#include "dataTypes.hpp"
#include "defines.hpp"

struct ChildList
{
    uint indices[3];
    uint count;
};
struct Window
{
    float x;
    float y;
    float width;
    float height;
};
struct WindowState
{
    Window window;
    ChildList childList;
};

void assert(bool result);
void createThread(LPTHREAD_START_ROUTINE startRoutine, LPVOID parameter);
void createThread(LPTHREAD_START_ROUTINE startRoutine, LPVOID parameter, HANDLE* threadHandle);
void createEvent(HANDLE* handle);
void createMutex(HANDLE* handle);
void createSemaphore(HANDLE* handle);

void getProcessHeap(HANDLE* handle);
void allocateMemory(uint64 size, void** memory);
void freeMemory(void* memory);
void allocateBigMemory(uint64 size, char** memory);
void freeBigMemory(void* memory);

void openFileHandle(WCHAR* filePath, DWORD flag, HANDLE* handle);
void closeFileHandle(HANDLE fileHandle);
void getFileSize(HANDLE fileHandle, uint64* fileSize);
void readFromFile(HANDLE fileHandle, uint64 memorySize, void* memory);
void writeToFile(HANDLE fileHandle, void* memory, uint64 memorySize);
void loadFile(WCHAR* filePath, void** memory, uint64* size);

void getModuleHandle(HINSTANCE* instance);
void createWindowClass(const WCHAR* className, WNDPROC callbackFunction);
void createWindow(const WCHAR* className, void* lParam, HWND* window);
void createOwnedWindow(const WCHAR* className, HWND parentHandle, void* lParam, HWND* window);
void createChildWindow(const WCHAR* className, HWND parentHandle, void* lParam, HWND* window);
LRESULT CALLBACK defaultWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

void setState(HWND window, LPARAM lParam);
void placeWindow(HWND window, int x, int y, int width, int height);
void placeWindow(HWND window, RECT* boundingBox);
void getRect(RECT* rectangle, int* left, int* top, int* right, int* bottom);
void getWindowRectangle(HWND windowHandle, int* left, int* top, int* right, int* bottom);
void getWindowDimension(HWND windowHandle, int* width, int* height);
void getWindowDimension(HWND windowHandle, Window** window);
void getWindowPosition(HWND windowHandle, int* x, int* y);
void setWindowSize(HWND windowHandle, int width, int height);

void mapToParent(HWND window, int* x, int* y);
void startMessageLoop(void);
void getFileCount(WPARAM wParam, uint* count);
void getFilePath(WPARAM wParam, WCHAR* filePath, uint* characterCount, uint index);
void setEventArray(HANDLE* eventArray, uint eventCount);
void waitForSemaphore(HANDLE semaphore);
void checkCompletion(HANDLE event, uint totalCount);

void sizeWindow(HWND windowHandle, float x, float y, float width, float height);
void moveWindow(HWND windowHandle, float x, float y, float width, float height);
