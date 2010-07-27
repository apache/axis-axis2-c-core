/*
 *   Copyright 2004-2004 The Apache Software Foundation.
// (c) Copyright IBM Corp. 2004, 2005 All Rights Reserved
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */


#include "platforms/axutil_platform_auto_sense.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <qusec.h>                      // Qus_EC_t
#include <mih/rslvsp.h>                 // rslvsp()
#include <mih/micommon.h>               // _AUTH_EXECUTE
#include <qleawi.h>                     // QleActBndPgm(), QleGetExp()
#include <qp0lstdi.h>                   // Qp0lCvtPathToQSYSObjName()
#include <unistd.h>                     // readlink()
#include <except.h>
#include <errno.h>
#include <qwcrtvca.h>                   // Retrieve job's ccsid API prototype 



/**********************************************************************/
/* Function:                                                          */
/*   retrieveJobCcsid                                                 */
/* Description:                                                       */
/*   Retrieves the ccsid of the current job.                          */
/*   If the current job's ccsid is 65535, the job's default ccsid is  */
/*   returned.                                                        */
/*   If the ccsid cannot be retrieved (error occurs), -1 is returned. */
/* Return:                                                            */
/*   int   status of call.  if 0, success; -1 failure.                */
/**********************************************************************/

#define RTV_CCSID_ATTR_LEN 512

static int retrieveJobCcsid(int *ccsid, char *langID)
{
  char receiverVariable[RTV_CCSID_ATTR_LEN];
  char format[8] = {'R', 'T', 'V', 'C', '0', '1', '0', '0'};
  int numberOfAttributes = 3;
  int attributeKeys[3] = {QWCA_KEY_CCSID, QWCA_KEY_DEFAULTCCSID,  QWCA_KEY_LANGID};
  Qwc_RTVC_Attribute_Data_t *attribute;
  int defaultCcsid;
  char errorCode[8];
  int i;
  memset(errorCode, 0x00, sizeof(errorCode));

#pragma exception_handler(RetrieveJobCcsidError,0,_C1_ALL,_C2_ALL,_CTLA_HANDLE)
  QWCRTVCA(receiverVariable,RTV_CCSID_ATTR_LEN,format,numberOfAttributes,attributeKeys,&errorCode);
#pragma disable_handler        
  if (((Qwc_RTVC0100_t *)receiverVariable)->Number_Fields_Rtnd != 3)
  {
    /* Unable to retrieve the ccsid information */
    return -1;
  }
  /* Retrieved ccsid, default CCSID and language ID */
  attribute = (Qwc_RTVC_Attribute_Data_t *)(receiverVariable + sizeof(int));
  for (i=0; i < 3; i++)
    {
      if (attribute->Key_Field == QWCA_KEY_CCSID)
          *ccsid = *(int *)((char *)attribute + sizeof(Qwc_RTVC_Attribute_Data_t));
      else if (attribute->Key_Field == QWCA_KEY_DEFAULTCCSID)
          defaultCcsid = *(int *)((char *)attribute + sizeof(Qwc_RTVC_Attribute_Data_t));
      else
          strncpy(langID, ((char *)attribute + sizeof(Qwc_RTVC_Attribute_Data_t)), 3);
      attribute = (Qwc_RTVC_Attribute_Data_t *)((char *)attribute + attribute->Length_Field_Info_Rtnd); 
  }
  if (*ccsid == 65535)
      *ccsid = defaultCcsid;

  return 0;

  RetrieveJobCcsidError:
    return -1;
}


typedef int HMODULE;

static char *dlErrorMessage = NULL;

/*
 * ==========================================
 * dlopen()
 * Gain access to an executable object file.
 * ==========================================
 */
