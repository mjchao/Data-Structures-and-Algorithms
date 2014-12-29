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
    testNodeInsertKey();
    //testNodeInsertKeyMemory();
    testInsert();
    //testMemoryManagement();
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
    cout << "Prepare to observe memory usage. Press Enter to continue." << endl;
    getchar();
    for ( int i=0 ; i<500000 ; i++ ) {
        BPTree< int , int >::BPNode test( 100 );
        for ( int i=0 ; i<99 ; i++ ) {
            test.insertKeyValue( i , i );
        }
    }
    cout << "Was memory usage constant? (y/n)" << endl;
    char expected = 'y';
    char found = tolower( getchar() );
    string errorMessage = "BPNode does not free memory!";
    evaluateTest( expected , found , errorMessage );
}

void BPTreeTests::testInsert() {
    string expected;
    string found;
    string errorMessage = "BPTree insert() failed!";
    BPTree< int , int > test( 5 );
    
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
    expected += "[13, 18, 23], [26, 33, 48]\n";
    expected += "[10, 11, 12], [13, 14, 15], [18, 20, 21], [23, 24, 25], ";
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
    cout << "Prepare to observe memory usage. Press enter to continue.";
    getchar();
    for ( int i=0 ; i<1000 ; i++ ) {
        BPTree<int , int> test;
        for ( int j=0 ; j<10000 ; j++ ) {
            test.insert( j , j );
        }
    }
    cout << "Was memory usage constant? (y/n)";
    char expected = 'y';
    char found = tolower( getchar() );
    evaluateTest( expected , found , "BPTree memory management failed!" );
}