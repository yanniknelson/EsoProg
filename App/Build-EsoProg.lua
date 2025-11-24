project "EsoProg"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"

   files { "Src/**.h", "Src/**.cpp" }

   includedirs
   {
      "Src",

	  -- Include Core
	  "%{wks.location}/Core/SmartEnums/Src",
	  "%{wks.location}/Core/ELanguages/Src",
	  "%{wks.location}/Core/ImGuiHelpers/FileDialogBox",
	  "%{wks.location}/Core/ImGuiHelpers/ImGuiValueChangeCallbacks",
      "%{wks.location}/Core/ITokeniser/Src",
      "%{wks.location}/Core/IStack/Src",
	  "%{wks.location}/Core/IRuntime/Src",
	  "%{wks.location}/Core/NullInterpreter/Src",
	  "%{wks.location}/Core/PietInterpreter/Src",

      -- Include externals
      "%{wks.location}/%{externals.stb}",
      "%{wks.location}/%{externals.imgui}",
      "%{wks.location}/%{externals.imgui}/misc/cpp",
      "%{wks.location}/%{externals.imgui}/backends",
      "%{wks.location}/%{externals.imgui}/examples/libs/glfw/include",
      "%{wks.location}/%{externals.FontAwesomeHeader}"
   }

   libdirs
   {
       "%{wks.location}/%{externals.imgui}/examples/libs/glfw/lib-vc2010-64"
   }

   links
   {
      "spdlog",
      "glfw3",
      "gdi32",
      "opengl32",
      "imm32",
      "imgui",
      "Logger",
      "ImGuiFileDialogBox",
      "ImGuiValueChangeCallbacks",
      "IStack",
      "PietInterpreter"
   }

   targetdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir)
   objdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir .. "/Intermediates")

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