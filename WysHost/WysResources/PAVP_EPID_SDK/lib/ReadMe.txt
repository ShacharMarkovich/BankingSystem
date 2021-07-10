In this folder you may find the libraries binaries for referencing in DAL SDK samples that use the Protected Transaction Display (PTD) feature, 
also known as What You See Is What You Sign (WYSIWYS or in short WYS): ProtectedDisplaySample, ProtectedOutputSample and WysSample.

Each one of the WYS samples references by default the compiled libraries for using when your host application is compiled with the Visual Studio 2010 C++ build tools.
The libraries in this folder allow you to build WYS samples with other Visual Studio versions.

When you want to change the used Visual Studio build tools version of the WYS samples, do the following steps:
- Open the host application sample solution in your preferred Visual Studio version. 
- Update VC++ compiler and libraries toolset:
	* Click "OK" in the automatic "Upgrade VC++ Compiler and Libraries" message when opening the host application solution
		OR
	* Right click on the project in the "Solution Explorer" window -> "Upgrade VC++ Compiler and Libraries" -> Click "OK".
		OR
	* Open the *Library project properties window (right click on the project in the "Solution Explorer" window -> "Properties"), go to Configuration Properties -> General 
	  and change the "Platform toolset" to the desired Visual Studio version
- Update the WYS libraries path:
	* Open the *Library project properties window (right click on the project in the "Solution Explorer" window -> "Properties")
	* Go to Configuration Properties -> Linker -> General
	* Change in the Additional Library Directories to the compatible Visual Studio libraries folder path  
- When compiling with Visual Studio VC++ 2010:
	* Make sure you have Windows SDK installed
	* Open the *Library project properties window (right click on the project in the "Solution Explorer" window -> "Properties")
	* Go to Configuration Properties -> VC++ Directories
	* Add the following directories to the Include Directories field:
		$(ProgramFiles)\Windows Kits\<installed Windows SDK version>\Include\um 
		$(ProgramFiles)\Windows Kits\<installed Windows SDK version>\Include\shared
- When compiling with Visual Studio VC++ 2015:
	* Open the *Library project properties window (right click on the project in the "Solution Explorer" window -> "Properties")
	* Go to Configuration Properties -> Linker -> Input
	* Add legacy_stdio_definitions.lib to the Additional Dependencies field
- Re-build and run the host application sample
