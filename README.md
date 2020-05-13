# ExcerEngine2D

ExcerEngine2D is a simple in-development 2D game engine utilizing SDL and OpenGL, with user interface utilizing ImGui.
Currently, it is only available via a demo game.

 ---------
Additional include & library properties in Visual Studio must be set for this to work.

(C/C++ -> General -> Additional Include Directories): 

**$(SolutionDir)include\GLEW\include\GL;$(SolutionDir)include\;$(SolutionDir)include\SDL\include\;$(SolutionDir)include\imgui\;$(SolutionDir)include\imgui_sdl\;%(AdditionalIncludeDirectories)**

(Linker -> General -> Additional Library Directories):

**$(SolutionDir)include\GLEW\lib\Release\Win32;$(SolutionDir)include\SDL\lib\x86;%(AdditionalLibraryDirectories)**

(Linker -> Input -> Additional Dependencies):

**SDL2.lib;SDL2main.lib;SDL2_image.lib;SDL2_mixer.lib;SDL2_ttf.lib;glew32.lib;glew32s.lib;%(AdditionalDependencies)**

