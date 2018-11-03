//
// Red-Black (balanced) tree
//
#ifndef RBTREE_H
#define RBTREE_H

class TreeNodeValue;    // Predefinition

// A node in Red-Black tree
class RBTreeNode {
public:
    RBTreeNode* left;   // pointer to the left son
    RBTreeNode* right;  // pointer to the right son
    RBTreeNode* parent; // pointer to the parent
    bool        red;    // the node is red (true) or black (false)
    TreeNodeValue* value;  // The value of tree node: normally, it is
                           //     a pair (key, value of key)
    RBTreeNode():
        left(0),
        right(0),
        parent(0),
        red(false),
        value(0)
    {
    }
};

typedef RBTreeNode* RBTreeNodePtr; // Pointer to the RBTreeNode

// An ABSTRACT class representing values in tree nodes
class TreeNodeValue {
public:
    TreeNodeValue() {}
    virtual ~TreeNodeValue() {}   // virtual destructor

    // Abstract class has virtual methods unimplemented

    // The values can be compared
    virtual int compareTo(const TreeNodeValue&) const = 0;

    bool operator==(const TreeNodeValue& k) const {
        return (compareTo(k) == 0);
    }
    bool operator!=(const TreeNodeValue& k) const {
        return (compareTo(k) != 0);
    }
    bool operator<(const TreeNodeValue& k) const {
        return (compareTo(k) < 0);
    }
    bool operator<=(const TreeNodeValue& k) const {
        return (compareTo(k) <= 0);
    }
    bool operator>(const TreeNodeValue& k) const {
        return (compareTo(k) > 0);
    }
    bool operator>=(const TreeNodeValue& k) const {
        return (compareTo(k) >= 0);
    }

    virtual TreeNodeValue& operator=(const TreeNodeValue&) = 0;
    virtual TreeNodeValue& operator+(const TreeNodeValue&) = 0;

    TreeNodeValue& operator+=(const TreeNodeValue& from) {
        (*this) = (*this) + from;
        return (*this);
    }
};

// Every node of Red-Black tree is of red of black color.
// A leaf is an external NULL-node and has the black color.
// The root is black.
// The sons of red node must be black.
// Every path from the root to a leaf has the same number
// of black nodes (not including a root, but including a leaf).
class RBTree {
public:
    // Header contains a pointer to the root of the tree
    // as a left son.
    // The tree may be empty, in this case header->left == 0
    RBTreeNode  header;
    int numNodes;

    RBTree():
        header(),
        numNodes(0)
    {
        header.red = true;      // The header has the red color!
    }

    void clear();
    void erase() { clear(); }
    void removeAll() { clear(); }

    ~RBTree() { clear(); }

    RBTreeNode* root() { return header.left; }
    const RBTreeNode* root() const { return header.left; }

    int size() const { return numNodes; }

    // Find a key in a subtree
    // In: key         -- a key to find;
    //     subTreeRoot -- a root of a subtree. If subTreeRoot == 0,
    //                    then find in complete tree
    // Out: node       -- a pointer to the node that contains a key,
    //                    if key is in the set,
    //                    or a pointer to a node that should be parent to
    //                    the node with the key given.
    // Return value: 0,   if the key is found;
    //               < 0, if the key is not found, and the key is less
    //                       than the value in the node;
    //               > 0, if the key is not found, and the key is greater
    //                       than the value in the node.
    int find(
        const TreeNodeValue* key,
        const RBTreeNode* subTreeRoot = 0,
        RBTreeNodePtr* node = 0
    ) const;

    // Insert a key into the tree:
    //     create a new node and insert it as a leaf.
    // The color of a new node is red.
    // Should be called after the "find" method, which has returned
    //        a nonzero value (i.e. a key was not found)
    // Input: compare = result of comparing with a key of a parent node
    void insert(
        RBTreeNode* parentNode,
        TreeNodeValue* v,
        int compare // negative => add as a left son, positive => right
    );

    // Rotate a node x to the left
    void rotateLeft(RBTreeNode* x);

    // Rotate a node x to the right
    void rotateRight(RBTreeNode* x);

    void rebalanceAfterInsert(RBTreeNode* x);

    // Remove a subtree and return the number of nodes removed
    int removeSubtree(RBTreeNode* subTreeRoot);

    const RBTreeNode* minimalNode(const RBTreeNode* subTreeRoot = 0) const;
    RBTreeNode* minimalNode(const RBTreeNode* subTreeRoot = 0) {
        return const_cast<RBTreeNode*>(
            ((const RBTree*) this)->minimalNode(subTreeRoot)
        );
    }

    const RBTreeNode* maximalNode(const RBTreeNode* subTreeRoot = 0) const;
    RBTreeNode* maximalNode(const RBTreeNode* subTreeRoot = 0) {
        return const_cast<RBTreeNode*>(
            ((const RBTree*) this)->maximalNode(subTreeRoot)
        );
    }

    const RBTreeNode* nextNode(const RBTreeNode* node) const;
    RBTreeNode* nextNode(const RBTreeNode* node) {
        return const_cast<RBTreeNode*>(
            ((const RBTree*) this)->nextNode(node)
        );
    }

    const RBTreeNode* previousNode(const RBTreeNode* node) const;
    RBTreeNode* previousNode(RBTreeNode* node) {
        return (RBTreeNode*)(
            const_cast<RBTree*>(this)->previousNode(node)
        );
    }

    void sumOneTier(const int tier, TreeNodeValue& sum);

    void sumOneTierHelper(RBTreeNode& node, const int tier, TreeNodeValue& curSum);

protected:
    void eraseNode(RBTreeNode* node);

public:
    class const_iterator {
    protected:
        const RBTree* tree;
        const RBTreeNode* node;
    public:
        const_iterator(): tree(0), node(0) {}

        const_iterator(
            const RBTree* t,
            const RBTreeNode* n
        ):
            tree(t),
            node(n)
        {}

        bool operator==(const const_iterator& i) const {
            return (tree == i.tree && node == i.node);
        }

        bool operator!=(const const_iterator& i) const {
            return !operator==(i);
        }

        const_iterator& operator++() {
            node = tree->nextNode(node);
            return *this;
        }

        const_iterator& operator--() {
            node = tree->previousNode(node);
            return *this;
        }

        const_iterator operator++(int) { // Post-increment (don't use it!)
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        const_iterator operator--(int) { // Post-decrement (don't use it!)
            const_iterator tmp = *this;
            --(*this);
            return tmp;
        }

        const RBTreeNode& operator*() const { // Dereference
            return *node;
        }
        const RBTreeNode* operator->() const {
            return node;
        }
    };

    class iterator: public const_iterator {
    public:
        iterator():  
            const_iterator()
        {}
        iterator(RBTree* t, RBTreeNode* n):
            const_iterator(t, n)
        {}

        RBTreeNode& operator*() const { // Dereference
            return const_cast<RBTreeNode&>(
                ((const const_iterator*) this)->operator*()
            );
        }
        RBTreeNode* operator->() const {
            return const_cast<RBTreeNode*>(
                ((const const_iterator*) this)->operator->()
            );
        }
    };

    const_iterator begin() const {
        return const_iterator(this, minimalNode());
    }
    iterator begin() {
        return iterator(this, minimalNode());
    }

    const_iterator end() const {
        return const_iterator(this, &header);
    }
    iterator end() {
        return iterator(this, &header);
    }
};

#endif /* RBTREE_H */
