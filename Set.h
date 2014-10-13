//
//  Set.h
//  Data Structures
//
//  Created by Mickey on 10/11/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_Set_h
#define Data_Structures_Set_h

/**
 * A mathematical set that contains some elements and provides functions for
 * operating to modify its contents.
 *
 * @param E             the type of element contained by this set
 *
 */
template< typename E >
class Set {
    
public:
    /**
     * determines if the given value is in the set
     *
     * @param value         the value for which to search
     * @return              whether or not the given value is in the set
     *
     */
    virtual bool contains( E value ) const = 0;
    
    /**
     * Inserts the given value into the set.
     *
     * @param value         the value to be inserted into the set
     *
     */
    virtual void insert( E value ) = 0;
    
    /**
     * Removes the given value from the set
     *
     * @param               the value to be removed from the set
     * @return              if the value was removed from the set
     *
     */
    virtual bool remove( E value ) = 0;
    
    /**
     * Removes all elements from the set
     *
     *
     */
    virtual void clear() = 0;
    
    /**
     * determines the size of the set.
     *
     * @return              the number of elements in the set, according to the
     *                      set's definition
     *
     */
    virtual int size() const = 0;
    
    /**
     * Creates the textual representation of the contents of this Set
     */
    virtual string toString() const = 0;

};

#endif
