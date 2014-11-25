//
//  ArrayList.h
//  Data Structures
//
//  Created by Mickey on 10/11/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef __Data_Structures__ArrayList__
#define __Data_Structures__ArrayList__

#include <string>
using std::string;

#include <algorithm>
using std::min;

#include "AbstractList.h"
#include "Message.h"
#include "ArrayListTests.h"

using std::to_string;
using std::runtime_error;

/**
 * An array-based list, where elements are stored in an array. Accessing
 * an element or changing the value of an element takes O(1) time, but
 * inserting new elements could take O(n) time in the worse case scenario,
 * where n is the total number of elements contained by the list.
 *
 * The list works by doubling the array size whenever its capacity is exceeded.
 * If the array had capacity 8 and we added the 9th element, the array
 * would resize to 16. The array capacity is halved whenever the number of
 * elements decreases below half capacity. If the array had
 * capacity 8 and we removed the 4th element (3 elements left), the array
 * would resize to 4.
 *
 * @param E                     the type of element stored in this list
 */
template< typename E >
class ArrayList : public AbstractList< E > {
    
    friend class ArrayListTests;
    
private:
    
    /**
     * the maximum number of elements the current list can contain if it is not
     * resized
     */
    int m_arraySize;
    
    /**
     * the number of elements this list currently contains
     */
    int m_numElements;
    
    /**
     * the actual list of elements in this list
     */
    E* m_values;
    
    /**
     * Copies the contents of this array into a new array and replaces the old
     * array-based list with the new array. If the new array has smaller size
     * than the old array, then elements that do not fit into the new array
     * are discarded. If the new array size is less than 1, 1 is automatically
     * used as the new array size
     *
     * @param newArraySize       the new size of the elements array
     */
    void useResizedArray( int newArraySize ) {
        int arraySize = newArraySize;
        if ( arraySize < 1 ) {
            arraySize = 1;
        }
        E* newArr = new E[ arraySize ];
        
        //copy elements into the new array
        int smallestSize = min( m_arraySize , arraySize );
        for ( int i=0 ; i<smallestSize ; i++ ) {
            newArr[ i ] = m_values[ i ];
        }
        
        //free memory of the old array
        delete[] m_values;
        
        //update the list to have the new array and the new size
        m_values = newArr;
        m_arraySize = arraySize;
    }
    
    /**
     * Determines if the given index is out of bounds or not
     *
     * @param index                 an index
     * @return                      if the given index is out of bounds (greater
     *                              than the maximum element index, or less than
     *                              the minimum element index)
     */
    bool isOutOfBounds( int index ) const {
        return ( index < 0 || index >= m_numElements );
    }
    
protected:
    
    /**
     * Gets the maximum number of elements the current array can contain
     *
     * @return                      the current size of the array of this list
     */
    int getArraySize() const {
        return m_arraySize;
    }
    
public:
    
    /**
     * Creates a default ArrayList with no elements
     */
    ArrayList() {
        m_arraySize = 1;
        m_numElements = 0;
        m_values = new E[ 1 ];
    }
    
    /**
     * Copies the contents of the given ArrayList into this ArrayList
     *
     * @param l                     the ArrayList to be copied
     */
    ArrayList( const ArrayList<E>& l ) {
        m_arraySize = l.m_arraySize;
        m_numElements = l.m_numElements;
        m_values = new E[ m_arraySize ];
        for ( int idx=0 ; idx<l.size() ; idx++ ) {
            m_values[ idx ] = l.get( idx );
        }
    }
    
    /**
     * Creates an ArrayList with the given initial size. If the given 
     * initial size is less than 1, then 1 is automatically used
     * as the initial size.
     *
     * @param initialSize           initial number of array slots for
     *                              storing elements of the list
     */
    ArrayList( int initialSize ) {
        int arraySize = initialSize;
        if ( arraySize < 1 ) {
            arraySize = 1;
        }
        m_arraySize = arraySize;
        m_numElements = 0;
        m_values = new E[ arraySize ];
    }
    
    ~ArrayList< E >() {
        delete[] m_values;
    }
    
    /**
     * Copies the contents of the given ArrayList into this ArrayList
     */
    ArrayList<E>& operator=( const ArrayList<E>& listToCopy ) {
        m_arraySize = listToCopy.m_arraySize;
        m_numElements = listToCopy.m_numElements;
        m_values = new E[ m_arraySize ];
        for ( int idx=0 ; idx<m_numElements ; idx++ ) {
            m_values[ idx ] = listToCopy.m_values[ idx ];
        }
        return *this;
    }
    
    /**
     * Determines if the list contains the given value
     *
     * @param value             the value for which to look
     * @return                  if the value was found in the list
     */
    bool contains( const E& value ) const {
        for ( int i=0 ; i<m_numElements ; i++ ) {
            if ( m_values[ i ] == value ) {
                return true;
            }
        }
        return false;
    }
    
    /**
     * Gets the element at the given index
     *
     * @param index             the index of an element in the list
     * @return                  the element at the given index in the list
     * @throws error            if the index given is out of bounds
     */
    E& get( int index ) const {
        
        //make sure not we are not asked to go out of bounds
        if ( isOutOfBounds( index ) ) {
            throw runtime_error( Message() <<
                    generateAccessOutOfBoundsMessage( index , m_numElements ) );
        }
        return m_values[ index ];
    }
    
