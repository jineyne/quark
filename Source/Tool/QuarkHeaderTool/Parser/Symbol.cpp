#include "Symbol.h"

FSymbol::~FSymbol() {
    for (auto child : children) {
        delete child;
    }
}