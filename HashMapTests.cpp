//
//  HashmapTests.cpp
//  Data Structures
//
//  Created by Mickey on 11/3/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include "HashmapTests.h"
#include "DefaultHasher.h"
#include <vector>
using std::vector;

void HashMapTests::test() {
    testDefaultHasher();
    reportTestStatistics( "HashMap" );
}

void HashMapTests::testDefaultHasher() {
    DefaultHasher< int > test( 15485863 );
    vector< long long > previousAddresses;
    
    //test generating hashcodes for 10000 different integers. since
    //they all have different memory addresses, they should most likely
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

