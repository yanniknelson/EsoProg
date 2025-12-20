#pragma once

#include <string>

/// If profiling is enabled
#if defined(ENABLE_PROFILER)

#include "optick.h"

/// Lifecycle
#define PROFILE_INIT(name) OPTICK_APP(name)
#define PROFILE_UPDATE() OPTICK_UPDATE()
#define PROFILE_SHUTDOWN() OPTICK_SHUTDOWN()

/// Core event macros
#define PROFILE_FRAME() OPTICK_FRAME("MainThread")
#define PROFILE_SCOPE(name) OPTICK_EVENT(name)
#define PROFILE_FUNCTION() OPTICK_EVENT(OPTICK_FUNC)

/// Thread management
#define PROFILE_THREAD(name) OPTICK_THREAD(name)

/// Capture controls
#define PROFILE_CAPTURE_START() OPTICK_START_CAPTURE()
#define PROFILE_CAPTURE_STOP() OPTICK_STOP_CAPTURE()
#define PROFILE_CAPTURE_SAVE(path) OPTICK_SAVE_CAPTURE(path)

/// If profiling is disabled
#else

/// No-ops
#define PROFILE_INIT(name)
#define PROFILE_UPDATE()
#define PROFILE_SHUTDOWN()
#define PROFILE_FRAME()
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#define PROFILE_THREAD(name)
#define PROFILE_CAPTURE_START()
#define PROFILE_CAPTURE_STOP()
#define PROFILE_CAPTURE_SAVE(path)

#endif // defined(ENABLE_PROFILER)
