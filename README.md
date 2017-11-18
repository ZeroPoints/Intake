# Intake
C++ Open AL Mic Input Output, Graphs in Allegro.
Work In Progress.
At the moment you can change the code and make it record and save to file.
Can read a file then try to draw a weird wavelength graph based on the file.

Note:
All paths would be your config settings
Setting up Open AL and Allegro in VS2015


In Project->Properties

VC++ Directories->Library 
	
	Directories: path\OpenAL 1.1 SDK\libs\Win32
  
C/C++->General

	Additional Include Directories: path\OpenAL 1.1 SDK\include
                                  	path\allegro-5.0.10-msvc-11.0\include
  
Linker->Input

	Additional Dependencies: OpenAL32.lib, 
				 allegro-5.0.10-monolith-md-debug.lib
 
 
Linker->General

	Additional Library Directories: path\allegro-5.0.10-msvc-11.0\lib


Debugging

	Environment: PATH=path\allegro-5.0.10-msvc-11.0\bin;%PATH%
	
  
