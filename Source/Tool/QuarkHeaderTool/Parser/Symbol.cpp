#include "Symbol.h"

Symbol::~Symbol() {
    for (auto child : children) {
        delete child;
    }
}