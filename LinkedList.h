//
//  LinkedList.h
//  Data Structures
//
//  Created by Mickey on 10/14/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_LinkedList_h
#define Data_Structures_LinkedList_h

#include "Message.h"
#include "AbstractList.h"
#include "AbstractSet.h"
#include "LinkedListTests.h"

using std::to_string;

/**
 * A doubly-linked list where every element only knows about the element
 * before and after it
 *
 * @param E                     the type of element in the list
 */
template< typename E >
class LinkedList : public AbstractList< E > {
    friend class LinkedListTests;
    
private:
    
    /**
     * Represents a single element, or link, in this LinkedList
     */
    struct Link {
        E value;
        Link* next;
        Link* prev;
    };
    
    /**
     * A pointer to the first element in this LinkedList
     */
    Link* m_head;
    
    /**
     * A pointer to the last element in this LinkedList
     */
    Link* m_tail;
    
    /**
     * The number of elements in this LinkedList
     */
    int m_numElements;
    
    /**
     * @return                  a pointer to the link at the given index
     *                          of the linked list
     */
    Link* getLinkPtr( int index ) const {
        
        //if the index is closer to the head of the list, start there and work
        //forwards
        if ( index < m_numElements/2 ) {
            Link* currElement = m_head;
            int currIdx = 0;
            while( currIdx < index ) {
                currElement = (*currElement).next;
                currIdx++;
            }
            return currElement;
        }
        
        //if the index is closer to the end of the list, start there and work
        //backwards
        else {
            Link* currElement = m_tail;
            int currIdx = m_numElements - 1;
            while( currIdx > index ) {
                currElement = (*currElement).prev;
                currIdx--;
            }
            return currElement;
        }
    }
    
    /**
     * Removes the given link from the list
     *
     * @param link              the link to remove
     */
    void removeLink( Link* link ) {
        Link* prevPtr = (*link).prev;
        Link* nextPtr = (*link).next;
        
        //remove the next and previous pointers to the element to be removed:
        //first, check that the previous element exists.
        if ( prevPtr == 0 ) {
            
            //if there is no previous element, then we removed the head
            //so update the head to point to the next element
            m_head = nextPtr;
        }
        else {
            (*prevPtr).next = nextPtr;
        }
        
        //second, check that the next element exists.
        if ( nextPtr == 0 ) {
            
            //if there is no next element, then we removed the tail
            //so update the tail to point to the previous element
            m_tail = prevPtr;
        }
        else {
            (*nextPtr).prev = prevPtr;
        }
        
        //delete the link
        delete link;
    }
    
public:
    
    /**
     * Creates a default LinkedList with no elements.
     */
    LinkedList() {
        m_head = 0;
        m_tail = 0;
        m_numElements = 0;
    }
    
    /**
     * Determines if the given element is contained by the list
     * 
     * @param value             the value for which to look
     * @return                  if the given value is contained by the list
     */
    bool contains( const E& value ) const {
        return indexOf( value ) != -1;
    }
    
    /**
     * Returns the index of the first occurence of the given element
     *
     * @param value             the value for which to look
     * @return                  the index of the first occurence of the given
     *                          value, or -1 if the value was not found
     */
    int indexOf( const E& value ) const {
        
        //if the list is empty, then the value is clearly not in the list.
        //this is a special case because there is no head of the linked list.
        if ( m_numElements == 0 ) {
            return -1;
        }
        
        //if the list is non-empty, then the head of the list must exist
        int currIdx = 0;
        Link* currElement = m_head;
        
        //go through the list and if we see the specified value,
        //return that value
        while ( currElement != m_tail ) {
            if ( (*currElement).value == value ) {
                return currIdx;
            }
            currElement = (*currElement).next;
            currIdx++;
        }
        
        //check if the tail contains the value for which we are looking
        if ( (*currElement).value == value ) {
            return currIdx;
        }
        
        //if the value was not found, then return -1
        return -1;
    }
    
    /**
     * Inserts the given element at the specified index in the list. Subsequent
     * elements are shifted down by 1 index value.
     *
     * @param index             the index at which to insert the element
     * @param value             the value to insert
     */
    void insert( int index , const E& value ) {
        
        //make sure we are not inserting out of bounds
        if ( index < 0 || index > m_numElements ) {
            throw std::runtime_error( generateInsertOutOfBoundsErrorMessage(
                                                    index , m_numElements ) );
        }
        
        //create the new element that must be inserted
        Link* newElement = new Link;
        (*newElement).value = value;
        (*newElement).prev = 0;
        (*newElement).next = 0;
        Link* nextElement;
        
        //inserting at the front is a special case because we have to modify
        //the head
        if ( index == 0 ) {
            nextElement = m_head;
            m_head = newElement;
            (*newElement).next = nextElement;
        }
        else {
            
            //go to the element just before the index at which we are inserting
            //this element will be the element just before the element we are
            //inserting
            Link* prevElement = getLinkPtr( index-1 );
            
            //and the element right after that will be the element just after
            //the elemnt we are inserting
            nextElement = (*prevElement).next;
            
            //update the previous element's next link
            (*prevElement).next = newElement;
            
            //update the new element's previous and next links
            (*newElement).prev = prevElement;
            (*newElement).next = nextElement;
        }
        
        //handle the prev link in the element directly after the new element we
        //inserted. note that if the element after the new element does not
        //exist, we must be careful about accessing a nonexisting link
        
        //if next element does not exist, then the new element must be the tail
        if ( nextElement == 0 ) {
            m_tail = newElement;
        }
        
        //if the next element exists, then update its previous link
        else {
            (*nextElement).prev = newElement;
        }
        m_numElements++;
    }
    
