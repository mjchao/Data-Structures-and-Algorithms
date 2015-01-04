//
//  BPTreeTests.cpp
//  Data Structures
//
//  Created by Mickey on 12/24/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include "BPTreeTests.h"
#include "BPTree.h"

void BPTreeTests::test() {
    /*testNodeInsertKey();
    //testNodeInsertKeyMemory();
    testInsert();
    //testMemoryManagement();
    testRange();
    testCopy();
    testLeftLinkage();//*/
    testRemove();
    testRemoveOrder5();
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
    expected += "[10, 11, 12], [13, 14, 15], [18, 20, 21], [23, 24, 25], [25.1, 25.2, 25.3], ";
    expected +=                     "[26, 30, 31], [33, 45, 47], [48, 50, 52]]";
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
}