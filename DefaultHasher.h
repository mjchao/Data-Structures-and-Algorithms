//
//  DefaultHasher.h
//  Data Structures
//
//  Created by Mickey on 11/10/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_DefaultHasher_h
#define Data_Structures_DefaultHasher_h

#include "Hasher.h"

/**
 * A default hasher that uses the memory addresses of objects as their 
 * hash-values
 *
 * @param T                     the type of object for which this Hasher
 *                              generates hash codes
 */
template< typename T >
class DefaultHasher : public Hasher< T > {
    
private:
    
public:
    
    /**
     * Creates a DefaultHasher where hashcodes are calculated from the
     * object's address
     */
    DefaultHasher() {

    }
    
    ~DefaultHasher() {
        //nothing to delete
    }
    
    /**
     * Returns the integer representation of the given object's memory address
     *
     * @param val               the object for which to generate a hash value
     * @return                  the decimal representation of the object's
     *                          memory address, which is the hash code by
     *                          default
     */
    long long hash( const T& val ) const {
        long long address = (long long)(void*)( &val );
        return address;
    }
  
    /**
     * Determines if two objects are equivalent. This will be used to determine
     * if two keys are the same or not.
     *
     * @param v1                an object
     * @param v2                another object
     * @return                  if the two objects are equivalent by some
     *                          arbitrary rules
     */
    bool areEquivalent( const T& v1 , const T& v2 ) const {
        
        //this hasher generates hashcodes from memory addresses,
        //so two objects should only be equivalent if they have the same
        //address
        long long address1 = (long long)(void*)( &v1 );
        long long address2 = (long long)(void*)( &v2 );
        return address1 == address2;
    }
};

#endif
