//
//  AbstractStack.h
//  Data Structures
//
//  Created by Mickey on 10/21/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_AbstractStack_h
#define Data_Structures_AbstractStack_h

#include "AbstractSet.h"

/**
 * A stack that provides the standard stack operations, such as push and pop.
 *
 * @param E                     the type of element in this stack
 */
template< typename E >
class AbstractStack {
    
public:
    
    /**
     * Pushes the given value to the top of stack
     *
     * @param value             the value to push to the top of the stack
     */
    virtual void push( const E& value ) = 0;
    
    /**
     * Gets the element at the top of the stack, but does not remove it
     *
     * @return                  the element at the top of the stack
     */
    virtual E& peek() const = 0;
    
    /**
     * Removes and returns the value at the top of the stack
     *
     * @return                  the element at the top of the stack
     */
    virtual E pop() = 0;
};

#endif
