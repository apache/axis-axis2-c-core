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
 
#include <axis2_ws_info_list.h>
#include <string.h>
#include <axis2_class_loader.h>

/** 
 * @brief
 * To do the common tasks for all Builder
 */ 
typedef struct axis2_ws_info_list_impl
{
	axis2_ws_info_list_t ws_info_list;
    
    	
} axis2_ws_info_list_impl_t;

#define AXIS2_INTF_TO_IMPL(ws_info_list) \
    ((axis2_ws_info_list_impl_t *) ws_info_list)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_ws_info_list_free (axis2_ws_info_list_t *ws_info_list, 
                            axis2_env_t **env);



                                
/************************** End of function prototypes ************************/

axis2_ws_info_list_t * AXIS2_CALL 
axis2_ws_info_list_create (axis2_env_t **env)
{
    axis2_ws_info_list_impl_t *ws_info_list_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	ws_info_list_impl = (axis2_ws_info_list_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_ws_info_list_impl_t));
	
	
	if(NULL == ws_info_list_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    ws_info_list_impl->ws_info_list.ops = NULL;
    
	ws_info_list_impl->ws_info_list.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_ws_info_list_ops_t));
	if(NULL == ws_info_list_impl->ws_info_list.ops)
    {
        axis2_ws_info_list_free(&(ws_info_list_impl->ws_info_list), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	ws_info_list_impl->ws_info_list.ops->free =  axis2_ws_info_list_free;
	
	return &(ws_info_list_impl->ws_info_list);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_ws_info_list_free (axis2_ws_info_list_t *ws_info_list, 
                            axis2_env_t **env)
{
    axis2_ws_info_list_impl_t *feature_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(ws_info_list, env, AXIS2_FAILURE);
    
    feature_impl = AXIS2_INTF_TO_IMPL(ws_info_list);
    
	if(NULL != ws_info_list->ops)
        AXIS2_FREE((*env)->allocator, ws_info_list->ops);
    
    AXIS2_FREE((*env)->allocator, feature_impl);
    feature_impl = NULL;
    
	return AXIS2_SUCCESS;
}

/**
 * This is to store all the jar files in a specified folder (WEB_INF)
 */
private static List jarlist = new ArrayList();

/**
 * All the curently updated jars
 */
public List currentjars = new ArrayList();

/**
 * Referance to DeploymentEngine to make update
 */
private DeploymentEngine deployer;

public WSInfoList(DeploymentEngine deploy_engine) {
    deployer = deploy_engine;
}

/**
 * This method is used to initialize the vector
 */
axis2_status_t AXIS2_CALL
axis2_ws_info_list_init(axis2_ws_info_list_t *info_list,
                            axis2_env_t **env) 
{
    jarlist.clear();
}

/**
 * First it check whether the file is already available in the
 * system call isFileExist , if it is not deployed yet then it will add
 * that to jarlist and to the deployment engine as new service or module
 * in adding new item to jarlist it first create optimice and requird object to
 * keep those infor call WSInfo and that will be added to jarist and actual
 * jar file will be added to DeploymentEngine
 * <p/>
 * If it is alredy exsit then it check whether it has been updated
 * then change the last update date of the wsInfo and added two entries to 
 * DeploymentEngine one for New Deployment and other for undeployment
 * @param file actual jar files for either Module or service
 * @param type indicate either Service or Module
 */
axis2_status_t AXIS2_CALL
axis2_ws_info_list_add_ws_info_item(axis2_ws_info_list_t *info_list,
                                    axis2_env_t **env,
                                    File file, 
                                    int type) 
{
    switch (type) {
        case SERVICE:
            {
                if (!isFileExist(file.getName())) { // chacking whether the file is already deployed
                    WSInfo wsInfo = new WSInfo(file.getName(),
                            file.lastModified(),
                            SERVICE);
                    jarlist.add(wsInfo);
                    ArchiveFileData archiveFileData = new ArchiveFileData(file, SERVICE);
                    deployer.addWSToDeploy(archiveFileData);//to inform that new web service is deployed
                } else {
                    if (deployer.isHotUpdate()) {
                        WSInfo tempWSInfo = getFileItem(file.getName());
                        if (isModified(file, tempWSInfo)) {  // caheck whether file is updated
                            tempWSInfo.setLastModifiedDate(file.lastModified());
                            WSInfo wsInfo = new WSInfo(tempWSInfo.getFilename(),
                                    tempWSInfo.getLastmodifieddate(),
                                    SERVICE);
                            deployer.addWSToUndeploy(wsInfo);  // add entry to undeploy list
                            ArchiveFileData archiveFileData = new ArchiveFileData(file, SERVICE);
                            deployer.addWSToDeploy(archiveFileData);   // add entry to deploylist

                        }
                    }
                }
                break;
            }
        case MODULE:
            {
                if (!isFileExist(file.getName())) {  // chacking whether the file is already deployed
                    WSInfo wsInfo = new WSInfo(file.getName(),
                            file.lastModified(),
                            MODULE);
                    jarlist.add(wsInfo);
                    ArchiveFileData archiveFileData = new ArchiveFileData(file, MODULE);
                    deployer.addWSToDeploy(archiveFileData);//to inform that new web service is deployed
                } else {
                    if (deployer.isHotUpdate()) {
                        WSInfo tempWSInfo = getFileItem(file.getName());
                        if (isModified(file, tempWSInfo)) {
                            tempWSInfo.setLastModifiedDate(file.lastModified());
                            WSInfo wsInfo = new WSInfo(tempWSInfo.getFilename(),
                                    tempWSInfo.getLastmodifieddate(),
                                    MODULE);
                            deployer.addWSToUndeploy(wsInfo);   // add entry to undeploy list
                            ArchiveFileData archiveFileData = new ArchiveFileData(file, MODULE);
                            deployer.addWSToDeploy(archiveFileData); // add entry to deploylist

                        }
                    }
                }
                break;
            }
    }
    String jarname = file.getName();
    currentjars.add(jarname);
}

/**
 * This method is to use to check the file exist and if so
 * it will return related wsinfo object to the file else return null;
 *
 * @param filename
 */
axis2_ws_info_t *AXIS2_CALL
axis2_ws_info_list_get_file_item(axis2_ws_info_list_t *info_list,
                                    axis2_env_t **env,
                                    axis2_char_t *file_name) 
{
    int sise = jarlist.size();
    for (int i = 0; i < sise; i++) {
        WSInfo wsInfo = (WSInfo) jarlist.get(i);
        if (wsInfo.getFilename().equals(filename)) {
            return wsInfo;
        }
    }
    return null;
}

/**
 * comapre the last update dates of both files and if those are differ
 * that will assume as the file is been modified
 *
 * @param file
 * @param wsInfo
 */
axis2_bool_t AXIS2_CALL
axis2_ws_info_list_is_modified(axis2_ws_info_list_t *info_list,
                                axis2_env_t **env,
                                File file, 
                                axis2_ws_info_t *ws_info) 
{
    return (wsInfo.getLastmodifieddate() != file.lastModified());
}

/**
 * to check whether the file is alredy in the list
 *
 * @param filename
 */
axis2_bool_t AXIS2_CALL
axis2_ws_info_list_is_file_exist(axis2_ws_info_list_t *info_list,
                                    axis2_env_t **env,
                                    axis2_char_t *file_name) 
{
    return !(getFileItem(filename) == null);
}

/**
 * this is to check , undeploye WS
 * what this relly does is it caheck older jars files and
 * current jars if name of the old jar file does not exit in the currecntjar
 * list then it is assumed that the jar file has been removed
 * that is hot undeployment
 */
axis2_status_t AXIS2_CALL
axis2_ws_info_list_check_for_undeploy(axis2_ws_info_list_t *info_list,
                                        axis2_env_t **env) 
{
    Iterator iter = jarlist.listIterator();
    int size = currentjars.size();
    List tempvector = new ArrayList();
    tempvector.clear();
    String filename;
    boolean exist ;
    while (iter.hasNext()) {
        WSInfo fileitem = (WSInfo) iter.next();
        exist = false;
        for (int i = 0; i < size; i++) {
            filename = (String) currentjars.get(i);
            if (filename.equals(fileitem.getFilename())) {
                exist = true;
                break;
            }
        }

        if (!exist) {
            tempvector.add(fileitem);
            WSInfo wsInfo = new WSInfo(fileitem.getFilename(),
                    fileitem.getLastmodifieddate());
            deployer.addWSToUndeploy(wsInfo);//this is to be undeploye
        }

    }

    for (int i = 0; i < tempvector.size(); i++) {
        WSInfo fileItem = (WSInfo) tempvector.get(i);
        jarlist.remove(fileItem);
    }
    tempvector.clear();
    currentjars.clear();
}


/**
 *
 */
axis2_status_t AXIS2_CALL
axis2_ws_info_list_update(axis2_ws_info_list_t *info_list,
                            axis2_env_t **env) 
{
    checkForUndeploye();
    if (deployer.isHotUpdate()) {
        deployer.unDeploy();
    }
    deployer.doDeploy();

}
