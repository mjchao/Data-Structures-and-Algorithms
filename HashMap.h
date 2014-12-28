//
//  HashMap.h
//  Data Structures
//
//  Created by Mickey on 11/4/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_HashMap_h
#define Data_Structures_HashMap_h

#include "AbstractMap.h"
#include "Hasher.h"
#include "DefaultHasher.h"
#include "ArrayList.h"

/**
 * A HashMap with buckets for collision resolution. The operations get(),
 * containsKey() and remove() are implemented with O(1) complexity; however
 * put() is generally O(1), but inserting into the map may sometimes cause
 * the entire map to be resized and reorganized, which will be O(n) in the size
 * of the underlying array of the map. Since the size of the underlying array
 * of the map is comparable in size to the number of entries (they differ by
 * a load factor between 0 and 1), this resize is also O(n) in the number of
 * entries in the HashMap.
 *
 * @param Key           the type of key used in the map
 * @param Value         the type of value to be stored in the map
 */
template< typename Key , typename Value >
class HashMap : public AbstractMap< Key , Value > {
    
    friend class HashMapTests;
    
private:
    
    /**
     * An entry in the HashMap. The HashMap is a doubly-linked list
     */
    struct Entry {
        
        //the hashcode for the key to this entry. it is stored so that
        //the map can be resized in the future without needing to recompute
        //hashcodes
        long long hashcode;
        
        //the key to this entry
        const Key& key;
        
        //the value associated with this entry
        Value value;
        
        //the entry preceeding this entry, or 0 if there is not entry before
        //this one
        Entry* prev;
        
        //the entry directly after this entry, or 0 if there is no entry
        //after this one
        Entry* next;
        
        Entry( long long hash , const Key& k , Value v ) : hashcode( hash ) ,
        key( k ) , value( v ) {
            next = 0;
        }
        
        bool operator==( const Entry& e ) {
            return hashcode == e.hashcode;
        }
    };
    
    /**
     * Ensures that the underlying array can hold the given number of elements
     *
     * @param size                  the minimum number of elements the array
     *                              must be capable of holding
     */
    void ensureCapacity( int size );
    
    /**
     * Resizes the HashMap to at minimum the specified size, or to the
     * maximize size allowed (2147483629)
     *
     * @param newSize               the new size for the HashMap
     */
    void resize( int newSize );
    
    /**
     * Transfers the entries in the current table array to a new table array
     *
     * @param newTable              the new table array for HashMap entries
     */
    void transferTo( ArrayList< Entry* >& newTable ) const;
    
    /**
     * Determines the index in the HashMap's array corresponding to the given
     * hashcode
     *
     * @param hashcode              a hashcode
     * @return                      the index in the array corresponding to the
     *                              given hash code
     */
    int indexOf( long long hashcode ) const;
    
    /**
     * Frees the memory associated with entries stored in  a bucket of the table
     *
     * @param e                     the top entry in a bucket
     */
    void freeBucket( Entry* e );
    
    /**
     * Clears the given table and frees all memory associated with it
     *
     * @param table                 the table to clear
     */
    void clearTable( ArrayList< Entry* >& table );
    
    /**
     * the size of the HashMap's underlying array
     */
    int m_size = 1;
    
    /**
     * the number of entries in the HashMap
     */
    int m_entries = 0;
    
    /**
     * the load factor of the HashMap
     */
    double m_loadFactor = 0.7;
    
    ArrayList< Entry* > m_table;
    
    const Hasher< Key >& m_hasher;
    
public:
    
    const int NUM_PREDEF_SIZES = 28;
    
    /**
     * a list of predefined sizes the HashMap can use. They are the 2^n-th
     * primes, except for the last size, which is the largest prime below
     * 2^31-1
     */
    const int SIZES[ 28 ] = { 2, 3 , 7 , 19 , 53 ,
        131 , 311 , 719 , 1619 , 3617 ,
        8161 , 17863 , 38873 , 84017 , 180503 ,
        386093 , 821641 , 1742537 , 3681131 , 7754077 ,
        16290047 , 34136029 , 71378569 , 148948139 , 310248241 ,
        645155197 , 1339484197 , 2147483629 };
    
    const int DEFAULT_SIZE = SIZES[ 2 ];
    
    const DefaultHasher< Key > DEFAULT_HASHER;
    
    /**
     * Creates a default HashMap with default hasher, size, and load factor
     */
    HashMap();
    
    /**
     * Creates a HashMap with the default hasher, the specified initial size,
     * and the default load factor
     *
     * @param initialSize           the number of empty entries the map should
     *                              initially contain
     */
    HashMap( int initialSize );
    
    /**
     * Creates a HashMap with the specified hasher , the default initial size,
     * and the default load factor
     *
     * @param hasher                the hasher for generating hash codes
     */
    HashMap( const Hasher< Key >& hasher );
    
