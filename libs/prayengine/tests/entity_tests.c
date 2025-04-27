
#include "common_types.h"
#include "linked_list.h"
#include "pray_component.h"
#include "pray_engine_tests.h"
#include "pray_entity.h"
#include "pray_entity_registry.h"
#include "test_components.h"
#include "tmem.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>

void createEntityTest()
{
    entityRegistryInit();
    registerTestComponents();

    u32 entityIDs[4];

    Entity *entity1 = entityNew(C(PLAYER, TRANSFORM, HEALTH), 3);
    entityIDs[0] = entity1->entityId;
    CU_ASSERT_EQUAL(entity1->componentLookup.length, 3);

    Entity *entity2 = entityNew(C(HEALTH, TRANSFORM), 2);
    entityIDs[1] = entity2->entityId;
    CU_ASSERT_EQUAL(entity2->componentLookup.length, 2);

    Entity *entity3 = entityNew(C(TRANSFORM), 1);
    entityIDs[2] = entity3->entityId;
    CU_ASSERT_EQUAL(entity3->componentLookup.length, 1);

    Entity *entity4 = entityNew(C(WORLD, TRANSFORM), 2);
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
    entityRegistryDestroy();

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

#define LUKE_HEALTH 87
#define VADER_HEALTH 34

void entityRegistryTest()
{
    entityRegistryInit();
    registerTestComponents();

    Entity *player = entityNew(C(PLAYER, TRANSFORM, HEALTH), 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(player);
    PlayerComponent *playerComponent = entityGetComponent(player, PLAYER);
    CU_ASSERT_PTR_NOT_NULL_FATAL(playerComponent);
    snprintf(playerComponent->playerName, sizeof(playerComponent->playerName), "Luke Skywalker");
    snprintf(playerComponent->origin, sizeof(playerComponent->origin), "Tatooine");
    playerComponent->playerAge = 20;
    HealthComponent *playerHealth = entityGetComponent(player, HEALTH);
    CU_ASSERT_PTR_NOT_NULL_FATAL(playerHealth);
    playerHealth->currentHealth = LUKE_HEALTH;

    Rc rc = entityRegistryRegister(player);
    CU_ASSERT_EQUAL(rc, RC_OK);

    rc = entityRegistryRegister(player);
    CU_ASSERT_EQUAL(rc, RC_BAD_PARAM);

    Entity *enemy = entityNew(C(ENEMY, TRANSFORM, HEALTH), 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(enemy);
    EnemyComponent *enemyComponent = entityGetComponent(enemy, ENEMY);
    CU_ASSERT_PTR_NOT_NULL_FATAL(enemyComponent);
    snprintf(enemyComponent->enemyName, sizeof(enemyComponent->enemyName), "Darth Vader");
    snprintf(enemyComponent->origin, sizeof(enemyComponent->origin), "Tatooine");
    enemyComponent->enemyAge = 45;
    HealthComponent *enemyHealth = entityGetComponent(enemy, HEALTH);
    CU_ASSERT_PTR_NOT_NULL_FATAL(enemyHealth);
    enemyHealth->currentHealth = VADER_HEALTH;

    rc = entityRegistryRegister(enemy);
    CU_ASSERT_EQUAL(rc, RC_OK);

    LList playerList;

    rc = entityRegistryLookupAll(&playerList, C(PLAYER, HEALTH), 2);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL_FATAL(playerList.size, 1);

    Entity *playerEntity = LListGetEntry(playerList.head, Entity);
    PlayerComponent *p = entityGetComponent(playerEntity, PLAYER);
    CU_ASSERT_PTR_NOT_NULL_FATAL(p);
    CU_ASSERT_STRING_EQUAL(p->playerName, "Luke Skywalker");
    CU_ASSERT_STRING_EQUAL(p->origin, "Tatooine");
    CU_ASSERT_EQUAL(p->playerAge, 20);
    HealthComponent *phealth = entityGetComponent(playerEntity, HEALTH);
    CU_ASSERT_PTR_NOT_NULL_FATAL(phealth);
    CU_ASSERT_EQUAL(phealth->currentHealth, LUKE_HEALTH);
    CU_ASSERT_PTR_NOT_NULL_FATAL(entityGetComponent(playerEntity, TRANSFORM));

    LList enemyList;

    rc = entityRegistryLookupAll(&enemyList, C(ENEMY, HEALTH), 2);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL_FATAL(enemyList.size, 1);

    Entity *enemyEntity = LListGetEntry(enemyList.head, Entity);
    EnemyComponent *e = entityGetComponent(enemyEntity, ENEMY);
    CU_ASSERT_PTR_NOT_NULL_FATAL(e);
    CU_ASSERT_STRING_EQUAL(e->enemyName, "Darth Vader");
    CU_ASSERT_STRING_EQUAL(e->origin, "Tatooine");
    CU_ASSERT_EQUAL(e->enemyAge, 45);
    HealthComponent *ehealth = entityGetComponent(enemyEntity, HEALTH);
    CU_ASSERT_PTR_NOT_NULL_FATAL(ehealth);
    CU_ASSERT_EQUAL(ehealth->currentHealth, VADER_HEALTH);
    CU_ASSERT_PTR_NOT_NULL_FATAL(entityGetComponent(enemyEntity, TRANSFORM));

    LList transportHealthList;
    llistInit(&transportHealthList);
    rc = entityRegistryLookupAll(&transportHealthList, C(TRANSFORM, HEALTH), 2);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(transportHealthList.size, 2);

    Entity *entity = entityRegistryLookupFirst(C(HEALTH, PLAYER), 2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(entity);
    CU_ASSERT_PTR_EQUAL(entity, playerEntity);

    LList worldList;

    rc = entityRegistryLookupAll(&worldList, C(TRANSFORM, HEALTH, WORLD), 3);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(worldList.size, 0);

    Entity *ent = entityRegistryLookupFirst(C(ENEMY, PLAYER), 2);
    CU_ASSERT_PTR_NULL(ent);

    rc = entityRegistryUnregister(playerEntity);
    CU_ASSERT_EQUAL(rc, RC_OK);

    rc = entityRegistryUnregister(playerEntity);
    CU_ASSERT_EQUAL(rc, RC_NOT_FOUND);

    rc = entityRegistryUnregister(enemyEntity);
    CU_ASSERT_EQUAL(rc, RC_OK);

    playerEntity = entityFree(playerEntity);
    CU_ASSERT_PTR_NULL(playerEntity);
    enemyEntity = entityFree(enemyEntity);
    CU_ASSERT_PTR_NULL(enemyEntity);

    componentsDestroy();
    entityRegistryDestroy();

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void entityWithComponentAllocations()
{
    entityRegistryInit();
    registerTestComponents();

    Entity *worldEntity = entityNew(C(WORLD), 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(worldEntity);
    WorldComponent *world = entityGetComponent(worldEntity, WORLD);
    CU_ASSERT_PTR_NOT_NULL_FATAL(world);
    CU_ASSERT_EQUAL(world->worldSize, 100);
    for (int i = 0; i < 100; i++)
    {
        CU_ASSERT_EQUAL(world->world[i], i);
    }

    entityRegistryRegister(worldEntity);

    Entity *worldEntity2 = entityRegistryLookupFirst(C(WORLD), 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(worldEntity2);
    WorldComponent *world2 = entityGetComponent(worldEntity2, WORLD);
    CU_ASSERT_PTR_NOT_NULL_FATAL(world2);
    CU_ASSERT_EQUAL(world2->worldSize, 100);
    for (int i = 0; i < 100; i++)
    {
        CU_ASSERT_EQUAL(world2->world[i], i);
    }

    CU_ASSERT_PTR_EQUAL(world, world2);


    entityRegistryUnregister(worldEntity);
    entityFree(worldEntity);

    componentsDestroy();
    entityRegistryDestroy();

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void registerEntityTests()
{
    CU_pSuite suite = CU_add_suite("Entity Tests", nullptr, nullptr);
    CU_add_test(suite, "Create Entity Test", createEntityTest);
    CU_add_test(suite, "Entity Registry Test", entityRegistryTest);
    CU_add_test(suite, "Entity with Component Allocations", entityWithComponentAllocations);
}
