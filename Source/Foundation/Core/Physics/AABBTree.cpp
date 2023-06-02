#include "AABBTree.h"

AABBTree::Node::~Node() {
    /*if (left && left->parent == this) {
        q_delete(left);
    }

    if (right && right->parent == this) {
        q_delete(right);
    }*/
}

void AABBTree::Node::replaceChild(AABBTree::Node *oldChild, AABBTree::Node *newChild) {
    if (this->left == oldChild) {
        this->left = newChild;
    } else if (this->right == oldChild) {
        this->right = newChild;
    } else {
        checkf(true, TEXT("oldChild is not a child of this node"));
        return;
    }

    if (newChild) {
        newChild->parent = this;
    }
}

AABBTree::AABBTree() {
    // Node *node = q_new<Node>();
}

AABBTree::~AABBTree() {
    q_delete(mRoot);
}

void AABBTree::insert(Collider *collider) {
    // 새로운 노드를 생성
    Node *newNode = createNode(collider, nullptr);

    // 트리가 비어 있는 경우, 새로운 노드를 루트로 설정
    if (!mRoot) {
        mRoot = newNode;
        return;
    }

    Node *node = mRoot;
    while (!node->isLeaf) {
        AABB combined = AABB::Union(node->bounds, newNode->bounds);

        float cost = combined.getArea() - node->bounds.getArea();
        float costLeft = cost + (node->left && node->left->isLeaf ? AABB::Union(node->left->bounds, newNode->bounds).getArea() - node->left->bounds.getArea() : 0.0f);
        float costRight = cost + (node->right && node->right->isLeaf ? AABB::Union(node->right->bounds, newNode->bounds).getArea() - node->right->bounds.getArea() : 0.0f);

        if (cost < costLeft && cost < costRight) {
            break;
        }

        node = (costLeft < costRight) ? node->left : node->right;
    }

    Node *oldParent = node->parent;
    Node *newParent = createNode(nullptr, oldParent);

    if (oldParent) {
        oldParent->replaceChild(node, newParent);
    } else {
        mRoot = newParent;
    }

    newParent->left = node;
    newParent->right = newNode;
    newNode->parent = newParent;
    node->parent = newParent;

    updateNode(newParent);
}

void AABBTree::remove(Collider *collider) {
    Node *node = findNode(mRoot, collider);
    if (node) {
        removeLeaf(node);
    }
}

void AABBTree::update(Collider *collider) {
    Node *node = findNode(mRoot, collider);
    if (node) {
        AABB oldBounds = node->bounds;
        node->bounds = collider->generateAABB();
        if (!oldBounds.contains(node->bounds)) {
            removeLeaf(node);
            insert(collider);
        } else {
            while (node->parent && !node->parent->bounds.contains(node->bounds)) {
                node->parent->bounds = AABB::Union(node->parent->left->bounds, node->parent->right->bounds);
                node = node->parent;
            }
        }
    }
}

TArray<Collider *> AABBTree::findCollisionObjects(Collider *collider) {
    TArray<Collider *> result;
    findCollisionObjects(result, mRoot, collider);
    return result;
}

AABBTree::Node *AABBTree::createNode(Collider *collider, Node *parent) {
    Node *node = q_new<Node>();
    node->bounds = collider ? collider->generateAABB() : AABB(Vector3::ZeroVector, Vector3::ZeroVector);
    node->collider = collider;
    node->parent = parent;
    node->isLeaf = true;
    return node;
}

void AABBTree::removeLeaf(AABBTree::Node *node) {
    if (node == nullptr) {
        return;
    }

    if (node == mRoot) {
        q_delete(mRoot);
        mRoot = nullptr;
        return;
    }

    auto parent = node->parent;
    auto grandParent = parent->parent;
    auto sibling = (node == parent->left) ? parent->right : parent->left;

    if (grandParent) {
        grandParent->replaceChild(parent, sibling);
        sibling->parent = grandParent;

        balanceTree(grandParent);
    } else {
        mRoot = sibling;
        sibling->parent = nullptr;
    }

    q_delete(parent);
    q_delete(node);

    updateNode(sibling);
}

void AABBTree::updateNode(AABBTree::Node *node) {
    if (node == nullptr) {
        return;
    }

    node->isLeaf = node->left == nullptr && node->right == nullptr;
    if (!node->isLeaf) {
        if (node->left == nullptr) {
            node->bounds = node->right->bounds;
        } else if (node->right == nullptr) {
            node->bounds = node->left->bounds;
        } else {
            node->bounds = AABB::Union(node->left->bounds, node->right->bounds);
        }

    }

    if (node->parent) {
        updateNode(node->parent);
    }
}

void AABBTree::balanceTree(AABBTree::Node *node) {
    while (node) {
        if (!node->isLeaf) {
            node->bounds = AABB::Union(node->left->bounds, node->right->bounds);
        }

        Node* left = node->left;
        Node* right = node->right;

        if (right->bounds.getArea() < left->bounds.getArea()) {
            node->left = right;
            node->right = left;
        }
        node->isLeaf = node->left == nullptr && node->right == nullptr;
        node = node->parent;
    }
}

/*void AABBTree::replaceNode(AABBTree::Node *oldNode, AABBTree::Node *newNode) {
    Node *parent = oldNode->parent;
    if (parent) {
        if (parent->left == oldNode) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
    } else {
        mRoot = newNode;
    }
    newNode->parent = parent;

    Node *newParent = q_new<Node>();
    newParent->left = oldNode;
    newParent->right = newNode;
    newParent->isLeaf = false;
    oldNode->parent = newParent;
    newNode->parent = newParent;
    newParent->bounds = AABB::Union(oldNode->bounds, newNode->bounds);

    oldNode->isLeaf = oldNode->left == nullptr && oldNode->right == nullptr;
    newNode->isLeaf = newNode->left == nullptr && newNode->right == nullptr;

    if (parent) {
        replaceNode(parent, newParent);
    } else {
        mRoot = newParent;
    }
}*/

AABBTree::Node *AABBTree::findNode(AABBTree::Node *node, Collider *collider) {
    if (node == nullptr) {
        return nullptr;
    }

    if (node->isLeaf) {
        return node->collider == collider ? node : nullptr;
    } else {
        Node *leftResult = findNode(node->left, collider);
        return leftResult ? leftResult : findNode(node->right, collider);
    }
}

void AABBTree::findCollisionObjects(TArray<Collider *> &result, AABBTree::Node *node, Collider *collider) {
    if (node == nullptr) {
        return;
    }

    // Check if the bounding boxes of the current node and the test object intersect
    if (!node->bounds.intersects(collider->generateAABB())) {
        return;
    }

    // If they do, add the object stored in the current node to the results
    if (node->isLeaf && node->collider != collider) {
        result.add(node->collider);
    }

    // Then recurse through the child nodes
    findCollisionObjects(result, node->left, collider);
    findCollisionObjects(result, node->right, collider);
}
