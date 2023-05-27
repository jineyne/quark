#pragma once

#include "CorePrerequisites.h"

class DLL_EXPORT EventQuery {
protected:
    bool mActive = false;

public:
    /**	Triggered when GPU starts processing the query. */
    TEvent<void()> onTriggered;

public:
    EventQuery() = default;
    virtual ~EventQuery() = default;

public:
    /**
     * Creates a new query, but does not schedule it on GPU.
     *
     * @param[in]	deviceIdx	Index of the GPU device to create the query on.
     */
    static EventQuery *New(uint32_t deviceIdx = 0);

public:
    /**
    * Starts the query.
    *
    * @param[in]	cb		Optional command buffer to queue the operation on. If not provided operation
    *						is executed on the main command buffer. Otherwise it is executed when
    *						RenderAPI::executeCommands() is called. Buffer must support graphics or compute operations.
    *
    * @note
    * Once the query is started you may poll isReady() method to check when query has finished, or you may hook up
    * an #onTriggered callback and be notified that way.
    */
    virtual void begin(CommandBuffer *cb = nullptr) = 0;

    /** Check if GPU has processed the query. */
    virtual bool isReady() const = 0;

protected:
    friend class QueryManager;

    /**	Returns true if the has still not been completed by the GPU. */
    bool isActive() const { return mActive; }
    void setActive(bool active) { mActive = active; }
};
