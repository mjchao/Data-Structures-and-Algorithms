//
//  BPTreeTests.cpp
//  Data Structures
//
//  Created by Mickey on 12/24/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include "BPTreeTests.h"
#include "BPTree.h"
#include "Comparator.h"
#include <cmath>

void BPTreeTests::test() {
    /*testNodeInsertKey();
    //testNodeInsertKeyMemory();
    testInsert();
    //testMemoryManagement();
    testRange();
    testCopy();
    testLeftLinkage();//
    testRemove();
    testRemoveOrder5();
    testRemoveInsertTestCase();
    testRemoveDistribution();//*/
    testSystem();
    reportTestStatistics( "BPTree" );
}

void BPTreeTests::testNodeInsertKey() {
    string expected;
    string found;
    string errorMessage = "BPNode insertKey() failed!";
    
    //test in order insertion
    BPTree<int , int>::BPNode testNode( 100 );
    
    testNode.insertKeyValue( 1 , 1 );
    expected = "[1]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 2 , 2 );
    expected = "[1, 2]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 3 , 3 );
    expected = "[1, 2, 3]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 4 , 4 );
    expected = "[1, 2, 3, 4]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test reverse order insertion
    testNode = BPTree<int , int>::BPNode( 100 );
    
    testNode.insertKeyValue( 4 , 4 );
    expected = "[4]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 3 , 3 );
    expected = "[3, 4]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 2 , 2 );
    expected = "[2, 3, 4]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 1 , 1 );
    expected = "[1, 2, 3, 4]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test mixed order insertion
    testNode = BPTree<int , int >::BPNode( 100 );

    testNode.insertKeyValue( 10 , 10 );
    expected = "[10]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 1 , 1 );
    expected = "[1, 10]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 5 , 5 );
    expected = "[1, 5, 10]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 3 , 3 );
    expected = "[1, 3, 5, 10]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 7 , 7 );
    expected = "[1, 3, 5, 7, 10]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test repeat inserstions
    testNode = BPTree<int , int>::BPNode( 100 );
    
    testNode.insertKeyValue( 10 , 10 );
    testNode.insertKeyValue( 10 , 10 );
    testNode.insertKeyValue( 10 , 10 );
    testNode.insertKeyValue( 10 , 10 );
    expected = "[10, 10, 10, 10]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 5 , 5 );
    testNode.insertKeyValue( 5 , 5 );
    testNode.insertKeyValue( 5 , 5 );
    expected = "[5, 5, 5, 10, 10, 10, 10]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );
    
    testNode.insertKeyValue( 7 , 7  );
    testNode.insertKeyValue( 7 , 7 );
    testNode.insertKeyValue( 7 , 7 );
    expected = "[5, 5, 5, 7, 7, 7, 10, 10, 10, 10]";
    found = testNode.toString();
    evaluateTest( expected , found , errorMessage );//*/
}

void BPTreeTests::testNodeInsertKeyMemory() {
    
    //run on valgrind
    for ( int i=0 ; i<500 ; i++ ) {
        BPTree< int , int >::BPNode test( 100 );
        for ( int i=0 ; i<99 ; i++ ) {
            test.insertKeyValue( i , i );
        }
    }
}

void BPTreeTests::testInsert() {
    string expected;
    string found;
    string errorMessage = "BPTree insert() failed!";
    BPTree< double , double > test( 5 );
    
    //test unique inserts
    test.insert( 10  , 10 );
    test.insert( 12 , 12 );
    test.insert( 23 , 23 );
    test.insert( 33 , 33 );
    test.insert( 48 , 48 );
    expected = "[[10, 12, 23, 33, 48]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 50 , 50 );
    test.insert( 15 , 15 );
    test.insert( 18 , 18 );
    test.insert( 20 , 20 );
    test.insert( 21 , 21 );
    test.insert( 31 , 31 );
    test.insert( 45 , 45 );
    test.insert( 47 , 47 );
    test.insert( 52 , 52 );
    expected = "[[18, 33, 48]\n";
    expected+="[10, 12, 15], [18, 20, 21, 23, 31], [33, 45, 47], [48, 50, 52]]";
    found = test.toString();
    evaluateTest(  expected , found , errorMessage );
    
    test.insert( 30 , 30 );
    expected = "[[18, 23, 33, 48]\n";
    expected += "[10, 12, 15], [18, 20, 21], [23, 30, 31], [33, 45, 47], ";
    expected +=                                                 "[48, 50, 52]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 24 , 24 );
    test.insert( 25 , 25 );
    test.insert( 26 , 26 );
    expected = "[[18, 23, 26, 33, 48]\n";
    expected += "[10, 12, 15], [18, 20, 21], [23, 24, 25], ";
    expected +=                     "[26, 30, 31], [33, 45, 47], [48, 50, 52]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 11 , 11 );
    test.insert( 13 , 13 );
    test.insert( 14 , 14 );
    expected = "[[26]\n";
    expected += "[13, 18, 23], [33, 48]\n";
    expected += "[10, 11, 12], [13, 14, 15], [18, 20, 21], [23, 24, 25], ";
    expected +=                     "[26, 30, 31], [33, 45, 47], [48, 50, 52]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test adding to the end of a node and see if linkage still holds
    test.insert( 25.1 , 25.1 );
    test.insert( 25.2 , 25.2 );
    test.insert( 25.3 , 25.3 );
    
    expected = "[[26]\n";
    expected += "[13, 18, 23, 25.1], [33, 48]\n";
    expected += "[10, 11, 12], [13, 14, 15], [18, 20, 21], [23, 24, 25], ";
    expected += "[25.1, 25.2, 25.3], [26, 30, 31], [33, 45, 47], [48, 50, 52]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    for ( int i=60 ; i<=100 ; i++ ) {
        test.insert( i ,  i );
    }
    
    int expectedVal;
    int foundVal;
    expectedVal = 60;
    foundVal = test.find( 60 );
    evaluateTest( expectedVal , foundVal , errorMessage );
    
    expectedVal = 0;
    foundVal = test.find( -50 );
    evaluateTest( expectedVal , foundVal , errorMessage );
    
    test.insert( 60 , 999 );
    expectedVal = 999;
    foundVal = test.find( 60 );
    evaluateTest( expectedVal , foundVal , errorMessage );
}

