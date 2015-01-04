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

#include <vector>
using std::vector;

using std::runtime_error;

#include "Message.h"


/**
 * A B+ Tree which maintains that each node is at least half full, except the
 * root. The order of the B+ Tree, m, defines the number of entries permitted
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
    
    Comparator< Key >* m_comparator;
    int m_order;
    
    void checkIfOrderValid() {
        if ( m_order < 2 ) {
            throw std::runtime_error("Invalid tree order. Must be at least 2");
        }
    }
    
    /**
     * A node in a B+ Tree. It stores some keys in increasing order, which
     * allows algorithms to quickly process a node and search for the next
     * branch to take (if the node is not a leaf) or search for the desired
     * key (if the node is a leaf).
     */
    class BPNode {
        
    private:
        
        Comparator< Key >* m_comparator;
        
        int compare( const Key& k1 , const Key& k2 ) const {
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
        
        bool inRange( const Key& lower, const Key& upper, const Key& k ) const {
            return compare( k , lower ) >= 0 && compare( k , upper ) <= 0;
        }
        
        struct BPEntry {
            
            Key key;
            Value* value;
            BPNode* leftNode;
            BPNode* rightNode;
            bool deleteRightFlag;
            bool deleteOnlyValueFlag;
            
            void initialize() {
                value = 0;
                leftNode = 0;
                rightNode = 0;
                deleteRightFlag = false;
                deleteOnlyValueFlag = false;
            }
            
            BPEntry( Key k ) {
                initialize();
                key = k;
            }
            
            BPEntry( Key k , Value v ) {
                initialize();
                key = k;
                value = new Value;
                *value = v;
            }
            
            BPEntry( const BPEntry& other ) {
                key = other.key;
                value = 0;
                if ( other.value != 0 ) {
                    value = new Value( *other.value );
                }
                
                leftNode = 0;
                if ( other.leftNode != 0 ) {
                    leftNode = new BPNode( *other.leftNode );
                }
                
                rightNode = 0;
                //we do not copy the right node, because the right node
                //is going to be another entry's left node
                
                deleteRightFlag = other.deleteRightFlag;
                deleteOnlyValueFlag = other.deleteOnlyValueFlag;
            }
            
            ~BPEntry() {
                if ( !deleteOnlyValueFlag ) {
                    if ( leftNode != 0 ) {
                        delete leftNode;
                    }
                    if ( rightNode != 0 ) {
                        if ( deleteRightFlag ) {
                            delete rightNode;
                        }
                    }
                }
                if ( value != 0 ) {
                    delete value;
                }
            }
            
            BPEntry& operator=( const BPEntry& other ) {
                key = other.key;
                value = 0;
                if ( other.value != 0 ) {
                    value = new Value( *other.value );
                }
                
                leftNode = 0;
                if ( other.leftNode != 0 ) {
                    leftNode = new BPNode( *other.leftNode );
                }
                
                rightNode = 0;
                //we do not copy the right node, because the right node
                //is going to be another entry's left node
                
                deleteOnlyValueFlag = other.deleteOnlyValueFlag;
                return *this;
            }
        };
        
        BPNode* m_parent;
        int m_order;
        int m_numRecords;
        bool m_isLeaf;
        BPEntry** m_entries;
        BPNode* m_leftSibling;
        BPNode* m_rightSibling;
        
        //signals the destructor on how to free memory
        //a node that was split will need to be treated differently
        bool m_splitFlag;
        
        /**
         * Initializes the node to reflect its initial properties, such as order
         */
        void initialize() {
            m_parent = 0;
            m_numRecords = 0;
            m_isLeaf = true;
            m_entries = new BPEntry*[ m_order ];
            m_leftSibling = 0;
            m_rightSibling = 0;
            m_splitFlag = false;
        }
        
        void deleteEntries() {
            for ( int i=0 ; i<m_numRecords-1 ; i++ ) {
                delete m_entries[ i ];
            }
            if ( m_numRecords-1 >= 0 ) {
                m_entries[ m_numRecords-1 ]->deleteRightFlag = true;
                delete m_entries[ m_numRecords-1 ];
            }
            delete[] m_entries;
        }
        
        /**
         * Links together the children of this node after the tree has been
         * copied
         */
        void linkCopies() {
            
            //first, within each entry, we must link the right nodes
            for ( int i=0 ; i<m_numRecords-1 ; i++ ) {
                m_entries[ i ]->rightNode = m_entries[ i+1 ]->leftNode;
            }
            //remember that the last entry's right node has already been linked
            //by the node's copy constructor
            
            //leaf nodes do not have children to link together
            if ( isLeaf() ) {
                return;
            }
            
            //link internally
            for ( int i=0 ; i<m_numRecords ; i++ ) {
                m_entries[ i ]->leftNode->m_rightSibling =
                                                    m_entries[ i ]->rightNode;
                m_entries[ i ]->rightNode->m_leftSibling =
                                                    m_entries[ i ]->leftNode;
            }
            
            //link externally with the left
            if ( m_leftSibling != 0 && m_leftSibling->m_numRecords > 0 ) {
                m_entries[ 0 ]->leftNode->m_leftSibling = m_leftSibling->
                        m_entries[ m_leftSibling->m_numRecords-1 ]->rightNode;
                m_leftSibling->m_entries[ m_leftSibling->m_numRecords-1 ]->
                        rightNode->m_rightSibling = m_entries[ 0 ]->leftNode;
            }
            
            //link externally with the right
            if ( m_rightSibling != 0 && m_rightSibling->m_numRecords > 0 ) {
                    m_entries[ m_numRecords-1 ]->rightNode->m_rightSibling =
                                m_rightSibling->m_entries[ 0 ]->leftNode;
                    m_rightSibling->m_entries[ 0 ]->leftNode->m_leftSibling =
                                m_entries[ m_numRecords-1 ]->rightNode;
            }
            
            //have children link together thier child nodes
            for ( int i=0 ; i<m_numRecords ; i++ ) {
                m_entries[ i ]->leftNode->linkCopies();
                m_entries[ i ]->rightNode->linkCopies();
            }
        }
        
        /**
         * Performs a binary search and determines the index at which the
         * given key should be inserted into the entries array
         *
         * @param entries           an array of entries through which to search
         * @param entriesSize       the size of the entry array
         * @param k                 the key to insert
         */
        int locateIdx( BPEntry** entries, int entriesSize, const Key& k ) const{
            int low = 0;
            int high = entriesSize-1;
            int guess = (low+high)/2;
            while( low < high ) {
                if ( compare( entries[ guess ]->key , k ) < 0 ) {
                    low = guess+1;
                }
                else if ( compare( entries[ guess ]->key , k ) == 0 ) {
                    return guess;
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
         * @return                  if the insertion was sucessful (it is
         *                          unsuccessful if the given key was already
         *                          in the entries array
         */
        void insertEntry( BPEntry** entries , int entriesSize ,
                         BPEntry* toInsert ) {
            int searchIdx = locateIdx( entries , entriesSize , toInsert->key );
            
            //determine the index at which we should insert the new entry
            int insertIdx;
            
            //if we are inserting past the end of the array, then
            //we insert at the end no matter what (just as safety)
            if ( searchIdx >= entriesSize ) {
                insertIdx = entriesSize-1;
            }
            
            //if we are inserting at a location in the array that's empty,
            //then we insert there no matter what
            else if ( entries[ searchIdx ] == 0 ) {
                insertIdx = searchIdx;
            }
            else {
                
                //if the entry currently in the array is less,
                //then we need to insert the new entry after the current element
                if ( compare( entries[ searchIdx ]->key, toInsert->key ) < 0 ) {
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
            
            //first, we have to remember to link the children's left and right
            //siblings
            BPNode* leftChildLeftSibling = 0;
            BPNode* rightChildRightSibling = 0;
            
            //make sure that the node to insert isn't a leaf node
            //if it is a leaf node (i.e. its left and right child nodes are 0),
            //then it can't have children, so we don't care about
            //linking the children's siblings together anymore
            if ( toInsert->rightNode != 0 ) {
                if ( insertIdx == 0 ) {
                    if ( entriesSize > 1 ) {
                        leftChildLeftSibling =
                                        entries[ 1 ]->leftNode->m_leftSibling;
                        rightChildRightSibling = entries[ 1 ]->rightNode;
                    }
                }
                else if ( insertIdx >= entriesSize-1 ) {
                    leftChildLeftSibling = entries[ insertIdx-1 ]->leftNode;
                    rightChildRightSibling =
                            entries[ insertIdx-1 ]->rightNode->m_rightSibling;
                }
                else {
                    leftChildLeftSibling = entries[ insertIdx-1 ]->leftNode;
                    rightChildRightSibling = entries[ insertIdx+1 ]->rightNode;
                }
            }
            
            //start with left neighbor
            if ( insertIdx-1 >= 0 ) {
                
                //first set up the node to insert's relationship with its left
                //neighbor
                if ( toInsert->rightNode != 0 ) {
                    toInsert->rightNode->m_rightSibling = rightChildRightSibling;
                    if ( rightChildRightSibling != 0 ) {
                        rightChildRightSibling->m_leftSibling =
                                                            toInsert->rightNode;
                    }
                    
                    toInsert->leftNode->m_leftSibling = leftChildLeftSibling;
                    toInsert->rightNode->m_leftSibling = toInsert->leftNode;
                }
                
                //then modify the left neighbor that was displaced
                //and update its relationship with the node we inserted
                entries[ insertIdx-1 ]->rightNode = toInsert->leftNode;
                if ( entries[ insertIdx-1 ]->rightNode != 0 ) {
                    entries[ insertIdx-1 ]->leftNode->m_rightSibling =
                                                            toInsert->leftNode;
                    entries[ insertIdx-1 ]->rightNode->m_rightSibling =
                                                            toInsert->rightNode;
                }
            }
            
            //now go to the right neighbor
            if ( insertIdx+1 < entriesSize+1 && entries[ insertIdx+1 ] != 0 ) {
                
                //set up the node to insert's relationship with its right
                //neighbor
                if ( toInsert->rightNode != 0 ) {
                    toInsert->leftNode->m_leftSibling = leftChildLeftSibling;
                    if ( leftChildLeftSibling != 0 ) {
                        leftChildLeftSibling->m_rightSibling = toInsert->leftNode;
                    }
                    
                    toInsert->rightNode->m_rightSibling = rightChildRightSibling;
                    toInsert->rightNode->m_leftSibling = toInsert->leftNode;
                }
                
                //then modify the right neighbor that was displaced
                //and update its relationship with the node we inserted
                entries[ insertIdx+1 ]->leftNode = toInsert->rightNode;
                if ( toInsert->rightNode != 0 ) {
                    entries[ insertIdx+1 ]->leftNode->m_leftSibling =
                    toInsert->leftNode;
                    entries[ insertIdx+1 ]->rightNode->m_leftSibling =
                    toInsert->rightNode;
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
            combinedEntries[ m_numRecords ] = 0;
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
            BPNode* leftNode = new BPNode( m_comparator , m_order );
            leftNode->m_isLeaf = this->m_isLeaf;
            leftNode->m_parent = parent;
            
            BPNode* rightNode = new BPNode( m_comparator , m_order );
            rightNode->m_isLeaf = this->m_isLeaf;
            rightNode->m_parent = parent;
            
            leftNode->m_rightSibling = rightNode;
            rightNode->m_leftSibling = leftNode;
            
            int middleIdx = (m_numRecords+1)/2;
            for ( int i=0 ; i<middleIdx ; i++ ) {
                leftNode->m_entries[ i ] = combinedEntries[ i ];
                if ( !leftNode->isLeaf() ) {
                    leftNode->m_entries[ i ]->leftNode->m_parent = leftNode;
                    leftNode->m_entries[ i ]->rightNode->m_parent = leftNode;
                }
                leftNode->m_numRecords++;
            }
            
            //if we are not splitting a leaf node, then the promoted key in the
            //middle does not stay in this node
            int startRight = middleIdx;
            if ( !isLeaf() ) {
                startRight = middleIdx+1;
            }
            for ( int i=startRight ; i<m_numRecords+1 ; i++ ) {
                rightNode->m_entries[ i-startRight ] = combinedEntries[ i ];
                if ( !rightNode->isLeaf() ) {
                    rightNode->m_entries[ i-startRight ]->leftNode->m_parent =
                                                                    rightNode;
                    rightNode->m_entries[ i-startRight ]->rightNode->m_parent =
                                                                    rightNode;
                }
                rightNode->m_numRecords++;
            }
            
            //promote the first key in the right branch into the parent's
            //entries
            Key promotedKey = combinedEntries[ middleIdx ]->key;
            
            //the memory associated with the middle, promoted node must be freed
            //because it does not stay in this node.
            if ( !isLeaf() ) {
                combinedEntries[ middleIdx ]->deleteOnlyValueFlag = true;
                delete combinedEntries[ middleIdx ];
            }
            
            
            //insert the promoted key into the parent
            BPNode* higherParent = parent->insertPromotedKey(
                                            promotedKey, leftNode, rightNode );
            
            //if we didn't create a new root at this level,
            //then we ask the node at the next highest level if there was
            //a new root created
            if ( rtn == 0 ) {
                rtn = higherParent;
            }
            
            m_splitFlag = true;
            delete this;
            
            return rtn;
        }
        
        
        void distributeNonLeafEntries( BPNode* n1 , BPNode* n2 ,
                                      const Key& middleKey ) {
            
            //we will have to change the key to the parent of these two nodes
            //as we are redistributing entries
            //note that these nodes must have a parent, or they cannot be
            //siblings
            BPNode* parent = n1->m_parent;
            int parentEntryIdx = locateIdx( parent->m_entries ,
                                           parent->m_numRecords , middleKey );
            BPEntry* parentEntry = parent->m_entries[ parentEntryIdx ];
            
            BPEntry* combinedEntries[ n1->m_numRecords + n2->m_numRecords ];
            for ( int i=0 ; i<n1->m_numRecords ; i++ ) {
                combinedEntries[ i ] = n1->m_entries[ i ];
            }
            for ( int i=0 ; i<n2->m_numRecords ; i++ ) {
                combinedEntries[ n1->m_numRecords+i ] = n2->m_entries[ i ];
            }
            
            int totalEntries = n1->m_numRecords + n2->m_numRecords;
            
            //add an additional 1 to favor the right node in case of odd
            //number of entries
            int middleIdx = (totalEntries + 1)/2;
            
            n1->m_numRecords = 0;
            for ( int i=0 ; i<middleIdx ; i++ ) {
                n1->m_entries[ i ] = combinedEntries[ i ];
                n1->m_numRecords++;
            }
            
            n2->m_numRecords = 0;
            for ( int i=middleIdx ; i<totalEntries ; i++ ) {
                n2->m_entries[ i-middleIdx ] = combinedEntries[ i ];
                n2->m_numRecords++;
            }
            
            //update their parent entry to have the correct indexing
            parentEntry->key = n2->m_entries[ 0 ]->key;
        }
        
        BPNode* mergeNonLeafNodes( BPNode* n1, BPNode* n2, const Key& middleKey ,
                               BPNode* extraRightNode ) {
            BPNode* parent = n1->m_parent;
            int parentEntryIdx = locateIdx( parent->m_entries ,
                                           parent->m_numRecords , middleKey );
            BPEntry* leftEntry = 0;
            if ( parentEntryIdx-1 >= 0 ) {
                leftEntry = parent->m_entries[ parentEntryIdx-1 ];
            }
            BPEntry* rightEntry = 0;
            if ( parentEntryIdx+1 < parent->m_numRecords ) {
                rightEntry = parent->m_entries[ parentEntryIdx+1 ];
            }
            BPNode* newRightNode = n2->m_rightSibling;
            
            //first, demote the parent's key to this node
            //demote the parent's key to this node
            Key deletedKey = parent->m_entries[ parentEntryIdx ]->key;
            extraRightNode->m_parent = this;
            BPEntry* mergedEntry = new BPEntry( deletedKey );
            
            mergedEntry->leftNode = n1->m_entries[ n1->m_numRecords-1 ]->rightNode;
            if ( n2->m_numRecords > 0 ) {
                mergedEntry->rightNode = n2->m_entries[ 0 ]->leftNode;
            }
            else {
                mergedEntry->rightNode = extraRightNode;
            }
            mergedEntry->leftNode->m_rightSibling = mergedEntry->rightNode;
            mergedEntry->rightNode->m_leftSibling = mergedEntry->leftNode;
            mergedEntry->leftNode->m_parent = n1;
            mergedEntry->rightNode->m_parent = n1;
            n1->m_entries[ n1->m_numRecords ] = mergedEntry;
            extraRightNode->m_parent = n1;
            n1->m_numRecords++;
            
            //then transfer the right node's entries into the left node
            for ( int i=0 ; i<n2->m_numRecords ; i++ ) {
                n1->m_entries[ n1->m_numRecords ] = n2->m_entries[ i ];
                n1->m_entries[ n1->m_numRecords ]->leftNode->m_parent = n1;
                n1->m_entries[ n1->m_numRecords ]->rightNode->m_parent = n1;
                n1->m_numRecords++;
            }
            
            //the right node and the shared parent entry
            //must also be deleted and removed from the tree
            n2->m_splitFlag = true;
            delete n2;
            
            //since n2 is gone, n1 may not necessarily have a right sibling
            //anymore
            n1->m_rightSibling = 0;
            
            //assign the parent's neighboring entries custody of the merged
            //child
            if ( leftEntry != 0 ) {
                leftEntry->rightNode = n1;
                leftEntry->leftNode->m_rightSibling = n1;
            }
            if ( rightEntry != 0 ) {
                rightEntry->leftNode = n1;
                rightEntry->rightNode->m_leftSibling = n1;
            }
            n1->m_rightSibling = newRightNode;
            
            //remove the parent entry from the parent
            return parent->removeNonLeafEntry( parentEntryIdx );
        }
        
        BPNode* removeNonLeafEntry( int entryIdx ) {
            BPEntry* entry = m_entries[ entryIdx ];
            Key deletedKey = entry->key;
            
            //detach the entry from its children and delete it
            BPNode* hangingLeftNode = entry->leftNode;
            if ( hangingLeftNode->m_rightSibling != 0 ) {
                hangingLeftNode = 0;
            }
            entry->leftNode = 0;
            entry->rightNode = 0;
            delete entry;
            m_entries[ entryIdx ] = 0;
            
            //subsequent entries must be shifted down to take the deleted
            //entry's place
            for ( int i=entryIdx ; i<m_numRecords-1 ; i++ ) {
                m_entries[ i ] = m_entries[ i+1 ];
            }
            
            //since the last entry was shifted down by 1 index,
            //the pointer at its previous location must be set to 0
            m_entries[ m_numRecords-1 ] = 0;
            m_numRecords--;
            
            //if the root underflowed, then we return the pointer to the
            //new root
            if ( m_parent == 0 && m_numRecords == 0 ) {
                hangingLeftNode->m_parent = 0;
                return hangingLeftNode;
            }
            
            //check for underflow
            if ( m_numRecords < (m_order+1)/2 ) {
                
                //the root is allowed to underflow
                if ( m_parent == 0 ) {
                    return 0;
                }
                
                //try redistributing with the sibling that has the most number
                //of records
                BPNode* sibling;
                bool distributingWithLeftSibling;
                
                //first we assume the left sibling has more records
                if ( m_leftSibling != 0 &&
                    m_leftSibling->m_parent == this->m_parent ) {
                    sibling = m_leftSibling;
                    distributingWithLeftSibling = true;
                    
                    //and if the right sibling actually has more records,
                    //then we update to reflect that
                    if ( m_rightSibling != 0 &&
                        m_rightSibling->m_parent == this->m_parent ) {
                        if ( m_rightSibling->m_numRecords >
                            m_leftSibling->m_numRecords ) {
                            sibling = m_rightSibling;
                            distributingWithLeftSibling = false;
                        }
                    }
                }
                else {
                    sibling = m_rightSibling;
                    distributingWithLeftSibling = false;
                }
                
                //check if it is possible to redistribute
                if ((sibling->m_numRecords + m_numRecords)/2 >= (m_order+1)/2) {
                    if ( distributingWithLeftSibling ) {
                        Key middleKey;
                        if ( entryIdx == 0 ) {
                            middleKey = deletedKey;
                        }
                        else {
                            middleKey = m_entries[ 0 ]->key;
                        }
                        distributeNonLeafEntries( sibling , this , middleKey );
                    }
                    else {
                        Key middleKey = sibling->m_entries[ 0 ]->key;
                        distributeNonLeafEntries( this , sibling , middleKey );
                    }
                    return 0;
                }
                
                //if it is not possible to distribute, then we will have
                //to demote a parent key
                sibling = 0;
                bool mergingWithLeftSibling;
                if ( m_leftSibling != 0 ) {
                    if ( m_leftSibling->m_parent == this->m_parent ) {
                        sibling = m_leftSibling;
                        mergingWithLeftSibling = true;
                    }
                    else {
                        sibling = m_rightSibling;
                        mergingWithLeftSibling = false;
                    }
                }
                else {
                    sibling = m_rightSibling;
                    mergingWithLeftSibling = false;
                }
                if ( mergingWithLeftSibling ) {
                    return mergeNonLeafNodes( sibling , this , deletedKey , hangingLeftNode );
                }
                else {
                    return mergeNonLeafNodes(this, sibling, sibling->m_entries[ 0 ]->key , hangingLeftNode );
                }
            }
            return 0;
        }
        
        /**
         * Distributes the entries of two sibling leaf nodes evenly and modifies 
         * their parent node to have the correct indexing. The nodes must be
         * leaf nodes and siblings or else the function will not work properly.
         *
         * @param n1                    the left node of the two nodes
         * @param n2                    the right node of the two nodes
         */
        void distributeLeafEntries( BPNode* n1 , BPNode* n2 ,
                                   const Key& middleKey ) {
            
            //we will have to change the key to the parent of these two nodes
            //as we are redistributing entries
            //note that these nodes must have a parent, or they cannot be
            //siblings
            BPNode* parent = n1->m_parent;
            int parentEntryIdx = locateIdx( parent->m_entries ,
                                    parent->m_numRecords , middleKey );
            BPEntry* parentEntry = parent->m_entries[ parentEntryIdx ];
            
            BPEntry* combinedEntries[ n1->m_numRecords + n2->m_numRecords ];
            for ( int i=0 ; i<n1->m_numRecords ; i++ ) {
                combinedEntries[ i ] = n1->m_entries[ i ];
            }
            for ( int i=0 ; i<n2->m_numRecords ; i++ ) {
                combinedEntries[ n1->m_numRecords+i ] = n2->m_entries[ i ];
            }
        
            int totalEntries = n1->m_numRecords + n2->m_numRecords;
            
            //add an additional 1 to favor the right node in case of odd
            //number of entries
            int middleIdx = (totalEntries + 1)/2;
            
            n1->m_numRecords = 0;
            for ( int i=0 ; i<middleIdx ; i++ ) {
                n1->m_entries[ i ] = combinedEntries[ i ];
                n1->m_numRecords++;
            }
            
            n2->m_numRecords = 0;
            for ( int i=middleIdx ; i<totalEntries ; i++ ) {
                n2->m_entries[ i-middleIdx ] = combinedEntries[ i ];
                n2->m_numRecords++;
            }
            
            //update their parent entry to have the correct indexing
            parentEntry->key = n2->m_entries[ 0 ]->key;
        }
        
        /**
         * Merges the entries of the two leaf nodes together and modifies their
         * parent to correctly index to the the combined node. The nodes must be
         * siblings and leaves and the order in which the nodes are passed to 
         * the function must be left to right or this function will not work
         * properly.
         *
         * @param n1                    the left node
         * @param n2                    the right node that will be merged into
         *                              the left node.
         * @param middleKey             the key to the first entry in the right
         *                              node
         * @return                      a pointer to the new root of the tree if
         *                              the root collapsed, or 0 otherwise
         */
        BPNode* mergeLeafNodes( BPNode* n1 , BPNode* n2 , const Key& middleKey ) {
            BPNode* parent = n1->m_parent;
            int parentEntryIdx = locateIdx( parent->m_entries ,
                                           parent->m_numRecords , middleKey );
            BPEntry* leftEntry = 0;
            if ( parentEntryIdx-1 >= 0 ) {
                leftEntry = parent->m_entries[ parentEntryIdx-1 ];
            }
            BPEntry* rightEntry = 0;
            if ( parentEntryIdx+1 < parent->m_numRecords ) {
                rightEntry = parent->m_entries[ parentEntryIdx+1 ];
            }
            
            //the right node that will be deleted may have a pointer
            //to a sibling in another branch that we will need later
            BPNode* newRightNode = n2->m_rightSibling;
            
            //transfer the right node's entries into the left node
            //if the right
            for ( int i=0 ; i<n2->m_numRecords ; i++ ) {
                n1->m_entries[ n1->m_numRecords ] = n2->m_entries[ i ];
                n1->m_numRecords++;
            }
            
            //the right node and the shared parent entry
            //must also be deleted and removed from the tree
            n2->m_splitFlag = true;
            delete n2;
            
            //since n2 is gone, n1 may not necessarily have a right sibling
            //anymore
            n1->m_rightSibling = 0;
            
            //assign the parent's neighboring entries custody of the merged
            //child
            if ( leftEntry != 0 ) {
                leftEntry->rightNode = n1;
                leftEntry->leftNode->m_rightSibling = n1;
            }
            if ( rightEntry != 0 ) {
                rightEntry->leftNode = n1;
                rightEntry->rightNode->m_leftSibling = n1;
            }
            
            //since the right node was deleted, the left node might have
            //a new right sibling
            n1->m_rightSibling = newRightNode;
            
            //remove the parent entry from the parent
            return parent->removeNonLeafEntry( parentEntryIdx );
        }
        
        /**
         * Removes the entry at the given index from this node
         * and rearranges the structure of the tree if underflow occurs. This
         * should only be called on leaf nodes. Otherwise, the function will not
         * work correctly.
         *
         * @param entryIdx              the index of the entry to remove
         * @return                      a pointer to the new root of the tree
         *                              if the root collapsed, or 0 otherwise
         */
        BPNode* removeLeafEntry( int entryIdx ) {
            
            //keep a copy of the deleted key in case we need it for the tree's
            //indexing again
            Key deletedKey = m_entries[ entryIdx ]->key;
            
            //first remove the entry - there is no need to set any delete flags
            //because leaf nodes have no more children
            delete m_entries[ entryIdx ];
            m_entries[ entryIdx ] = 0;
            
            for ( int i=entryIdx+1 ; i<m_numRecords ; i++ ) {
                m_entries[ i-1 ] = m_entries[ i ];
            }
            m_entries[ m_numRecords-1 ] = 0;
            m_numRecords--;

            //check if we underflowed
            if ( m_numRecords < (m_order+1)/2 ) {
                
                //the root is allowed to underflow
                if ( m_parent == 0 ) {
                    return 0;
                }
                
                //try redistributing with the sibling that has the most number
                //of records
                BPNode* sibling;
                bool distributingWithLeftSibling;
                
                //first we assume the left sibling has more records
                if ( m_leftSibling != 0 &&
                                m_leftSibling->m_parent == this->m_parent ) {
                    sibling = m_leftSibling;
                    distributingWithLeftSibling = true;
                    
                    //and if the right sibling actually has more records,
                    //then we update to reflect that
                    if ( m_rightSibling != 0 &&
                                m_rightSibling->m_parent == this->m_parent ) {
                        if ( m_rightSibling->m_numRecords >
                                                m_leftSibling->m_numRecords ) {
                            sibling = m_rightSibling;
                            distributingWithLeftSibling = false;
                        }
                    }
                }
                else {
                    sibling = m_rightSibling;
                    distributingWithLeftSibling = false;
                }
                
                //check if it is possible to distribute
                if ( (sibling->m_numRecords + m_numRecords)/2 >= (m_order+1)/2 ) {
                    if ( distributingWithLeftSibling ) {
                        Key middleKey;
                        if ( entryIdx == 0 ) {
                            middleKey = deletedKey;
                        }
                        else {
                            middleKey = m_entries[ 0 ]->key;
                        }
                        distributeLeafEntries( sibling , this , middleKey );
                    }
                    else {
                        Key middleKey = sibling->m_entries[ 0 ]->key;
                        distributeLeafEntries( this , sibling , middleKey );
                    }
                    return 0;
                }
                
                //if neither left nor right can lend, then merge with sibling.
                //we can merge for certain because the siblings
                //could not lend us an entry so they were exactly half full,
                //and this node is less than half full
                sibling = 0;
                bool mergingWithLeftSibling;
                if ( m_leftSibling != 0 ) {
                    if ( m_leftSibling->m_parent == this->m_parent ) {
                        sibling = m_leftSibling;
                        mergingWithLeftSibling = true;
                    }
                    else {
                        sibling = m_rightSibling;
                        mergingWithLeftSibling = false;
                    }
                }
                else {
                    sibling = m_rightSibling;
                    mergingWithLeftSibling = false;
                }
                if ( mergingWithLeftSibling ) {
                    return mergeLeafNodes( sibling , this , deletedKey );
                }
                else {
                    return mergeLeafNodes(this, sibling, sibling->m_entries[ 0 ]->key);
                }
            }
            return 0;
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
            
            //only the comparator and order can be defined before the tree
            //is initialized
            m_comparator = other.m_comparator;
            m_order = other.m_order;
            initialize();
            
            m_parent = other.m_parent;
            m_numRecords = other.m_numRecords;
            m_isLeaf = other.m_isLeaf;
            m_splitFlag = other.m_splitFlag;
            for ( int i=0 ; i<m_numRecords ; i++ ) {
                if ( other.m_entries[ i ] != 0 ) {
                    m_entries[ i ] = new BPEntry( *other.m_entries[ i ] );
                }
            }
            
            //manually copy the rightmost node that is not linked to another
            //entry's left node
            if ( other.m_entries[ m_numRecords-1 ] != 0 && !other.isLeaf() ) {
                m_entries[ m_numRecords-1 ]->rightNode =
                    new BPNode( *other.m_entries[ m_numRecords-1 ]->rightNode );
            }
            linkCopies();
        }
        
        ~BPNode() {
            //if we are deleting a node that was not split, then
            //delete the entries associated with it.
            if ( !m_splitFlag ) {
                deleteEntries();
            }
            //if the node was split, then the entries were given to
            //the two new nodes and we do not want to delete those.
            //we just want to delete the array pointer
            else {
                delete[] m_entries;
            }
        }
        
        BPNode& operator=( const BPNode& other ) {
            
            if ( this == &other ) {
                return *this;
            }
            
            //delete the old data
            deleteEntries();
            
            //only the comparator and order can be defined before the tree
            //is initialized
            m_comparator = other.m_comparator;
            m_order = other.m_order;
            initialize();
            
            m_parent = other.m_parent;
            m_numRecords = other.m_numRecords;
            m_isLeaf = other.m_isLeaf;
            m_splitFlag = other.m_splitFlag;
            for ( int i=0 ; i<m_numRecords ; i++ ) {
                if ( other.m_entries[ i ] != 0 ) {
                    m_entries[ i ] = new BPEntry( *other.m_entries[ i ] );
                }
            }
            
            //manually copy the right node that is not linked to another entry's
            //left node
            if ( m_numRecords-1 >= 0 && other.m_entries[m_numRecords-1] != 0 &&
                !other.isLeaf() ) {
                m_entries[ m_numRecords-1 ]->rightNode =
                    new BPNode( *other.m_entries[ m_numRecords-1 ]->rightNode );
            }
            linkCopies();
            return *this;
        }
        
        /**
         * @return                  if this node is a leaf node or not. Leaf
         *                          nodes have no children.
         */
        bool isLeaf() const {
            return m_isLeaf;
        }
        
        /**
         * @return                  the number of records stored in this node
         */
        int getNumRecords() const {
            return m_numRecords;
        }
        
        /**
         * Gets a pointer to the parent of this node. The pointer is 0 if the
         * parent does not exist (i.e. this node is the root node). The parent
         * should not be modified to maintain the B+ Tree's correct internal
         * structure
         *
         * @return                  a pointer to the parent of this node
         */
        BPNode* getParent() const {
            return m_parent;
        }
        
        /**
         * Gets a pointer to the leftmost child that is a direct descendant of
         * this node. The child should not be modified to maintain the B+
         * Tree's correct internal structure.
         *
         * @return                  a pointer tothe leftmost child that is a 
         *                          direct descendant of this node.
         */
        BPNode* getLeftmostChild() {
            return m_entries[ 0 ]->leftNode;
        }
        
        /**
         * Gets a pointer to the rightmost child that is a direct descendant of
         * this node. The child should not be modified to maintain the B+
         * Tree's correct internal structure.
         *
         * @return                  a pointer to the rightmost child that is a 
         *                          direct descendant of this node.
         */
        BPNode* getRightmostChild() const {
            return m_entries[ m_numRecords-1 ]->rightNode;
        }
        
        /**
         * Finds the node that should contain the given key
         *
         * @param k                 the key for which to look
         * @return                  the branch that should contain the given key
         *                          or 0 if no branch was found
         */
        BPNode* findNode( const Key& k ) const {
            int locationIdx = locateIdx( m_entries , m_numRecords , k );
            if ( m_entries[ locationIdx ] == 0 ) {
                return 0;
            }
            else {
                BPEntry* guideEntry = m_entries[ locationIdx ];
                if ( compare( k , guideEntry->key ) < 0 ) {
                    return guideEntry->leftNode;
                }
                else {
                    return guideEntry->rightNode;
                }
            }
        }
        
        /**
         * Inserts the given key-value pair into the B+ Tree
         *
         * @param k                 the key
         * @param v                 the value associated with the given key
         * @return                  a pointer to the new root of the B+ Tree
         *                          if the root node was split, or 0 otherwise.
         */
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
        
        /**
         * Finds the entry with the given key
         * 
         * @param k                 a key value
         * @return                  the entry associated with the given key, or
         *                          0 if the key was not found in the tree
         */
        Value* find( const Key& k ) const {
            if ( isLeaf() ) {
                int findIdx = locateIdx( m_entries , m_numRecords , k );
                if ( m_numRecords == 0 ) {
                    return 0;
                }
                else if ( compare( m_entries[ findIdx ]->key , k ) == 0 ) {
                    return m_entries[ findIdx ]->value;
                }
                else {
                    return 0;
                }
            }
            else {
                return findNode( k )->find( k );
            }
        }
        
        /**
         * Gets all values in the tree that are associated with a key in the
         * given range (inclusive).
         *
         * @param lower                 the lower bound
         * @param upper                 the upper bound
         * @param collection            the vector in which to store values
         *                              that are in the given key range
         */
        void range( const Key& lower , const Key& upper ,
                   vector< Value >& collection ) const {
            if ( isLeaf() ) {
                
                //put everything in this node that is within the range
                //into the collections vector
                int findIdx = locateIdx( m_entries , m_numRecords , lower );
                for ( int i=findIdx ; i<m_numRecords ; i++ ) {
                    if ( inRange( lower , upper , m_entries[ i ]->key ) ) {
                        collection.push_back( *m_entries[ i ]->value );
                    }
                    else {
                        
                        //the entries are stored in increasing key order.
                        //therefore, if we've passed the upper bound, then we're
                        //finished
                        if ( compare( m_entries[ i ]->key , upper ) > 0 ) {
                            return;
                        }
                    }
                }
                
                //check if the next node to the right might have any more keys
                //in the given range
                if ( m_rightSibling != 0 ) {
                    m_rightSibling->range( lower , upper , collection );
                }
            }
            else {
                return findNode( lower )->range( lower , upper , collection );
            }
        }
        
        /**
         * Removes the given key and the value associated with it from this node
         *
         * @param k                 the key to remove
         * @return                  a pointer to the new root, if the root
         *                          collapsed, or 0 otherwise
         */
        BPNode* remove( const Key& k ) {
            int idxToRemove = locateIdx( m_entries , m_numRecords , k );
            if ( compare( m_entries[ idxToRemove ]->key , k ) == 0 ) {
                return removeLeafEntry( idxToRemove );
            }
            else {
                return 0;
            }
        }
        
        /**
         * @return                  the textual representation of the keys of
         *                          this node as a list
         */
        string toString() const {
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
         * @return                  the textual representation of the keys of
         *                          this node as a reversed list (right to left)
         */
        string toStringRightToLeft() const {
            Message rtn;
            rtn << "[";
            if ( m_numRecords == 0 ) {
                rtn << "]";
            }
            else if ( m_numRecords == 1 ) {
                rtn << m_entries[ 0 ]->key << "]";
            }
            else {
                rtn << m_entries[ m_numRecords-1 ]->key;
                for ( int i=m_numRecords-2 ; i>=0 ; i-- ) {
                    rtn << ", " << m_entries[ i ]->key;
                }
                rtn << "]";
            }
            return rtn.str();
        }
        
        /**
         * @return                      the textual representation of this node 
         *                              and all its siblings to the right.
         */
        string toStringSiblings() const {
            Message rtn;
            rtn << this->toString();
            BPNode* currNode = this->m_rightSibling;
            while( currNode != 0 ) {
                rtn << ", " << currNode->toString();
                currNode = currNode->m_rightSibling;
            }
            return rtn.str();
        }
        
        /**
         * @return                      the textual representation of this node
         *                              and all its siblings to the left.
         */
        string toStringLeftSiblings() const {
            Message rtn;
            rtn << this->toStringRightToLeft();
            BPNode* currNode = this->m_leftSibling;
            while( currNode != 0 ) {
                rtn << ", " << currNode->toStringRightToLeft();
                currNode = currNode->m_leftSibling;
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
    
    BPTree( Comparator< Key >* comparator ) : m_order( DEFAULT_ORDER ) ,
                m_comparator( comparator ) , m_root( new BPNode( m_order ) ) {
        m_root = new BPNode( m_comparator , m_order );
    }
    
    BPTree( int order ) : m_order( order ) , m_comparator( 0 ) ,
                                            m_root( new BPNode( m_order ) ) {
        checkIfOrderValid();
    }
    
    BPTree( int order , Comparator< Key >* comparator ) : m_order( order ) , m_comparator ( comparator ) {
        checkIfOrderValid();
        m_root = new BPNode( m_comparator , m_order );
    }
    
    BPTree( const BPTree& other ) {
        m_order = other.m_order;
        m_comparator = other.m_comparator;
        m_root = new BPNode( *other.m_root );
    }
    
    ~BPTree() {
        delete m_root;
    }
    
    BPTree& operator=( const BPTree& other ) {
        if ( this == &other ) {
            return *this;
        }
        clear();
        m_order = other.m_order;
        m_comparator = other.m_comparator;
        *m_root = *other.m_root;
        return *this;
    }
    
    void insert( const Key& k , const Value& v ) {
        
        //overide value if the key is already in the tree
        if ( contains( k ) ) {
            *m_root->find( k ) = v;
            return;
        }
        BPNode* curr = m_root;
        while( !curr->isLeaf() ) {
            curr = curr->findNode( k );
        }
        BPNode* newRoot = curr->insertKeyValue( k , v );
        if ( newRoot != 0 ) {
            m_root = newRoot;
        }
    }
    
    /**
     * Determines if the given key is in the tree
     *
     * @param k                     a key value for which to search
     * @return                      if the given key is in the tree
     */
    bool contains( const Key& k ) const {
        return m_root->find( k ) != 0;
    }
    
    /**
     * Finds the value associated with the given key, or returns a default value
     * if the key is not in the tree
     *
     * @param k                     a key value for which to search
     * @return                      the value associated with the given key, or
     *                              the default value if the key is not found
     */
    Value find( const Key& k ) const {
        Value* v = m_root->find( k );
        if ( v == 0 ) {
            return Value();
        }
        else {
            return *v;
        }
    }
    
    /**
     * Gets all values in the tree that are associated with a key in the given
     * key range (inclusive). Values are reported from lowest key to highest
     * key
     *
     * @param lower                 the lower key bound
     * @param higher                the higher key bound
     * @return                      a list of values associated with keys in the
     *                              specified range
     */
    vector< Value > range( const Key& lower , const Key& upper ) const {
        vector< Value > rtn;
        m_root->range( lower , upper , rtn );
        return rtn;
    }
    
    /**
     * Removes the given key and the value associated with it from the tree.
     *
     * @param k                     the key to remove
     * @return                      if the key was found and removed
     */
    bool remove( const Key& k ) {
        if ( !contains( k ) ) {
            return false;
        }
        BPNode* node = m_root;
        while( !node->isLeaf() ) {
            node = node->findNode( k );
        }
        BPNode* newRoot = node->remove( k );
        if ( newRoot != 0 ) {
            delete m_root;
            m_root = newRoot;
        }
        return true;
    }
    
    /**
     * Removes all entries from the tree
     */
    void clear() {
        delete m_root;
        m_root = new BPNode( m_comparator , m_order );
    }
    
    /**
     * Determines the textual representation of the tree by traversing the
     * linked list of each level from right to left.
     *
     * @return              the textual representation of the tree traversed
     *                      from right to left.
     */
    string toString() const {
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
    
    /**
     * Determines the textual representation of the tree by traversing the
     * linked list of each level from left to right. 
     * 
     * @return              the textual representation of the tree traversed
     *                      from left to right
     */
    string toStringLeft() const {
        Message rtn;
        rtn << "[" << m_root->toStringLeftSiblings();
        if ( !m_root->isLeaf() ) {
            rtn << "\n";
            BPNode* curr = m_root->getRightmostChild();
            while( !curr->isLeaf() ) {
                rtn << curr->toStringLeftSiblings() << "\n";
                curr = curr->getRightmostChild();
            }
            rtn << curr->toStringLeftSiblings();
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
