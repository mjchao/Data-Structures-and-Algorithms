//
//  BPTree.h
//  Data Structures
//
//  Created by Mickey on 12/24/14.
//  Copyright (c) 2014 Mickey. All rights reserved.
//

#ifndef Data_Structures_BPTree_h
#define Data_Structures_BPTree_h

#include "Comparator.h"

#include <string>
using std::string;

#include "Message.h"


/**
 * A B+ Tree which maintains that each node is at least half full, except the
 * root. The order if the B+ Tree, m, defines the number of entries permitted 
 * in each node.
 *
 * The tree is balanced, and given that there are n entries total, the height of
 * the tree is log(n). Thus, the operations to insert, find, and delete can be
 * performed in log(n) time. The base of the logarithm is m, the order of the
 * B+ Tree.
 */
template< typename Key , typename Value >
class BPTree {
    
    friend class BPTreeTests;
    
private:
    
    const Comparator< Key >* m_comparator;
    const int m_order;
    
    /**
     * A node in a B+ Tree. It stores some keys in increasing order, which
     * allows algorithms to quickly process a node and search for the next
     * branch to take (if the node is not a leaf) or search for the desired
     * key (if the node is a leaf).
     */
    class BPNode {
        
    private:
        
        Comparator< Key >* m_comparator;
        
        int compare( Key k1 , Key k2 ) {
            if ( m_comparator == 0 ) {
                if ( k1 < k2 ) {
                    return -1;
                }
                else if (k1 == k2 ) {
                    return 0;
                }
                else {
                    return 1;
                }
            }
            else {
                return m_comparator->compare( k1 , k2 );
            }
        }
        
        struct BPEntry {
            
            Key key;
            Value* value;
            BPNode* leftNode;
            BPNode* rightNode;
            
            BPEntry( Key k ) {
                key = k;
                value = 0;
                leftNode = 0;
                rightNode = 0;
                rightNode = 0;
            }
            
            BPEntry( Key k , Value v ) {
                key = k;
                value = new Value;
                *value = v;
                leftNode = 0;
                rightNode = 0;
                rightNode = 0;
            }
            
            BPEntry( const BPEntry& other ) {
                key = other.key;
                value = other.value;
                leftNode = other.leftNode;
                rightNode = other.rightNode;
            }
            
            ~BPEntry() {
                if ( value != 0 ) {
                    delete value;
                }
            }
            
            BPEntry& operator=( const BPEntry& other ) {
                key = other.key;
                value = other.value;
                leftNode = other.leftNode;
                rightNode = other.rightNode;
                return *this;
            }
        };
        
        BPNode* m_parent;
        int m_order;
        int m_numRecords;
        bool m_isLeaf;
        BPEntry** m_entries;
        BPNode* m_rightSibling;
        
        /**
         * Initializes the node to reflect its initial properties, such as order
         */
        void initialize() {
            m_parent = 0;
            m_numRecords = 0;
            m_isLeaf = true;
            m_entries = new BPEntry*[ m_order ];
            m_rightSibling = 0;
        }
        
        /**
         * Performs a binary search and determines the index at which the
         * given key should be inserted into the entries array
         *
         * @param entries           an array of entries through which to search
         * @param entriesSize       the size of the entry array
         * @param k                 the key to insert
         */
        int locateIdx( BPEntry** entries , int entriesSize , const Key& k ) {
            int low = 0;
            int high = entriesSize-1;
            int guess = (low+high)/2;
            while( low < high ) {
                if ( compare( entries[ guess ]->key , k ) < 0 ) {
                    low = guess+1;
                }
                else {
                    high = guess-1;
                }
                guess = (low+high)/2;
            }
            return guess;
        }
        
