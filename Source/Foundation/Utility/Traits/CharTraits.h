#pragma once

#include "Prerequisites/PrerequisitesUtil.h"

template<typename T> struct TIsCharType            { enum { Value = false }; };
template<>           struct TIsCharType<ANSICHAR>  { enum { Value = true  }; };
template<>           struct TIsCharType<WIDECHAR>  { enum { Value = true  }; };