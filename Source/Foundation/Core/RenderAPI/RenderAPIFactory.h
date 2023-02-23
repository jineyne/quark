#pragma once

#include "CorePrerequisites.h"

class FRenderAPIFactory {
public:
    virtual ~FRenderAPIFactory() = default;

public:
    virtual void create() = 0;
    virtual const TCHAR *name() const = 0;
};