//
//  Hasher.h
//  Data Structures
//
//  Created by Mickey on 11/10/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_Hasher_h
#define Data_Structures_Hasher_h

template< typename T >
class Hasher {
    
public:
    
    virtual ~Hasher() {
        //nothing to delete
    }
    
    /**
     * Returns a hash value for a given object for use in a HashMap. For
     * effective use of the HashMap, the hash function should generally
     * have an even distribution of hash values for a set of objects. The
     * hash function must return the same value for the same object every time.
     *
     * @param val               the object for which to generate the hash value
     * @return                  a hash value for the given object
     */
    virtual long long hash( const T& val ) const = 0;
    
    /**
     * Determines if two objects are equivalent. This will be used to determine
     * if two keys are the same or not. It is advisable that if two objects
     * generate the same hashcode, then they are equivalent.
     *
     * @param v1                an object
     * @param v2                another object
     * @return                  if the two objects are equivalent by some
     *                          arbitrary rules
     */
    virtual bool areEquivalent( const T& v1 , const T& v2 ) const = 0;
    
    /**
     * When the HashMap's underlying array resizes, it may be necessary to
     * update the hash function. If it is not necessary, then this function
     * can be left empty
     *
     * @param                   the new size of the HashMap array after resizing
     */
    virtual void handleHashMapResize( int newSize ) = 0;
};

#endif
