#include "AABBTree.h"

AABBTree::Node::~Node() {

}

AABBTree::AABBTree() {
    // Node *node = q_new<Node>();
}

AABBTree::~AABBTree() {
    q_delete(mRoot);
}

void AABBTree::insert(Collider *collider) {
    insert(collider, mRoot);
}

void AABBTree::insert(Collider *collider, AABBTree::Node *node) {

}

void AABBTree::createNode(Collider *collider, Node *parent) {
    Node *node = q_new<Node>();
    node->bounds = collider->generateAABB();
    node->collider = collider;

    node->parent = parent;
}
