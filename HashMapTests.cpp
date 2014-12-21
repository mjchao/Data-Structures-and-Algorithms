//
//  HashMapTests.cpp
//  Data Structures
//
//  Created by Mickey on 11/3/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include "HashMap.h"
#include "HashMapTests.h"
#include "DefaultHasher.h"
#include <vector>
using std::vector;

void HashMapTests::test() {
    testDefaultHasher();
    testPut();
    testGet();
    testRemove();
    //testDestructor();
    testResize();
    testClear();
    reportTestStatistics( "HashMap" );
}

void HashMapTests::testDefaultHasher() {
    DefaultHasher< int > test;
    vector< long long > previousAddresses;
    
    //test generating hashcodes for 10000 different integers. since
    //they all have different memory addresses, they should
    //all have different hashcodes
    int NUM_ADDRESSES_TO_TEST = 10000;
    int* testInts[ NUM_ADDRESSES_TO_TEST ];
    for ( int i=0 ; i<NUM_ADDRESSES_TO_TEST ; i++ ) {
        testInts[ i ] = new int;
        *testInts[ i ] = 0;
        long long newAddress = test.hash( *testInts[ i ] );
        for ( int i=0 ; i<previousAddresses.size() ; i++ ) {
            if ( newAddress == previousAddresses[ i ] ) {
                cout << newAddress << " " << previousAddresses[ i ];
                evaluateTest( true , false , "DefaultHasher has collision!" );
                return;
            }
        }
        previousAddresses.push_back( newAddress );
    }
    
    //report successful test
    evaluateTest( true , true , "DefaultHasher failed!" );
    for ( int i=0 ; i<1000 ; i++ ) {
        delete testInts[ i ];
    }
}

void HashMapTests::testConstructors() {

}

void HashMapTests::testPut() {
    
    int expected;
    int found;
    string errorMessage = "HashMap put() failed!";
    
    HashMap< string , int > test;
    string hello = "hello";
    test.put( hello , 1 );
    int hashCode = test.indexOf( test.m_hasher.hash( hello ) );
    expected = 1;
    found = (test.m_table.get( hashCode )->value);
    evaluateTest( expected , 1 , errorMessage );
    
    string hello2 = "hello";
    test.put( hello2 , 2 );
    int hashCode2 = test.indexOf( test.m_hasher.hash( hello2 ) );
    
    expected = 2;
    found = (test.m_table.get( hashCode2 )->value);
    evaluateTest( expected , found , errorMessage );
    
    if ( hashCode2 == hashCode ) {
        expected = 1;
        found = (test.m_table.get( hashCode2 )->next->value);
        evaluateTest( expected , found , errorMessage );
    }
}

void HashMapTests::testGet() {
    string errorMessage = "HashMap get() failed!";
    
    HashMap< string , int > test;
    string one = "one";
    test.put( one , 1 );
    string two = "two";
    test.put( two , 2 );
    string three = "three";
    test.put( three , 3 );
    string four = "four";
    test.put( four , 4 );
    string five = "five";
    test.put( five , 5 );
    string six = "six";
    test.put( six , 6 );
    string seven = "seven";
    test.put( seven , 7 );
    string eight = "eight";
    test.put( eight , 8 );
    string nine = "nine";
    test.put( nine , 9 );
    string ten = "ten";
    test.put( ten , 10 );
    string eleven = "eleven";
    test.put( eleven , 11 );
    string twelve = "twelve";
    test.put( twelve , 12 );
    string thirteen = "thirteen";
    test.put( thirteen , 13 );
    string fourteen = "fourteen";
    test.put( fourteen , 14 );
    
    
    evaluateTest( 1 , test.get( one ) , errorMessage );
    evaluateTest( 2 , test.get( two ) , errorMessage );
    evaluateTest( 3 , test.get( three ), errorMessage );
    evaluateTest( 4 , test.get( four ) , errorMessage );
    evaluateTest( 5 , test.get( five ) , errorMessage );
    evaluateTest( 6 , test.get( six ) , errorMessage );
    evaluateTest( 7 , test.get( seven ) , errorMessage );
    evaluateTest( 8 , test.get( eight ) , errorMessage );
    evaluateTest( 9 , test.get( nine ) , errorMessage );
    evaluateTest( 10 , test.get( ten ) , errorMessage );
    evaluateTest( 11 , test.get( eleven ) , errorMessage );
    evaluateTest( 12 , test.get( twelve ) , errorMessage );
    evaluateTest( 13 , test.get( thirteen ) , errorMessage );
    
    //we've gone past the default size by now, so there should certainly
    //have been collisions by now
    evaluateTest( 14 , test.get( fourteen ) , errorMessage );
    
    //our hasher hashes by memory address, so the key "eleven" should be
    //different from the previously defined eleven variable and nothing
    //should be found
    evaluateTest( 0 , test.get( "eleven" ), errorMessage );
    
    //we should be able to overwrite entries in the map as well
    test.put( one , 11111 );
    evaluateTest( 11111 , test.get( one ) , errorMessage );
    
    //in addition, if we change the value of one of the keys, it should
    //still map to the same value because the address has not changed
    eleven.at( 0 ) = 'E';
    evaluateTest( 11 , test.get( eleven ) , errorMessage );
    eleven = "jdhfjdhfjhdjfahsdkjhfjsdhfcjsfadffdsafhsjfhdsjfhsda";
    evaluateTest( 11 , test.get( eleven ) , errorMessage );
}

