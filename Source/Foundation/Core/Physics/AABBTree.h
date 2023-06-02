#pragma once

#include "CorePrerequisites.h"
#include "Math/AABB.h"
#include "Collider.h"

class DLL_EXPORT AABBTree {
private:
    struct Node {
        AABB bounds;
        Collider *collider;

        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;
        bool isLeaf = true;

        ~Node();

        void replaceChild(Node *oldChild, Node *newChild);
    };

    Node *mRoot;

public:
    AABBTree();
    ~AABBTree();

public:
    void insert(Collider *collider);
    void remove(Collider *collider);
    void update(Collider *collider);
    TArray<Collider *> findCollisionObjects(Collider *collider);

private:
    Node *createNode(Collider *collider, Node *parent);
    void removeLeaf(Node *node);
    void updateNode(Node *node);

    void balanceTree(Node* node);

    Node *findNode(Node *node, Collider *collider);

    void findCollisionObjects(TArray<Collider *> &result, Node *node, Collider *collider);
};
