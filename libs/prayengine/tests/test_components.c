
#include "test_components.h"
#include "common_types.h"
#include "pray_components.h"
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
    world->world = tmemcalloc(1, 10);
    world->worldSize = 10;
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
    componnentInitialize();

    componentRegister(PLAYER, sizeof(PlayerComponent), initPlayer, nullptr);

    componentRegister(TRANSFORM, sizeof(TransformComponent), nullptr, nullptr);

    componentRegister(HEALTH, sizeof(PlayerComponent), nullptr, nullptr);

    componentRegister(WORLD, sizeof(WorldComponent), initWorld, deinitWorld);
}

void registerComponentTest()
{
    Rc rc = componentRegister(PLAYER, sizeof(PlayerComponent), initPlayer, nullptr);
    CU_ASSERT_EQUAL(rc, RC_OK);

    rc = componentRegister(PLAYER, sizeof(PlayerComponent), initPlayer, nullptr);
    CU_ASSERT_EQUAL(rc, RC_BAD_PARAM);

    rc = componentRegister(TRANSFORM, sizeof(TransformComponent), nullptr, nullptr);
    CU_ASSERT_EQUAL(rc, RC_OK);

    rc = componentRegister(HEALTH, sizeof(HealthComponent), nullptr, nullptr);
    CU_ASSERT_EQUAL(rc, RC_OK);

    rc = componentRegister(WORLD, sizeof(WorldComponent), initWorld, deinitWorld);
    CU_ASSERT_EQUAL(rc, RC_OK);

    ComponentInitializer ci = {};
    rc = componentGetInitializer(PLAYER, &ci);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(ci.id, PLAYER);
    CU_ASSERT_EQUAL(ci.size, sizeof(PlayerComponent));
    CU_ASSERT_PTR_NOT_NULL(ci.initialize);
    CU_ASSERT_PTR_EQUAL(ci.initialize, initPlayer);
    CU_ASSERT_PTR_NULL(ci.deinitialize);

    rc = componentGetInitializer(TRANSFORM, &ci);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(ci.id, TRANSFORM);
    CU_ASSERT_EQUAL(ci.size, sizeof(TransformComponent));
    CU_ASSERT_PTR_NULL(ci.initialize);
    CU_ASSERT_PTR_NULL(ci.deinitialize);

    rc = componentGetInitializer(HEALTH, &ci);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(ci.id, HEALTH);
    CU_ASSERT_EQUAL(ci.size, sizeof(HealthComponent));
    CU_ASSERT_PTR_NULL(ci.initialize);
    CU_ASSERT_PTR_NULL(ci.deinitialize);

    rc = componentGetInitializer(WORLD, &ci);
    CU_ASSERT_EQUAL(rc, RC_OK);
    CU_ASSERT_EQUAL(ci.id, WORLD);
    CU_ASSERT_EQUAL(ci.size, sizeof(WorldComponent));
    CU_ASSERT_PTR_NOT_NULL(ci.initialize);
    CU_ASSERT_PTR_EQUAL(ci.initialize, initWorld);
    CU_ASSERT_PTR_NOT_NULL(ci.deinitialize);
    CU_ASSERT_PTR_EQUAL(ci.deinitialize, deinitWorld);

    componentsDestroy();

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void initDeinitComponents()
{
    registerTestComponents();

    ComponentInitializer componentInitializer = {0};
    Rc rc = componentGetInitializer(PLAYER, &componentInitializer);
    CU_ASSERT_EQUAL_FATAL(rc, RC_OK);

    PlayerComponent *playerComponent = tmemcalloc(1, componentInitializer.size);
    CU_ASSERT_PTR_NOT_NULL_FATAL(componentInitializer.initialize);

    componentInitializer.initialize(playerComponent);

    CU_ASSERT_STRING_EQUAL(playerComponent->playerName, "Deckard");
    CU_ASSERT_STRING_EQUAL(playerComponent->origin, "Los Angeles");
    CU_ASSERT_EQUAL(playerComponent->playerAge, 45);

    tmemfree(playerComponent);

    rc = componentGetInitializer(WORLD, &componentInitializer);
    CU_ASSERT_EQUAL_FATAL(rc, RC_OK);

    WorldComponent *worldComponent = tmemcalloc(1, componentInitializer.size);
    CU_ASSERT_PTR_NOT_NULL_FATAL(componentInitializer.initialize);
    CU_ASSERT_PTR_NOT_NULL_FATAL(componentInitializer.deinitialize);

    CU_ASSERT_PTR_NULL(worldComponent->world);
    CU_ASSERT_EQUAL(worldComponent->worldSize, 0);

    componentInitializer.initialize(worldComponent);

    CU_ASSERT_PTR_NOT_NULL(worldComponent->world);
    CU_ASSERT_EQUAL(worldComponent->worldSize, 10);

    componentInitializer.deinitialize(worldComponent);

    CU_ASSERT_PTR_NULL(worldComponent->world);
    CU_ASSERT_EQUAL(worldComponent->worldSize, 0);

    tmemfree(worldComponent);

    componentsDestroy();

    auto stats = tMemGetStats();
    CU_ASSERT_EQUAL(stats.current, 0);
}

void registerComponentsTests()
{
    CU_pSuite suite = CU_add_suite("Pray Components Test", nullptr, nullptr);
    CU_add_test(suite, "Register Components", registerComponentTest);
    CU_add_test(suite, "Init/Deinit Components", initDeinitComponents);
}
