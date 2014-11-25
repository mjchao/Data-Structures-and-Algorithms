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
#include "LinkedList.h"

template< typename Key , typename Value >
class HashMap : public AbstractMap< Key , Value > {
  
    friend class HashMapTests;
    
private:
    
    /**
     * The hasher used to generate hash code for the keys in this map
     */
    Hasher< Key >* m_hasher;
    
    /**
     * A key-value pair to be stored in the HashMap. In a HashMap, keys
     * should not be modifiable, so we make them constant; however, values
     * can be changed, so we do not make them constant.
     */
    struct KVPair {
        Hasher< Key >* hasher = 0;
        
        const Key* key;
        Value value;
        
        ~KVPair() {
            
        }
        
        bool operator==( const KVPair& p ) const {
            if ( hasher == 0 ) {
                return (*key) == *(p.key);
            }
            else {
                return hasher->areEquivalent( *key , *(p.key) );
            }
        }
    };
    
    /**
     * The entries in this map
     */
    ArrayList< LinkedList< KVPair >* > m_entries;
    
    /**
     * The key chains that will need to be deleted. Since ArrayList does not
     * use smart pointers, it does not delete the key chain's allocated memory.
     * Thus, we will have to delete them manually ourselves.
     */
    ArrayList< LinkedList< KVPair >* > m_allocatedMemory;
    
    int m_loadFactor;
    
    /**
     * Ensures that the array can hold the given number of value chains
     * by filling the underlying ArrayList with 0-pointers until the
     * required size is fulfilled.
     *
     * @param size              the number of chains this HashMap must be
     *                          able to contain
     */
    void ensureArrayCapacity( int size ) {
        m_entries.ensureCapacity( size );
        for ( int i=m_entries.size()-1 ; i<size ; i++ ) {
            m_entries.append( 0 );
        }
    }
    
    /**
     * Searches the chain of entries for an entry with the given key
     *
     * @param chain             the chain of entries in which to look
     * @param k                 the key for which to look
     * @return                  the entry in the chain with the given key,
     *                          or 0 if no entry was found
     */
    KVPair* searchChain( LinkedList< KVPair* > chain , Key k ) {
        KVPair testPair;
        testPair.key = k;
        int idx = chain.indexOf( testPair );
        if ( idx == -1 ) {
            return 0;
        }
        else {
            return chain.get( idx );
        }
    }
    
public:
    
    const int DEFAULT_ARRAY_SIZE = 13;
    const int DEFAULT_LOAD_FACTOR = 0.7;
    
    /**
     * Creates a default HashMap that uses a default hasher, load size, and
     * entry array size
     */
    HashMap() {
        m_hasher = new DefaultHasher< Key >();
        m_loadFactor = DEFAULT_LOAD_FACTOR;
        ensureArrayCapacity( DEFAULT_ARRAY_SIZE );
        m_hasher->handleHashMapResize( DEFAULT_ARRAY_SIZE );
    }
    
    ~HashMap() {
        delete m_hasher;
        while( m_allocatedMemory.size() > 0 ) {
            delete m_allocatedMemory.removeAt( m_allocatedMemory.size()-1 );
        }
        m_entries.clear();
    }
    
    /**
     * Creates a HashMap that uses the given hasher and a default load size
     * and entry array size
     *
     * @param h                 the hasher to use to generate hashcodes for
     *                          objects
     */
    HashMap( Hasher< Key >& h ) {
        m_hasher = &h;
        ensureArrayCapacity( DEFAULT_ARRAY_SIZE );
        m_loadFactor = DEFAULT_LOAD_FACTOR;
    }
    
    /**
     * Creates a Hashmap that uses the given initial entry array size
     * and uses the default hasher for the given array size and
     * default load factor
     *
     * @param initialSize       the initial size of the entry array
     */
    HashMap( int initialSize ) {
        m_hasher = new DefaultHasher< Key >( initialSize );
        m_loadFactor = DEFAULT_LOAD_FACTOR;
        ensureArrayCapacity( initialSize );
    }
    
    /**
     * Creates a HashMap that uses the given hasher, initial entry array size
     * and default load value
     *
     * @param h                 the hasher to use to generate hashcodes for keys
     * @param initialSize       the initial size of the entry-array size
     */
    HashMap( Hasher< Key > h , int initialSize ) {
        m_hasher = &h;
        ensureArrayCapacity( initialSize );
        m_loadFactor = DEFAULT_LOAD_FACTOR;
    }
    
    /**
     * Creates a HashMap that uses the given hasher, initial entry array size
     * and load factor
     *
     * @param h                 the hasher to use to generate hashcodes for keys
     * @param initialSize       the intiial size of the entry-array size
     * @param loadFactor        the ratio of entries to entry-array size to
     *                          maintain
     */
    HashMap( const Hasher< Key >& h , int initialSize , int loadFactor ) {
        m_hasher = &h;
        ensureArrayCapacity( initialSize );
        m_loadFactor = loadFactor;
    }
    
    /**
     * Puts the given {key, value} pair into the map.
     *
     * @param k             the key
     * @param v             the value corresponding to the key
     */
    virtual void put( const Key& k , Value v ) {
        KVPair newPair;
        
        newPair.hasher = m_hasher;
        
        newPair.key = &k;
        
        newPair.value = v;
        
        int chainIdx = (int)m_hasher->hash( k );
        LinkedList< KVPair >* keyChain = m_entries.get( chainIdx );
        
        if ( keyChain == 0 ) {
            LinkedList< KVPair >* newChain = new LinkedList< KVPair >();
            m_allocatedMemory.append( newChain );
            m_entries.set( chainIdx , newChain );
            m_entries.get( chainIdx )->append( newPair );
        }
        else {
            int pairIdx = keyChain->indexOf( newPair );
            if ( pairIdx == -1 ) {
                keyChain->append( newPair );
            }
            else {
                keyChain->get( pairIdx ).value = v;
            }
        }
    }
    
    /**
     * Returns the value mapped to the given key
     *
     * @param k             the key
     * @return              the value corresponding to the given key, or 0
     *                      if the key was never in the map
     */
    virtual Value get( const Key& k ) const {
        int hashCode = (int)m_hasher->hash( k );
        LinkedList< KVPair >* chain = m_entries.get( hashCode );
        if ( chain == 0 ) {
            return 0;
        }
        else {
            KVPair entryToSearch;
            entryToSearch.hasher = m_hasher;
            entryToSearch.key = &k;
            int idx = chain->indexOf( entryToSearch );
            if ( idx == -1 ) {
                return 0;
            }
            else {
                return chain->get( idx ).value;
            }
        }
    }
    
    /**
     * Determines if the given key is in the map.
     *
     * @param k             the key
     * @return              if the key is in the map
     */
    virtual bool containsKey( const Key& k ) const {
        return get( k ) != 0;
    }
    
    /**
     * Removes the given key from the map.
     *
     * @param k             the key to remove
     * @return              a pointer to the value that was removed, or 0
     *                      if the value was not found.
     */
    virtual Value remove( const Key& k ) {
        int hashCode = (int)m_hasher->hash( k );
        LinkedList< KVPair >* chain = m_entries.get( hashCode );
        if ( chain == 0 ) {
            return 0;
        }
        else {
            KVPair entryToSearch;
            entryToSearch.hasher = m_hasher;
            entryToSearch.key = &k;
            int idx = chain->indexOf( entryToSearch );
            if ( idx == -1 ) {
                return 0;
            }
            else {
                Value rtn = chain->get( idx ).value;
                chain->removeAt( idx );
                return rtn;
            }
        }
        return 0;
    }

    
};

#endif
