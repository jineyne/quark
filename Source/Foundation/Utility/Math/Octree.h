#pragma once

#include "UtilityCore.h"
#include "AABB.h"
#include "Vector3.h"

// TODO: fix: insert issue
struct DefaultOctreeOptions {
    static constexpr size_t MinElementsPerNode = 0;
    static constexpr size_t MaxElementsPerNode = 1024;
};

template <typename Type, class Options = DefaultOctreeOptions>
class DLL_EXPORT Octree {
    struct Node {
        Node *parent = nullptr;
        AABB bounds;
        TArray<TPair<AABB, Type>> elements;
        TArray<Node *> childs;

        Node(AABB bounds, Node *parent) : bounds(bounds), parent(parent) {}
        ~Node() {
            for (auto child : childs) {
                q_delete(child);
            }
        }
    };

private:
    Node *mRoot;

public:
    Octree(Vector3 size) {
        mRoot = q_new<Node>(AABB(Vector3::ZeroVector - size / 2, Vector3::ZeroVector + size / 2), nullptr);
    }

    ~Octree() {
        q_delete(mRoot);
    }

    static bool CompareAABB(const TPair<AABB, Type> & a, const TPair<AABB, Type> &b) {
        return a.key.getSize().length() > b.key.getSize().length();
    }

public:

    bool insert(const Type &item, const AABB &bounds) {
        // invalid position
        if (!mRoot->bounds.contains(bounds.getCenter())) {
            return false;
        }

        return insert(item, bounds, mRoot);
    }

    bool update(const Type &item, const AABB &oldBounds, const AABB &newBounds) {
        if (!mRoot->bounds.contains(oldBounds.getCenter())) {
            return false;
        }

        if (!remove(item, oldBounds, mRoot)) {
            return false;
        }

        return insert(item, newBounds, mRoot);
    }

    bool remove(const Type &item, const AABB &bounds) {
        // invalid position
        if (!mRoot->bounds.contains(bounds.getCenter())) {
            return false;
        }

        return remove(item, bounds, mRoot);
    }

    void recombine() {
        recombine(mRoot);
    }

    TArray<TPair<Type, Type>> resolveCollisions(float msec) {
        TArray<TPair<Type, Type>> collisions;
        resolveCollisions(mRoot, collisions);
        return collisions;
    }

private:
    bool insert(const Type &item, const AABB &bounds, Node *node) {
        if (node->elements.length() < Options::MaxElementsPerNode) {
            node->elements.add(TPair(bounds, item));
            std::sort(node->elements.begin(), node->elements.end(), CompareAABB);
            return true;
        } else {
            // split node if it hasn't been split yet!
            if (node->childs.empty()) {
                for (int i = 0; i < 8; i++) {
                    Vector3 pos = node->bounds.getCenter();
                    Vector3 size = node->bounds.getSize();

                    if ((i & 1) == 1) pos.x += size.x / 2;
                    if ((i & 2) == 2) pos.y += size.y / 2;
                    if ((i & 4) == 4) pos.z += size.z / 2;

                    Node *newNode = q_new<Node>(AABB(pos - size / 2, pos + size / 2), node);

                    node->childs.add(newNode);
                }

                for (auto element : node->elements) {
                    insert(element.value, element.key, node);
                }

                node->elements.clear();
            }

            bool success = false;
            // Recursively add item to the appropriate child childs.
            for (Node *child : node->childs) {
                // Insert item into child node if it fits within its bounds.
                if (child->bounds.intersects(bounds)) {
                    success |= insert(item, bounds, child);
                }
            }

            return success;
        }
    }

    bool remove(const Type& item, const AABB& bounds, Node *node) {
        // If the node is null, the item was not found.
        if (!node) return false;

        // Check if the item is in the current node.
        for (auto i = 0; i < node->elements.length(); ++i) {
            if (node->elements[i].value == item) {
                node->elements.removeAt(i);
                return true;
            }
        }

        // If the item is not in the current node, check the appropriate child nodes.
        bool success = false;
        for (Node* child : node->childs) {
            if (child->bounds.intersects(bounds)) {
                success |= remove(item, bounds, child);
            }
        }

        // The item was not found in the current node or any child nodes.
        return success;
    }

    void resolveCollisions(Node *node, TArray<TPair<Type, Type>> &collisions) {
        if (!node) {
            return;
        }

        for (int i = 0; i < node->elements.length(); ++i) {
            for (int j = i + 1; j < node->elements.length(); ++j) {
                if (node->elements[i].key.intersects(node->elements[j].key)) {
                    collisions.add(TPair(node->elements[i].value, node->elements[j].value));
                }
            }
        }

        for (auto &child : node->childs) {
            resolveCollisions(child, collisions);
        }
    }

    void recombine(Node *node) {
        if (!node) return;

        int count = 0;
        for (auto child : node->childs) {
            count += child->elements.length();
        }

        if (count < Options::MinElementsPerNode) {
            for (auto i = 0; i < node->childs.length(); ++i) {
                node->elements.append(node->childs[i]->elements);
                q_delete(node->childs[i]);
            }
        } else {
            for (Node *child : node->childs) {
                recombine(child);
            }
        }
    }
};