        /**
         * Inserts the given key and value into the given entries array. This
         * must be a leaf node to work properly.
         *
         * @param entries           an array of entries into which to insert
         * @param entriesSize       the size of the entries array
         * @param toInsert          the entry to insert
         */
        void insertEntry( BPEntry** entries , int entriesSize ,
                         BPEntry* toInsert ) {
            int searchIdx = locateIdx( entries , entriesSize , toInsert->key );
            
            //determine the index at which we should insert the new entry
            int insertIdx;
            
            //if we are inserting at the end of the array, then
            //we insert there no matter what
            if ( searchIdx >= entriesSize ) {
                insertIdx = searchIdx;
            }
            
            //if we are inserting at a location in the array that's empty,
            //then we insert there no matter what
            else if ( entries[ searchIdx ] == 0 ) {
                insertIdx = searchIdx;
            }
            else {
                
                //if the entry currently in the array is less,
                //then we need to insert the new entry after the current element
                if ( compare( entries[ searchIdx ]->key, toInsert->key ) <= 0 ) {
                    insertIdx = searchIdx+1;
                }
                
                //on the other hand, if the entry currently in the array is
                //greater, than we need to insert the new entry before
                //that current element
                else {
                    insertIdx = searchIdx;
                }
            }
            
            //shift everything after the index down the array by 1
            for ( int i=entriesSize-1 ; i>=searchIdx ; i-- ) {
                entries[ i+1 ] = entries[ i ];
            }
            
            //then put the new entry in its place
            entries[ insertIdx ] = toInsert;
            
            //and update the neighbors and their siblings
            if ( insertIdx-1 >= 0 ) {
                entries[ insertIdx-1 ]->rightNode = toInsert->leftNode;
                if ( entries[ insertIdx-1 ]->rightNode != 0 ) {
                    
                    //TODO explain why would code fail without this line?
                    entries[ insertIdx-1 ]->leftNode->m_rightSibling =
                                            entries[ insertIdx-1 ]->rightNode;
                    entries[ insertIdx-1 ]->rightNode->m_rightSibling =
                                                            toInsert->rightNode;
                }
            }
            if ( insertIdx+1 < entriesSize+1 ) {
                if ( entries[ insertIdx+1 ] != 0 ) {
                    entries[ insertIdx+1 ]->leftNode = toInsert->rightNode;
                    if ( toInsert->rightNode != 0 ) {
                        toInsert->leftNode->m_rightSibling =
                                                    toInsert->rightNode;
                        toInsert->rightNode->m_rightSibling =
                                        entries[ insertIdx+1 ]->rightNode;
                    }
                }
            }
        }
        
        /**
         * Inserts a key that was just promoted into this node. This must be
         * a non-leaf node for this function to work properly.
         *
         * @param k                 the key that was just promoted
         * @param leftNode          the left node attached to the key
         * @param rightNode         the right node attached to the key
         * @return                  a pointer to the new root node if applicable
         */
        BPNode* insertPromotedKey( const Key& k , BPNode* leftNode ,
                               BPNode* rightNode );
        
        /**
         * Splits this node in half when it is full. This function will not work
         * correctly if the node is not full.
         *
         * @param toInsert          the entry to be inserted into this full node
         * @return                  a pointer to the new root if a new root was
         *                          created
         */
        BPNode* split( BPEntry* toInsert) {
            BPEntry* combinedEntries[ m_numRecords+1 ];
            for ( int i=0 ; i<m_numRecords ; i++ ) {
                combinedEntries[ i ] = m_entries[ i ];
            }
            insertEntry( combinedEntries , m_numRecords+1 , toInsert );
            
            BPNode* parent = this->m_parent;
            BPNode* rtn = 0;
            
            //create the parent if the parent does not yet exist
            //this must mean that the root has split
            if ( parent == 0 ) {
                parent = new BPNode( m_comparator , m_order );
                
                //since a new root was created, we must indicate that in the
                //return value
                rtn = parent;
            }
            parent->m_isLeaf = false;
            
            //split the overfilled array evenly
            int middleIdx = (m_numRecords+1)/2;
            BPNode* leftNode = new BPNode( m_comparator , m_order );
            leftNode->m_isLeaf = this->m_isLeaf;
            BPNode* rightNode = new BPNode( m_comparator , m_order );
            rightNode->m_isLeaf = this->m_isLeaf;
            leftNode->m_rightSibling = rightNode;
            for ( int i=0 ; i<middleIdx ; i++ ) {
                leftNode->m_entries[ i ] = combinedEntries[ i ];
                if ( !leftNode->isLeaf() ) {
                    leftNode->m_entries[ i ]->leftNode->m_parent = parent;
                    leftNode->m_entries[ i ]->rightNode->m_parent = parent;
                }
                leftNode->m_numRecords++;
            }
            for ( int i=middleIdx ; i<m_numRecords+1 ; i++ ) {
                rightNode->m_entries[ i-middleIdx ] = combinedEntries[ i ];
                if ( !rightNode->isLeaf() ) {
                    rightNode->m_entries[ i-middleIdx ]->leftNode->m_parent =
                                                                        parent;
                    rightNode->m_entries[ i-middleIdx ]->rightNode->m_parent =
                                                                        parent;
                }
                rightNode->m_numRecords++;
            }
            
            //promote the first key in the right branch into the parent's
            //entries
            Key& promotedKey = rightNode->m_entries[ 0 ]->key;
            
            //insert the promoted key into the parent
            BPNode* higherParent = parent->insertPromotedKey(
                                            promotedKey, leftNode, rightNode );
            
            //if we didn't create a new root at this level,
            //then we ask the node at the next highest level if there was
            //a new root created
            if ( rtn == 0 ) {
                rtn = higherParent;
            }
            
            //update the left and right branches of their parents
            leftNode->m_parent = parent;
            rightNode->m_parent = parent;
            
            return rtn;
        }
        
