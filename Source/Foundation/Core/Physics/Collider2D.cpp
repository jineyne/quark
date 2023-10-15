//
// Created by jiney on 2023-10-15.
//

#include "Collider2D.h"

Actor *Collider2D::getOwner() const {
    return mOwner;
}

void Collider2D::setOwner(Actor *owner) {
    mOwner = owner;
}
