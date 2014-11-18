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
    reportTestStatistics( "HashMap" );
}

void HashMapTests::testDefaultHasher() {
    DefaultHasher< int > test( 15485863 );
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
    int hashCode = (int)test.m_hasher->hash( hello );
    expected = 1;
    found = *(test.m_entries.get( hashCode )->get( 0 ).value);
    evaluateTest( expected , 1 , errorMessage );
    
    string hello2 = "hello";
    test.put( hello2 , 2 );
    int hashCode2 = (int)test.m_hasher->hash( hello2 );
    
    expected = 2;
    if ( hashCode2 == hashCode ) {
        found = *(test.m_entries.get( hashCode2 )->get( 1 ).value);
    }
    else {
        found = *(test.m_entries.get( hashCode2 )->get( 0 ).value);
    }
    evaluateTest( expected , found , errorMessage );
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
    
    
    evaluateTest( 1 , *test.get( one ) , errorMessage );
    evaluateTest( 2 , *test.get( two ) , errorMessage );
    evaluateTest( 3 , *test.get( three ), errorMessage );
    evaluateTest( 4 , *test.get( four ) , errorMessage );
    evaluateTest( 5 , *test.get( five ) , errorMessage );
    evaluateTest( 6 , *test.get( six ) , errorMessage );
    evaluateTest( 7 , *test.get( seven ) , errorMessage );
    evaluateTest( 8 , *test.get( eight ) , errorMessage );
    evaluateTest( 9 , *test.get( nine ) , errorMessage );
    evaluateTest( 10 , *test.get( ten ) , errorMessage );
    evaluateTest( 11 , *test.get( eleven ) , errorMessage );
    evaluateTest( 12 , *test.get( twelve ) , errorMessage );
    evaluateTest( 13 , *test.get( thirteen ) , errorMessage );
    
    //we've gone past the default size by now, so there should certainly
    //have been collisions by now
    evaluateTest( 14 , *test.get( fourteen ) , errorMessage );
    
    //our hasher hashes by memory address, so the key "eleven" should be
    //different from the previously defined eleven variable and nothing
    //should be found
    int* zeroPtr = 0;
    evaluateTest( zeroPtr , test.get( "eleven" ), errorMessage );
    
    //in addition, if we change the value of one of the keys, it should
    //still map to the same value because the address has not changed
    eleven.at( 0 ) = 'E';
    evaluateTest( 11 , *test.get( eleven ) , errorMessage );
    eleven = "jdhfjdhfjhdjfahsdkjhfjsdhfcjsfadffdsafhsjfhdsjfhsda";
    evaluateTest( 11 , *test.get( eleven ) , errorMessage );
}

void HashMapTests::testRemove() {
    string errorMessage = "HashMap remove() failed!";
    
    HashMap< string , int > test;
    int expected;
    int found;
    int* noKeyFound = 0;
    string keys[] = { "zero" , "one" , "two" , "three" , "four" , "five" ,
        "six" , "seven" , "eight" , "nine" , "ten" , "eleven" , "twelve" ,
        "thirteen" , "fourteen" , "fifteen" };
    int values[] = { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 ,
                    14 , 15 };

    for ( int i=0 ; i<=15 ; i++ ) {
        test.put( keys[ i ] , values[ i ] );
    }

    //check that "two" is still in the map
    evaluateTest( *test.get( keys[ 2 ] ) , 2 , errorMessage );
    
    //remove "one"
    expected = 1;
    found = *test.remove( keys[ 1 ] );
    evaluateTest( expected , found , errorMessage );
    
    //then check that "one" is gone and "two" is unaffected
    evaluateTest( test.get( keys[ 1 ] ) , noKeyFound , errorMessage );
    evaluateTest( test.get( "onee" ) , noKeyFound , errorMessage );
    evaluateTest( *test.get( keys[ 2 ] ) , 2 , errorMessage );
    
    //check that removing "one" again does not do anything
    evaluateTest( test.remove( keys[ 1 ] ), noKeyFound , errorMessage );
    
    //try putting "one" back into the map
    test.put( keys[ 1 ] , values[ 1 ] );
    
    //try removing everything
    for ( int i=0 ; i<=15 ; i++ ) {
        evaluateTest( *test.remove( keys[ i ] ) , values[ i ] , errorMessage );
        evaluateTest( test.get( keys[ i ] ) , noKeyFound , errorMessage );
    }
    
    //try removing everything again
    for ( int i=0 ; i<=15 ; i++ ) {
        evaluateTest( test.remove( keys[ i ] ) , noKeyFound , errorMessage );
    }
}

