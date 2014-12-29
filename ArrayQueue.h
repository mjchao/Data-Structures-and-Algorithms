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
    
    /**
     * Determines if the given index is out of bounds in the queue
     *
     * @return                  if the given element is
     */
    bool isOutOfBounds( int index ) const {
        return index < 0 || index >= m_numElements;
    }
    
    /**
     * Determines the index in the queue's underlying array, given the index
     * of an element in the queue
     *
     * @return                  the index in the queue converted to the index
     *                          in the array
     */
    int getIndexInArray( int queueIndex ) const {
        
        //queue size of 0 is special case, because we can't really define
        //wrapping around
        if ( m_numElements == 0 ) {
            return 0;
        }
        
        
        if ( queueIndex >= 0 ) {
            return (m_headIdx+queueIndex) % ArrayList< E >::getArraySize();
        }
        else {
            
            //if we are "unwrapping" around with a negative index
            //just keep adding the size of the array
            int wraparoundTimes = abs( queueIndex / m_numElements )+1;
            int offset = wraparoundTimes * m_numElements;
            return (m_headIdx + queueIndex + offset ) %
                                            ArrayList< E >::getArraySize();
        }
    }
    
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
            int previousArraySize = ArrayList< E >::getArraySize();
            ArrayList< E >::ensureCapacity( (m_numElements)*2 );
            E tmp[ m_numElements ];
            for ( int i=0 ; i<m_numElements ; i++ ) {
                tmp[ i ] = ArrayList< E >::get((m_headIdx+i)%previousArraySize);
            }
            for ( int i=0 ; i<m_numElements ; i++ ) {
                ArrayList< E >::set( i , tmp[ i ] );
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
    const E& peek() const {
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
     * Gets the element at the given index, relative to the head of the queue -
     * not the underlying array-based list. Therefore, the head of the queue
     * always corresponds to index 0, even if it is not at index 0 in the array
     *
     * @param index             the index of the element to obtain
     * @return                  the element at the given index in the queue
     */
    E& get( int index ) const {
        if ( isOutOfBounds( index ) ) {
            throw std::runtime_error(
                    ArrayList< E >::generateAccessOutOfBoundsMessage(
                                                    index , m_numElements ) );
        }
        int queueIndex = getIndexInArray( index );
        return ArrayList< E >::get( queueIndex );
    }
    
    /**
     * Determines if the given element is in the queue.
     *
     * @param value             the value for which to look
     * @return                  if the given value is in the queue
     */
    bool contains( const E& value ) const {
        for ( int i=0 ; i<m_numElements ; i++ ) {
            if ( get( i ) == value ) {
                return true;
            }
        }
        return false;
    }
    
    /**
     * Sets the element at the given index to the given value. The index is
     * relative to the head of the queue - not to the start of the underlying
     * array-based list. Therefore, the head of the queue always corresponds to
     * index 0, even if it is not at index 0 in the array
     *
     * @param index             the index of an element in the queue
     * @param newValue          the new value for the for the specified element
     */
    void set( int index , const E& newValue ) {
        if ( isOutOfBounds( index ) ) {
            throw std::runtime_error(
                ArrayList< E >::generateAccessOutOfBoundsMessage(
                                                    index , m_numElements ) );
        }
        int queueIndex = getIndexInArray( index );
        ArrayList< E >::set( queueIndex , newValue );
    }
    
    /**
     * Removes the element at the given index from the queue. The index is
     * relative to the head of the queue - not to the start of the underlying
     * array-based list.
     *
     * @param index             the index of the element in the queue
     * @return                  the element that was removed
     */
    E removeAt( int index ) {
        if ( isOutOfBounds( index ) ) {
            throw std::runtime_error(
                ArrayList< E >::generateAccessOutOfBoundsMessage(
                                                    index , m_numElements ) );
        }
        
        E& rtn = get( index );
        
        //shift subsequent elements down by 1, overwriting the element to remove
        for ( int i=index+1 ; i<m_numElements ; i++ ) {
            set( i-1 , get( i ) );
        }
        m_numElements--;
        m_tailIdx = (m_tailIdx - 1 + ArrayList< E >::getArraySize()) %
                                                ArrayList< E >::getArraySize();
        return rtn;
    }
    
    /**
     * Removes the given element from the queue, if it exists
     *
     * @param value             the value to remove from the queue
     * @return                  if the value was found and removed or not
     */
    bool remove( const E& value ) {
        for ( int i=0 ; i<m_numElements ; i++ ) {
            if ( get( i ) == value ) {
                removeAt( i );
                return true;
            }
        }
        return false;
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
        
        //the queue with 1 element is a special case because
        //there are no commas in the textual representation
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