    public:
        
        BPNode() : m_comparator( 0 ) , m_order( DEFAULT_ORDER ) {
            initialize();
        }
        
        BPNode( int order ) : m_comparator( 0 ) , m_order( order ) {
            initialize();
        }
        
        BPNode( Comparator< Key >* comp ) : m_comparator( comp ) ,
        m_order( DEFAULT_ORDER ) {
            initialize();
        }
        
        BPNode( Comparator< Key >* comp , int order ) : m_comparator( comp ) ,
        m_order( order ) {
            initialize();
        }
        
        BPNode( const BPNode& other ) {
            m_comparator = other.m_comparator;
            m_parent = other.m_parent;
            m_order = other.m_order;
            m_numRecords = other.m_numRecords;
            m_isLeaf = other.m_isLeaf;
            for ( int i=0 ; i<m_numRecords ; i++ ) {
                if ( other.m_entries[ i ] != 0 ) {
                    m_entries[ i ] = other.m_entries[ i ];
                }
            }
        }
        
        ~BPNode() {
            for ( int i=0 ; i<m_numRecords ; i++ ) {
                if ( m_entries[ i ] != 0 ) {
                    delete m_entries[ i ];
                }
            }
            delete[] m_entries;
        }
        
        BPNode& operator=( const BPNode& other ) {
            m_comparator = other.m_comparator;
            m_parent = other.m_parent;
            m_order = other.m_order;
            m_numRecords = other.m_numRecords;
            m_isLeaf = other.m_isLeaf;
            for ( int i=0 ; i<m_numRecords ; i++ ) {
                if ( other.m_entries[ i ] != 0 ) {
                    m_entries[ i ] = other.m_entries[ i ];
                }
            }
            return *this;
        }
        
        bool isLeaf() {
            return m_isLeaf;
        }
        
        int getNumRecords() {
            return m_numRecords;
        }
        
        BPNode* getParent() {
            return m_parent;
        }
        
        BPNode* getLeftmostChild() {
            return m_entries[ 0 ]->leftNode;
        }
        
        /**
         * Finds the branch that should contain the given key
         *
         * @param k                 the key for which to look
         * @return                  the branch that should contain the given key
         *                          or 0 if no branch was found
         */
        BPNode* findBranch( const Key& k ) {
            int locationIdx = locateIdx( m_entries , m_numRecords , k );
            if ( m_entries[ locationIdx ] == 0 ) {
                return 0;
            }
            else {
                BPEntry* guideEntry = m_entries[ locationIdx ];
                if ( compare( guideEntry->key , k ) < 0 ) {
                    return guideEntry->rightNode;
                }
                else {
                    return guideEntry->leftNode;
                }
            }
        }
        