    /**
     * Gets the index of the first occurrence of the given value
     *
     * @param value             the value for which to look in the list
     * @return                  the first index at which the given value occurs
     */
    int indexOf( const E& value ) const {
        for ( int i=0 ; i<m_numElements ; i++ ) {
            if ( m_values[ i ] == value ) {
                return i;
            }
        }
        return -1;
    }
    
    /**
     * Appends the given element at the end of the list. It is the same as
     * calling append( value );
     *
     * @param value             the element to add to the end of the list
     */
    void insert( const E& value ) {
        append( value );
    }
    
    /**
     * Inserts the given element at the specified index
     *
     * @param index             the index in the ArrayList at which to insert
     *                          the given value
     * @param value             the value to insert
     * @throws error            if the index is out of bounds
     */
    void insert( int index , const E& value ) {
        
        //make sure we aren't asked to insert the value out of bounds
        //we allow insertion at the index following the last element
        //of the array, as that is equivalent to appending to the end
        if ( index < 0 || index > m_numElements ) {
            throw std::runtime_error(
                Message() << generateInsertOutOfBoundsMessage(
                                            index , m_numElements ) );
        }
        
        //resize the elements array if we are over capacity
        //we resize it so that after the insert operation, we have exactly
        //twice as many empty slots as elements
        if ( m_numElements+1 > m_arraySize ) {
            useResizedArray( (m_numElements)*2 );
        }
        
        //shift all the elements including and after the given index
        //down by 1
        for ( int i=m_numElements-1 ; i>=index ; i-- ) {
            m_values[ i+1 ] = m_values[ i ];
        }
        
        //insert the new value
        m_values[ index ] = value;
        m_numElements++;
    }
    
    /**
     * Sets the given index of the list to contain the given element, and if
     * necessary, replaces an element that is currently there
     *
     * @param index             the index at which to set the new value
     * @param value             the new value to be set
     * @throws error            if the index is out of bounds
     */
    void set( int index , const E& value ) {
        if ( isOutOfBounds( index ) ) {
            throw std::runtime_error(
                Message() << generateAccessOutOfBoundsMessage(
                                            index , m_numElements ) );
        }
        
        m_values[ index ] = value;
    }
    
    
    /**
     * Adds the given element to the end of the list
     *
     * @param value             the element to add
     */
    void append( const E& value ) {
        insert( m_numElements , value );
    }
    
    /**
     * Removes the element at the specified index and shifts subsequent elements
     * down by 1 index value
     *
     * @param index             the index of the element to be removed
     * @return                  the value of the element that was removed
     */
    E removeAt( int index ) {
        
        //make sure we are not going out of bounds
        if ( isOutOfBounds( index ) ) {
            throw std::runtime_error(
                    Message() << generateAccessOutOfBoundsMessage(
                                            index , m_numElements ) );
        }
        
        E rtn = m_values[ index ];
        
        //move all elements down by 1 index
        for ( int idx=index ; idx<m_numElements-1 ; idx++ ) {
            m_values[ idx ] = m_values[ idx+1 ];
        }
        
        //clear the last spot in the list, because we lost 1 element
        //m_values[ m_numElements ] = 0;
        
        //update the number of elements
        m_numElements--;
        
        //resize array capacity if we have too many unused spaces.
        //we resize the list so that it has exactly twice as many empty
        //slots as elements
        if ( m_numElements*2 < m_arraySize ) {
            useResizedArray( m_arraySize/2 );
        }
        
        return rtn;
    }
    
    /**
     * Tries to remove the given element from the list, and return if the
     * operation was successful or not
     *
     * @param value             the element to be removed
     * @return                  if the element was removed or not
     */
    bool remove( const E& value ) {
        for ( int idx=0 ; idx<m_numElements ; idx++ ) {
            if ( m_values[ idx ] == value ) {
                removeAt( idx );
                return true;
            }
        }
        return false;
    }
    
    /**
     * Removes all elements from the ArrayList
     */
    void clear() {
        useResizedArray( 0 );
        m_numElements = 0;
        m_arraySize = 0;
    }
    
    int size() const {
        return this->m_numElements;
    }
    
    /**
     * Ensures that the ArrayList can contain the given number of elements
     * without resizing
     *
     * @param capacity          a minimum number of elements the list must
     *                          be able to contain without resizing
     */
    void ensureCapacity( int capacity ) {
        if ( capacity > m_arraySize ) {
            useResizedArray( capacity );
        }
    }
    
    string toString() const {
        Message strRepresentation;
        strRepresentation << "[";
        if ( m_numElements >= 1 ) {
            strRepresentation << m_values[ 0 ];
        }
        for ( int idx=1 ; idx<m_numElements ; idx++ ) {
            strRepresentation << ", " << m_values[ idx ];
        }
        strRepresentation << "]";
        return strRepresentation.str();
    }
    
    string generateAccessOutOfBoundsMessage( int index , int size ) const {
        return "Error: Out of bounds access at index " + to_string( index ) +
        " in list with size " + to_string( size );
    }
    
    string generateInsertOutOfBoundsMessage( int index , int size ) const {
        return "Error: Out of bounds insertion at index " + to_string( index ) +
        " in list with size " + to_string( size );
    }
};

#endif /* defined(__Data_Structures__ArrayList__) */
