
#include "camera_system.h"
#include "pray_camera.h"
#include "pray_globals.h"
#include "pray_system.h"
#include <raylib.h>
#include <stdio.h>

static float cameraSpeed = 1600.0f;
static Vector2 cameraTarget;


Vector2 getScreenCenter()
{
    return (Vector2) {(float) SCREEN_WIDTH / 2, (float) SCREEN_HEIGHT / 2};
}


void start()
{
    Camera2D *camera = prayGetCamera();

    cameraTarget = getScreenCenter();
    camera->target = cameraTarget;
    camera->offset = (Vector2) {0, 0};
    camera->rotation = 0.0f;
    camera->zoom = 0.25f;
}

void close()
{

}

void gameUpdate()
{
    Camera2D *camera = prayGetCamera();

    cameraTarget.x -= (float) IsKeyDown(KEY_A) * (1.0f * GetFrameTime() * cameraSpeed);
    cameraTarget.x += (float) IsKeyDown(KEY_D) * (1.0f * GetFrameTime() * cameraSpeed);
    cameraTarget.y -= (float) IsKeyDown(KEY_W) * (1.0f * GetFrameTime() * cameraSpeed);
    cameraTarget.y += (float) IsKeyDown(KEY_S) * (1.0f * GetFrameTime() * cameraSpeed);

    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        const float zoomIncrement = 0.05f;
        camera->zoom += (wheel * zoomIncrement);
        if (camera->zoom < 0.1f)
        {
            camera->zoom = 0.1f;
        }

        if (camera->zoom > 8.0f)
        {
            camera->zoom = 8.0f;
        }
    }

    camera->target = cameraTarget;

    float width = (float) GetScreenWidth();
    float height = (float) GetScreenHeight();
    camera->offset = (Vector2) {width / 2, height / 2};
}


void registerCameraSystem()
{
    System cameraSystem = {
        .name = "Camera System",
        .start = start,
        .stop = close,
        .gameUpdate = gameUpdate,
    };

    praySystemsRegister(cameraSystem);
}