    /**
     * Inserts the given element at the front of the list. This has the same
     * functionality as insert( 0 , value );
     *
     * @param value             the value to insert at the front of the list
     */
    void insert( const E& value ) {
        insert( 0 , value );
    }
    
    /**
     * Sets the element at the given index to the specified value
     *
     * @param index             index of the element in the list to modify
     * @param value             the new value to be set at the given index
     *                          of the list
     */
    void set( int index , const E& value ) {
        if ( index < 0 || index >= m_numElements ) {
            throw std::runtime_error( generateAccessOutOfBoundsErrorMessage(
                                                    index , m_numElements ) );
        }
        Link* elementToModifyPtr = getLinkPtr( index );
        (*elementToModifyPtr).value = value;
    }
    
    /**
     * Appends the given element to the end of the list in O(1) time.
     *
     * @param value             the value to append to the end of the list
     */
    void append( const E& value ) {
        
        //if list has 0 or 1 element, we can just insert it to the front and the
        //efficiency is approximately the same
        if ( m_numElements == 0 ) {
            insert( m_numElements , value );
        }
        
        //if list is not empty, we might be more efficient just accessing
        //the last element and inserting this new element there
        else {
            Link* currLastElement = m_tail;
            Link* newElement = new Link;
            (*newElement).value = value;
            (*newElement).prev = currLastElement;
            (*newElement).next = 0;
            (*currLastElement).next = newElement;
            
            //don't forget to update the tail to point to the new last element
            m_tail = newElement;
            m_numElements++;
        }
    }
    
    /**
     * Returns the element at the specified index
     *
     * @param index             the index of an element in the list
     */
    E& get( int index ) const {
        
        //make sure not accessing out of bounds
        if ( index < 0 || index >= m_numElements ) {
            throw std::runtime_error( generateAccessOutOfBoundsErrorMessage(
                                                    index , m_numElements ) );
        }
        
        Link* elementPtr = getLinkPtr( index );
        return (*elementPtr).value;
    }
    
    /**
     * Removes the given value from the list if it exists.
     *
     * @param value             the value to remove from the list
     * @return                  if the value was sucessfully removed
     */
    bool remove( const E& value ) {
        Link* toRemove = 0;
        
        //check if the tail contains the value we wish to remove
        //it's okay if it is not the first occurence, because
        //we will find the actual first occurrence later
        if ( m_tail != 0 ) {
            if ( (*m_tail).value == value ) {
                toRemove = m_tail;
            }
        }
        
        Link* currElement = m_head;
        //go through the list from the start and look for the first
        //occurence of the given value
        while( currElement != m_tail ) {
            if ( (*currElement).value == value ) {
                toRemove = currElement;
                break;
            }
            currElement = (*currElement).next;
        }
        
        if ( toRemove == 0 ) {
            return false;
        }
        
        //if we found the element to be removed, then remove it
        else {
            removeLink( toRemove );
            m_numElements--;
            return true;
        }
    }
    
    /**
     * Removes the value at the given index from the list.
     *
     * @param index             the index of the value to remove
     * @return                  the element that was removed
     */
    E removeAt( int index ) {
        if ( index < 0 || index >= m_numElements ) {
            throw std::runtime_error( generateAccessOutOfBoundsErrorMessage(
                                                    index , m_numElements ) );
        }
        
        //find the element we would like to remove
        Link* elementPtr = getLinkPtr( index );
        
        //store the value of the link to be removed
        E rtn = (*elementPtr).value;
        
        //remove that link from the list
        removeLink( elementPtr );
        
        //update the number of elements in the list
        m_numElements--;
        
        //return the value we removed
        return rtn;
    }
    
    /**
     * Clears the list so that it contains 0 elements.
     */
    void clear() {
        
        //an empty list is a special case, because we cannot access the tail
        if ( m_numElements == 0 ) {
            return;
        }
        else {
            
            //start from the end of the LinkedList
            Link* toDelete = m_tail;
            
            //and go backwards to delete
            //note that it is okay to not modify the prev and next pointers of
            //each element because we are deleting everything
            while ( (*toDelete).prev != 0 ) {
                toDelete = (*toDelete).prev;
                delete (*toDelete).next;
            }
            
            //and we will be left with 1 element that is not deleted,
            //which is the head of the list
            delete m_head;
        }
        
        //reset fields
        m_head = 0;
        m_tail = 0;
        m_numElements = 0;
    }
    
    /**
     * Determines the size of the list
     *
     * @return size             the size of the list
     */
    int size() const {
        return m_numElements;
    }
    
    /**
     * Determines the textual representation of the list. The list is
     * represented as "[<element 1>, <element 2>, <element 3>, ...]"
     *
     * @return                  the textual representation of the list
     */
    string toString() const {
        if ( m_numElements == 0 ) {
            return "[]";
        }
        else {
            Message representation;
            representation << "[";
            
            //there must be at least one element
            Link* currElement = m_head;
            representation << (*currElement).value;
            
            //go through the entire list and add the textual representation
            //of each element
            currElement = (*currElement).next;
            int currIdx = 1;
            while( currIdx < m_numElements ) {
                representation << ", " << (*currElement).value;
                currIdx++;
                currElement = (*currElement).next;
            }
            representation << "]";
            return representation.str();
            return "";
        }
    }
    
    string generateAccessOutOfBoundsErrorMessage( int index , int size ) const {
        return "Error: Out of bounds access at index " + to_string( index ) +
                                    " in list with size " + to_string( size );
    }
    
    string generateInsertOutOfBoundsErrorMessage( int index ,
                                                            int size ) const {
        return "Error: Out of bounds insertion at index " + to_string( index ) +
                                    " in list with size " + to_string( size );
    }
};

#endif
