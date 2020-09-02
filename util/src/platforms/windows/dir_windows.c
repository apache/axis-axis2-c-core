/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <malloc.h>
#include <string.h>
#include <errno.h>

#include <platforms/windows/axutil_dir_windows.h>

/*dirent.h style mehtods for win32*/

AXIS2_DIR *AXIS2_CALL
axis2_opendir(
    const char *_dirname)
{
    AXIS2_DIR *dirp;
    char *filespec;
    HANDLE handle;
    int index;

    filespec = (char*)malloc(strlen(_dirname) + 2 + 1);
    strcpy(filespec, _dirname);
    index = (int)strlen(filespec) - 1;
    if(index >= 0 && (filespec[index] == '/' || (filespec[index] == '\\' && !IsDBCSLeadByte(
        filespec[index - 1]))))
        filespec[index] = '\0';
    strcat(filespec, "/*");

    dirp = (AXIS2_DIR *)malloc(sizeof(AXIS2_DIR));
    dirp->offset = 0;
    dirp->finished = 0;

	handle = FindFirstFile(filespec, &(dirp->fileinfo));
	if (handle == INVALID_HANDLE_VALUE)
	{ 
		DWORD dwErr = GetLastError();
		if (dwErr == ERROR_FILE_NOT_FOUND || dwErr == ERROR_PATH_NOT_FOUND
						|| dwErr == ERROR_NO_MORE_FILES || dwErr == ERROR_NOT_READY)
		{
            errno = dwErr;
			dirp->finished = 1;
		}
		else
		{
			free(dirp);
			free(filespec);
            errno = dwErr;
			return NULL;
		}
	}
		
    /* We are using the ISO C++ conformant name: _strdup, as demanded by VS 2005 */
    dirp->dirname = _strdup(_dirname);
    dirp->handle = handle;
    free(filespec);

    return dirp;
}

int AXIS2_CALL
axis2_closedir(
    AXIS2_DIR * _dirp)
{
    int iret = -1;
    if(!_dirp)
        return iret;
    iret = FindClose(_dirp->handle);
    if(_dirp->dirname)
        free(_dirp->dirname);
    if(_dirp)
        free(_dirp);

    return iret;
}

struct dirent *AXIS2_CALL
axis2_readdir(
    AXIS2_DIR * _dirp)
{
    if(!_dirp || _dirp->finished)
        return NULL;

    if(_dirp->offset != 0)
    {
        if(FindNextFile(_dirp->handle, &(_dirp->fileinfo)) == 0)
        {
            _dirp->finished = 1;
            return NULL;
        }
    }
    _dirp->offset++;

    strcpy(_dirp->dent.d_name, _dirp->fileinfo.cFileName); /*, _MAX_FNAME+1); */
    _dirp->dent.d_ino = 1;
    _dirp->dent.d_reclen = (unsigned short)strlen(_dirp->dent.d_name);
    _dirp->dent.d_off = _dirp->offset;

    return &(_dirp->dent);
}

int AXIS2_CALL
axis2_readdir_r(
    AXIS2_DIR * _dirp,
    struct dirent *_entry,
    struct dirent **__result)
{
    if(!_dirp || _dirp->finished)
    {
        *__result = NULL;
        return -1;
    }

    if(_dirp->offset != 0)
    {
        if(FindNextFile(_dirp->handle, &(_dirp->fileinfo)) < 0)
        {
            _dirp->finished = 1;
            *__result = NULL;
            return -1;
        }
    }
    _dirp->offset++;

    strcpy(_dirp->dent.d_name, _dirp->fileinfo.cFileName); /*, _MAX_FNAME+1); */
    _dirp->dent.d_ino = 1;
    _dirp->dent.d_reclen = (unsigned short)strlen(_dirp->dent.d_name);
    _dirp->dent.d_off = _dirp->offset;

    memcpy(_entry, &_dirp->dent, sizeof(*_entry));

    *__result = &_dirp->dent;

    return 0;
}

int AXIS2_CALL
axis2_rewinddir(
    AXIS2_DIR * dirp)
{
    char *filespec;
    HANDLE handle;
    int index;

    FindClose(dirp->handle);

    dirp->offset = 0;
    dirp->finished = 0;

    filespec = malloc(strlen(dirp->dirname) + 2 + 1);
    strcpy(filespec, dirp->dirname);
    index = (int)(strlen(filespec) - 1);
    if(index >= 0 && (filespec[index] == '/' || filespec[index] == '\\'))
        filespec[index] = '\0';
    strcat(filespec, "/*");

	handle = FindFirstFile(filespec, &(dirp->fileinfo));
    if(handle == INVALID_HANDLE_VALUE)
    {
		DWORD dwErr = GetLastError();
		if (dwErr == ERROR_FILE_NOT_FOUND || dwErr == ERROR_PATH_NOT_FOUND
						|| dwErr == ERROR_NO_MORE_FILES || dwErr == ERROR_NOT_READY)
		{
			dirp->finished = 1;
		}
    }

    dirp->handle = handle;
    free(filespec);

    return 0;
}

int
alphasort(
    const struct dirent **__d1,
    const struct dirent **__d2)
{
    return strcoll((*__d1)->d_name, (*__d2)->d_name);
}

int AXIS2_CALL
axis2_scandir(
    const char *_dirname,
    struct dirent **__namelist[],
    int
    (*selector)(
        const struct dirent * entry),
    int
    (*compare)(
        const struct dirent ** __d1,
        const struct dirent ** __d2))
{
    AXIS2_DIR *dirp = NULL;
    struct dirent **vector = NULL;
    struct dirent *dp = NULL;
    int vector_size = 0;
    int nfiles = 0;

    if(__namelist == NULL)
    {
        return -1;
    }
    dirp = axis2_opendir(_dirname);
    if(!dirp)
    {
        return -1;
    }
    dp = axis2_readdir(dirp);
    while(dp)
    {
        int dsize = 0;
        struct dirent *newdp = NULL;

        if(selector && (*selector)(dp) == 0)
        {
            dp = axis2_readdir(dirp);
            continue;
        }

        if(nfiles == vector_size)
        {
            struct dirent **newv;
            if(vector_size == 0)
            {
                vector_size = 10;
            }
            else
            {
                vector_size *= 2;
            }

            newv = (struct dirent **)realloc(vector, vector_size * sizeof(struct dirent *));
            if(!newv)
            {
                return -1;
            }
            vector = newv;
        }

        /*dsize =
         (int) sizeof(struct dirent) +
         (int) ((strlen(dp->d_name) + 1) * sizeof(char));*/
        dsize = (int)sizeof(struct dirent);
        newdp = (struct dirent *)malloc(dsize);

        if(newdp == NULL)
        {
            while(nfiles-- > 0)
            {
                free(vector[nfiles]);
            }
            free(vector);
            return -1;
        }

        vector[nfiles++] = (struct dirent *)memcpy(newdp, dp, dsize);
        dp = axis2_readdir(dirp);
    }

    axis2_closedir(dirp);

    *__namelist = vector;

    if(compare)
    {
        qsort(*__namelist, nfiles, sizeof(struct dirent *), compare);
    }

    return nfiles;
}
