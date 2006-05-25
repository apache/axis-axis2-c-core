#include <axis2_dir_handler.h> 
#include <platforms/axis2_platform_auto_sense.h>
#include <axis2_string.h>
#include <zlib.h>
#include <fcntl.h>
#include <unzip.h>
#include <axis2_crypt.h>

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

axis2_status_t aar_select();

extern int AXIS2_ALPHASORT();
	
AXIS2_DECLARE(axis2_status_t *)
axis2_aar_extract()
{
	struct dirent **namelist;
	int n;
	axis2_char_t *ptr;

	n = scandir(".", &namelist, 0, alphasort);
	if (n < 0)
		return AXIS2_FALSE;
	else 
	{
		while(n--) 
		{
			if ((strcmp(namelist[n]->d_name, ".")== 0) ||
			(strcmp(namelist[n]->d_name, "..") == 0))
				return (AXIS2_FALSE);

			ptr = AXIS2_RINDEX(namelist[n]->d_name, '.');

			if ((ptr != NULL) &&
			(((strcmp(ptr, AXIS2_AAR_SUFFIX) == 0)) || (strcmp(ptr, AXIS2_MAR_SUFFIX) == 0)))
			{
				aar_extract(namelist[n]->d_name);
			}
			free(namelist[n]);
		}
	free(namelist);
	}
	return (AXIS2_TRUE);

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
	zlib_filefunc_def ffunc;
	if (zipfilename!=NULL)
	{
		strncpy(filename_try, zipfilename,MAXFILENAME-1);
		filename_try[ MAXFILENAME ] = '\0';
		
		axis2_fill_win32_filefunc(&ffunc);
		uf = AXIS2_UNZOPEN2(zipfilename,ffunc);
		if (uf==NULL)
		{
			strcat(filename_try,".zip");

			uf = AXIS2_UNZOPEN2(zipfilename,ffunc);
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