        BPNode* insertKeyValue( const Key& k , const Value& v ) {
            BPEntry* toInsert = new BPEntry( k , v );
            if ( m_numRecords == 0 ) {
                m_entries[ 0 ] = toInsert;
            }
            else if ( m_numRecords == m_order ) {
                return split( toInsert );
            }
            else {
                insertEntry( m_entries , m_numRecords , toInsert );
            }
            m_numRecords++;
            return 0;
        }
        
        string toString() {
            Message rtn;
            rtn << "[";
            if ( m_numRecords == 0 ) {
                rtn << "]";
            }
            else if ( m_numRecords == 1 ) {
                rtn << m_entries[ 0 ]->key << "]";
            }
            else {
                rtn << m_entries[ 0 ]->key;
                for ( int i=1 ; i<m_numRecords ; i++ ) {
                    rtn << ", " << m_entries[ i ]->key;
                }
                rtn << "]";
            }
            return rtn.str();
        }
        
        /**
         * Returns the textual representation of this node and all its 
         * siblings to the right.
         */
        string toStringSiblings() {
            Message rtn;
            rtn << this->toString();
            BPNode* currNode = this->m_rightSibling;
            while( currNode != 0 ) {
                rtn << ", " << currNode->toString();
                currNode = currNode->m_rightSibling;
            }
            return rtn.str();
        }
    };
    
    /**
     * root node of the tree, which may be less than half full
     */
    BPNode* m_root;
    
public:
    
    /**
     * the default number of entries per node in the BP Tree
     */
    const static int DEFAULT_ORDER = 100;
    
    BPTree() : m_order( DEFAULT_ORDER ) , m_comparator( 0 ) ,
                                    m_root( new BPNode( m_order ) ) {
    }
    
    BPTree( Comparator< Key > comparator ) : m_order( DEFAULT_ORDER ) ,
                m_comparator( comparator ) , m_root( new BPNode( m_order ) ) {
    }
    
    BPTree( int order ) : m_order( order ) , m_comparator( 0 ) ,
                                            m_root( new BPNode( m_order ) ) {
    }
    
    BPTree( int order , Comparator< Key > comparator ) : m_order( order ) ,
                m_comparator( comparator ) , m_root( new BPNode( m_order ) ) {
    }
    
    void insert( const Key& k , const Value& v ) {
        BPNode* curr = m_root;
        while( !curr->isLeaf() ) {
            curr = curr->findBranch( k );
        }
        BPNode* newRoot = curr->insertKeyValue( k , v );
        if ( newRoot != 0 ) {
            m_root = newRoot;
        }
    }
    
    string toString() {
        Message rtn;
        rtn << "[" << m_root->toStringSiblings();
        if ( !m_root->isLeaf() ) {
            rtn << "\n";
            BPNode* curr = m_root->getLeftmostChild();
            while( !curr->isLeaf() ) {
                rtn << curr->toStringSiblings() << "\n";
                curr = curr->getLeftmostChild();
            }
            rtn << curr->toStringSiblings();
        }
        rtn << "]";
        return rtn.str();
    }
};

/**
 * Inserts a key that was just promoted into this node. This must be
 * a non-leaf node for this function to work properly.
 *
 * @param k                 the key that was just promoted
 * @param leftNode          the left node attached to the key
 * @param rightNode         the right node attached to the key
 * @return                  a pointer to the new root node if applicable
 */
template< typename Key , typename Value >
typename BPTree< Key , Value >::BPNode*
BPTree< Key , Value >::BPNode::insertPromotedKey(
        const Key &k, BPTree::BPNode *leftNode , BPTree::BPNode *rightNode ) {
    BPEntry* toInsert = new BPEntry( k );
    toInsert->leftNode = leftNode;
    toInsert->rightNode = rightNode;
    if ( m_numRecords == 0 ) {
        m_entries[ 0 ] = toInsert;
    }
    else if ( m_numRecords == m_order ) {
        return split( toInsert );
    }
    else {
        insertEntry( m_entries , m_numRecords , toInsert );
    }
    m_numRecords++;
    return 0;
}

#endif
