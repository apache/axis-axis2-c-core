/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_dir_handler.h>
#include <axis2_file.h>
#include <platforms/axis2_platform_auto_sense.h>
#include <axis2_string.h>
#include <zlib.h>
#include <fcntl.h>
#include <unzip.h>

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

#ifndef S_ISDIR
#   define S_ISDIR(m) ((m & S_IFMT) == S_IFDIR)
#endif

extern int AXIS2_ALPHASORT();

int dir_select(struct dirent *entry);
int aar_select();

/**
 * List the dll files in the given service or module folder path
 * @param pathname path to your service or module directory
 * @return array list of dll file names
 */
AXIS2_DECLARE(axis2_array_list_t *)
axis2_dir_handler_list_services_or_modules_in_dir(axis2_env_t **env,
                            axis2_char_t *pathname)   
{ 
    axis2_array_list_t *file_list = NULL;
    struct stat *buf = NULL;
	int count = 1;
    int i = 0;
	struct dirent **files = NULL;
	int file_select();
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, NULL);
    file_list = axis2_array_list_create(env, 100);
	count = AXIS2_SCANDIR(pathname, &files, file_select, AXIS2_ALPHASORT);
	/* If no files found, make a non-selectable menu item */
	if (count <= 0)
	{		 
        AXIS2_ARRAY_LIST_FREE(file_list, env);
        file_list = NULL;
        AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "No files in the path %s.", pathname);
		return NULL;
	}
    
	for (i = 1; i < (count + 1) ; ++i )
    {
        axis2_char_t *fname = NULL;
        axis2_file_t *arch_file = NULL;
        axis2_char_t *path = NULL;
        axis2_char_t *temp_path = NULL;
       
        fname = files[i-1]->d_name;
        arch_file = (axis2_file_t *) axis2_file_create(env);
        if(!arch_file)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;
            
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for(j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            /*AXIS2_FREE((*env)->allocator, *files);
            AXIS2_FREE((*env)->allocator, files);*/
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_FILE_SET_NAME(arch_file, env, fname);
        temp_path = AXIS2_STRACAT(pathname, AXIS2_PATH_SEP_STR, env);
        path = AXIS2_STRACAT(temp_path, fname, env);
        AXIS2_FREE((*env)->allocator, temp_path);
        if(!path)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            AXIS2_FILE_FREE(arch_file, env); 
            arch_file = NULL;
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for(j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            /*AXIS2_FREE((*env)->allocator, *files);
            AXIS2_FREE((*env)->allocator, files);*/
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_FILE_SET_PATH(arch_file, env, path);
        buf = AXIS2_MALLOC((*env)->allocator, sizeof(struct stat));
        if(!buf)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            AXIS2_FREE((*env)->allocator, path);
            path = NULL;
            AXIS2_FILE_FREE(arch_file, env); 
            arch_file = NULL;
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for(j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            /*AXIS2_FREE((*env)->allocator, *files);
            AXIS2_FREE((*env)->allocator, files);*/
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        stat(path, buf);
        AXIS2_FILE_SET_TIMESTAMP(arch_file, env, (time_t) buf->st_ctime);
        status = AXIS2_ARRAY_LIST_ADD(file_list, env, arch_file);
        if(AXIS2_SUCCESS != status)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            AXIS2_FILE_FREE(arch_file, env); 
            arch_file = NULL;
            AXIS2_FREE((*env)->allocator, path);
            path = NULL;
            AXIS2_FREE((*env)->allocator, buf);
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for(j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            /*AXIS2_FREE((*env)->allocator, *files);
            AXIS2_FREE((*env)->allocator, files);*/
            return NULL;
        }
        AXIS2_FREE((*env)->allocator, path);
        path = NULL;
        AXIS2_FREE((*env)->allocator, buf);
        buf = NULL;
        arch_file = NULL;
        fname = NULL;
    }
    /*AXIS2_FREE((*env)->allocator, *files);
    AXIS2_FREE((*env)->allocator, files);*/
    return file_list;
}

/**
 * List services or modules directories in the services or modules folder
 * respectively
 * @param pathname path  your modules or services folder
 * @return array list of contents of services or modules folder
 */
AXIS2_DECLARE(axis2_array_list_t *)
axis2_dir_handler_list_service_or_module_dirs(axis2_env_t **env,
                            axis2_char_t *pathname)   
{ 
    axis2_array_list_t *file_list = NULL;
    struct stat *buf = NULL;
    int count = 1;
    int i = 0;
	struct dirent **files = NULL;
	char cwd[500];
	
/**FIXME:
 * This magic number 500 was selected as a temperary solution. It has to be
 * replaced with dinamic memory allocation. This will be done once the use of
 * errno after getwcd() on Windows is figured out.
 */

	/*int dir_select();*/
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_ENV_CHECK(env, NULL);
	/*if ((handle = opendir(pathname)) != NULL && (handle->finished == 1))
	{
		printf("Path Name does not exist:%s\n",pathname);
		return NULL;
	}*/
    file_list = axis2_array_list_create(env, 0);
	if (!getcwd(cwd, 500)) exit(1);
	chdir(pathname);
    aar_select();
	count = AXIS2_SCANDIR(pathname, &files, dir_select, AXIS2_ALPHASORT);
	chdir(cwd);

	/* If no files found, make a non-selectable menu item */
	if (count <= 0)
	{		 
        AXIS2_ARRAY_LIST_FREE(file_list, env);
        AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "No files in the path %s.", pathname);
		return NULL;
	}
    
	for (i = 1; i < (count + 1) ; ++i )
    {
        axis2_char_t *fname = NULL;
        axis2_file_t *arch_file = NULL;
        axis2_char_t *path = NULL;
        axis2_char_t *temp_path = NULL;
       
        fname = files[i-1]->d_name;
        arch_file = (axis2_file_t *) axis2_file_create(env);
        if(!arch_file)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;
            
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for(j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            /*AXIS2_FREE((*env)->allocator, *files);*/
            /*AXIS2_FREE((*env)->allocator, files);*/
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_FILE_SET_NAME(arch_file, env, fname);
        
        temp_path = AXIS2_STRACAT(pathname, AXIS2_PATH_SEP_STR, env);
        path = AXIS2_STRACAT(temp_path, fname, env);
        if(!path)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            AXIS2_FILE_FREE(arch_file, env); 
            arch_file = NULL;
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for(j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            /*AXIS2_FREE((*env)->allocator, *files);*/
            /*AXIS2_FREE((*env)->allocator, files);*/
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_FILE_SET_PATH(arch_file, env, path);
        /* free temp vars */
        AXIS2_FREE((*env)->allocator, temp_path);
        buf = AXIS2_MALLOC((*env)->allocator, sizeof(struct stat));
        if(!buf)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;
            
            AXIS2_FILE_FREE(arch_file, env);
            arch_file = NULL;
            AXIS2_FREE((*env)->allocator, path);
            path = NULL;
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for(j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            /*AXIS2_FREE((*env)->allocator, *files);*/
            /*AXIS2_FREE((*env)->allocator, files);*/
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        stat(path, buf);
        AXIS2_FILE_SET_TIMESTAMP(arch_file, env, (time_t) buf->st_ctime);
        status = AXIS2_ARRAY_LIST_ADD(file_list, env, arch_file);
        if(AXIS2_SUCCESS != status)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;
            
            AXIS2_FILE_FREE(arch_file, env);
            arch_file = NULL;
            AXIS2_FREE((*env)->allocator, path);
            path = NULL;
            AXIS2_FREE((*env)->allocator, buf);
            buf = NULL;
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for(j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            /*AXIS2_FREE((*env)->allocator, *files);*/
            /*AXIS2_FREE((*env)->allocator, files);*/
            return NULL;
            
        }
        AXIS2_FREE((*env)->allocator, path);
        path = NULL;
        AXIS2_FREE((*env)->allocator, buf);
        buf = NULL;
        arch_file = NULL;
        fname = NULL;
    }
#ifndef WIN32
    for(i = 0; i < count; i++)
    {
        free(files[i]);
    }
    free(files);
#endif 
    return file_list;
}


int file_select(struct dirent *entry)
 
{
	axis2_char_t *ptr;
	/*axis2_char_t *rindex(const axis2_char_t *s, int c);*/
 
	if ((strcmp(entry->d_name, ".")== 0) ||
			(strcmp(entry->d_name, "..") == 0))
		return (AXIS2_FALSE);
 
	/* Check for filename extensions */
	ptr = AXIS2_RINDEX(entry->d_name, '.');
	if ((ptr != NULL) &&
		((strcmp(ptr, AXIS2_LIB_SUFFIX) == 0) ))
    {
		return (AXIS2_TRUE);
    }
	else
		return(AXIS2_FALSE);
}

int dir_select(struct dirent *entry)
 
{
	struct stat stat_p;

	if (-1 ==  stat (entry->d_name, &stat_p))
		return (AXIS2_FALSE);

	axis2_char_t *rindex(const axis2_char_t *s, int c);

	if ((entry->d_name[0] == '.') || (!S_ISDIR(stat_p.st_mode)))
    {
		return (AXIS2_FALSE);
	}

	/* Check for filename extensions */
	/*ptr = AXIS2_RINDEX(entry->d_name, '.');
	if (ptr != NULL)
    {
		return (AXIS2_FALSE);
    }*/
	return AXIS2_TRUE;
}

int aar_select()
{
	struct dirent **namelist;
	int n;
	axis2_char_t *ptr;

	n = scandir(".", &namelist, 0, alphasort);
	if (n < 0)
		perror("scandir");
	else 
	{
		while(n--) 
		{
			if ((strcmp(namelist[n]->d_name, ".")== 0) ||
			(strcmp(namelist[n]->d_name, "..") == 0))
				return (AXIS2_FALSE);

			ptr = AXIS2_RINDEX(namelist[n]->d_name, '.');

			if ((ptr != NULL) &&
			((strcmp(ptr, AXIS2_AAR_SUFFIX) == 0) ))
			{
				aar_extract(namelist[n]->d_name);
			}
			free(namelist[n]);
		}
	free(namelist);
	}
}

int aar_extract(axis2_char_t *d_name)
{
	const axis2_char_t *zipfilename=NULL;
	const axis2_char_t *filename_to_extract=NULL;
	const axis2_char_t *password=NULL;
	axis2_char_t filename_try[MAXFILENAME+16] = "";
	int opt_do_extract_withoutpath=0;
	int opt_overwrite=0;
	int opt_extractdir=0;
	const axis2_char_t *dir_name=NULL;
	unzFile uf=NULL;

	if (zipfilename == NULL)

	zipfilename = d_name;
	filename_to_extract = d_name;
	if (zipfilename!=NULL)
	{
		#if defined( WIN32 )
			zlib_filefunc_def ffunc;
		#endif

		strncpy(filename_try, zipfilename,MAXFILENAME-1);
		filename_try[ MAXFILENAME ] = '\0';
		
		#if defined( WIN32 )
			fill_win32_filefunc(&ffunc);
			uf = unzOpen2(zipfilename,&ffunc);
		#else
			uf = unzOpen(zipfilename);
		#endif
		if (uf==NULL)
		{
			strcat(filename_try,".zip");

			#if defined( WIN32 )
				uf = unzOpen2(filename_try,&ffunc);
			#else
				uf = unzOpen(filename_try);
			#endif
		}
	}

	if (uf==NULL)
		return -1;

	if (opt_extractdir && chdir(dir_name))
		exit(-1);

	return axis2_extract(uf,opt_do_extract_withoutpath,opt_overwrite,password);
	unzCloseCurrentFile(uf);
	return 0;
}

int axis2_extract_onefile(uf,filename,opt_extract_without_path,opt_overwrite,password)
	    unzFile uf;
		const axis2_char_t* filename;
		int opt_extract_without_path;
		int opt_overwrite;
		const axis2_char_t* password;
{
	int err = UNZ_OK;
	if (unzLocateFile(uf,filename,CASESENSITIVITY)!=UNZ_OK)
		return -1;

	if (axis2_extract_currentfile(uf,&opt_extract_without_path,
		&opt_overwrite,
		password) == UNZ_OK)
		return 0;
	else
		return -1;
}



int axis2_extract(uf,opt_extract_without_path,opt_overwrite,password)
	    unzFile uf;
		int opt_extract_without_path;
		int opt_overwrite;
		const axis2_char_t* password;
{
	uLong i;
	unz_global_info gi;
	int err;
	FILE* fout=NULL;

	err = unzGetGlobalInfo (uf,&gi);
	if (err!=UNZ_OK)
		return -1;

	for (i=0;i<gi.number_entry;i++)
	{
		if (axis2_extract_currentfile(uf,&opt_extract_without_path,
		&opt_overwrite,
		password) != UNZ_OK)
		break;

		if ((i+1)<gi.number_entry)
		{
			err = unzGoToNextFile(uf);
			if (err!=UNZ_OK)
			{
				return -1;
				break;
			}
		}
	}

	return 0;
}

int axis2_extract_currentfile(uf,popt_extract_without_path,popt_overwrite,password)
	unzFile uf;
	const int* popt_extract_without_path;
	int* popt_overwrite;
	const axis2_char_t* password;
{
	axis2_char_t filename_inzip[256];
	axis2_char_t* filename_withoutpath;
	axis2_char_t* p;
	int err=UNZ_OK;
	FILE *fout=NULL;
	void* buf;
	uInt size_buf;

	unz_file_info file_info;
	err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,sizeof(filename_inzip),NULL,0,NULL,0);

	if (err!=UNZ_OK)
	{
		return err;
	}

	size_buf = WRITEBUFFERSIZE;
	buf = (void*)malloc(size_buf);
	if (buf==NULL)
		return UNZ_INTERNALERROR;

    p = filename_withoutpath = filename_inzip;
	while ((*p) != '\0')
	{
		if (((*p)=='/') || ((*p)=='\\'))
		filename_withoutpath = p+1;
		p++;
	}

	if ((*filename_withoutpath)=='\0')
	{
		if ((*popt_extract_without_path)==0)
		{
			axis2_mkdir(filename_inzip);
		}
	}
	else
	{
		const axis2_char_t* write_filename;
		int skip=0;

		if ((*popt_extract_without_path)==0)
		write_filename = filename_inzip;
		else
		write_filename = filename_withoutpath;

		err = unzOpenCurrentFilePassword(uf,password);

		if ((skip==0) && (err==UNZ_OK))
		{
			fout=fopen(write_filename,"wb");

		if ((fout==NULL) && ((*popt_extract_without_path)==0) &&
		(filename_withoutpath!=(axis2_char_t*)filename_inzip))
		{
			axis2_char_t c=*(filename_withoutpath-1);
			*(filename_withoutpath-1)='\0';
			axis2_create_dir(write_filename);
			*(filename_withoutpath-1)=c;
			fout=fopen(write_filename,"wb");
		}

	}

	if (fout!=NULL)
	{
		do
		{
			err = unzReadCurrentFile(uf,buf,size_buf);
			if (err<0)
			{
				break;
			}
			if (err>0)
			if (fwrite(buf,err,1,fout)!=1)
			{
				err=UNZ_ERRNO;
				break;
			}
		}
		while (err>0);
		if (fout)
		fclose(fout);

	}

	if (err==UNZ_OK)
	{
		err = unzCloseCurrentFile (uf);
	}
	else
		unzCloseCurrentFile(uf);
	}

	free(buf);
	return err;
}


int axis2_create_dir (new_dir)
	axis2_char_t *new_dir;
{
	axis2_char_t *buffer ;
	axis2_char_t *p;
	int  len = (int)strlen(new_dir);

	if (len <= 0)
	return 0;

	buffer = (axis2_char_t*)malloc(len+1);
	strcpy(buffer,new_dir);

	if (buffer[len-1] == '/') 
	{
		buffer[len-1] = '\0';
	}

	if (axis2_mkdir(buffer) == 0)
	{
		free(buffer);
		return 1;
	}

	p = buffer+1;
	while (1)
	{
		axis2_char_t hold;

		while(*p && *p != '\\' && *p != '/')
		p++;
		hold = *p;
		*p = 0;
		if ((axis2_mkdir(buffer) == -1))
		{
			free(buffer);
			return 0;
		}
		if (hold == 0)
		break;
		*p++ = hold;
	}
	free(buffer);
	return 1;
}

int axis2_mkdir(dir_name)
	const axis2_char_t* dir_name;
{

    int value=0;
	#ifdef WIN32
	    value = mkdir(dir_name);
	#else
		    value = mkdir (dir_name,0775);
	#endif
	return value;
}

