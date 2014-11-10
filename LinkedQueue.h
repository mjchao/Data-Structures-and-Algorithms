//
//  LinkedQueue.h
//  Data Structures
//
//  Created by Mickey on 10/22/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_LinkedQueue_h
#define Data_Structures_LinkedQueue_h

#include "LinkedList.h"
#include "AbstractQueue.h"

using std::runtime_error;

/**
 * A link-based queue. It has a link-based structure with queue operations. It
 * provides offer() and poll() operations in O(1), but get() and remove() have
 * complexity O(n).
 *
 * The element at the front of the list is the front of the queue. The element
 * at the end of the list is the back of the queue.
 *
 * @param E                         the type of element in this queue
 */
template< typename E >
class LinkedQueue : public LinkedList< E > , public AbstractQueue< E > {
    
public:
    
    /**
     * Constructs a default queue with no elements
     */
    LinkedQueue() : LinkedList< E >() {
        
    }
    

    //there is no need to provide a destructor, copy constructor, or copy
    //assignment operator because the LinkedList's are used
    
    /**
     * Inserts the given element into the back of the queue
     *
     * @param value                 the value to be inserted
     */
    void offer( const E& value ) {
        LinkedList< E >::append( value );
    }
    
    /**
     * Gets, but does not remove, the element at the front of the queue
     *
     * @return                      the element at the front of the queue
     */
    const E& peek() const {
        if ( LinkedList< E >::size() == 0 ) {
            throw runtime_error( generateEmptyQueueErrorMessage() );
        }
        return LinkedList< E >::get( 0 );
    }
    
    /**
     * Removes and returns the element at the front of the queue
     *
     * @return                      the element at the front of the queue
     */
    E poll() {
        if ( LinkedList< E >::size() == 0 ) {
            throw runtime_error( generateEmptyQueueErrorMessage() );
        }
        return LinkedList< E >::removeAt( 0 );
    }
    
    string generateEmptyQueueErrorMessage() const {
        return "Error: cannot poll from or peek into queue with no elements.";
    }
    
};

#endif
