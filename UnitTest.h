//
//  UnitTest.h
//  Data Structures
//
//  Created by Mickey on 10/11/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef __Data_Structures__UnitTest__
#define __Data_Structures__UnitTest__

#include <iostream>
    using std::cout;
    using std::endl;

#include <string>
    using std::string;

class UnitTest {
    
private:
    int m_numSucceeded;
    int m_numFailed;
    int m_numTests;
    
    void incrementSucceededTests();
    
    void incrementFailedTests();
    
    void incrementTotalTests();
    
protected:
    
    template< typename T >
    void evaluateTest( T expected , T found , string descriptionIfFailed ) {
        if ( expected == found ) {
            incrementSucceededTests();
        }
        else {
            cout << descriptionIfFailed << endl;
            cout << "Expected: " << expected << endl;
            cout << "Found: " << found << endl;
            incrementFailedTests();
        }
        incrementTotalTests();
    }
    
    void testTrue( bool expression , string descriptionIfFailed );
    
    void reportTestStatistics();
    
public:
    UnitTest();
    ~UnitTest();
    
    virtual void test() {
        reportTestStatistics();
    }
};
#endif /* defined(__Data_Structures__UnitTest__) */
