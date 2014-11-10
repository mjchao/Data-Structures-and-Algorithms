//
//  Comparator.h
//  Data Structures
//
//  Created by Mickey on 11/4/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_Comparator_h
#define Data_Structures_Comparator_h

template< typename T >
class Comparator {
    
public:
    /**
     * Compares two elements.
     *
     * @return              1 if v1 is greater than v2, 0 if v1 == v2, and 
     *                      -1 if v1 is less than v2
     */
    virtual int compare( const T& v1 , const T& v2 ) const = 0;
    
};

#endif
