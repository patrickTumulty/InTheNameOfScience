
#include "test_components.h"
#include "common_types.h"
#include "pray_component.h"
#include "tmem.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/TestRun.h>
#include <stdio.h>

void initPlayer(void *ptr)
{
    PlayerComponent *player = ptr;
    snprintf(player->playerName, sizeof(player->playerName), "Deckard");
    snprintf(player->origin, sizeof(player->origin), "Los Angeles");
    player->playerAge = 45;
}

void initWorld(void *ptr)
{
    WorldComponent *world = ptr;
    world->world = tmemcalloc(1, 100);
    for (int i = 0; i < 100; i++)
    {
        world->world[i] = i;
    }
    world->worldSize = 100;
}

void deinitWorld(void *ptr)
{
    WorldComponent *world = ptr;
    tmemfree(world->world);
    world->world = nullptr;
    world->worldSize = 0;
}

void registerTestComponents()
{
    prayComponnentInitialize();

    prayComponentRegister(PLAYER, sizeof(PlayerComponent), initPlayer, nullptr);

    prayComponentRegister(TRANSFORM, sizeof(TransformComponent), nullptr, nullptr);

    prayComponentRegister(HEALTH, sizeof(PlayerComponent), nullptr, nullptr);

    prayComponentRegister(WORLD, sizeof(WorldComponent), initWorld, deinitWorld);

    prayComponentRegister(ENEMY, sizeof(EnemyComponent), nullptr, nullptr);
}

void registerComponentTest()
{
    Rc rc = prayComponentRegister(PLAYER, sizeof(PlayerComponent), initPlayer, nullptr);
    CU_ASSERT_EQUAL(rc, RC_OK);

    rc = prayComponentRegister(PLAYER, sizeof(PlayerComponent), initPlayer, nullptr);
    CU_ASSERT_EQUAL(rc, RC_BAD_PARAM);

    rc = prayComponentRegister(TRANSFORM, sizeof(TransformComponent), nullptr, nullptr);
    CU_ASSERT_EQUAL(rc, RC_OK);

    rc = prayComponentRegister(HEALTH, sizeof(HealthComponent), nullptr, nullptr);
    CU_ASSERT_EQUAL(rc, RC_OK);

    rc = prayComponentRegister(WORLD, sizeof(WorldComponent), initWorld, deinitWorld);
    CU_ASSERT_EQUAL(rc, RC_OK);

    ComponentInitializer ci = {};
    rc = prayComponentGetInitializer(PLAYER, &ci);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(ci.id, PLAYER);
    CU_ASSERT_EQUAL(ci.size, sizeof(PlayerComponent));
    CU_ASSERT_PTR_NOT_NULL(ci.initialize);
    CU_ASSERT_PTR_EQUAL(ci.initialize, initPlayer);
    CU_ASSERT_PTR_NULL(ci.deinitialize);

    rc = prayComponentGetInitializer(TRANSFORM, &ci);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(ci.id, TRANSFORM);
    CU_ASSERT_EQUAL(ci.size, sizeof(TransformComponent));
    CU_ASSERT_PTR_NULL(ci.initialize);
    CU_ASSERT_PTR_NULL(ci.deinitialize);

    rc = prayComponentGetInitializer(HEALTH, &ci);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(ci.id, HEALTH);
    CU_ASSERT_EQUAL(ci.size, sizeof(HealthComponent));
    CU_ASSERT_PTR_NULL(ci.initialize);
    CU_ASSERT_PTR_NULL(ci.deinitialize);

    rc = prayComponentGetInitializer(WORLD, &ci);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(ci.id, WORLD);
    CU_ASSERT_EQUAL(ci.size, sizeof(WorldComponent));
    CU_ASSERT_PTR_NOT_NULL(ci.initialize);
    CU_ASSERT_PTR_EQUAL(ci.initialize, initWorld);
    CU_ASSERT_PTR_NOT_NULL(ci.deinitialize);
    CU_ASSERT_PTR_EQUAL(ci.deinitialize, deinitWorld);

    prayComponentsDestroy();

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void initDeinitComponents()
{
    registerTestComponents();

    ComponentInitializer componentInitializer = {0};
    Rc rc = prayComponentGetInitializer(PLAYER, &componentInitializer);
    CU_ASSERT_EQUAL_FATAL(rc, RC_OK);

    PlayerComponent *playerComponent = tmemcalloc(1, componentInitializer.size);
    CU_ASSERT_PTR_NOT_NULL_FATAL(componentInitializer.initialize);

    componentInitializer.initialize(playerComponent);

    CU_ASSERT_STRING_EQUAL(playerComponent->playerName, "Deckard");
    CU_ASSERT_STRING_EQUAL(playerComponent->origin, "Los Angeles");
    CU_ASSERT_EQUAL(playerComponent->playerAge, 45);

    tmemfree(playerComponent);

    rc = prayComponentGetInitializer(WORLD, &componentInitializer);
    CU_ASSERT_EQUAL_FATAL(rc, RC_OK);

    WorldComponent *worldComponent = tmemcalloc(1, componentInitializer.size);
    CU_ASSERT_PTR_NOT_NULL_FATAL(componentInitializer.initialize);
    CU_ASSERT_PTR_NOT_NULL_FATAL(componentInitializer.deinitialize);

    CU_ASSERT_PTR_NULL(worldComponent->world);
    CU_ASSERT_EQUAL(worldComponent->worldSize, 0);

    componentInitializer.initialize(worldComponent);

    CU_ASSERT_PTR_NOT_NULL(worldComponent->world);
    CU_ASSERT_EQUAL(worldComponent->worldSize, 100);

    componentInitializer.deinitialize(worldComponent);

    CU_ASSERT_PTR_NULL(worldComponent->world);
    CU_ASSERT_EQUAL(worldComponent->worldSize, 0);

    tmemfree(worldComponent);

    prayComponentsDestroy();

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void registerComponentsTests()
{
    CU_pSuite suite = CU_add_suite("Pray Components Test", nullptr, nullptr);
    CU_add_test(suite, "Register Components", registerComponentTest);
    CU_add_test(suite, "Init/Deinit Components", initDeinitComponents);
}
