project "BrainFckInterpreter"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"

   files { "Src/**.h", "Src/**.cpp", "README.md" }

   includedirs
   {
      "Src",

	  -- Include Core
	  "%{wks.location}/Core/ImGuiHelpers/ImGuiValueChangeCallbacks",
      "%{wks.location}/Core/SmartEnums/Src",
      "%{wks.location}/Core/LanguageHelpers/ELanguages/Src",
      "%{wks.location}/Core/LanguageHelpers/IAST/Src",
      "%{wks.location}/Core/LanguageHelpers/IParser/Src",
	  "%{wks.location}/Core/LanguageHelpers/ITokeniser/Src",
	  "%{wks.location}/Core/LanguageHelpers/IMemoryArray/Src",
	  "%{wks.location}/Core/LanguageHelpers/IRuntime/Src",

      -- Include externals
      "%{wks.location}/%{externals.spdlog}/include",
      "%{wks.location}/%{externals.stb}",
      "%{wks.location}/%{externals.imgui}",
      "%{wks.location}/%{externals.imgui}/misc/cpp",
      "%{wks.location}/%{externals.imgui}/backends",
      "%{wks.location}/%{externals.imgui}/examples/libs/glfw/include",
      "%{wks.location}/%{externals.FontAwesomeHeader}"
   }

   targetdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir)
   objdir ("%{wks.location}/bin/%{prj.name}/" .. OutputDir .. "/Intermediates")

   filter "system:windows"
       systemversion "latest"
       defines { }

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