void * os400_dlopen(const char *file)
{
    Qus_EC_t err = { sizeof(err),  0};
    int rc;
    char  dllPath[4*1024+1];
    char *fp;
    char pathNameBuffer[8*1024];
    Qlg_Path_Name_T *pathName;
    Qp0l_QSYS_Info_t  qsys_info;
    char objectName[11];
    char objectLibrary[11];
    _SYSPTR sysP;
    _OBJ_TYPE_T objectType;
    HMODULE handle;
    int actInfoLen;
    Qle_ABP_Info_t activationInfo;
    void *returnHandle;

    dlErrorMessage = NULL;

    // Assume symbolic link, if error, assume actual path

    memset(dllPath, 0x00, 4*1024+1);
    rc = readlink ( file  , dllPath, 4*1024);
    if (rc == -1)
       strcpy(dllPath, file);

    // Uppercase file
    fp = dllPath;
    while (*fp++) *fp = toupper(*fp);

    // Parse the path to its QSYS.LIB file system name: library and service program.
    
    pathName = (Qlg_Path_Name_T *)pathNameBuffer;
  
    memset(pathNameBuffer, 0x00, sizeof (pathNameBuffer));
    pathName->Path_Length = strlen(dllPath);
    memcpy( &(((char *) pathName)[sizeof(Qlg_Path_Name_T)]), dllPath, pathName->Path_Length);
    pathName->Path_Name_Delimiter[0] = '/';

    Qp0lCvtPathToQSYSObjName(pathName,&qsys_info,"QSYS0100",sizeof(Qp0l_QSYS_Info_t), 0, &err);

    if (err.Bytes_Available)
    {
        dlErrorMessage = "Path to shared library not valid.";
        return NULL;
    }

    // blank pad object name and library in order to use on rslvsp().

    sprintf(objectName,   "%-10.10s", qsys_info.Obj_Name);
    sprintf(objectLibrary,"%-10.10s", qsys_info.Lib_Name);

#pragma exception_handler (LBL_RSLV_EH, 0,_C1_ALL,_C2_MH_ESCAPE |_C2_MH_FUNCTION_CHECK, _CTLA_HANDLE)

    // Resolve pointer to DLL.
    objectType = WLI_SRVPGM;
    sysP = rslvsp(objectType,objectName, objectLibrary,_AUTH_EXECUTE);

#pragma disable_handler

    // We got a pointer to the DLL.  Activate it (i.e. load it).
    actInfoLen = sizeof(activationInfo);
    QleActBndPgm (&sysP,&handle,&activationInfo,&actInfoLen,&err);
    if (err.Bytes_Available)
    {
        dlErrorMessage = "Unable to activate shared library.";        
        return NULL;
    }

    // Return the dlopen object.
    returnHandle = malloc(sizeof(HMODULE));
    memcpy(returnHandle, &handle, sizeof(HMODULE));
    return returnHandle;

    LBL_RSLV_EH:
      dlErrorMessage = "Unable to resolve to shared library.";
      return NULL;
}

/*
 * dlsym()
 * Obtain the address to symbol from a dlopen() object.
 */
void * os400_dlsym(void *handle, const char * name)
{
    void *symbolAddress = NULL;
    int exportType;

    Qus_EC_t err = {sizeof(err),0 };
    dlErrorMessage = NULL;

#pragma exception_handler (LBL_RSLV_EH, 0,_C1_ALL,_C2_MH_ESCAPE |_C2_MH_FUNCTION_CHECK, _CTLA_HANDLE)

    // Get the function pointer.
    // Export type of 1 means that that the pointer is to a procedure.

    QleGetExp ((int *)handle,0,0,(char *)name,&symbolAddress,&exportType,&err);
    if (err.Bytes_Available)
    {
        dlErrorMessage = "Unable to resolve to procedure in shared library.";
        return NULL;
    }

    return symbolAddress;

#pragma disable_handler

    LBL_RSLV_EH:
      dlErrorMessage = "Unable to resolve to procedure in shared library.";
      return NULL;
}


/*
 * ==========================================
 * dlclose()
 * Close a dlopen() object.
 * ==========================================
 */
int os400_dlclose(void *handle)
{
    dlErrorMessage = NULL;
    *(int *)handle = -1;
    free(handle);
    return 0;
}

/*
 * ==========================================
 * dlclose()
 * Close a dlopen() object.
 * ==========================================
 */
char * os400_dlerror()
{
    char *retError = dlErrorMessage;
    dlErrorMessage = NULL;
    return retError;
}

/* ---------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------*/
/* Below are routines to handle conversion between ascii and ebcdic                 */
/*                                                                                  */
/* The tables below are used to translate single byte data, and should never        */
/* be used to translate data that is not single byte.                               */
/* ---------------------------------------------------------------------------------*/
/* ---------------------------------------------------------------------------------*/


