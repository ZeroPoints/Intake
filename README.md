# Intake
C++ Open AL Mic Input Output


Takes user input until user pushes esc then it plays it back to the user.


All paths would be your config props

Setting up Open AL in VS2015

In Project->Properties

  VC++ Directories->Library Directories: C:\Program Files %28x86%29\OpenAL 1.1 SDK\libs\Win32
  
C/C++->General

  Additional Include Directories: C:\Program Files %28x86%29\OpenAL 1.1 SDK\include
  
Linker->Input

  Additional Dependencies: OpenAL32.lib
  
  
