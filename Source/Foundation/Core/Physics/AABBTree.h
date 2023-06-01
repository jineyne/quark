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
    };

    Node *mRoot;

public:
    AABBTree();
    ~AABBTree();

public:
    void insert(Collider *collider);

private:
    void insert(Collider *collider, Node *node);

    void createNode(Collider *collider, Node *parent);
};