/* constToAsc conversion table generated by ebcdic 37 -> ascii 819 */
const char EBCDICtoASCII[256] = {
/* 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F */
   0,   1,   2,   3, 156,   9, 134, 127, 151, 141, 142,  11,  12,  13,  14,  15,    
  16,  17,  18,  19, 157, 133,   8, 135,  24,  25, 146, 143,  28,  29,  30,  31,    
 128, 129, 130, 131, 132,  10,  23,  27, 136, 137, 138, 139, 140,   5,   6,   7,    
 144, 145,  22, 147, 148, 149, 150,   4, 152, 153, 154, 155,  20,  21, 158,  26,    
  32, 160, 226, 228, 224, 225, 227, 229, 231, 241, 162,  46,  60,  40,  43, 124,    
  38, 233, 234, 235, 232, 237, 238, 239, 236, 223,  33,  36,  42,  41,  59, 172,    
  45,  47, 194, 196, 192, 193, 195, 197, 199, 209, 166,  44,  37,  95,  62,  63,    
  248, 201, 202, 203, 200, 205, 206, 207, 204,  96,  58,  35,  64,  39,  61,  34,   
  216,  97,  98,  99, 100, 101, 102, 103, 104, 105, 171, 187, 240, 253, 254, 177,   
  176, 106, 107, 108, 109, 110, 111, 112, 113, 114, 170, 186, 230, 184, 198, 164,   
  181, 126, 115, 116, 117, 118, 119, 120, 121, 122, 161, 191, 208, 221, 222, 174,   
   94, 163, 165, 183, 169, 167, 182, 188, 189, 190,  91,  93, 175, 168, 180, 215,   
  123,  65,  66,  67,  68,  69,  70,  71,  72,  73, 173, 244, 246, 242, 243, 245,   
  125,  74,  75,  76,  77,  78,  79,  80,  81,  82, 185, 251, 252, 249, 250, 255,   
   92, 247,  83,  84,  85,  86,  87,  88,  89,  90, 178, 212, 214, 210, 211, 213,   
   48,  49,  50,  51,  52,  53,  54,  55,  56,  57, 179, 219, 220, 217, 218, 159
};
 
/* ascToString conversion table generated by ascii 819 -> ebcdic 37 */
const char ASCIItoEBCDIC[256] = {
/* 0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F */
   0,   1,   2,   3,  55,  45,  46,  47,  22,   5,  37,  11,  12,  13,  14,  15,      
  16,  17,  18,  19,  60,  61,  50,  38,  24,  25,  63,  39,  28,  29,  30,  31,
  64,  90, 127, 123,  91, 108,  80, 125,  77,  93,  92,  78, 107,  96,  75,  97,
 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 122,  94,  76, 126, 110, 111,
 124, 193, 194, 195, 196, 197, 198, 199, 200, 201, 209, 210, 211, 212, 213, 214,
 215, 216, 217, 226, 227, 228, 229, 230, 231, 232, 233, 186, 224, 187, 176, 109,      
 121, 129, 130, 131, 132, 133, 134, 135, 136, 137, 145, 146, 147, 148, 149, 150,
 151, 152, 153, 162, 163, 164, 165, 166, 167, 168, 169, 192,  79, 208, 161,   7,
  32,  33,  34,  35,  36,  21,   6,  23,  40,  41,  42,  43,  44,   9,  10,  27,      
  48,  49,  26,  51,  52,  53,  54,   8,  56,  57,  58,  59,   4,  20,  62, 255,      
  65, 170,  74, 177, 159, 178, 106, 181, 189, 180, 154, 138,  95, 202, 175, 188,      
 144, 143, 234, 250, 190, 160, 182, 179, 157, 218, 155, 139, 183, 184, 185, 171,      
 100, 101,  98, 102,  99, 103, 158, 104, 116, 113, 114, 115, 120, 117, 118, 119,      
 172, 105, 237, 238, 235, 239, 236, 191, 128, 253, 254, 251, 252, 173, 174,  89,      
  68,  69,  66,  70,  67,  71, 156,  72,  84,  81,  82,  83,  88,  85,  86,  87,      
 140,  73, 205, 206, 203, 207, 204, 225, 112, 221, 222, 219, 220, 141, 142, 223   
};


