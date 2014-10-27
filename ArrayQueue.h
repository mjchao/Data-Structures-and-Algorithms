//
//  ArrayQueue.h
//  Data Structures
//
//  Created by Mickey on 10/25/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_ArrayQueue_h
#define Data_Structures_ArrayQueue_h

#include "ArrayList.h"
#include "AbstractQueue.h"

using std::runtime_error;

/**
 * A circular array-based queue. It provides functions for poll() and offer()
 * in O(1) by shifting the index of the head and tail of the list; however,
 * when the elements exceed the array's capacity, resizing the array will take
 * O(n) time.
 *
 * The ArrayList functions are not provided because the queue's
 * boundaries shift around in the array, making the ArrayList properties
 * difficult to use.
 */
template< typename E >
class ArrayQueue : protected ArrayList< E > , public AbstractQueue< E > {

friend class ArrayQueueTests;

private:
    
    /**
     * the index of the first element in the queue, which will be the first
     * element polled
     */
    int m_headIdx;
    
    /**
     * the index of the last element in the queue PLUS ONE.
     */
    int m_tailIdx;
    
    /**
     * the number of elements in the queue. we must store the number of elements
     * and not just calculate it from the index of the head and tail due to
     * wrap-around problems. if the head and tail indices are the same
     * then we cannot be sure if the queue has size 0, or if it has
     * reached full capacity.
     */
    int m_numElements;
    
public:
    /**
     * Constructs a default queue with no elements
     */
    ArrayQueue() : ArrayList< E >() {
        m_headIdx = 0;
        m_tailIdx = 0;
        m_numElements = 0;
        
        //we cannot have an array size of 0, because that would mess up
        //wrap around calculations
        ArrayList< E >::ensureCapacity( 1 );
    }
    
    //no copy constructor, copy assignment operator, or destructor is needed,
    //as we are just using the ones from ArrayList
    
    /**
     * Puts the given value into the back of the queue
     *
     * @param value             the value to insert into the queue
     */
    void offer( const E& value ) {
        
        //if we exceed capacity, we might as well just resize the array
        //and start everything over with the head at the 0-th index
        if ( m_numElements+1 > ArrayList< E >::getArraySize() ) {
            E* tmp[ m_numElements ];
            for ( int i=0 ; i<m_numElements ; i++ ) {
                tmp[ i ] = &ArrayList< E >::get( (m_headIdx+i)%ArrayList< E >::getArraySize() );
            }
            ArrayList< E >::ensureCapacity( (m_numElements)*2 );
            for ( int i=0 ; i<m_numElements ; i++ ) {
                ArrayList< E >::set( i , *tmp[ i ] );
            }
            m_headIdx = 0;
            m_tailIdx = m_numElements;
        }
        
        //insert the element at the index of the tail
        int insertIdx = m_tailIdx % ArrayList< E >::getArraySize();
        
        //if the ArrayList representation already has an element at the
        //insertion index, just overwrite it
        if ( insertIdx < ArrayList< E >::size() ) {
            ArrayList< E >::set( insertIdx , value );
        }
        
        //if the ArrayList representation doesn't have an element at the
        //insertion index already, then we actually need to insert the
        //the new element. Otherwise, we'd get an access out of bounds
        //exception
        else {
            ArrayList< E >::insert( insertIdx , value );
        }
        
        m_tailIdx = (m_tailIdx+1) % ArrayList< E >::getArraySize();
        m_numElements++;
    }
    
    /**
     * Gets, but does not remove, the value at the front of the queue
     *
     * @return                  the value at the front of the queue
     */
    E& peek() const {
        if ( size() == 0 ) {
            throw runtime_error( generateEmptyQueueException() );
        }
        return ArrayList< E >::get( m_headIdx );
    }
    
    /**
     * Removes and returns the value at the front of the queue
     *
     * @return                  the value at the front of the queue
     */
    E poll() {
        if ( size() == 0 ) {
            throw runtime_error( generateEmptyQueueException() );
        }
        E rtn = ArrayList< E >::get( m_headIdx );
        m_headIdx = ( m_headIdx+1 ) % ArrayList< E >::getArraySize();
        m_numElements--;
        return rtn;
    }
    
    /**
     * Clears the queue so that it contains no elements
     */
    void clear() {
        ArrayList< E >::clear();
        m_headIdx = 0;
        m_tailIdx = -1;
        m_numElements = 0;
        
        //we cannot have an array size of 0 because that would mess up wrap
        //around calculations
        ArrayList< E >::ensureCapacity( 1 );
    }
    
    int size() const {
        return m_numElements;
    }
    
    string generateEmptyQueueException() const {
        return "Error: cannot peek into or poll from an empty queue.";
    }
    
    string toString() const {
        
        //the empty queue is a special case because
        //we cannot access the first element
        if ( size() == 0 ) {
            return "[]";
        }
        else if ( size() == 1 ) {
            Message rtn;
            rtn << "[" << ArrayList< E >::get( m_headIdx ) << "]";
            return rtn.str();
        }
        else {
            Message rtn;
            rtn << "[" << ArrayList< E >::get( m_headIdx );
            int currIdx = (m_headIdx+1) % (ArrayList< E >::getArraySize());
            for ( int i=1 ; i<m_numElements ; i++ ) {
                rtn << ", " << ArrayList< E >::get( currIdx );
                currIdx = (currIdx+1) % (ArrayList< E >::getArraySize());
            }
            rtn << "]";
            return rtn.str();
        }
    }

    
};

#endif
