isVisualStudio = false
if _ACTION == "vs2010" or _ACTION == "vs2012" or _ACTION == "vs2015" or _ACTION == "vs2017" or _ACTION == "vs2022" then
	isVisualStudio = true
end

project "spdlog"
	kind "StaticLib"
	targetdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir)
    objdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir .. "/Intermediates")
	
	language "C++"
	includedirs { "spdlog/include" }
	defines { "SPDLOG_COMPILED_LIB" }
	
	files { "spdlog/src/**.cpp", "spdlog/include/**.h" }

project "imgui"
    kind "StaticLib"
    targetdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir)
    objdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir .. "/Intermediates")
    
    language "C++"
    includedirs { "imgui", "imgui/backends", "imgui/examples/libs/glfw/include" }
    files { "imgui/examples/libs/glfw/include/**.h", "imgui/backends/imgui_impl_glfw.h", "imgui/backends/imgui_impl_glfw.cpp", "imgui/backends/imgui_impl_opengl3.h", "imgui/backends/imgui_impl_opengl3.cpp",
			"imgui/*.h", "imgui/*.cpp", "imgui/misc/cpp/imgui_stdlib.*", "imgui/misc/debugger/imgui.natvis", "imgui/misc/debugger/imgui.natstepfilter" }

	filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"
       kind "WindowedApp"

project "OptickCore"
	kind "SharedLib"
	targetdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir)
    objdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir .. "/Intermediates")
	
	language "C++"
	defines { "_CRT_SECURE_NO_WARNINGS", "OPTICK_LIB=1", "OPTICK_EXPORTS", "OPTICK_ENABLE_GPU_D3D12=0", "OPTICK_ENABLE_GPU_VULKAN=0" }

if isVisualStudio then
    cppdialect "C++11"
else
	cppdialect "gnu++11"
end

	includedirs { "optick/src" }
	
	files {
		"optick/src/**.cpp",
		"optick/src/**.h", 
	}
	
	vpaths {
		["api"] = { 
			"optick/src/optick.h",
			"optick/src/optick.config.h",
		},
	}
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"