//
//  AbstractMap.h
//  Data Structures
//
//  Created by Mickey on 10/25/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_AbstractMap_h
#define Data_Structures_AbstractMap_h

/**
 * A map of keys to values.
 */
template< typename Key, typename Value >
class AbstractMap {
    
public:
    
    /**
     * Puts the given {key, value} pair into the map
     *
     * @param k             the key
     * @param v             the value corresponding to the key
     */
    virtual void put( const Key& k , Value v ) = 0;
    
    /**
     * Returns the value mapped to the given key
     *
     * @param k             the key
     * @return              the value corresponding to the given key
     */
    virtual Value* get( const Key& k ) const = 0;

    /**
     * Determines if the given key is in the map.
     *
     * @param k             the key
     * @return              if the key is in the map
     */
    virtual bool containsKey( const Key& k ) const = 0;
    
    /**
     * Determines if the given value is in the map.
     *
     * @param v             the value
     * @return              if the value is in the map
     */
    virtual bool containsValue( const Value& v ) const = 0;
    
    /**
     * Removes the given key from the map.
     *
     * @param k             the key to remove
     * @return              a pointer to the value that was removed, or 0
     *                      if the value was not found.
     */
    virtual Value* remove( const Key& k ) = 0;
    
};

#endif