/* converts an existing non constant character buffer from Ebcdic ( 37 ) */
/* to Ascii ( 819 ). */
char* strtoasc( char *string )
{
   char* pch = string;
   if( string == NULL ) return NULL;
 
   /* while not EOL... */
   while( *pch != (char)0 )
   {
         *pch = EBCDICtoASCII[*pch];
         pch++;
   }
   return string;
}
char* buftoasc( char *b, int len )
{
   char* pch = b;
   if( b == NULL ) return NULL;
 
   /* while not EOL... */
   while( len > 0 )
   {
         *pch = EBCDICtoASCII[*pch];
         pch++;len--;
   }
   return b;
}

/* converts an existing non-constant character buffer from ASCII ( 819 ) */
/* to EBCDIC ( 37 ).                                                       */
char* asctostr( char *string )
{
   char* pch = string;
   if( string == NULL ) return NULL;
 
   /* while not EOL... */
   while( *pch != (char)0 )
   {
         *pch = ASCIItoEBCDIC[*pch];
         pch++;
   }
   return string;
}
char* asctobuf( char *b, int len )
{
   char* pch = b;
   if( b == NULL ) return NULL;
 
   /* while not EOL... */
   while( len > 0 )
   {
         *pch = ASCIItoEBCDIC[*pch];
         pch++;len--;
   }
   return b;
}


char PLATFORM_DOUBLE_QUOTE_S[]               = "\"";
char PLATFORM_DOUBLE_QUOTE_C                 = '\"';
char PLATFORM_XML_ENTITY_REFERENCE_CHARS_S[] = "<>&\"\'";

static int initializePlatform(void)
{
    char *language= "En_US";
    char langID[3] = {'E' , 'N' , 'U'};
    int jobCCSID = 37;
    
    int rc = retrieveJobCcsid(&jobCCSID, langID);
    if (rc == 0)
    {
    	// double quote character is not invariant when running 
    	// turkish ccsid (1026).  That is, the hexadecimal value
    	// of double quote is different than when running in 
    	// any other language.  So use correct double quote character.
    	if (jobCCSID == 1026)
    	{
    		strcpy(PLATFORM_DOUBLE_QUOTE_S, "\xFC");
    		PLATFORM_DOUBLE_QUOTE_C = '\xFC';
    		strcpy(PLATFORM_XML_ENTITY_REFERENCE_CHARS_S, "<>&\xFC\'");
    	}
    }

	return rc;
}

// TODO-AMRA static int platformRc = initializePlatform();


int
os400_alphasort(
    const struct dirent **__d1,
    const struct dirent **__d2)
{
    return strcoll((*__d1)->d_name, (*__d2)->d_name);
}

int 
os400_scandir(
    const char *_dirname,
    struct dirent **__namelist[],
    int (*selector) (const struct dirent * entry),
    int (*compare) (const struct dirent ** __d1,
                    const struct dirent ** __d2))
{
    DIR *dirp = NULL;
    struct dirent **vector = NULL;
    struct dirent *dp = NULL;
    int vector_size = 0;
    int nfiles = 0;

    if (__namelist == NULL)
    {
        return -1;
    }
    dirp = opendir(_dirname);
    if (!dirp)
    {
        return -1;
    }
    dp = readdir(dirp);
    while (dp)
    {
        int dsize = 0;
        struct dirent *newdp = NULL;

        if (selector && (*selector) (dp) == 0)
        {
            dp = readdir(dirp);
            continue;
        }

        if (nfiles == vector_size)
        {
            struct dirent **newv;
            if (vector_size == 0)
            {
                vector_size = 10;
            }
            else
            {
                vector_size *= 2;
            }

            newv =
                (struct dirent **) realloc(vector,
                                           vector_size *
                                           sizeof(struct dirent *));
            if (!newv)
            {
                return -1;
            }
            vector = newv;
        }

        /*dsize =
            (int) sizeof(struct dirent) +
            (int) ((strlen(dp->d_name) + 1) * sizeof(char));*/
        dsize = (int) sizeof(struct dirent);
        newdp = (struct dirent *) malloc(dsize);

        if (newdp == NULL)
        {
            while (nfiles-- > 0)
            {
                free(vector[nfiles]);
            }
            free(vector);
            return -1;
        }

        vector[nfiles++] = (struct dirent *) memcpy(newdp, dp, dsize);
        dp = readdir(dirp);
    }

    closedir(dirp);

    *__namelist = vector;

    if (compare)
    {
        // TODO-AMRA qsort(*__namelist, nfiles, sizeof(struct dirent *), compare);
    }

    return nfiles;
}
