
#include "components.h"
#include "common_types.h"
#include <raylib.h>


constexpr static ComponentInitializer cinfoArray[] = {
    {   CID_PLAYER,                 0, nullptr, nullptr},
    {CID_TRANSFORM, sizeof(Transform), nullptr, nullptr},
};

const static u32 cinfoArrayLen = sizeof(cinfoArray) / sizeof(ComponentInitializer);

ComponentInitializer getComponentInitializer(ComponentID cid)
{
    for (int i = 0; i < cinfoArrayLen; i++)
    {
        auto info = cinfoArray[i];
        if (info.id == cid)
        {
            return info;
        }
    }
    return (ComponentInitializer) {CID_EMPTY, 0, nullptr, nullptr};
}
