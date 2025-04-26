
#include "camera_system.h"
#include "pray_globals.h"
#include "system.h"
#include <raylib.h>
#include <stdio.h>

static float cameraSpeed = 200.0f;
static Camera2D camera;
static Vector2 cameraTarget;


Vector2 getScreenCenter()
{
    return (Vector2) {(float) SCREEN_WIDTH / 2, (float) SCREEN_HEIGHT / 2};
}

Camera2D getCamera()
{
    return camera;
}

void start()
{
    cameraTarget = getScreenCenter();
    camera.target = cameraTarget;
    camera.offset = (Vector2) {0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.125f;
}

void close()
{

}

void gameUpdate()
{
    cameraTarget.x -= (float) IsKeyDown(KEY_A) * (1.0f * GetFrameTime() * cameraSpeed);
    cameraTarget.x += (float) IsKeyDown(KEY_D) * (1.0f * GetFrameTime() * cameraSpeed);
    cameraTarget.y -= (float) IsKeyDown(KEY_W) * (1.0f * GetFrameTime() * cameraSpeed);
    cameraTarget.y += (float) IsKeyDown(KEY_S) * (1.0f * GetFrameTime() * cameraSpeed);

    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        const float zoomIncrement = 0.125f;
        camera.zoom += (wheel * zoomIncrement);
        if (camera.zoom < 1.0f)
        {
            camera.zoom = 1.0f;
        }

        if (camera.zoom > 8.0f)
        {
            camera.zoom = 8.0f;
        }
    }

    camera.target = cameraTarget;
}

void worldSpaceRender()
{
    float width = (float) GetScreenWidth();
    float height = (float) GetScreenHeight();
    camera.offset = (Vector2) {width / 2, height / 2};
}

void registerCameraSystem()
{
    System *system = systemNew();
    snprintf(system->systemName, sizeof(system->systemName), "Camera");
    system->start = start;
    system->stop = close;
    system->gameUpdate = gameUpdate;
    system->renderUpdateWorldSpace = worldSpaceRender;
}

