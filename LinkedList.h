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
     * A reference to the first element in this LinkedList
     */
    Link* m_head;
    
    /**
     * A reference to the last element in this LinkedList
     */
    Link* m_tail;
    
    /**
     * The number of elements in this LinkedList
     */
    int m_numElements;
    
public:
    LinkedList() {
        m_head = 0;
        m_tail = 0;
        m_numElements = 0;
    }
    
    bool contains( const E& value ) const {
        //TODO
        return true;
    }
    
    int indexOf( const E& value ) const {
        //TODO
        return 0;
    }
    
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
            int currIdx = 0;
            Link* prevElement = m_head;
            while( currIdx < index-1 ) {
                prevElement = (*prevElement).next;
                currIdx++;
            }
            
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
    
    void insert( const E& value ) {
        //TODO
    }
    
    void set( int index , const E& value ) {
        //TODO
    }
    
    void append( const E& value ) {
        //TODO
    }
    
    E& get( int index ) const {
        
        //make sure not accessing out of bounds
        if ( index < 0 || index >= m_numElements ) {
            throw std::runtime_error( generateAccessOutOfBoundsErrorMessage(
                                                    index , m_numElements ) );
        }
        
        int currIdx = 0;
        Link* currElement = m_head;
        while( currIdx < index ) {
            currElement = (*currElement).next;
            currIdx++;
        }
        
        return (*currElement).value;
    }
    
    bool remove( const E& value ) {
        //TODO
        return 0;
    }
    
    E removeAt( int index ) {
        //TODO
        return (*m_head).value;
    }
    
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
    
    int size() const {
        return m_numElements;
    }
    
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
