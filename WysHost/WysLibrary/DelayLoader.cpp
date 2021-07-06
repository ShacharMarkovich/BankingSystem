/**
***
*** Copyright  (C) 2014 Intel Corporation. All rights reserved.
***
*** The information and source code contained herein is the exclusive
*** property of Intel Corporation. and may not be disclosed, examined
*** or reproduced in whole or in part without explicit written authorization
*** from the company.
***
*** ----------------------------------------------------------------------------
**/

#include <windows.h>
#include <DelayImp.h>
#include <iostream>

using namespace std;

/*
This is the definition of the registry path that the JHI path can be found in.
The path differs between JHI services of the emulation to that of a real silicon-based platform and, therefore, is compilation dependent.
*/
#if AMULET
	#define REGISTRY_DAL_PATH			L"Software\\Intel\\Services\\DAL_EMULATION"
#else
	#define REGISTRY_DAL_PATH			L"Software\\Intel\\Services\\DAL"
#endif
//The registry value to read JHI path from
#define REGISTRY_PATH_FILES_VALUE	L"FILELOCALE"

#define DEFAULT_LEN					256
#define REGISTRY_BASE				HKEY_LOCAL_MACHINE

//Prototypes for JHI.dll signature verification functions

bool VerifyFileSignature(const std::wstring &filePath);
bool VerifyCertificateName(const std::wstring &filePath);
//helper function for reading JHI string from registry
int ReadRegistryString(HKEY base, 
				  wchar_t *path, 
				  wchar_t *valueName,
				  wchar_t *value,
				  DWORD	 *valueSize
				  )
{
	HKEY hKey;
	LONG  ret;
	DWORD dwType = REG_SZ;

	if (!base || !path || !valueName || !value)
	{
		return -1;
	}

	//Open the registry key
	if (RegOpenKeyExW(base,
					path,
					0,
					KEY_READ|KEY_WOW64_64KEY,  //The JHI writes with KEY_WOW64_64KEY
					&hKey) != ERROR_SUCCESS)
	{
		return -1;
	}
	
	//Query out the JHI.dll path
	ret = RegQueryValueExW(hKey, 
					valueName, 
					0,
					&dwType, 
					(LPBYTE)value, 
					valueSize);

	if (ret != ERROR_SUCCESS)
	{
		return -1;
	}
	
	//Check value type
	if(dwType != REG_SZ && dwType != REG_EXPAND_SZ)
	{
		// value not the expected type
		return -1;
	}
	
	*valueSize = (*valueSize - sizeof(wchar_t))/sizeof(wchar_t);

	if(*valueSize<=0)
	{
		// value size is wrong
		return -1;
	}
	
	//Close registry key
	RegCloseKey(hKey);

	return 0;
}


/*
This is the delay load hook function. 
This function is called the first time any of the JHI functions are called with the dliNotePreLoadLibrary argument.
We then extract the JHI path from the registry, for a more secured load, verify the dll signature and publisher, and load the dll dynamically.
At all other notification hooks we return control to Microsoft delay-load helper function.
See http://msdn.microsoft.com/en-us/library/z9h1h6ty(v=vs.100).aspx for a more detailed description regarding delay-load hook.
*/
FARPROC WINAPI delayHook(unsigned dliNotify, PDelayLoadInfo pdli)
{
	switch (dliNotify) {
		
		case dliStartProcessing :
		case dliNotePreGetProcAddress :
		case dliFailLoadLib :
		case dliFailGetProc :
		case dliNoteEndProcessing : 
		//we are not interested in those events, let the default behavior
		//return 0 returns control to the default helper
		  return 0;
		  break;

		  //This is the event we are interested in - JHI.dll needs to be loaded
		  //we will perform the load here, for security reasons
		case dliNotePreLoadLibrary :
			{
				//get the name of the dll the application is trying to delay load
				LPCSTR delayedDllName=pdli->szDll;
				if(strcmp(delayedDllName,"JHI.dll")!=0)
					//this is not JHI.dll, let the default behavior
					//return 0 returns control to the default helper
					return 0;
				wchar_t dllName[9] ; 
				mbstowcs(dllName, "\\JHI.dll",9);
				dllName[8]='\0';

				DWORD	regValSize = DEFAULT_LEN;
				wchar_t	*wtRegValue = new wchar_t[regValSize];

				//read the registry path to find where the JHI.dll of the current platform\emulation is located
				if(ReadRegistryString(REGISTRY_BASE, 
									REGISTRY_DAL_PATH, 
									REGISTRY_PATH_FILES_VALUE,
									wtRegValue,
								&regValSize) == 0) 
				{
					regValSize = DEFAULT_LEN; 
					wcsncat_s(wtRegValue, DEFAULT_LEN, dllName, _TRUNCATE);	
					//wtRegValue now contains the path to JHI.dll
				} 
				else
				{
					//Oops - some error occurred during reading the value
					delete[] wtRegValue;
					throw std::exception("JHI.dll could not be located.");					
				}
				
				LPWSTR ptr = wtRegValue; 
//In AMULET mode there is no need to verify the JHI.dll signature.
#ifndef AMULET
				//Verify JHI.dll signature and publisher
				cout << "Verifying JHI.dll signature and publisher." << endl;
				std::wstring path(wtRegValue);
				if(!VerifyFileSignature(path) || !VerifyCertificateName(path))//JHI dll is not signed by Intel
				{
					delete[] wtRegValue;
					throw std::exception("JHI.dll signature could not be verified.");
				}

				//Load the dll dynamically at run time
				cout << "Verification was successful. Loading JHI.dll." << endl;
#endif
				HMODULE lib = LoadLibraryW(wtRegValue);

				delete[] wtRegValue;

				//return the JHI.dll handle to the helper function
				return reinterpret_cast<FARPROC>(lib); 
				
			}
			break;		
		default :
			return NULL;
	}
	return NULL;
}

