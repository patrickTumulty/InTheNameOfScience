
#include "pray_camera.h"
#include "raylib.h"

static Camera2D camera = { 0 };

Camera2D *prayGetCamera()
{
    return &camera;
}
