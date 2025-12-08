#pragma once

#include <string>

/// If profiling is enabled
#if defined(ENABLE_PROFILER)

#include "optick.h"

/// Lifecycle
#define PROFILE_INIT(name) OPTICK_APP(name)
#define PROFILE_UPDATE()   OPTICK_UPDATE()
#define PROFILE_SHUTDOWN() OPTICK_SHUTDOWN()

/// Core event macros
#define PROFILE_FRAME()     OPTICK_FRAME("MainThread")
#define PROFILE_SCOPE(name) OPTICK_EVENT(name)
#define PROFILE_FUNCTION()  OPTICK_EVENT(OPTICK_FUNC)

/// Thread management
#define PROFILE_THREAD(name)       OPTICK_THREAD(name)

/// Capture controls
#define PROFILE_CAPTURE_START()    OPTICK_START_CAPTURE()
#define PROFILE_CAPTURE_STOP()     OPTICK_STOP_CAPTURE()
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

/**
 * @namespace Profiler
 * @brief Profiler-specific classes, and functions.
 */
namespace Profiler
{

/**
 * @brief Profiler class.
 */
class CProfilerManager
{
public:
    /// Lifecycle functions

    /**
     * @brief Centralized initialization routine for the entire profiler system.
     * This must be called once at application startup.
     */
    static void Init(const std::string& app_name)
    {
        PROFILE_INIT(app_name.c_str());
    }

    /**
     * @brief Centralized deinitialization routine for the entire profiler system.
     * This must be called once at application termination.
     */
    static void Shutdown()
    {
        PROFILE_SHUTDOWN();
    }

    /**
     * @brief Centralized update routine for the entire profiler system.
     */
    static void Update()
    {
        PROFILE_UPDATE();
    }

    /// Core events

    /**
     * @brief Function to profile a frame.
     */
    static void ProfileFrame()
    {
        PROFILE_FRAME();
    }

    /**
     * @brief Function to profile a scope.
     *
     * @param[in] scope_name The name of given scope.
     */
    static void ProfileScope(const std::string& scope_name)
    {
        PROFILE_SCOPE(scope_name.c_str());
    }

    /**
     * @brief Function to profile a function.
     */
    static void ProfileFunction()
    {
        PROFILE_FUNCTION();
    }

    /// Thread management

    /**
     * @brief Function to profile a thread.
     *
     * @param[in] thread_name The name of given thread.
     */
    static void ProfileThread(const std::string& thread_name)
    {
        PROFILE_THREAD(thread_name.c_str());
    }

    /// Capture controls

    /**
     * @brief Function to start the capture of the profile.
     */
    static void ProfileCaptureStart()
    {
        PROFILE_CAPTURE_START();
    }

    /**
     * @brief Function to stop the capture of the profile.
     */
    static void ProfileCaptureStop()
    {
        PROFILE_CAPTURE_STOP();
    }

    /**
     * @brief Function to save the capture of the profile.
     *
     * @param[in] filepath The path to save the capture.
     */
    static void ProfileCaptureSave(const std::string& filepath)
    {
        PROFILE_CAPTURE_SAVE(filepath.c_str());
    }

    /**
     * @brief Function to stop, and save the capture of the profile.
     *
     * @param[in] filepath The path to save the capture.
     */
    static void ProfileCaptureStopAndSave(const std::string& filepath)
    {
        PROFILE_CAPTURE_STOP();
        PROFILE_CAPTURE_SAVE(filepath.c_str());
    }

    /// State getter function

    /**
     * @brief Whether profiler is enabled or not.
     */
    static bool IsRunning()
    {
#if defined(ENABLE_PROFILER)
        return true;
#else
        return false;
#endif
    }
};

} // namespace Profiler
