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
#include <iostream>

/**
 * A default hasher that uses the memory address of objects as their hash-value
 * for linear probing modulo a prime number
 *
 * @param T                     the type of object for which this Hasher
 *                              generates hash codes
 */
template< typename T >
class DefaultHasher : public Hasher< T > {
    
private:
    int m_modulus;
    
public:
    
    //the predefined moduli are the 2^n-th primes not exceeding the maximum
    //integer value
    const int NUM_PREDEF_MODULI = 28;
    const int MODULI[ 28 ] = { 1 , 2, 3 , 7 , 19 , 53 ,
                        131 , 311 , 719 , 1619 , 3617 ,
                        8161 , 17863 , 38873 , 84017 , 180503 ,
                        386093 , 821641 , 1742537 , 3681131 , 7754077 ,
                        16290047 , 34136029 , 71378569 , 148948139 , 310248241 ,
                        645155197 , 1339484197 };
    
    const int DEFAULT_MODULUS = 19;
    
    /**
     * Creates a DefaultHasher where hashcodes are calculated modulo the default
     * modulus, 13
     */
    DefaultHasher() {
        m_modulus = DEFAULT_MODULUS;
    }
    
    ~DefaultHasher() {
        //nothing to delete
    }
    
    /**
     * Creates a DefaultHasher where hashcodes are calculated modulo the given
     * modulus
     *
     * @param modulus           the value by which hashcodes are modulo'ed
     *                          For effective hashcode generation, this
     *                          value should be a prime
     */
    DefaultHasher( int modulus ) {
        m_modulus = modulus;
    }
    
    /**
     * Returns the decimal representation of the given object's memory address
     * modulo a prime as the hash value
     *
     * @param val               the object for which to generate a hash value
     * @return                  the decimal representation of the object's
     *                          memory address, which is the hash code by
     *                          default
     */
    long long hash( const T& val ) const {
        long long address = (long long)(void*)( &val );
        return address % m_modulus;
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
    
    /**
     * Selects a new prime for the modulus. The new prime is the largest
     * prime from a list of pre-selected primes that does not exceed the new
     * array size
     *
     * @param                   the new size of the HashMap's underlying array
     */
    void handleHashMapResize( int newSize ) {
        int newModulus = MODULI[ 0 ];
        for ( int i=1 ; i<NUM_PREDEF_MODULI ; i++ ) {
            if ( MODULI[ i ] <= newSize ) {
                newModulus = MODULI[ i ];
            }
        }
        if ( MODULI[ NUM_PREDEF_MODULI-1 ] <= newSize ) {
            newModulus = MODULI[ NUM_PREDEF_MODULI-1 ];
        }
        setModulus( newModulus );
    }
    
    /**
     * Sets the number by which hashcodes of objects are modulo'ed. For
     * effective hashcode generation, the value should be a prime number
     * 
     *
     * @param modulus           the value by which hashcodes are modulo'ed
     */
    void setModulus( int modulus ) {
        if ( modulus == 0 ) {
            throw std::runtime_error( "Modulus cannot be 0" );
        }
        m_modulus = modulus;
    }
    
    /**
     * @return                  the value by which hashcodes are modulo'ed
     */
    int getModulus() {
        return m_modulus;
    }
};

#endif