void BPTreeTests::testMemoryManagement() {
    
    //run on valgrind
    for ( int i=0 ; i<10 ; i++ ) {
        BPTree<int , int> test;
        for ( int j=0 ; j<10000 ; j++ ) {
            test.insert( j , j );
        }
    }
}

void BPTreeTests::testRange() {
    BPTree< int , int > test;
    vector< int > expected;
    vector< int > found;
    string errorMessage = "BPTree range() failed!";
    
    //TEST CASE 1: test empty tree
    test.clear();
    expected = vector< int >();
    found = test.range( -99999 , 99999 );
    evaluateTest( expected , found , errorMessage );
    
    //TEST CASE 2: test a tree with 1 element
    test.clear();
    test.insert( 1 , 1 );
    expected = vector< int >();
    expected.push_back( 1 );
    
    //check the boundaries for inclusiveness
    found = test.range( -1 , 1 );
    evaluateTest( expected , found , errorMessage );
    
    found = test.range( 1 , 23 );
    evaluateTest( expected , found , errorMessage );
    
    found = test.range(-23 , 25 );
    evaluateTest( expected , found , errorMessage );
    
    //TEST CASE 3: test tree with many elements
    test.clear();
    //insert 25, 30, ... , 55, 60
    for ( int i=25 ; i<=60 ; i+= 5 ) {
        test.insert( i , i );
    }
    
    //insert -18, -15, ... , 0
    for ( int i=-18 ; i<=0 ; i+= 3 ) {
        test.insert( i , i );
    }
    
    //overwrite -18, 0, and 60
    test.insert( -18 , -9999 );
    test.insert( 0 , 1 );
    test.insert( 60 , 9999 );
    
    //check lower range
    expected = vector< int >();
    expected.push_back( -12 );
    expected.push_back( -9 );
    found = test.range( -12 , -9 );
    evaluateTest( expected , found , errorMessage );
    
    //check the lower range and its overwritten key of 18
    expected = vector< int >();
    expected.push_back( -9999 );
    expected.push_back( -15 );
    expected.push_back( -12 );
    found = test.range( -18 , -11 );
    evaluateTest( expected , found , errorMessage );
    
    //check the middle range and its overwritten key of 0
    expected = vector< int >();
    expected.push_back( -3 );
    expected.push_back( 1 );
    expected.push_back( 25 );
    expected.push_back( 30 );
    found = test.range( -5 , 33 );
    evaluateTest( expected , found , errorMessage );
    
    //check the upper range and its overwritten key of 60
    expected = vector< int >();
    expected.push_back( 50 );
    expected.push_back( 55 );
    expected.push_back( 9999 );
    found = test.range( 46 , 1000 );
    evaluateTest( expected , found , errorMessage );
    
    //TEST CASE 4: Check that values do not need to be in increasing order
    //with the keys
    test = BPTree< int , int >();
    test.insert( -9 , -86 );
    test.insert( -2 , 87 );
    test.insert( 0 , 12 );
    test.insert( 23 , 48 );
    test.insert( 43 , 39 );
    test.insert( 50 , -3 );
    
    expected = vector< int >();
    expected.push_back( -86 );
    expected.push_back( 87 );
    expected.push_back( 12 );
    expected.push_back( 48 );
    expected.push_back( 39 );
    expected.push_back( -3 );
    found = test.range( -100 , 100 );
    evaluateTest( expected , found , errorMessage );
}

