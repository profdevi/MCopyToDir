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

//v1.1 copyright Comine.com 20160714R1810
#include "MStdLib.h"
#include "MFileOps.h"
#include "MDirReader.h"
#include "MStringList.h"
#include "MString.h"
#include "MDirTreeOps.h"
#include "MFileSet.h"


//******************************************************
//**  MFileSet class
//******************************************************
void MFileSet::ClearObject(void)
	{
	}


////////////////////////////////////////////////
MFileSet::MFileSet(bool create)
	{
	ClearObject();
	if(create==true && Create()==false)
		{
		return;
		}
	}


////////////////////////////////////////////////
MFileSet::~MFileSet(void)
	{  Destroy();  }


////////////////////////////////////////////////
bool MFileSet::Create(void)
	{
	Destroy();

	if(mFileList.Create()==false)
		{
		Destroy();
		return false;
		}

	return true;
	}


////////////////////////////////////////////////
bool MFileSet::Destroy(void)
	{
	mFileList.Destroy();
	ClearObject();
	return true;
	}


///////////////////////////////////////////////
bool MFileSet::AddFiles(const char *argfilepath)
	{
	MDirReader reader;
	if(reader.Create(argfilepath)==false)
		{
		return false;
		}
	
	MFileOps fileops(true);
	MString canonicalfilepath;

	while(reader.Read()==true)
		{
		if(fileops.GetCanonicalName(reader.GetFullFilePath(),canonicalfilepath)==false)
			{
			continue;
			}

		// Check if file exists already
		if(mFileList.IsMember(canonicalfilepath.Get())==true)
			{
			continue;
			}

		if(mFileList.AddString(canonicalfilepath.Get())==false)
			{
			continue;
			}
		}

	return true;
	}


//////////////////////////////////////////////
bool MFileSet::AddFiles(const char *argrootdir,const char *argpattern)
	{
	MDirTreeOps dirtreeops(true);
	MFileOps fileops(true);
	MString rootdir;

	if(fileops.GetAbsolutePath(argrootdir,rootdir)==false)
		{
		return false;
		}

	MStringQueue filequeue;
	if(dirtreeops.FindFiles(rootdir.Get(),argpattern,filequeue)==false)
		{
		return false;
		}

	MString filepath;
	for(  ;filequeue.GetQueueLength()>0;filequeue.Dequeue())
		{
		if(fileops.GetCanonicalName(filequeue.GetFront(),filepath)==false)
			{
			continue;
			}

		if(mFileList.IsMember(filepath.Get())==true)
			{
			continue;
			}

		if(mFileList.AddString(filepath.Get())==false)
			{
			continue;
			}
		}

	return true;
	}


//////////////////////////////////////////////////////////
bool MFileSet::ReadReset(void)
	{
	return mFileList.ReadReset();
	}


//////////////////////////////////////////////////////////
bool MFileSet::Read(const char *&dataptr)
	{
	dataptr=mFileList.ReadString();
	if(dataptr==NULL) { return false;  }
	return true;
	}


////////////////////////////////////////////////////////
int MFileSet::GetCount(void)
	{
	return mFileList.GetCount();
	}


