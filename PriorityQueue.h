//
//  PriorityQueue.h
//  Data Structures
//
//  Created by Mickey on 11/4/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_PriorityQueue_h
#define Data_Structures_PriorityQueue_h

#include "ArrayQueue.h"
#include "Comparator.h"

/**
 * A queue where elements are ordered by their priority (i.e. smaller elements
 * are stored first). 
 *
 * The ArrayQueue's functions are hidden because
 * the elements of the priority queue should not be modified. If they are to be
 * modified, they should be removed then re-inserted - otherwise, the queue
 * could no longer be guaranteed to be ordered by priority.
 */
template< typename E >
class PriorityQueue : protected ArrayQueue< E > {

private:
    Comparator< E >* m_comparator;
    
    /**
     * Compares two elements and determines which one is larger
     *
     * @return                      1 if e1 is greater, 0 if e1 == e2, and -1
     *                              if e2 is greater
     */
    int compare( const E& e1 , const E& e2 ) {
        if ( m_comparator == 0 ) {
            if ( e1 < e2 ) {
                return -1;
            }
            else if ( e1 == e2 ) {
                return 0;
            }
            else {
                return 1;
            }
        }
        else {
            return m_comparator->compare( e1 , e2 );
        }
    }
    
    /**
     * Converts an index in the priority queue to an index in the underlying
     * array-based queue. The array-based queue starts at index 0. The priority
     * queue starts at index 1.
     *
     * @return                      the index in the array-based queue
     */
    int getIdxInArrayQueue( int pqIdx ) {
        return pqIdx-1;
    }
    
    /**
     * Swaps the elements at the given indices.
     *
     * @param idx1                  an index of an element to swap
     * @param idx2                  an index of an element to swap
     */
    void swap( int idx1 , int idx2 ) {
        int arrayIdx1 = getIdxInArrayQueue( idx1 );
        int arrayIdx2 = getIdxInArrayQueue( idx2 );
        
        E tmp = ArrayQueue< E >::get( arrayIdx1 );
        ArrayQueue< E >::set( arrayIdx1 , ArrayQueue< E >::get( arrayIdx2 ) );
        ArrayQueue< E >::set( arrayIdx2 , tmp );
    }
    
    /**
     * Sifts the element at the given index upwards in the priority queue
     *
     * @param idx                   index of the element to sift upwards
     */
    void siftUp( int idx ) {
        int currIdx = idx;
        while( currIdx > 1 ) {
            if ( compare( ArrayQueue< E >::get( getIdxInArrayQueue( currIdx ) ),
                    ArrayQueue< E >::get( getIdxInArrayQueue( currIdx/2 ) ) )
                                                                    < 0 ) {
                swap( currIdx , currIdx/2 );
                currIdx = currIdx/2;
            }
            else {
                break;
            }
        }
    }
    
    /**
     * Sifts the element at the given index downwards in the priority queue
     *
     * @param idx                   index of the element to sift downwards
     */
    void siftDown( int idx ) {
        int currIdx = idx;
        while( currIdx*2 <= ArrayQueue< E >::size() ) {
            int smallerIdx = currIdx*2;
            int otherIdx = currIdx*2 + 1;
            if ( otherIdx <= ArrayQueue< E >::size() ) {
                if ( compare(
                        ArrayQueue< E >::get( getIdxInArrayQueue( otherIdx ) ) ,
                        ArrayQueue< E >::get( getIdxInArrayQueue( smallerIdx ) )
                                                                    ) < 0 ) {
                    smallerIdx = otherIdx;
                }
            }
            
            if ( compare(
             ArrayQueue< E >::get( getIdxInArrayQueue( currIdx ) ),
             ArrayQueue< E >::get( getIdxInArrayQueue( smallerIdx ) ) ) >= 0 ) {
                swap( currIdx , smallerIdx );
                currIdx = smallerIdx;
            }
            else {
                break;
            }
        }
    }
    
public:
    
    /**
     * Creates an empty priority queue with no comparator. The < operator
     * will have to be defined for elements.
     */
    PriorityQueue() {
        m_comparator = 0;
    }
    
    /**
     * Creates an empty priority queue with the given comparator
     *
     * @param comparator            the comparator for determining the order of
     *                              the elements
     */
    PriorityQueue( Comparator< E >& comparator ) {
        m_comparator = comparator;
    }
    
    /**
     * Places the given value into the priority queue
     *
     * @param value                 the value to insert into the queue
     */
    void offer( const E& value ) {
        ArrayQueue< E >::offer( value );
        
        //remember that the priority queue elements start at index 1,
        //so ArrayQueue< E >::size() actually corresponds to the last element
        //in the priority queue
        siftUp( ArrayQueue< E >::size() );
    }
    
    /**
     * Gets, but does not remove, the first element in the priority queue
     *
     * @return                      the element with highest priority
     */
    const E& peek() const {
        if ( ArrayQueue< E >::size() <= 0 ) {
            throw std::runtime_error(
                    ArrayQueue< E >::generateEmptyQueueException() );
        }
        return ArrayQueue< E >::peek();
    }
    
    /**
     * Gets and removes the element with highest priority from the queue
     *
     * @return                      the element with highest priority
     */
    E poll() {
        if ( ArrayQueue< E >::size() <= 0 ) {
            throw std::runtime_error(
                            ArrayQueue< E >::generateEmptyQueueException() );
        }
        swap( 1 , ArrayQueue< E >::size() );
        E rtn = ArrayQueue< E >::removeAt(
                            getIdxInArrayQueue( ArrayQueue< E >::size() ) );
        if ( ArrayQueue< E >::size() > 0 ) {
            siftDown( 1 );
        }
        return rtn;
    }
    
    
    string toString() {
        if ( ArrayQueue< E >::size() == 0 ) {
            return "[]";
        }
        else if ( ArrayQueue< E >::size() == 1 ) {
            return Message() << "[" << ArrayQueue< E >::get( 0 ) << "]";
        }
        else {
            Message rtn;
            rtn << "[" << ArrayQueue< E >::get( 0 );
            for ( int i=1 ; i<ArrayQueue< E >::size() ; i++ ) {
                rtn << ", " << ArrayQueue< E >::get( i );
            }
            rtn << "]";
            return rtn;
        }
    }
    
};

#endif
