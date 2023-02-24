#include "EventQuery.h"

FEventQuery *FEventQuery::New(uint32_t deviceIdx) {
    return new FEventQuery(deviceIdx);
}
