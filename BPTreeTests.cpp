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
    evaluateTest( expected , found , errorMessage );
}

void BPTreeTests::testNodeInsertKeyMemory() {
    cout << "Prepare to observe memory usage. Press Enter to continue." << endl;
    getchar();
    for ( int i=0 ; i<100000 ; i++ ) {
        BPTree< int , int >::BPNode test( 100 );
        for ( int i=0 ; i<50 ; i++ ) {
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
    BPTree< int , int > test( 5 );
    
    //currently just stepping through with debugger to ensure correctness
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
    test.insert( 53 , 53 );//*/
}