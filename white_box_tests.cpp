//======= Copyright (c) 2024, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2024-02-14
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>

#include "gtest/gtest.h"

#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem 
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v 
//       indexu
//============================================================================//

class HTableTest : public ::testing::Test
{
    protected:
        hash_map_t* map;

    virtual void SetUp() override
    {
        map = hash_map_ctor();
        ASSERT_NE(map, nullptr);
    }
    virtual void TearDown() override
    {
        hash_map_dtor(map);
        EXPECT_EQ(map->allocated, 0);
    }
};

TEST_F(HTableTest, clearTEST){
    hash_map_clear(map);
    EXPECT_EQ(map->first, nullptr);
    EXPECT_EQ(map->last, nullptr);
    EXPECT_EQ(map->used, 0);
}

TEST_F(HTableTest, reserveTEST){
    hash_map_put(map, "key1", 1);
    hash_map_put(map, "key2", 2);
    EXPECT_EQ(hash_map_reserve(map, 0), VALUE_ERROR);
    EXPECT_EQ(hash_map_reserve(map, -1), MEMORY_ERROR);
    EXPECT_EQ(hash_map_reserve(map, 10), OK);
    EXPECT_EQ(hash_map_reserve(map, 10), OK);
    hash_map_remove(map, "key1");
    hash_map_remove(map, "key2");
}

TEST_F(HTableTest, reserveTOOMUCHTEST){
    EXPECT_EQ(hash_map_reserve(map, 5), OK);
    EXPECT_EQ(hash_map_put(map, "key1", 1), OK);
    EXPECT_EQ(hash_map_put(map, "key2", 2), OK);
    EXPECT_EQ(hash_map_put(map, "key3", 3), OK);
    EXPECT_EQ(hash_map_put(map, "key4", 4), OK);
    EXPECT_EQ(hash_map_put(map, "key5", 5), OK);
    EXPECT_EQ(hash_map_put(map, "key6", 6), OK);
}

TEST_F(HTableTest, reserveMORETEST){
    int alloc;
    EXPECT_EQ(hash_map_put(map, "key1", 1), OK);
	EXPECT_EQ(hash_map_put(map, "key2", 2), OK);
	alloc = map->allocated;
	EXPECT_EQ(hash_map_reserve(map, alloc+1), OK);
	EXPECT_EQ(hash_map_capacity(map), alloc+1);
}

TEST_F(HTableTest, reserveTOOSMALLTEST){
    //fixit
    /*
    EXPECT_EQ(hash_map_put(map, "key1", 1), OK);
	EXPECT_EQ(hash_map_put(map, "key2", 2), OK);
    EXPECT_ANY_THROW(hash_map_reserve(map, map->used));
    */
}

TEST_F(HTableTest, reserveMAXTEST){
    EXPECT_EQ(hash_map_reserve(map, SIZE_MAX), MEMORY_ERROR);
}

TEST_F(HTableTest, sizeTEST){
    hash_map_put(map, "key1", 1);
    hash_map_put(map, "key2", 2);
    EXPECT_EQ(hash_map_size(map), 2);
    EXPECT_NE(hash_map_size(map), 3);
    hash_map_remove(map, "key1");
    hash_map_remove(map, "key2");
    //EXPECT_EQ(hash_map_size(map), 0);
}

TEST_F(HTableTest, capacityTEST){
    hash_map_reserve(map, 10);
    EXPECT_EQ(hash_map_capacity(map), 10);
    ASSERT_NE(hash_map_capacity(map), 5);
}

TEST_F(HTableTest, containTEST){
    hash_map_put(map, "key1", 1);
    hash_map_put(map, "key2", 2);
    EXPECT_EQ(hash_map_contains(map, "key1"), true);
    EXPECT_EQ(hash_map_contains(map, "key2"), true);
    hash_map_remove(map, "key1");
    hash_map_remove(map, "key2");
    EXPECT_EQ(hash_map_contains(map, "key1"), false);
    EXPECT_EQ(hash_map_contains(map, "key2"), false);
}

TEST_F(HTableTest, putTEST){
    EXPECT_EQ(hash_map_reserve(map, 5), OK);
    EXPECT_EQ(hash_map_put(map, "key1", 1), OK);
    EXPECT_EQ(hash_map_put(map, "key2", 2), OK);
    EXPECT_EQ(hash_map_put(map, "key1", 1), KEY_ALREADY_EXISTS);
    EXPECT_EQ(hash_map_put(map, "key3", 3), OK);
    EXPECT_EQ(hash_map_put(map, "key4", 4), OK);
}

TEST_F(HTableTest, putColisionTEST){
    EXPECT_EQ(hash_map_put(map, "key1", 1), OK);
    EXPECT_EQ(hash_map_put(map, "1yek", 1), OK);
    EXPECT_EQ(hash_map_contains(map, "1yek"), true);
}

TEST_F(HTableTest, getTEST){
    int hodnota;
    EXPECT_EQ(hash_map_put(map, "key1", 1), OK);
    EXPECT_EQ(hash_map_get(map, "key1", &hodnota), OK);
    EXPECT_EQ(hodnota, 1);
    EXPECT_EQ(hash_map_get(map, "key3", &hodnota), KEY_ERROR);
}

TEST_F(HTableTest, getColisionTEST){
    int hodnota;
    EXPECT_EQ(hash_map_put(map, "key1", 1), OK);
    EXPECT_EQ(hash_map_put(map, "1yek", 2), OK);
    EXPECT_EQ(hash_map_put(map, "e1yk", 3), OK);
    EXPECT_EQ(hash_map_get(map, "key1", &hodnota), OK);
    EXPECT_EQ(hodnota, 1);
    EXPECT_EQ(hash_map_get(map, "1yek", &hodnota), OK);
    EXPECT_EQ(hodnota, 2);
    EXPECT_EQ(hash_map_get(map, "e1yk", &hodnota), OK);
    EXPECT_EQ(hodnota, 3);
}

TEST_F(HTableTest, removeTEST){
    EXPECT_EQ(hash_map_put(map, "key1", 1), OK);
    EXPECT_EQ(hash_map_remove(map, "key1"), OK);
    EXPECT_EQ(hash_map_remove(map, "key1"), KEY_ERROR);
}

TEST_F(HTableTest, popTEST){
    int cil;
    EXPECT_EQ(hash_map_pop(map, "key1", &cil), 3);
    EXPECT_EQ(hash_map_put(map, "key1", 1), 0);
    EXPECT_EQ(hash_map_pop(map, "key1", &cil), 0);
    EXPECT_EQ(cil, 1);
    EXPECT_EQ(hash_map_put(map, "key1", 1), 0);
    EXPECT_EQ(hash_map_put(map, "key2", 2), 0);
    EXPECT_EQ(hash_map_put(map, "key3", 3), 0);
    EXPECT_EQ(hash_map_pop(map, "key2", &cil), 0);
    EXPECT_EQ(cil, 2);
}

TEST_F(HTableTest, popColisionTEST){
    int cil;
    EXPECT_EQ(hash_map_put(map, "key1", 1), 0);
    EXPECT_EQ(hash_map_put(map, "1yek", 2), 0);
    EXPECT_EQ(hash_map_pop(map, "key1", &cil), 0);
    EXPECT_EQ(cil, 1);
    EXPECT_EQ(hash_map_pop(map, "1yek", &cil), 0);
    EXPECT_EQ(cil, 2);
}




/*** Konec souboru white_box_tests.cpp ***/
