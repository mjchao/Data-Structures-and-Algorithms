//
//  AbstractQueue.h
//  Data Structures
//
//  Created by Mickey on 10/21/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_AbstractQueue_h
#define Data_Structures_AbstractQueue_h

template < typename E >
class AbstractQueue {
    
    /**
     * Puts the given value into the back of the queue
     * 
     * @param value             the value to insert into the queue
     */
    virtual void offer( const E& value ) = 0;
    
    /**
     * Gets, but does not remove, the value at the front of the queue
     *
     * @return                  the value at the front of the queue
     */
    virtual const E& peek() const = 0;
    
    /**
     * Removes and returns the value at the front of the queue
     *
     * @return                  the value at the front of the queue
     */
    virtual E poll() = 0;
};

#endif