    /**
     * Creates a HashMap with the specified hasher and initial size and the
     * default load factor
     *
     * @param hasher                the hasher for generating hash codes
     * @param initialSize           the number of empty entries the map should
     *                              initially contain
     */
    HashMap( const Hasher< Key >& hasher , int initialSize );
    
    /**
     * Creates a HashMap with the specified hasher, initial size and load factor
     *
     * @param hasher                the hasher for generating hash codes
     * @param initialSize           the number of empty entries the map should
     *                              initially contain
     * @param loadFactor            how full (a value in the interval (0,1) )
     *                              the map should be before resizing it
     */
    HashMap( const Hasher< Key >& hasher , int initialSize , double loadFactor );
    
    /**
     * Frees all memory associated with the HashMap
     */
    ~HashMap();
    
    /**
     * Puts the specified key-value entry into the HashMap
     *
     * @param key                   the key to be used to access the value
     * @param value                 the value to be associated with the key
     */
    virtual void put( const Key& key , Value value );
    
    /**
     * Gets the value associated with the given key
     *
     * @param key                   the key to be used to access a value
     * @return                      the value associated with the key,
     *                              or a default value if the key was never
     *                              placed in the HashMap
     */
    virtual Value get( const Key& key ) const;
    
    /**
     * Determines if the given key is in the HashMap
     *
     * @param key                   a key that could be in the HashMap
     * @return                      if there is an entry with the given key
     *                              stored in the HashMap
     */
    virtual bool containsKey( const Key& key ) const;
    
    /**
     * Removes the given key from the HashMap. The value associated with the key
     * is also removed
     *
     * @param key                   the key to the entry to be removed
     * return                       the value associated with the key that was
     *                              removed
     */
    virtual Value remove( const Key& key );
    
    /**
     * Removes all entries from the HashMap, but does not change the size of the
     * underlying table array.
     */
    void clear();
    
    /**
     * Sets the load factor for this HashMap
     *
     * @param loadFactor             the load factor that determines how full
     *                               the map should be before resizing
     */
    void setLoadFactor( double loadFactor );
};

template< typename Key , typename Value >
HashMap< Key , Value >::HashMap() : m_hasher( DEFAULT_HASHER ) {
    m_size = DEFAULT_SIZE;
    resize( m_size );
}

template< typename Key , typename Value >
HashMap< Key , Value >::HashMap( int initialSize ) :
m_hasher( DEFAULT_HASHER ) {
    m_size = initialSize;
    resize( m_size );
}

template< typename Key , typename Value >
HashMap< Key , Value >::HashMap( const Hasher< Key >& hasher ) :
m_hasher( hasher ) {
    m_size = DEFAULT_SIZE;
    resize( m_size );
}

template< typename Key , typename Value >
HashMap< Key, Value >::HashMap( const Hasher< Key >& hasher, int initialSize ) :
m_hasher( hasher ) {
    m_size = initialSize;
    resize( m_size );
}

template< typename Key , typename Value >
HashMap< Key, Value >::HashMap( const Hasher< Key >& hasher, int initialSize ,
                               double loadFactor ) : m_hasher( hasher ) {
    m_size = initialSize;
    resize( m_size );
    setLoadFactor( loadFactor );
}

template< typename Key , typename Value >
HashMap< Key , Value >::~HashMap() {
    clearTable( m_table );
}

template< typename Key , typename Value >
void HashMap< Key , Value >::freeBucket( Entry* topEntry ) {
    Entry* e = topEntry;
    while( e != 0 ) {
        Entry* next = e->next;
        delete e;
        e = next;
    }
}

template< typename Key , typename Value >
void HashMap< Key , Value >::clearTable( ArrayList< Entry* >& table ) {
    for ( int i=0 ; i<table.size() ; i++ ) {
        if ( table.get( i ) != 0 ) {
            freeBucket( table.get( i ) );
        }
    }
    table.clear();
}

template< typename Key , typename Value >
void HashMap< Key , Value >::ensureCapacity( int size ) {
    
    //we must append 0-pointers to the end of the table array so that
    //the size of the table is big enough and so that we do not set() out
    //of bounds at higher indices
    for ( int i=m_table.size() ; i<size ; i++ ) {
        m_table.append( 0 );
    }
}

template< typename Key , typename Value >
void HashMap< Key , Value >::resize( int size ) {
    
    //pick the smallest predefined prime number that is larger than the
    //specified size.
    
    //if no size meets the size requirement, then we can only use the largest
    //predeinfed prime number available
    int newSize = SIZES[ 0 ];
    if ( newSize < size ) {
        for ( int i=0 ; i< NUM_PREDEF_SIZES ; i++ ) {
            newSize = SIZES[ i ];
            if ( newSize >= size ) {
                break;
            }
        }
    }
    
    ArrayList< Entry* > newTable;
    newTable.ensureCapacity( newSize );
    for ( int i=0 ; i<newSize ; i++ ) {
        newTable.append( 0 );
    }
    transferTo( newTable );
    m_size = newSize;
    m_table = newTable;
}

