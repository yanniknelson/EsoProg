// Main.cpp : Defines the entry point for the application. 

#include "Main.h"

#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <ImGuiSetStyles.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
    GLFWwindow* window = glfwCreateWindow((int)(1280 * main_scale), (int)(800 * main_scale), EsoProg::i_ProgramName, nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    SetImGuiShapingStyle();
    SetImGuiDarkStyle();

    EsoProg* pProgramInstance = new EsoProg(window);
    pProgramInstance->i_ProgramName;

    std::thread runtimeWorker([&]()
        {
            RuntimeSyncronisationStruct& rSync = pProgramInstance->m_sync;
            rSync.runtimeStateMtx.lock(); // lock the state for the first iterations
            while (!rSync.exit)
            {
                while (rSync.iterations != 0) // run while we have iterations to run
                {
                    if (rSync.wantsReset)
                    {
                        pProgramInstance->Reset();
                        rSync.wantsReset = false;
                    }

                    if (rSync.iterations > 0)
                    {
                        --rSync.iterations;
                    }

                    if (!pProgramInstance->UpdateRuntime())
                    {
                        rSync.iterations = 0;
                    }
                    if (rSync.renderWantsState) // if the rendering thread has flagged it wants to copy the current state then unlock the current state
                    {
                        rSync.runtimeStateMtx.unlock();
                        std::unique_lock<std::mutex> lck(rSync.finishedWithStateMtx); // once we've unlocked the current state we want to wait until the copyingStateCV is notified (indication copying is complete)
                        rSync.finishedStateWithCv.wait(lck, [&rSync]() { return !rSync.renderWantsState.load(); });
                        rSync.runtimeStateMtx.lock(); // now that copying is complete we can re-gain the lock to continue executing as many iterations as we can while the render thread is rendering
                    }

                    if (pProgramInstance->IsRuntimeWaitingOnInput())
                    {
                        rSync.runtimeStateMtx.unlock();
                        std::unique_lock<std::mutex> lck(rSync.waitingOnInputMtx);
                        rSync.waitingOnInputCV.wait(lck, [&]() { return pProgramInstance->m_sync.exit || !pProgramInstance->IsRuntimeWaitingOnInput(); });
                        rSync.runtimeStateMtx.lock();
                    }

                    if (rSync.instructionWaitTime > 0) // sleep for speed milliseconds to creation running speed
                    {
                        rSync.runtimeStateMtx.unlock();
                        std::this_thread::sleep_for(std::chrono::milliseconds(rSync.instructionWaitTime));
                        rSync.runtimeStateMtx.lock();
                    }
                }

                if (rSync.renderWantsState) // if the rendering thread has flagged it wants to copy the current state then unlock the current state
                {
                    rSync.runtimeStateMtx.unlock();
                    std::unique_lock<std::mutex> lck(rSync.finishedWithStateMtx); // once we've unlocked the current state we want to wait until the copyingStateCV is notified (indication copying is complete)
                    rSync.finishedStateWithCv.wait(lck, [&rSync]() { return !rSync.renderWantsState.load(); });
                    rSync.runtimeStateMtx.lock(); // now that copying is complete we can re-gain the lock to continue executing as many iterations as we can while the render thread is rendering
                }
            }
            rSync.runtimeStateMtx.unlock(); // make sure we unlock the state when we're being told to shut down.
        });

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        pProgramInstance->CopyState();
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        pProgramInstance->Render();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags)//& ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            //ImGui::UpdatePlatformWindows();
            //ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }
    pProgramInstance->m_sync.iterations = 0;
    pProgramInstance->m_sync.exit = true;
    if (runtimeWorker.joinable())
    {
        if (pProgramInstance->IsRuntimeWaitingOnInput())
        {
            pProgramInstance->m_sync.waitingOnInputCV.notify_all();
        }
        runtimeWorker.join();
    }
    delete pProgramInstance;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
