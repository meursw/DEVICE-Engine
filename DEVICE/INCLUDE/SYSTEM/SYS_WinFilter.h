#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif

#include <sdkddkver.h>
// The following #defines disable a bunch of unused windows stuff. If you 
// get weird errors when trying to do some windows stuff, try removing some
// (or all) of these defines (it will increase build time though).
#ifndef FULL_WINTARD

#define NOTAPE
#endif

#define NOMINMAX

#ifndef STRICT
#define STRICT
#endif

#include <Windows.h>