//
//  LinkedStack.h
//  Data Structures
//
//  Created by Mickey on 10/22/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_LinkedStack_h
#define Data_Structures_LinkedStack_h

#include "AbstractStack.h"
#include "LinkedList.h"

#include <string>
using std::string;

using std::runtime_error;

/**
 * A link-based stack. It has a LinkedList structure with stack operations.
 * It provides push and pop operations in O(1), but get and remove have
 * complexity O(n).
 *
 * The front of the list represents the top of the stack, while the
 * back of the list represents the bottom.
 */
template< typename E >
class LinkedStack : public LinkedList< E > , public AbstractStack< E > {
   
public:
    
    /**
     * Pushes the given value to the top of the stack
     *
     * @param value                 the value to push
     */
    void push( const E& value ) {
        LinkedList< E >::insert( value );
    }
    
    /**
     * Gets, but does not remove, the value at the top of the stack
     *
     * @return                      the value at the top of the stack
     */
    E& peek() const {
        if ( LinkedList< E >::size() == 0 ) {
            throw std::runtime_error( generateEmptyStackErrorMessage() );
        }
        return LinkedList< E >::get( 0 );
    }
    
    /**
     * Removes and returns the value at the top of the stack
     *
     * @return                      the value at the top of the stack
     */
    E pop() {
        if ( LinkedList< E >::size() == 0 ) {
            throw std::runtime_error( generateEmptyStackErrorMessage() );
        }
        return LinkedList< E >::removeAt( 0 );
    }
    
    /**
     * Determines the textual representation of the stack
     *
     * @return                      the textual representation of the stack,
     *                              similar to that of a list. the top element
     *                              is first.
     */
    string toString() const {
        return LinkedList< E >::toString();
    }
    
    string generateEmptyStackErrorMessage() const {
        return "Error: cannot pop from stack with no elements.";
    }
};

#endif
