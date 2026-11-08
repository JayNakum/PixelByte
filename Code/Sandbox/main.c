#include "platform.h"
#include "Pushpak2D/pushpak2D.h"

static bool gDidExit = false;

int main(int argc, char** argv)
{
    for (int i = 0; i < argc; ++i)
    {
        PXB_LOG(SANDBOX, "%s", argv[i]);
    }

    const uint32_t WINDOW_WIDTH = 800;
    const uint32_t WINDOW_HEIGHT = 600;

    if (!platform_init("PixelByte", WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        return 1;
    }

    initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

    struct InputActions inputs = (struct InputActions){false, false, false, false};

    double lastTime = platform_timeSeconds();
    while (!gDidExit)
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
                gDidExit = true;
                break;
            }
            case PLATFORM_EVENT_RESIZED:
            {
                resize(event.width, event.height);
                break;
            }
            case PLATFORM_EVENT_KEY_PRESSED:
            {
                switch (event.key)
                {
                    case PLATFORM_KEY_A: inputs.left = true; break;
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
                    case PLATFORM_KEY_ESCAPE: gDidExit = true; break;
                    case PLATFORM_KEY_F12: inputs.screenshot = true; break;
                    case PLATFORM_KEY_A:
                    case PLATFORM_KEY_D:
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
