//
//  OrderedSet.h
//  Data Structures
//
//  Created by Mickey on 10/11/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_OrderedSet_h
#define Data_Structures_OrderedSet_h

#include "AbstractSet.h"

/**
 * An ordered list of values, where every element has a corresponding index
 *
 * @param E                 the type of element stores in this list
 */
template< typename E >
class AbstractList : public AbstractSet< E > {
    
public:

    /**
     * Gets the element at the specified ordering index.
     *
     * @param index         an index corresponding to an element in the set
     * @return              the element at the specified index
     *
     */
    virtual E& get( int index ) const = 0;
    
    /**
     * Gets the index of the first occurence of the given value
     *
     * @param value         the value for which to look
     * @return              the index of the first occurence of the given value
     */
    virtual int indexOf( const E& value ) const = 0;
    
    /**
     * Inserts the given element at the specified index in the OrderedSet.
     *
     * @param index         the index at which to insert the element
     * @param value         the value to insert into the OrderedSet
     */
    virtual void insert( int index , const E& value ) = 0;
    
    /**
     * Sets the given index of the list to contain the given element, and if
     * necessary, replaces an element that is currently there
     */
    virtual void set( int index , const E& value ) = 0;
    
    /**
     * Adds the given element to the end of the OrderedSet.
     *
     * @param value         the value to append to the end of the OrderedSet
     */
    virtual void append( const E& value ) = 0;
    
    /**
     * Removes the given element at the specified index from the OrderedSet.
     *
     * @param index         the index corresponding to the element to remove.
     * @return              the element that was removed
     */
    virtual E removeAt( int index ) = 0;
    
};

#endif
