//
//  UnitTest.cpp
//  Data Structures
//
//  Created by Mickey on 10/11/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#include <iostream>
    using std::cout;
    using std::endl;

#include "UnitTest.h"

UnitTest::UnitTest() {
    m_numFailed = 0;
    m_numSucceeded = 0;
    m_numTests = 0;
}

UnitTest::~UnitTest() {
    
}

void UnitTest::testTrue( bool expression , string descriptionIfFailed ) {
    if ( expression == true ) {
        incrementSucceededTests();
    }
    else {
        cout << "Assert failed: " << endl <<
                descriptionIfFailed << endl;
        incrementFailedTests();
    }
    incrementSucceededTests();
}

void UnitTest::incrementSucceededTests() {
    m_numSucceeded++;
}

void UnitTest::incrementFailedTests() {
    m_numFailed++;
}

void UnitTest::incrementTotalTests() {
    m_numTests++;
}

void UnitTest::reportTestStatistics( string className ) {
    cout << className << " unit tests finished. Passed " <<
        m_numSucceeded << " of " << m_numTests << " tests." << endl;
}