void BPTreeTests::testCopy() {
    
    string errorMessage = "BPTree operator= failed!";
    BPTree< int , int >* original = new BPTree< int , int >( 3 );
    original->insert( 1 , 1 );
    original->insert( 2 , 2 );
    original->insert( 3 , 3 );
    original->insert( 4 , 4 );
    original->insert( 5 , 5 );
    original->insert( 6 , 6 );
    
    //test the copying produces identical trees
    BPTree< int , int >* copy = new BPTree< int , int >;
    *copy = *original;
    evaluateTest( original->toString() , copy->toString() , errorMessage );
    
    //test that the copy has no pointers/references to the original
    copy->insert( 7 , 7 );
    evaluateTest( original->toString()==copy->toString(), false, errorMessage );
    
    //test that the original has no pointers/references to the copy
    original->insert( 8 , 8 );
    delete original;
    
    vector< int > expRng;
    expRng.push_back( 4 );
    expRng.push_back( 5 );
    expRng.push_back( 6 );
    expRng.push_back( 7 );
    vector< int > foundRng = copy->range( 4 , 10 );
    evaluateTest( expRng , foundRng , errorMessage );
    
    //test that the copy constructor works properly
    BPTree< int , int >* copycopy = new BPTree< int , int >( *copy );
    evaluateTest( copycopy->toString() , copy->toString() , errorMessage );
    delete copycopy;
    
    delete copy;
    
    BPTree< int , int >* bigTree = new BPTree< int , int >( 3 );
    for ( int i=0 ; i<100000 ; i++ ) {
        bigTree->insert( i , i );
    }
    
    BPTree< int , int >* bigCopy = new BPTree< int , int >( 3 );
    *bigCopy = *bigTree;
    evaluateTest( bigTree->toString() , bigCopy->toString() , errorMessage );
    
    delete bigTree;
    delete bigCopy;
}

void BPTreeTests::testLeftLinkage() {
    string expected;
    string found;
    string errorMessage = "BPTree left linkage failed!";
    
    BPTree< double , double > test( 5 );
    test.insert( 10 , 10 );
    test.insert( 12 , 12 );
    test.insert( 23 , 23 );
    test.insert( 33 , 33 );
    test.insert( 48 , 48 );
    
    expected = "[[48, 33, 23, 12, 10]]";
    found = test.toStringLeft();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 79 , 79 );
    expected = "[[33]\n";
    expected +="[79, 48, 33], [23, 12, 10]]";
    found = test.toStringLeft();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 34 , 34 );
    test.insert( 0 , 0 );
    test.insert( -100 , -100 );
    expected = "[[33]\n";
    expected +="[79, 48, 34, 33], [23, 12, 10, 0, -100]]";
    found = test.toStringLeft();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 32 , 32 );
    expected = "[[33, 12]\n";
    expected+= "[79, 48, 34, 33], [32, 23, 12], [10, 0, -100]]";
    found = test.toStringLeft();
    evaluateTest( expected , found , errorMessage );
    
    test = BPTree< double , double >( 2 );
    test.insert( 10 , 10 );
    test.insert( 12 , 12 );
    test.insert( 23 , 23 );
    test.insert( 33 , 33 );
    test.insert( 48 , 48 );
    expected = "[[23]\n";
    expected +="[33], [12]\n";
    expected +="[48, 33], [23], [12], [10]]";
    found = test.toStringLeft();
    evaluateTest( expected , found , errorMessage );
    
    test.insert( 79 , 79 );
    test.insert( 34 , 34 );
    test.insert( 0 , 0 );
    test.insert( -100 , -100 );
    test.insert( 32 , 32 );
    expected = "[[23]\n";
    expected +="[48, 33], [12, 0]\n";
    expected +="[79, 48], [34, 33], [32, 23], [12], [10, 0], [-100]]";
    found = test.toStringLeft();
    evaluateTest( expected , found , errorMessage );
    
    BPTree< double , double >* testCopy = new BPTree< double , double >( test );
    string copyLeftRep = testCopy->toStringLeft();
    evaluateTest( copyLeftRep , test.toStringLeft(), errorMessage );
    delete testCopy;
    evaluateTest( copyLeftRep , test.toStringLeft() , errorMessage );
    
    //repeat the testInsertion() tests, but make sure that the left linkage is
    //correct
    test = BPTree< double , double >( 5 );
    test.insert( 10  , 10 );
    test.insert( 12 , 12 );
    test.insert( 23 , 23 );
    test.insert( 33 , 33 );
    test.insert( 48 , 48 );
    test.insert( 50 , 50 );
    test.insert( 15 , 15 );
    test.insert( 18 , 18 );
    test.insert( 20 , 20 );
    test.insert( 21 , 21 );
    test.insert( 31 , 31 );
    test.insert( 45 , 45 );
    test.insert( 47 , 47 );
    test.insert( 52 , 52 );
    test.insert( 30 , 30 );
    test.insert( 24 , 24 );
    test.insert( 25 , 25 );
    test.insert( 26 , 26 );
    test.insert( 11 , 11 );
    test.insert( 13 , 13 );
    test.insert( 14 , 14 );
    test.insert( 25.1 , 25.1 );
    test.insert( 25.2 , 25.2 );
    test.insert( 25.3 , 25.3 );
    expected = "[[26]\n";
    expected +="[48, 33], [25.1, 23, 18, 13]\n";
    expected +="[52, 50, 48], [47, 45, 33], [31, 30, 26], [25.3, 25.2, 25.1], ";
    expected +="[25, 24, 23], [21, 20, 18], [15, 14, 13], [12, 11, 10]]";
    found = test.toStringLeft();
    evaluateTest( expected , found , errorMessage );
}

