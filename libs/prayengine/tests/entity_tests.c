
#include "pray_components.h"
#include "pray_engine_tests.h"
#include "pray_entity.h"
#include "test_components.h"
#include "tmem.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>


void createEntityTest()
{
    registerTestComponents();

    u32 entityIDs[4];

    Entity *entity1 = entityNew(3, PLAYER, TRANSFORM, HEALTH);
    entityIDs[0] = entity1->entityId;
    CU_ASSERT_EQUAL(entity1->componentLookup.length, 3);

    Entity *entity2 = entityNew(2, HEALTH, TRANSFORM);
    entityIDs[1] = entity2->entityId;
    CU_ASSERT_EQUAL(entity2->componentLookup.length, 2);

    Entity *entity3 = entityNew(1, TRANSFORM);
    entityIDs[2] = entity3->entityId;
    CU_ASSERT_EQUAL(entity3->componentLookup.length, 1);

    Entity *entity4 = entityNew(2, WORLD, TRANSFORM);
    entityIDs[3] = entity4->entityId;
    CU_ASSERT_EQUAL(entity4->componentLookup.length, 2);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                continue;
            }
            CU_ASSERT_NOT_EQUAL(entityIDs[i], entityIDs[j]);
        }
    }

    PlayerComponent *playerComponent = entityGetComponent(entity1, PLAYER);
    CU_ASSERT_PTR_NOT_NULL_FATAL(playerComponent);
    CU_ASSERT_STRING_EQUAL(playerComponent->playerName, "Deckard");
    CU_ASSERT_STRING_EQUAL(playerComponent->origin, "Los Angeles");
    CU_ASSERT_EQUAL(playerComponent->playerAge, 45);

    playerComponent = entityGetComponent(entity2, PLAYER);
    CU_ASSERT_PTR_NULL_FATAL(playerComponent);
    TransformComponent *transformComponent = entityGetComponent(entity2, TRANSFORM);
    CU_ASSERT_PTR_NOT_NULL_FATAL(transformComponent);
    playerComponent = entityGetComponent(entity3, PLAYER);
    CU_ASSERT_PTR_NULL_FATAL(playerComponent);
    playerComponent = entityGetComponent(entity4, PLAYER);
    CU_ASSERT_PTR_NULL_FATAL(playerComponent);

    entity1 = entityFree(entity1);
    CU_ASSERT_PTR_NULL_FATAL(entity1);

    entity2 = entityFree(entity2);
    CU_ASSERT_PTR_NULL_FATAL(entity2);

    entity3 = entityFree(entity3);
    CU_ASSERT_PTR_NULL_FATAL(entity3);

    entity4 = entityFree(entity4);
    CU_ASSERT_PTR_NULL_FATAL(entity4);

    componentsDestroy();

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}


void registerEntityTests()
{
    CU_pSuite suite = CU_add_suite("Entity Tests", nullptr, nullptr);
    CU_add_test(suite, "Create Entity Test", createEntityTest);
}