void HashMapTests::testRemove() {
    string errorMessage = "HashMap remove() failed!";
    
    HashMap< string , int > test;
    int expected;
    int found;
    int noKeyFound = 0;
    string keys[] = { "zero" , "one" , "two" , "three" , "four" , "five" ,
        "six" , "seven" , "eight" , "nine" , "ten" , "eleven" , "twelve" ,
        "thirteen" , "fourteen" , "fifteen" };
    int values[] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 ,
                    14 , 15 };

    for ( int i=0 ; i<=15 ; i++ ) {
        test.put( keys[ i ] , values[ i ] );
    }

    //check that "two" is still in the map
    evaluateTest( test.get( keys[ 2 ] ) , 2 , errorMessage );
    
    //remove "one"
    expected = 1;
    found = test.remove( keys[ 1 ] );
    evaluateTest( expected , found , errorMessage );
    
    //then check that "one" is gone and "two" is unaffected
    evaluateTest( test.get( keys[ 1 ] ) , noKeyFound , errorMessage );
    evaluateTest( test.get( "onee" ) , noKeyFound , errorMessage );
    evaluateTest( test.get( keys[ 2 ] ) , 2 , errorMessage );
    
    //check that removing "one" again does not do anything
    evaluateTest( test.remove( keys[ 1 ] ), noKeyFound , errorMessage );
    
    //try putting "one" back into the map
    test.put( keys[ 1 ] , values[ 1 ] );
    
    //try removing everything
    for ( int i=0 ; i<=15 ; i++ ) {
        evaluateTest( test.remove( keys[ i ] ) , values[ i ] , errorMessage );
        evaluateTest( test.get( keys[ i ] ) , noKeyFound , errorMessage );
    }
    
    //try removing everything again
    for ( int i=0 ; i<=15 ; i++ ) {
        evaluateTest( test.remove( keys[ i ] ) , noKeyFound , errorMessage );
    }
}

void HashMapTests::testDestructor() {
    HashMap< string , int >* testPtr;
    testPtr = new HashMap< string , int >( 10000 );
    cout << "Prepare to observe memory usage. Press enter to continue." << endl;
    getchar();
    for ( int i=0 ; i<100000 ; i++ ) {
        testPtr->put( "1" , 1 );
        testPtr->put( "2" , 2 );
        testPtr->put( "3" , 3 );
        testPtr->put( "4" , 4 );
        testPtr->put( "5" , 5 );
        delete testPtr;
        testPtr = new HashMap< string , int >( 10000 );
    }
    delete testPtr;
    cout << "Was memory usage constant? (y/n)" << endl;
    string errorMessage = "HashMap destructor failed!";
    char expected = 'y';
    char found = tolower( getchar() );
    evaluateTest( expected, found , errorMessage );
}

void HashMapTests::testResize() {
    string errorMessage = "HashMap resize() failed!";
    
    HashMap< string , int > test( 2 );
    evaluateTest( 2 , test.m_size , errorMessage );
    
    string one = "one";
    test.put( one , 1 );
    evaluateTest( 2 , test.m_size , errorMessage );

    string two = "two";
    test.put( two , 2 );
    evaluateTest( 3 , test.m_size , errorMessage );
    
    string three = "three";
    test.put( three , 3 );
    evaluateTest( 7 , test.m_size , errorMessage );
    
    string four = "four";
    test.put( four , 4 );
    evaluateTest( 7 , test.m_size , errorMessage );
    
    //very unlikely that all 4 numbers get hashed to same index
    int oneIdx = test.indexOf( test.m_hasher.hash( one ) );
    int twoIdx = test.indexOf( test.m_hasher.hash( two ) );
    int threeIdx = test.indexOf( test.m_hasher.hash( three ) );
    int fourIdx = test.indexOf( test.m_hasher.hash( four ) );
    bool allSameIdx = (oneIdx == twoIdx) && (twoIdx == threeIdx ) &&
                                                        (threeIdx==fourIdx);
    evaluateTest( allSameIdx , false , errorMessage );
    
    //make sure that four is actually at the index its supposed to be
    evaluateTest( test.m_table.get( fourIdx )->value , 4 , errorMessage );
    test.remove( four );
    
    //make sure that three is at the correct index
    evaluateTest( test.m_table.get( threeIdx )->value , 3 , errorMessage );
    test.remove( three );
    
    //make sure that two is at the correct index
    evaluateTest( test.m_table.get( twoIdx )->value , 2 , errorMessage );
    test.remove( two );

    //make sure that one is at the correct index
    int oneValue = test.m_table.get( oneIdx )->value;
    evaluateTest( oneValue , 1 , errorMessage );
    
    HashMap< int , int > test2;
    for ( int i=0 ; i<100000 ; i++ ) {
        test2.put( i , i );
    }
    evaluateTest( test2.m_table.size() , 180503, errorMessage );
    evaluateTest( test2.m_size , 180503 , errorMessage );
    
    //test boundary resize
    //0.7*180503 = 126352.1, so we add 126352 entries
    //then we add another entry and see if the map resizes
    test2.clear();
    for ( int i=0 ; i<126352 ; i++ ) {
        test2.put( i , i );
    }
    evaluateTest( test2.m_table.size() , 180503 , errorMessage );
    evaluateTest( test2.m_size , 180503 , errorMessage );
    
    test2.put( 126353 , 126353 );
    evaluateTest( test2.m_table.size() , 386093, errorMessage );
    evaluateTest( test2.m_size , 386093 , errorMessage );
}

void HashMapTests::testClear() {
    string errorMessage = "HashMap clear() failed!";
    
    HashMap< string , int > test;
    
    string one = "one";
    test.put( one , 1 );
    
    string two = "two";
    test.put( two , 2 );
    
    string three = "three";
    test.put( three , 3 );
    
    test.clear();
    evaluateTest( test.get( one ) , 0 , errorMessage );
    evaluateTest( test.m_table.size() , test.DEFAULT_SIZE , errorMessage );
}