void BPTreeTests::testRemove() {
    string expected;
    string found;
    string errorMessage = "BPTree remove() failed!";
    BPTree< double , double > test( 2 );
    for ( int i=0 ; i<8 ; i++ ) {
        test.insert( i , i );
    }
    /* tree structure:
                [2     ,     4]
          [1]         [3]        [5  ,  6]
      [0]     [1]  [2]   [3]   [4]  [5]   [6, 7]
     */
    
    //test borrowing form the right by removing 5:
    //removing 5 should borrow a 6
    test.remove( 5 );
    
    /* tree structure after removing 5:
               [2     ,     4]
         [1]         [3]        [5  ,  7]
     [0]     [1]  [2]   [3]   [4]  [6]   [7]
     */
    expected = "[[2, 4]\n";
    expected +="[1], [3], [5, 7]\n";
    expected +="[0], [1], [2], [3], [4], [6], [7]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.clear();
    for ( int i=0 ; i<16 ; i+= 2 ) {
        test.insert( i , i );
    }
    test.insert( 5 , 5 );
    
    /* tree structure:
               [4       ,     8]
         [2]           [6]        [10  , 12]
     [0]     [2]  [4, 5]   [6]   [8]  [10]   [12, 14]
     */
    
    //test borrowing from the left by removing 6:
    //removing 6 should borrow a 5
    test.remove( 6 );
    /* tree structure:
               [4       ,     8]
         [2]           [5]         [10  , 12]
     [0]     [2]   [4]     [5]   [8]  [10]   [12, 14]
     */
    expected = "[[4, 8]\n";
    expected+= "[2], [5], [10, 12]\n";
    expected+= "[0], [2], [4], [5], [8], [10], [12, 14]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test borrowing without underflow
    //removing 14 should not cause any borrowing or merging
    test.remove( 14 );
    
    /* tree structure:
               [4       ,     8]
         [2]           [5]         [10  , 12]
     [0]     [2]   [4]     [5]   [8]  [10]   [12]
     */
    expected = "[[4, 8]\n";
    expected+= "[2], [5], [10, 12]\n";
    expected+= "[0], [2], [4], [5], [8], [10], [12]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test 1 level merging with left sibling by removing 12:
    //removing 12 should cause both 12s in the tree to be removed
    test.remove( 12 );
    
    /* tree structure:
              [4       ,      8]
        [2]            [5]           [10]
     [0]     [2]   [4]     [5]   [8]      [10]
     */
    expected = "[[4, 8]\n";
    expected+= "[2], [5], [10]\n";
    expected+= "[0], [2], [4], [5], [8], [10]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //put 12 back in and then test 1 level merging with right sibling by
    //removing 8
    //removing 8 should cause the 8 and the 10 to be removed
    test.insert( 12 , 12 );
    test.remove( 8 );
    
    /* tree structure:
                [4       ,      8]
         [2]            [5]           [12]
     [0]     [2]   [4]     [5]   [10]      [12]
     */
    expected = "[[4, 8]\n";
    expected+= "[2], [5], [12]\n";
    expected+= "[0], [2], [4], [5], [10], [12]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //make all leaf nodes have two entries
    test.insert( 6 , 6 );
    test.remove( 5 );
    test.insert( 8 , 8 );
    test.remove( 12 );
    test.insert( 1 , 1 );
    test.insert( 3 , 3 );
    test.insert( 4.5 , 4.5 );
    test.insert( 7 , 7 );
    test.insert( 9 , 9 );
    test.insert( 11 , 11 );
    
    /* tree structure
                      [4          ,          8]
               [2]               [5]               [10]
         [0, 1]   [2, 3] [4, 4.5]   [6, 7]   [8, 9]    [10, 11]
     */
    
    //remove every other leaf node and test multiple leaf node
    //merges with siblings
    test.remove( 0 );
    test.remove( 1 );
    test.remove( 4 );
    test.remove( 4.5 );
    test.remove( 9 );
    test.remove( 8 );
    
    /* tree structure
                        [4          ,          8]
                [3]               [7]               [11]
            [2]     [3]       [6]     [7]       [10]    [11]
     */
    expected = "[[4, 8]\n";
    expected +="[3], [7], [11]\n";
    expected +="[2], [3], [6], [7], [10], [11]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //remove 11, causing the 11 non-leaf to be deleted and 8 to be demoted
    test.remove( 11 );
    /* tree structure
                 [4]
         [3]               [7   ,    8]
     [2]     [3]       [6]     [7]       [10]
     */
    expected = "[[4]\n";
    expected +="[3], [7, 8]\n";
    expected +="[2], [3], [6], [7], [10]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //remove 10, causing the [7, 8] node to shrink, but no merges are forced
    test.remove( 10 );
    /* tree structure
                 [4]
         [3]               [7]
     [2]     [3]       [6]     [7]
     */
    expected = "[[4]\n";
    expected +="[3], [7]\n";
    expected +="[2], [3], [6], [7]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //remove 7, causing the 7 non-leaf node to be deleted, the 4 to be
    //demoted, and the root to collapse into [3, 4]
    test.remove( 7 );
    /* tree structure
         [3    ,    4]
     [2]     [3]       [6]
     */
    expected = "[[3, 4]\n";
    expected +="[2], [3], [6]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //remove 6, causing the root to shrink, but no merges are forced
    test.remove( 6 );
    /* tree structure
         [3]
     [2]     [3]
     */
    expected = "[[3]\n";
    expected +="[2], [3]]";
    
    //remove 3, causing the root to collapse and be replaced by just the 2
    test.remove( 3 );
    /* tree structure
         [2]
     */
    expected = "[[2]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //remove the last element in the tree - straightforward
    test.remove( 2 );
    /* tree structure
        []
     */
    expected = "[[]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void BPTreeTests::testRemoveOrder5() {
    string expected;
    string found;
    string errorMessage = "BPTree remove() failed for order 5!";
    BPTree< double , double > test( 5 );
    for ( int i=0 ; i<10 ; i++ ) {
        test.insert( i ,  i );
    }
    /* tree structure:
                 [4,        6]
     [0, 1, 2, 3]    [4, 5]     [6, 7, 8, 9]
     */
    
    //remove up to first merge, resulting in demotion of the 4 in the root
    test.remove( 0 );
    test.remove( 1 );
    /* tree structure:
                [6]
     [2, 3, 4, 5]  [6, 7, 8, 9]
     */
    expected = "[[6]\n";
    expected+= "[2, 3, 4, 5], [6, 7, 8, 9]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //remove up to second merge, which is a redistribution
    test.remove( 2 );
    test.remove( 3 );
    /* tree structure:
                [7]
     [4, 5, 6]      [7, 8, 9]
     */
    expected = "[[7]\n";
    expected+= "[4, 5, 6], [7, 8, 9]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //remove up to third merge, resulting in root collapsing
    test.remove( 4 );
    /* tree structure
       [5, 6, 7, 8, 9]
     */
    expected = "[[5, 6, 7, 8, 9]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //remove the rest, ensuring that no errors occur
    for ( int i=0 ; i<=9 ; i++ ) {
        test.remove( i );
    }
    expected = "[[]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

class DoubleComparator : public Comparator< double > {
    
public:
    DoubleComparator() {
        
    }
    
    virtual int compare( const double& v1 , const double& v2 ) const {
        if ( std::abs( v2-v1 ) < 0.00001 ) {
            return 0;
        }
        else {
            if ( v1-v2 < 0 ) {
                return -1;
            }
            else {
                return 1;
            }
        }
    }
};

void BPTreeTests::testRemoveInsertTestCase() {
    DoubleComparator comparator;
    string expected;
    string found;
    string errorMessage = "BPTree remove() failed for insert test case!";
    BPTree< double , double > test( 5 , &comparator );
    
    //use the insert test case data, except we remove everything now
    test.insert( 10  , 10 );
    test.insert( 12 , 12 );
    test.insert( 23 , 23 );
    test.insert( 33 , 33 );
    test.insert( 48 , 48 );
    test.insert( 50 , 50 );
    test.insert( 15 , 15 );
    test.insert( 18 , 18 );
    test.insert( 20 , 20 );
    test.insert( 21 , 21 );
    test.insert( 31 , 31 );
    test.insert( 45 , 45 );
    test.insert( 47 , 47 );
    test.insert( 52 , 52 );
    test.insert( 30 , 30 );
    test.insert( 24 , 24 );
    test.insert( 25 , 25 );
    test.insert( 26 , 26 );
    test.insert( 11 , 11 );
    test.insert( 13 , 13 );
    test.insert( 14 , 14 );
    test.insert( 25.1 , 25.1 );
    test.insert( 25.2 , 25.2 );
    test.insert( 25.3 , 25.3 );
    /* tree structure
                                                                  [26]
           [13     ,    18    ,     23     ,    25.1                ]         [33 ,         48]
 [10,11,12]   [13,14,15]  [18,20,21]  [23,24,25]    [25.1,25.2,25.3] [26,30,31]  [33, 45, 47]  [48,50,52]
     
     */
    
    //try removing 25.1, which will cause the [25.1,25.2,25.3] node to merge
    //with the [23,24,25] node
    test.remove( 25.1 );
    
    /* tree structure
                                                         [26]
            [13     ,    18    ,     23]                              [33 ,         48]
  [10,11,12]   [13,14,15]  [18,20,21]  [23,24,25,25.2,25.3] [26,30,31]  [33, 45, 47]  [48,50,52]
     
     */
    expected = "[[26]\n";
    expected +="[13, 18, 23], [33, 48]\n";
    expected +="[10, 11, 12], [13, 14, 15], [18, 20, 21], [23, 24, 25, 25.2, 25.3], ";
    expected +="[26, 30, 31], [33, 45, 47], [48, 50, 52]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //try removing 25.2 and 25.3 which will not cause any underflowing
    test.remove( 25.2 );
    test.remove( 25.3 );
    /* tree structure
                                                [26]
             [13     ,    18    ,     23]                   [33 ,         48]
   [10,11,12]   [13,14,15]  [18,20,21]  [23,24,25] [26,30,31]  [33, 45, 47]  [48,50,52]
     
     */
    
    //try removing 52, which will cause the [48, 50, 52] to underflow
    //the remaining [48, 50] will have to merge with [33, 45, 47]
    //the parent node [33, 48] will lose its 48 key and have to collapse the
    //root and with [13, 18, 23] to become [13, 18, 23, 26, 33]
    test.remove( 52 );
    /* tree structure
               [13     ,    18    ,     23     ,    26    ,   33]
     [10,11,12]   [13,14,15]  [18,20,21]  [23,24,25] [26,30,31]  [33,45,47,48,50]
     */
    expected = "[[13, 18, 23, 26, 33]\n";
    expected +="[10, 11, 12], [13, 14, 15], [18, 20, 21], [23, 24, 25], ";
    expected +="[26, 30, 31], [33, 45, 47, 48, 50]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //check that the left linkage is still correct
    expected = "[[33, 26, 23, 18, 13]\n";
    expected +="[50, 48, 47, 45, 33], [31, 30, 26], [25, 24, 23], [21, 20, 18], ";
    expected +="[15, 14, 13], [12, 11, 10]]";
    found = test.toStringLeft();
    evaluateTest( expected , found , errorMessage );
    
    //try removing 31, which should cause a simple redistribution between
    //[26, 30, 31] and [33, 45, 47, 48, 50]
    test.remove( 31 );
    /* tree structure
               [13     ,    18    ,     23     ,    26     ,     47]
     [10,11,12]   [13,14,15]  [18,20,21]  [23,24,25] [26,30,33,45]  [47,48,50]
     */
    expected = "[[13, 18, 23, 26, 47]\n";
    expected +="[10, 11, 12], [13, 14, 15], [18, 20, 21], [23, 24, 25], ";
    expected +="[26, 30, 33, 45], [47, 48, 50]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //try removing 10, which should cause a merge of [11, 12] with [13, 14, 15]
    //and cause 13 to be removed
    test.remove( 10 );
    /* tree structure
                    [ 18    ,     23     ,    26     ,     47]
     [11,12,13,14,15]  [18,20,21]  [23,24,25] [26,30,33,45]  [47,48,50]
     */
    expected = "[[18, 23, 26, 47]\n";
    expected +="[11, 12, 13, 14, 15], [18, 20, 21], [23, 24, 25], ";
    expected +="[26, 30, 33, 45], [47, 48, 50]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );

    //try removing 25, which should cause a redistribution between
    //[23, 24, 25] and [26, 30, 33, 45]
    test.remove( 25 );
    /* tree structure
                    [18    ,     23     ,    30     ,   47]
     [11,12,13,14,15]  [18,20,21]  [23,24,26] [30,33,45]  [47,48,50]
     */
    expected = "[[18, 23, 30, 47]\n";
    expected +="[11, 12, 13, 14, 15], [18, 20, 21], [23, 24, 26], [30, 33, 45], ";
    expected +="[47, 48, 50]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //remove from [11, 12, 13, 14, 15] until it has 3 elements left
    //thus, any further removals will force the tree to rearrange
    test.remove( 11 );
    test.remove( 12 );
    
    //try removing 18. this will cause another merge and demotion
    test.remove( 18 );
    /* tree structure
                    [23     ,    30     ,   47]
     [13,14,15,20,21]  [23,24,26] [30,33,45]  [47,48,50]
     */
    expected = "[[23, 30, 47]\n";
    expected +="[13, 14, 15, 20, 21], [23, 24, 26], [30, 33, 45], [47, 48, 50]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //13-20 and 47-50
    for ( int i=13 ; i<=20 ; i++ ) {
        test.remove( i );
    }
    for ( int i=47 ; i<=50 ; i++ ) {
        test.remove( i );
    }
    /* tree structure
                         [30]
            [21, 23,24,26] [30,33,45]
     */
    expected = "[[30]\n";
    expected +="[21, 23, 24, 26], [30, 33, 45]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //try removing 30 and check that the single entry in the root will be
    //modified after the redistribution
    test.remove( 30 );
    expected = "[[26]\n";
    expected +="[21, 23, 24], [26, 33, 45]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //remove all but 45
    for ( int i=21 ; i<45 ; i++ ) {
        test.remove( i );
    }
    expected = "[[45]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test.remove( 45 );
    expected = "[[]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void BPTreeTests::testRemoveDistribution() {
    DoubleComparator comparator;
    string expected;
    string found;
    string errorMessage = "BPTree remove() non-leaf distribution failed!";
    BPTree< double , double > test( 6 );
    
    //insert all multiples of 2 from 0 to 100
    for ( int i=100 ; i>=0 ; i-=2 ) {
        test.insert( i , i );
    }
    
    //then remove all the multiples of 4, so we should only have
    //multiples of 2 that are not multiples of 4.
    for ( int i=0 ; i<=96 ; i+=4 ) {
        test.remove( i );
    }
    test.remove( 100 );
    /* tree structure
                                             [50]
            [14,        26,       38]                         [70,        82,        94]
     [2,6,10] [14,18,22] [26,30,34] [48,42,46] [50,54,58,62,66] [70,74,78] [82,86,90] [94,98,100]
     */
    //removing 100 should cause a leaf distribution between [94, 98] and
    //[82, 86, 90]. This is a left sibling giving to a right sibling
    test.remove( 100 );
    expected = "[[14, 26, 38, 50, 70, 82]\n";
    expected +="[2, 6, 10], [14, 18, 22], [26, 30, 34], [38, 42, 46], ";
    expected +="[50, 54, 58, 62, 66], [70, 74, 78], [82, 86, 90, 94, 98]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test distributing non-leaf entries with the right giving entries and the
    //left receives them
    test = BPTree< double , double >( 4 );
    
    //add all multiples of 5 from 0 to 75
    for ( int i=0 ; i<=80 ; i+= 5 ) {
        test.insert( i , i );
    }
    /* tree structure
                      [30]
     [10   ,  20]              [40   ,   50    ,   60   ,   70  ]
[0, 5] [10, 15] [20, 25] [30, 35] [40, 45] [50, 55] [60, 65] [70, 75, 80]
     */
    
    //now, removing 5 will cause [0] to merge with [10, 15]. this deletes the
    //10 in [10, 20]. Since [20] is underflowing, it will be combined with
    //[40, 50, 60, 70] to make [20, 40, 50, 60, 70]. Then, the
    //entries will be redistributed has [20, 40, 50] and [60, 70]; however,
    //reindexing the keys will need to give us [20, 30, 40] and [60, 70],
    //as follows
    test.remove( 5 );
    
    /* tree structure
                                           [50]
              [20   ,    30    ,  40]                [60   ,  70]
     [0, 10, 15] [20, 25] [30, 35]  [40, 45]  [50, 55] [60, 65] [70, 75 80]
     */
    expected = "[[50]\n";
    expected +="[20, 30, 40], [60, 70]\n";
    expected +="[0, 10, 15], [20, 25], [30, 35], [40, 45], [50, 55], [60, 65], ";
    expected +="[70, 75, 80]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test distributing non-leaf entries with the left giving entries to the
    //right
    
    //now, removing 60 will cause the remaining [65] to merge with [50, 55].
    //this deletes [60] from the [60, 70] parent node. The remaining [70] will
    //gain a [40] from its left sibling, as follows
    test.remove( 50 );
    
    /* tree structure
                                 [40]
               [20  ,   30]               [50     ,     70]
     [0, 10, 15] [20, 25] [30, 35] [40, 45] [55, 60, 65], [70, 75, 80]
     */
    expected = "[[40]\n";
    expected +="[20, 30], [50, 70]\n";
    expected +="[0, 10, 15], [20, 25], [30, 35], [40, 45], [55, 60, 65], [70, 75, 80]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test = BPTree< double , double >( 5 );
    for ( int i=0 ; i<=80 ; i+=5 ) {
        test.insert( i , i );
    }
    test.insert( 1 , 1 );
    test.insert( 2 , 2 );
    test.insert( 3 , 3 );
    test.insert( 4 , 4 );
    test.insert( 6 , 6 );
    test.insert( 7 , 7 );
    test.insert( 8 , 8 );
    test.insert( 9 , 9 );
    test.insert( 11 , 11 );
    test.insert( 12 , 12 );
    test.insert( 13 , 13 );
    test.insert( 14 , 14 );
    /* tree structure
                                                     [30]
      [3   ,   6   ,   9    ,    12    ,    15]                  [45    ,    60]
[0,1,2] [3,4,5] [6,7,8] [9,10,11] [12,13,14] [15,20,25] [30,35,40] [45,50,55] [60,65,70,75,80]
     */
    //now, removing 30 will cause the node [35, 40] to merge with [45, 50, 55]
    //and this in turn will cause 45 to be removed from the parent [45, 60]
    //since [60] is underflowing, we get redistribution from [3,6,9,12,15],
    //which lends [12,15] to its sibling. The [12, 15] will be re-indexed to
    //15 and 30, and the parent will be re-indexed to 12, as can be seen below:
    test.remove( 30 );
    /* tree structure
                               [12]
     [3   ,   6   ,   9 ]                  [15   ,    30       ,        60]
[0,1,2] [3,4,5] [6,7,8] [9,10,11] [12,13,14] [15,20,25] [35,40,45,50,55] [60,65,70,75,80]
     */
    expected = "[[12]\n";
    expected +="[3, 6, 9], [15, 30, 60]\n";
    expected +="[0, 1, 2], [3, 4, 5], [6, 7, 8], [9, 10, 11], [12, 13, 14], ";
    expected +="[15, 20, 25], [35, 40, 45, 50, 55], [60, 65, 70, 75, 80]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //test redistribution where the underflowing node has both a left and
    //right sibling, but chooses to redistribute with right because it has
    //more entries
    test = BPTree< double , double >( 2 );
    for ( int i=0 ; i<16 ; i++ ) {
        test.insert( i , i );
    }
    /* tree structure
                   [4       ,       8]
           [2]             [6]             [10  ,   13]
       [1]     [3]     [5]     [7]     [9]     [11]      [13 , 14]
     [0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [11] [12] [13] [14,15]
     */
    //now, removing 11 will cause it to merge with its leaf sibling [10].
    //this deletes the [11] parent node, which underflows. It has a choice
    //to try and redistribute with the left or the right. it chooses the
    //right because it has more entries, and the new redistribution is
    //shown below:
    
    test.remove( 11 );
    /* tree structure
                   [4       ,       8]
           [2]             [6]             [10  ,   13]
       [1]     [3]     [5]     [7]     [9]     [12]      [14]
     [0] [1] [2] [3] [4] [5] [6] [7] [8] [9] [10] [12] [13] [14,15]
     */
    expected = "[[4, 8]\n";
    expected +="[2], [6], [10, 13]\n";
    expected +="[1], [3], [5], [7], [9], [12], [14]\n";
    expected +="[0], [1], [2], [3], [4], [5], [6], [7], [8], [9], [10], [12], [13], [14, 15]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //now, removing 9 will cause the [9] non-leaf node to underflow.
    //it will have the choice of merging either left or right, but it must
    //choose right because the left node does not share the same parent
    test.remove( 9 );
    /* tree structure
                   [4       ,       8]
           [2]             [6]                 [13]
       [1]     [3]     [5]     [7]      [10, 12]      [14]
     [0] [1] [2] [3] [4] [5] [6] [7] [8]  [10] [12] [13] [14,15]
     */
    expected = "[[4, 8]\n";
    expected +="[2], [6], [13]\n";
    expected +="[1], [3], [5], [7], [10, 12], [14]\n";
    expected +="[0], [1], [2], [3], [4], [5], [6], [7], [8], [10], [12], [13], [14, 15]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    test = BPTree< double , double >( 4 );
    for ( int i=0 ; i<13 ; i++ ) {
        test.insert( i , i );
    }
    for ( int i=0 ; i<6 ; i++ ) {
        double toInsert = static_cast<double>(i) * 0.1;
        test.insert( toInsert , toInsert );
    }
    test.remove( 10 );
    /* tree structure
                                                   [6]
           [0.2   ,    0.4      ,    2    , 4]           [8   ,   10 ]
     [0, 0.1] [0.2, 0.3] [0.4, 0.5, 1] [2, 3] [4, 5] [6, 7] [8, 9], [11, 12]
     */
    //removing 11 will cause a redistribution, but the node underflowed due to
    //the removal of a non-leading entry, 10.
    test.remove( 11 );
    /* tree structure
                                            [4]
            [0.2   ,    0.4      ,    2 ]          [6   ,  8]
     [0, 0.1] [0.2, 0.3] [0.4, 0.5, 1] [2, 3] [4, 5] [6, 7] [8, 9, 12]
     */
    expected = "[[4]\n";
    expected +="[0.2, 0.4, 2], [6, 8]\n";
    expected +="[0, 0.1], [0.2, 0.3], [0.4, 0.5, 1], [2, 3], [4, 5], [6, 7], [8, 9, 12]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
}

void BPTreeTests::testSystem() {
    string expected;
    string found;
    vector< double > expectedValues;
    vector< double > foundValues;
    string errorMessage = "BPTree system test failed!";
    DoubleComparator comparator;
    BPTree< double , double > test ( 5 , &comparator );
    
    //insert 1 to 100
    for ( int i=1 ; i<=100 ; i++ ) {
        test.insert( i , i );
    }
    
    //remove all but the multiples of 7
    for ( int i=1 ; i<=100 ; i++ ) {
        if ( i%7 == 0 ) {
            continue;
        }
        test.remove( i );
    }
    
    for ( int i=1 ; i<=14 ; i++ ) {
        expectedValues.push_back( i*7 );
    }
    foundValues = test.range( 0 , 100 );
    evaluateTest( expectedValues , foundValues , errorMessage );
    
    /* tree structure
             [28    ,    49    ,    70]
     [7,14,21] [28,35,42] [49,56,63] [70,77,84,91,98]
     */
    //remove a 14 and watch the tree merge
    test.remove( 14 );
    expectedValues.erase( expectedValues.begin()+1 );
    foundValues = test.range( 0 , 100 );
    evaluateTest( expectedValues , foundValues , errorMessage );
    
    /* tree structure
                   [49    ,   70]
     [7,21,28,35,42] [49,56,63] [70,77,84,91,98]
     */
    
    //remove a 56 and watch the tree redistribute
    test.remove( 56 );
    expectedValues.erase( expectedValues.begin()+6 );
    foundValues = test.range( 0 , 100 );
    evaluateTest( expectedValues , foundValues , errorMessage );
    
    /* tree structure
                [42    ,   70]
     [7,21,28,35] [42,49,63] [70,77,84,91,98]
     */
    
    test.remove( 49 );
    test.remove( 63 );
    /* tree structure
                [42    ,      84]
     [7,21,28,35] [42,70,77] [84,91,98]
     */
    expected = "[[42, 84]\n";
    expected +="[7, 21, 28, 35], [42, 70, 77], [84, 91, 98]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //watch tree merge
    test.remove( 70 );
    test.remove( 42 );
    expected = "[[84]\n";
    expected +="[7, 21, 28, 35, 77], [84, 91, 98]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    expectedValues = vector< double >();
    expectedValues.push_back( 77 );
    expectedValues.push_back( 84 );
    foundValues = test.range( 70 , 85 );
    evaluateTest( expectedValues , foundValues , errorMessage );
    
    
    //cause a insertion split again
    test.insert( 6 , 6 );
    expected = "[[28, 84]\n";
    expected +="[6, 7, 21], [28, 35, 77], [84, 91, 98]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //repeatedly merge then split then merge then split...
    //nothing should have changed
    for ( int i=0 ; i<10 ; i++ ) {
        test.remove( 6 );
        test.insert( 6 ,  6);
    }
    for ( int i=0 ; i<10 ; i++ ) {
        test.remove( 21 );
        test.insert( 21 , 21 );
    }
    for ( int i=0 ; i<10 ; i++ ) {
        test.remove( 6 );
        test.remove( 7 );
        test.remove( 21 );
        test.insert( 7 , 7 );
        test.insert( 21 , 21 );
        test.insert( 6 , 6 );
    }
    evaluateTest( expected , found , errorMessage );
    
    //test tree structure similar to the 2-3 tree
    test = BPTree< double , double >( 2 );
    
    //test empty tree representation
    expected = "[[]]";
    found = test.toString();
    evaluateTest( expected , found , errorMessage );
    
    //cout << test.toString() << endl;
    //cout << test.toStringLeft() << endl;
}