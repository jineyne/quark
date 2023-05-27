#pragma once

#include "CorePrerequisites.h"

class RenderAPIFactory {
public:
    virtual ~RenderAPIFactory() = default;

public:
    virtual void create() = 0;
    virtual const TCHAR *name() const = 0;
};