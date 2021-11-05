#include "renderer/Window.h"

int main(void)
{
    // Initialize the window
    Window window {};
    window.Initialize();

    // Run the main while loop
    window.RunWindowLoop();

    // When the window get's closed the window get's destroyed.
    window.Shutdown();

    return 0;
}
