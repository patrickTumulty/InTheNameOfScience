
#include "pray_component.h"
#include "pray_default_components.h"

void prayRegisterDefaultComponents()
{
    registerComponent(Transform2DComponent, nullptr, nullptr);
    registerComponent(Sprite2DComponent, nullptr, nullptr);
}
