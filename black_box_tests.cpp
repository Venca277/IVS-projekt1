//======= Copyright (c) 2024, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Václav Sovák <xsovakv00@stud.fit.vutbr.cz>
// $Date:       $2024-02-14
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Václav Sovák
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

class EmptyTree : public ::testing::Test{
    protected:
    BinaryTree tree;
};

class NonEmptyTree : public ::testing::Test
{
    protected:
        BinaryTree tree;
        virtual void SetUp(){
            int values[] = {1,2,3,4,5};
            for (size_t i = 0; i < 6; i++){
                tree.InsertNode(values[i]);
            }
        }
};

class TreeAxioms : public ::testing::Test
{
    protected:
        BinaryTree tree;
        virtual void SetUp()
        {
            int values[] = {1,2,3,4,5};
            for (unsigned int i = 0; i < 6; i++){
                tree.InsertNode(values[i]);
            }
        }    
};

TEST_F(NonEmptyTree, InsertNode)
{
    
    tree.DeleteNode(0);
    EXPECT_TRUE(tree.InsertNode(0).first);
    EXPECT_FALSE(tree.InsertNode(tree.GetRoot()->key).first);

    
    for (size_t i = 0; i < 5; i++)
    {
        tree.DeleteNode(i);
    }
    
    for (size_t i = 0; i < 5; i++)
    {
        EXPECT_TRUE(tree.InsertNode(i).first);
    }
}

TEST_F(NonEmptyTree, FindNode)
{
    std::vector<Node_t*> nodeVector;
    tree.GetLeafNodes(nodeVector);
    for (auto element : nodeVector)
    {
        int color;
        color = element->color;
        std::cout << color;
    }

    int rootkey = tree.GetRoot()->key; //finds and saves id of root
    EXPECT_TRUE(tree.FindNode(rootkey)); //attempts to find root, expecting true when nonemptytree

    tree.InsertNode(6);
    EXPECT_TRUE(tree.FindNode(6));
    tree.DeleteNode(6);
    EXPECT_FALSE(tree.FindNode(6));
    
    EXPECT_TRUE(tree.FindNode(rand()%5));
}

TEST_F(NonEmptyTree, DeleteNode)
{
    int rootkey = tree.GetRoot()->key;
    EXPECT_TRUE(tree.DeleteNode(rootkey));
    EXPECT_FALSE(tree.DeleteNode(rootkey));

    if(tree.GetRoot()->pLeft == nullptr)
        EXPECT_FALSE(tree.DeleteNode(tree.GetRoot()->pLeft->key));
    else
        EXPECT_TRUE(tree.DeleteNode(tree.GetRoot()->pLeft->key));

    if(tree.GetRoot()->pRight == nullptr)
        EXPECT_FALSE(tree.DeleteNode(tree.GetRoot()->pRight->key));
    else
        EXPECT_TRUE(tree.DeleteNode(tree.GetRoot()->pRight->key));
}

TEST_F(EmptyTree, InsertNode)
{
    EXPECT_TRUE(tree.InsertNode(0).first);
    EXPECT_EQ(tree.FindNode(0)->pParent, nullptr);
    tree.DeleteNode(0);

    int random = rand()%5;
    EXPECT_TRUE(tree.InsertNode(random).first);
    EXPECT_EQ(tree.FindNode(random)->key, random);
    EXPECT_EQ(tree.FindNode(random)->pParent, nullptr);
    tree.DeleteNode(random);
    EXPECT_NO_FATAL_FAILURE(tree.FindNode(random));
}

TEST_F(EmptyTree, DeleteNode)
{
    int random = rand()%5;
    EXPECT_NO_THROW(tree.DeleteNode(random));
    EXPECT_NO_FATAL_FAILURE(tree.DeleteNode(random));
    tree.InsertNode(0);
    EXPECT_FALSE(tree.InsertNode(0).first);
    EXPECT_TRUE(tree.DeleteNode(0));
}

TEST_F(EmptyTree, FindNode)
{
    EXPECT_NO_FATAL_FAILURE(tree.FindNode(rand()%5));
    EXPECT_NO_THROW(tree.FindNode(rand()%5));
    tree.InsertNode(0);
    EXPECT_TRUE(tree.FindNode(0));
    tree.DeleteNode(0);
    EXPECT_FALSE(tree.FindNode(0));
}

TEST_F(TreeAxioms, Axiom1)
{
    std::vector<Node_t*> vectors{};
    tree.GetLeafNodes(vectors);
    for (auto item : vectors)
    {
        EXPECT_EQ(item->color, 1);
    }
}

TEST_F(TreeAxioms, Axiom2)
{
        for (size_t i = 1; i < 6; i++)
        {
            if (((tree.FindNode(i)->pLeft) != nullptr) && (tree.FindNode(i)->color == 0))
                EXPECT_EQ(tree.FindNode(i)->pLeft->color, 1);

            if(((tree.FindNode(i)->pRight) != nullptr) && (tree.FindNode(i)->color == 0))
                EXPECT_EQ(tree.FindNode(i)->pRight->color, 1);
        }
}

TEST_F(TreeAxioms, Axiom3)
{
    int Llenght = 0;
    int Rlenght = 0;

    auto Broot1 = tree.GetRoot();
    auto Broot2 = tree.GetRoot();
    
    while ((Broot1 = Broot1->pLeft) != nullptr)
    {
        if(Broot1->color)
        {
            Llenght++;
        }
    }

    while ((Broot2 = Broot2->pRight) != nullptr)
    {
        if(Broot2->color)
        {
            Rlenght++;
        }
    }
    EXPECT_EQ(Rlenght, Llenght);
    
}

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

/*** Konec souboru black_box_tests.cpp ***/
