#include "platform.h"
#include "Pushpak2D/pushpak2D.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        PXB_LOG(SANDBOX, "%s", argv[i]);
    }

    if (!platform_init("PixelByte", WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        return 1;
    }

    initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    struct InputActions inputs = (struct InputActions){false, false, false, false};

    bool didExit = false;
    double lastTime = platform_timeSeconds();
    while (!didExit)
    {
        const double now = platform_timeSeconds();
        const float elapsedTime = (float)(now - lastTime);
        lastTime = now;

        platform_event_t event;
        while (platform_pollEvent(&event))
        {
            switch (event.type)
            {
            case PLATFORM_EVENT_QUIT:
            {
                didExit = true;
                break;
            }
            case PLATFORM_EVENT_RESIZED:
            {
                // resize(event.width, event.height);
                break;
            }
            case PLATFORM_EVENT_KEY_PRESSED:
            {
                switch (event.key)
                {
                    case PLATFORM_KEY_LEFT:
                    case PLATFORM_KEY_A: inputs.left = true; break;
                    case PLATFORM_KEY_RIGHT:
                    case PLATFORM_KEY_D: inputs.right = true; break;
                    case PLATFORM_KEY_ESCAPE:
                    case PLATFORM_KEY_F12:
                    case PLATFORM_KEY_UNKNOWN:
                    default: break;
                }
                break;
            }
            case PLATFORM_EVENT_KEY_RELEASED:
            {
                switch (event.key)
                {
                    case PLATFORM_KEY_LEFT:
                    case PLATFORM_KEY_A: inputs.left = false; break;
                    case PLATFORM_KEY_RIGHT:
                    case PLATFORM_KEY_D: inputs.right = false; break;
                    case PLATFORM_KEY_ESCAPE: didExit = true; break;
                    case PLATFORM_KEY_F12:
                    case PLATFORM_KEY_UNKNOWN:
                    default: break;
                }
                break;
            }
            default: break;
            }
        }

        update(elapsedTime, inputs);
        const pxb_pixelbuffer_t* presentBuffer = render();

        const bool presentResult = platform_present(presentBuffer);
        PXB_ASSERT(presentResult);
    }

    platform_shutdown();
    return 0;
}
