
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
    prayEntityRegistryInit();
    registerTestComponents();

    u32 entityIDs[4];

    Entity *entity1 = prayEntityNew(C(CID(PlayerComponent), CID(TransformComponent), CID(HealthComponent)), 3);
    entityIDs[0] = entity1->entityId;
    CU_ASSERT_EQUAL(entity1->componentLookup.length, 3);

    Entity *entity2 = prayEntityNew(C(CID(HealthComponent), CID(TransformComponent)), 2);
    entityIDs[1] = entity2->entityId;
    CU_ASSERT_EQUAL(entity2->componentLookup.length, 2);

    Entity *entity3 = prayEntityNew(C(CID(TransformComponent)), 1);
    entityIDs[2] = entity3->entityId;
    CU_ASSERT_EQUAL(entity3->componentLookup.length, 1);

    Entity *entity4 = prayEntityNew(C(CID(WorldComponent), CID(TransformComponent)), 2);
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

    PlayerComponent *playerComponent = prayEntityGetComponent(entity1, CID(PlayerComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(playerComponent);
    CU_ASSERT_STRING_EQUAL(playerComponent->playerName, "Deckard");
    CU_ASSERT_STRING_EQUAL(playerComponent->origin, "Los Angeles");
    CU_ASSERT_EQUAL(playerComponent->playerAge, 45);

    playerComponent = prayEntityGetComponent(entity2, CID(PlayerComponent));
    CU_ASSERT_PTR_NULL_FATAL(playerComponent);
    TransformComponent *transformComponent = prayEntityGetComponent(entity2, CID(TransformComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(transformComponent);
    playerComponent = prayEntityGetComponent(entity3, CID(PlayerComponent));
    CU_ASSERT_PTR_NULL_FATAL(playerComponent);
    playerComponent = prayEntityGetComponent(entity4, CID(PlayerComponent));
    CU_ASSERT_PTR_NULL_FATAL(playerComponent);

    entity1 = prayEntityFree(entity1);
    CU_ASSERT_PTR_NULL_FATAL(entity1);

    entity2 = prayEntityFree(entity2);
    CU_ASSERT_PTR_NULL_FATAL(entity2);

    entity3 = prayEntityFree(entity3);
    CU_ASSERT_PTR_NULL_FATAL(entity3);

    entity4 = prayEntityFree(entity4);
    CU_ASSERT_PTR_NULL_FATAL(entity4);

    prayComponentsDestroy();
    prayEntityRegistryDestroy();

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

#define LUKE_CID(HealthComponent) 87
#define VADER_CID(HealthComponent) 34

void entityRegistryTest()
{
    prayEntityRegistryInit();
    registerTestComponents();

    Entity *player = prayEntityNew(C(CID(PlayerComponent), CID(TransformComponent), CID(HealthComponent)), 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(player);
    PlayerComponent *playerComponent = prayEntityGetComponent(player, CID(PlayerComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(playerComponent);
    snprintf(playerComponent->playerName, sizeof(playerComponent->playerName), "Luke Skywalker");
    snprintf(playerComponent->origin, sizeof(playerComponent->origin), "Tatooine");
    playerComponent->playerAge = 20;
    HealthComponent *playerHealth = prayEntityGetComponent(player, CID(HealthComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(playerHealth);
    playerHealth->currentHealth = LUKE_CID(HealthComponent);

    Rc rc = prayEntityRegister(player);
    CU_ASSERT_EQUAL(rc, RC_OK);

    rc = prayEntityRegister(player);
    CU_ASSERT_EQUAL(rc, RC_BAD_PARAM);

    Entity *enemy = prayEntityNew(C(CID(EnemyComponent), CID(TransformComponent), CID(HealthComponent)), 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(enemy);
    EnemyComponent *enemyComponent = prayEntityGetComponent(enemy, CID(EnemyComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(enemyComponent);
    snprintf(enemyComponent->enemyName, sizeof(enemyComponent->enemyName), "Darth Vader");
    snprintf(enemyComponent->origin, sizeof(enemyComponent->origin), "Tatooine");
    enemyComponent->enemyAge = 45;
    HealthComponent *enemyHealth = prayEntityGetComponent(enemy, CID(HealthComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(enemyHealth);
    enemyHealth->currentHealth = VADER_CID(HealthComponent);

    rc = prayEntityRegister(enemy);
    CU_ASSERT_EQUAL(rc, RC_OK);

    LList playerList;

    rc = prayEntityLookupAll(&playerList, C(CID(PlayerComponent), CID(HealthComponent)), 2);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL_FATAL(playerList.size, 1);

    Entity *playerEntity = LListGetEntry(playerList.head, Entity);
    PlayerComponent *p = prayEntityGetComponent(playerEntity, CID(PlayerComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(p);
    CU_ASSERT_STRING_EQUAL(p->playerName, "Luke Skywalker");
    CU_ASSERT_STRING_EQUAL(p->origin, "Tatooine");
    CU_ASSERT_EQUAL(p->playerAge, 20);
    HealthComponent *phealth = prayEntityGetComponent(playerEntity, CID(HealthComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(phealth);
    CU_ASSERT_EQUAL(phealth->currentHealth, LUKE_CID(HealthComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(prayEntityGetComponent(playerEntity, CID(TransformComponent)));

    LList enemyList;

    rc = prayEntityLookupAll(&enemyList, C(CID(EnemyComponent), CID(HealthComponent)), 2);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL_FATAL(enemyList.size, 1);

    Entity *enemyEntity = LListGetEntry(enemyList.head, Entity);
    EnemyComponent *e = prayEntityGetComponent(enemyEntity, CID(EnemyComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(e);
    CU_ASSERT_STRING_EQUAL(e->enemyName, "Darth Vader");
    CU_ASSERT_STRING_EQUAL(e->origin, "Tatooine");
    CU_ASSERT_EQUAL(e->enemyAge, 45);
    HealthComponent *ehealth = prayEntityGetComponent(enemyEntity, CID(HealthComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(ehealth);
    CU_ASSERT_EQUAL(ehealth->currentHealth, VADER_CID(HealthComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(prayEntityGetComponent(enemyEntity, CID(TransformComponent)));

    LList transportHealthList;
    llistInit(&transportHealthList);
    rc = prayEntityLookupAll(&transportHealthList, C(CID(TransformComponent), CID(HealthComponent)), 2);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(transportHealthList.size, 2);

    Entity *entity = prayEntityLookup(C(CID(HealthComponent), CID(PlayerComponent)), 2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(entity);
    CU_ASSERT_PTR_EQUAL(entity, playerEntity);

    LList worldList;

    rc = prayEntityLookupAll(&worldList, C(CID(TransformComponent), CID(HealthComponent), CID(WorldComponent)), 3);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(worldList.size, 0);

    Entity *ent = prayEntityLookup(C(CID(EnemyComponent), CID(PlayerComponent)), 2);
    CU_ASSERT_PTR_NULL(ent);

    rc = prayEntityUnregister(playerEntity);
    CU_ASSERT_EQUAL(rc, RC_OK);

    rc = prayEntityUnregister(playerEntity);
    CU_ASSERT_EQUAL(rc, RC_NOT_FOUND);

    rc = prayEntityUnregister(enemyEntity);
    CU_ASSERT_EQUAL(rc, RC_OK);

    playerEntity = prayEntityFree(playerEntity);
    CU_ASSERT_PTR_NULL(playerEntity);
    enemyEntity = prayEntityFree(enemyEntity);
    CU_ASSERT_PTR_NULL(enemyEntity);

    prayComponentsDestroy();
    prayEntityRegistryDestroy();

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void entityWithComponentAllocations()
{
    prayEntityRegistryInit();
    registerTestComponents();

    Entity *worldEntity = prayEntityNew(C(CID(WorldComponent)), 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(worldEntity);
    WorldComponent *world = prayEntityGetComponent(worldEntity, CID(WorldComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(world);
    CU_ASSERT_EQUAL(world->worldSize, 100);
    for (int i = 0; i < 100; i++)
    {
        CU_ASSERT_EQUAL(world->world[i], i);
    }

    prayEntityRegister(worldEntity);

    Entity *worldEntity2 = prayEntityLookup(C(CID(WorldComponent)), 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(worldEntity2);
    WorldComponent *world2 = prayEntityGetComponent(worldEntity2, CID(WorldComponent));
    CU_ASSERT_PTR_NOT_NULL_FATAL(world2);
    CU_ASSERT_EQUAL(world2->worldSize, 100);
    for (int i = 0; i < 100; i++)
    {
        CU_ASSERT_EQUAL(world2->world[i], i);
    }

    CU_ASSERT_PTR_EQUAL(world, world2);


    prayEntityUnregister(worldEntity);
    prayEntityFree(worldEntity);

    prayComponentsDestroy();
    prayEntityRegistryDestroy();

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
