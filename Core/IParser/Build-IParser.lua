project "IParser"
   kind "StaticLib"
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
	  "%{wks.location}/Core/IAST/Src",
	  "%{wks.location}/Core/ITokeniser/Src",

      -- Include externals
      "%{wks.location}/%{externals.spdlog}/include"
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