template< typename Key , typename Value >
void HashMap< Key , Value >::transferTo( ArrayList< Entry* >& newTable ) const {
    for ( int i=0 ; i<m_table.size() ; i++ ) {
        Entry* e = m_table.get( i );
        while ( e != 0 ) {
            Entry* nextElement = e->next;
            
            //the indices of elements in the array must be recalculated
            //because the size has changed
            int newIdx = e->hashcode % newTable.size();
            if ( newTable.get( newIdx ) == 0 ) {
                e->next = 0;
            }
            else {
                e->next = newTable.get( newIdx );
            }
            newTable.set( newIdx , e );
            e = nextElement;
        }
    }
}

template< typename Key , typename Value >
int HashMap< Key , Value >::indexOf( long long hashcode ) const {
    return static_cast<int>( hashcode % m_size );
}

template< typename Key , typename Value >
void HashMap< Key , Value >::put( const Key& key , Value value ) {
    long long hashcode = m_hasher.hash( key );
    int idx = indexOf( hashcode );
    
    Entry* arrPtr = m_table.get( idx );
    
    //check if the key is already in the bucket. if it is, then overwrite it
    for ( Entry* currPtr = arrPtr ; currPtr != 0 ; currPtr = currPtr->next ) {
        if ( m_hasher.areEquivalent( key , currPtr->key ) ) {
            currPtr->value = value;
            //cout << "ok" << endl;
            return;
        }
    }
    
    //check if resize is needed
    if ( m_entries+1 >= m_size*m_loadFactor ) {
        int newSize = (int)(m_size/m_loadFactor+1);
        resize( newSize );
        idx = indexOf( hashcode );
        arrPtr = m_table.get( idx );
    }
    
    Entry* e = new Entry( hashcode , key , value );
    e->next = 0;
    if ( arrPtr != 0 ) {
        e->next = arrPtr;
    }
    m_table.set( idx , e );
    m_entries++;
}

template< typename Key , typename Value >
Value HashMap< Key , Value >:: get( const Key& key ) const {
    Entry* e = m_table.get( indexOf( m_hasher.hash( key ) ) );
    
    //search the bucket for the given key
    while( e != 0 && !m_hasher.areEquivalent( e->key , key ) ) {
        e = e->next;
    }
    if ( e == 0 ) {
        return Value();
    }
    else {
        return e->value;
    }
}

template< typename Key , typename Value >
bool HashMap< Key , Value >::containsKey( const Key& key ) const {
    Entry* e = m_table.get( indexOf( m_hasher.hash( key ) ) );
    while( e != 0 && !m_hasher.areEquivalent( e->key , key ) ) {
        e = e->next;
    }
    if ( e == 0 ) {
        return false;
    }
    else {
        return true;
    }
}

template< typename Key , typename Value >
Value HashMap< Key , Value >::remove( const Key& key ) {
    int idx = indexOf( m_hasher.hash( key ) );
    Entry* e = m_table.get( idx );
    
    if ( e == 0 ) {
        return Value();
    }
    
    //if the entry to remove is the first in the bucket, remove it
    if ( m_hasher.areEquivalent( e->key , key ) ) {
        m_table.set( idx , e->next );
        Value rtn = e->value;
        delete e;
        m_entries--;
        return rtn;
    }
    
    //otherwise, search the bucket for the entry and then remove it
    while( e->next != 0 && !m_hasher.areEquivalent( e->next->key , key ) ) {
        e = e->next;
    }
    if ( e->next == 0 ) {
        return Value();
    }
    else {
        Value rtn = e->next->value;
        Entry* newNext = e->next->next;
        delete e->next;
        m_entries--;
        e->next = newNext;
        return rtn;
    }
}

template< typename Key , typename Value >
void HashMap< Key , Value >::clear() {
    for ( int i=0 ; i<m_table.size() ; i++ ) {
        if ( m_table.get( i ) != 0 ) {
            freeBucket( m_table.get( i ) );
            m_table.set( i , 0 );
        }
    }
    m_entries = 0;
}

template< typename Key , typename Value >
void HashMap< Key , Value >::setLoadFactor( double loadFactor ) {
    if ( m_loadFactor <= 0 || m_loadFactor >= 1 ) {
        throw new std::runtime_error(
                                     "Illegal load factor. Must be between 0 and 1 exclusive.");
    }
    m_loadFactor = loadFactor;
}

#endif