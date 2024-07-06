#include "include.hpp"
#include "main.hpp"

template <typename N>
struct test
{
};
template <uint N>
test(const char (&)[N]) -> test<int>;
int WINAPI wWinMain(_In_ HINSTANCE windowInstance, _In_opt_ HINSTANCE previousInstance, _In_ LPWSTR commandline, _In_ int showState)
{
	NOT_USING(windowInstance);
	NOT_USING(previousInstance);
	NOT_USING(showState);

    Window window = {0.0f, 0.0f, 1280.0f, 720.0f};
    mainWindow::create(&window);
    startMessageLoop();
}
