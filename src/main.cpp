/*

Copyright (C) 2011-2022, Comine.com ( comine.com@gmail.com )
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
- The the names of the contributors of this project may not be used to 
  endorse or promote products derived from this software without specific 
  prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
`AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

//v1.3 copyright Comine.com 20230518R0831
#include "MStdLib.h"
#include "MCommandArg.h"
#include "MString.h"
#include "MBuffer.h"
#include "MFileSet.h"
#include "MPathNameList.h"


//******************************************************
//* Module Elements
//******************************************************
static const char *GApplicationName="MCopyToDir";	// Used in Help
static const char *GApplicationVersion="1.3";	// Used in Help

////////////////////////////////////////////////////
static void GDisplayHelp(void);
static bool GDisplaySearch(const char *file,void *arg);

////////////////////////////////////////////////////
int main(int argn,const char *argv[])
	{
	MCommandArg args(argn,argv);

	///////////////////////////////////////////////
	if(args.GetArgCount()<4)
		{
		GDisplayHelp();
		return 0;
		}


	bool verbose=false;
	if(args.CheckRemoveArg("-v")==true)
		{  verbose=true;  }

	// Get target dir Target
	MStdArray<char> targetdir;
	if(MStdPathGetAbsolute(args.GetArg(1),targetdir)==false)
		{
		MStdPrintf("**Unable to get Path to directory: %s\n",args.GetArg(1) );
		return 1;
		}

	// Get Pattern 
	const char *pattern;
	pattern=args.GetArg(2);

	MFileSet fileset(true);

	// Process All the src directories
	int i;
	for(i=3;i<args.GetArgCount();++i)
		{
		const char *dirname;
		dirname=args.GetArg(i);

		// Get Absolute Path to source directory
		MStdArray<char> absdirname;
		if(MStdPathGetAbsolute(dirname,absdirname)==false)
			{
			MStdPrintf("**Unable to get abs path for %s\n",dirname);
			return 1;
			}

		bool retflag;
		retflag=fileset.AddFiles(absdirname.Get(),pattern);

		if(verbose==true)
			{
			if(retflag==true)
				{
				MStdPrintf("%-50s : Dir Added \n",dirname);
				}
			else
				{
				MStdPrintf("%-50s : FAILED \n",dirname);
				}
			}
		else if(retflag==false)
			{
			MStdPrintf("**Error: Unable to add directory %s\n",dirname);
			}
		}

	//= We Now have all the src files
	
	//**We Now Process All the files
	fileset.ReadReset();

	const char *filename;
	while(fileset.Read(filename)==true)
		{
		MPathNameList targetfile;
		if(targetfile.Create(targetdir.Get() )==false)
			{
			MStdPrintf("**System Failure\n");
			return 1;
			}
		
		MPathNameList srcfilepath;
		if(srcfilepath.Create(filename)==false)
			{
			MStdPrintf("**System Failure\n");
			return 2;
			}

		if(targetfile.Push(srcfilepath.GetTop())==false)
			{
			MStdPrintf("**System Failure\n");
			return 3;
			}

		// Get Target Path
		MBuffer fulltargetpath;
		if(targetfile.GetFullPath(fulltargetpath)==false)
			{
			MStdPrintf("**System Failure\n");
			return 4;
			}

		if(verbose==true)
			{
			MStdPrintf("%-50s : Copied\n",filename);
			}

		MStdFileCopy(filename,fulltargetpath.GetBuffer());
		}
	
	return 0;
	}


////////////////////////////////////////////////////
static void GDisplayHelp(void)
	{
	MStdPrintf(	"\n"
				"   usage:  %s [-v] <target> <paternmatch> <srcdirs>+[-?]\n"
				"           v%s copyright Comine.com\n"
				"\n"
				"           Tool copies all files that match the pattern to target \n"
				"           location recursively from src directory\n"
				"\n"
				"                -v : verbose output\n"
				"\n"

				"   example:\n"
				"           C:> %s c:\\work\\tmp *.dll c:\\windows\n"
				"                 (This will copy all dlls to c:\\work\\tmp)\n"
				"\n"
				,GApplicationName,GApplicationVersion,GApplicationName);
	}


///////////////////////////////////////////////////////////
static bool GDisplaySearch(const char *file,void *arg)
	{
	bool *verbose=(bool *)arg;
	if(*verbose==true)
		{
		MStdPrintf("Searching %s\n",file);
		}

	return true;
	}

