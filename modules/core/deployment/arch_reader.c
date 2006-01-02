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
 
#include <axis2_arch_reader.h>
#include <string.h>
#include <axis2_class_loader.h>

/** 
 * @brief
 * To do the common tasks for all Builder
 */ 
typedef struct axis2_arch_reader_impl
{
	axis2_arch_reader_t arch_reader;
    
    	
} axis2_arch_reader_impl_t;

#define AXIS2_INTF_TO_IMPL(arch_reader) \
    ((axis2_arch_reader_impl_t *) arch_reader)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_arch_reader_free (axis2_arch_reader_t *arch_reader, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_arch_reader_build_svc_grp(axis2_arch_reader_t *arch_reader,
                                axis2_env_t **env,
                                axis2_char_t *file_path,
                                struct axis2_dep_engine *dep_engine,
                                struct axis2_svc_grp *svc_grp);



                                
/************************** End of function prototypes ************************/

axis2_arch_reader_t * AXIS2_CALL 
axis2_arch_reader_create_with_input_stream_and_dep_engine (axis2_env_t **env)
{
    axis2_arch_reader_impl_t *arch_reader_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	arch_reader_impl = (axis2_arch_reader_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_arch_reader_impl_t));
	
	
	if(NULL == arch_reader_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    arch_reader_impl->arch_reader.ops = NULL;
    
	arch_reader_impl->arch_reader.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_arch_reader_ops_t));
	if(NULL == arch_reader_impl->arch_reader.ops)
    {
        axis2_arch_reader_free(&(arch_reader_impl->arch_reader), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	arch_reader_impl->arch_reader.ops->free =  axis2_arch_reader_free;
	
	return &(arch_reader_impl->arch_reader);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_arch_reader_free (axis2_arch_reader_t *arch_reader, 
                            axis2_env_t **env)
{
    axis2_arch_reader_impl_t *feature_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(arch_reader, env, AXIS2_FAILURE);
    
    feature_impl = AXIS2_INTF_TO_IMPL(arch_reader);
    
	if(NULL != arch_reader->ops)
        AXIS2_FREE((*env)->allocator, arch_reader->ops);
    
    AXIS2_FREE((*env)->allocator, feature_impl);
    feature_impl = NULL;
    
	return AXIS2_SUCCESS;
}

/**
 * To create a ServiceDescrption <code>AxisService</code>   using given wsdl.
 * If the service.wsdl is there in the arcive file AxisService will be creted 
 * using that, else default AxisService will be created
 * @param file
 * @return
 */
struct axis2_svc *AXIS2_CALL
axis2_arch_reader_create_svc(axis2_arch_reader_t *arch_reader,
                                axis2_env_t **env,
                                struct axis2_arch_file_data *file) 
{
    struct axis2_svc *svc = NULL;
    
    /* TODO comment this until WOM implementation is done */
    /*InputStream in = file.getClassLoader().getResourceAsStream(
        AXIS2_SVC_WSDL_WITH_FOLDER);
    if (in != null) 
    {
        WOMBuilder builder = WOMBuilderFactory.getBuilder(WSDLConstants.WSDL_1_1);
        WSDLVersionWrapper wsdlVersionWrapper = builder.build(in,
                new AxisDescWSDLComponentFactory());
        WSDLDescription womDescription = wsdlVersionWrapper.getDescription();
        Iterator iterator = womDescription.getServices().keySet()
                .iterator();
        if (iterator.hasNext()) {
            foundservice = true;
            // remove <wsdl:service> and <wsdl:binding> elements from the service
            // description we read in as we will be replacing them anyway.

            WSDLServiceImpl serviceimpl = (WSDLServiceImpl) womDescription.
                getServices().get(iterator.next());
            service = new AxisService(serviceimpl);
        }
        if (!foundservice) {
            service = new AxisService();
        }
        service.setWSDLDefinition(wsdlVersionWrapper.getDefinition());
        in.close();
    } else {
    */
    svc = axis2_svc_create(env);
    /*TODO log */
    /*log.info(Messages.getMessage(DeploymentErrorMsgs.WSDL_FILE_NOT_FOUND,
            file.getName()));
    */

    return svc;
}

/* TODO comment this until WOM implementation is done */
/*
private void processWSDLFile(InputStream in , DeploymentEngine depengine)  
{
    try {
        WOMBuilder builder = WOMBuilderFactory.getBuilder(WSDLConstants.WSDL_1_1);
        WSDLVersionWrapper wsdlVersionWrapper = builder.build(in,
                new AxisDescWSDLComponentFactory());
        WSDLDescription womDescription = wsdlVersionWrapper.getDescription();

        //removing binding
        Map bindings = wsdlVersionWrapper.getDefinition().getBindings();
        Iterator binfingIterator = bindings.keySet().iterator();
        while (binfingIterator.hasNext()) {
            Object o = binfingIterator.next();
            bindings.remove(o) ;

        }

        Iterator iterator = womDescription.getServices().keySet()
                .iterator();
        if (iterator.hasNext()) {
            // remove <wsdl:service> and <wsdl:binding> elements from the service
            // description we read in as we will be replacing them anyway.
            WSDLServiceImpl serviceimpl = (WSDLServiceImpl)
                    womDescription.getServices().get(iterator.next());
            AxisService service = new AxisService(serviceimpl);
            service.setName(serviceimpl.getName());
            service.setWSDLDefinition(wsdlVersionWrapper.getDefinition());
            depengine.getCurrentFileItem().addService(service);
        }
    } catch (WSDLException e) {
        throw new DeploymentException(e);
    }
}
*/

/**
 * To create service objects out form wsdls file inside a service archive file
 * @param file <code>ArchiveFileData</code>
 * @param depengine <code>DeploymentEngine</code>
 * @throws DeploymentException  <code>DeploymentException</code>
 */
 /* TODO comment this until WOM implementation is done */
/*
public void processWSDLs(ArchiveFileData file , DeploymentEngine depengine) throws DeploymentException {
    File serviceFile = file.getFile();
    boolean isDirectory = serviceFile.isDirectory();
    if(isDirectory){
        try {
            File meta_inf = new File(serviceFile,META_INF);
            if(meta_inf.exists()){
                File files [] = meta_inf.listFiles();
                for (int i = 0; i < files.length; i++) {
                    File file1 = files[i];
                    String fileName = file1.getName();
                    if(fileName.endsWith(".wsdl") || fileName.endsWith(".WSDL")){
                        InputStream in = new FileInputStream(file1);
                        processWSDLFile(in,depengine);
                        try {
                            in.close();
                        } catch (IOException e) {
                            log.info(e);
                        }
                    }
                }
            } else {
                throw new DeploymentException(Messages.getMessage(
                        DeploymentErrorMsgs.INVALID_SERVICE));
            }
        } catch (FileNotFoundException e) {
            throw new DeploymentException(e);
        } catch (IOException e) {
            throw new DeploymentException(e);
        }
    }   else {

        ZipInputStream zin;
        try {
            zin = new ZipInputStream(new FileInputStream(serviceFile));
            ZipEntry entry;
            byte[] buf = new byte[1024];
            int read;
            ByteArrayOutputStream out ;
            while ((entry = zin.getNextEntry()) != null) {
                String entryName = entry.getName();
                if ((entryName.startsWith(META_INF) ||
                        entryName.startsWith(META_INF.toLowerCase()))
                        && (entryName.endsWith(".wsdl")
                        || entryName.endsWith(".WSDL"))) {
                    out = new ByteArrayOutputStream();
                    while ((read = zin.read(buf)) > 0) {
                        out.write(buf, 0, read);
                    }
                    ByteArrayInputStream in = new ByteArrayInputStream(out.toByteArray());
                    processWSDLFile(in,depengine);
                }
            }
            try {
                zin.close();
            } catch (IOException e) {
                log.info(e);
            }
        } catch (FileNotFoundException e) {
            throw new DeploymentException(e);
        } catch (IOException e) {
            throw new DeploymentException(e);
        }
    }

}
*/
/**
 * it take two arguments filename and refereance to DeployEngine
 *
 * @param filename
 * @param engine
 */
axis2_status_t AXIS2_CALL
axis2_arch_reader_process_svc_grp(axis2_arch_reader_t *arch_reader,
                                    axis2_env_t **env,
                                    axis2_char_t *file_path,
                                    struct axis2_dep_engine *dep_engine,
                                    axis2_svc_grp_t *svc_grp)
{
    axis2_char_t *file_name = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(arch_reader, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, file_path, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, dep_engine, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);
    
    file_name = AXIS2_STRACAT(file_path, AXIS2_SVC_XML, env);
    if(!file_name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    status = axis2_file_handler_access(file_name, AXIS2_F_OK);
    if(AXIS2_SUCCESS == status)
    {
        struct axis2_arch_file_data *arch_file_data = NULL;
        axis2_char_t *svc_name = NULL;    
        
        status = axis2_arch_reader_build_svc_grp(arch_reader, env, file_name, 
            dep_engine, svc_grp);
        if(AXIS2_FAILURE == status)
        {
            return AXIS2_FAILURE;
        }
        arch_file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(dep_engine, env);
        svc_name = AXIS2_ARCH_FILE_DATA_GET_SVC_NAME(arch_file_data, env);
        AXIS2_SVC_GRP_SET_NAME(svc_grp, env, svc_name);
    } else 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SERVICE_XML_NOT_FOUND, 
            AXIS2_FAILURE);
        status = AXIS2_FAILURE;
    }
    return status;
}

axis2_status_t AXIS2_CALL
axis2_arch_reader_build_svc_grp(axis2_arch_reader_t *arch_reader,
                                axis2_env_t **env,
                                axis2_char_t *file_path,
                                struct axis2_dep_engine *dep_engine,
                                struct axis2_svc_grp *svc_grp)
{                       
    struct axis2_desc_builder *desc_builder = NULL;
    axis2_char_t *root_element_name = NULL;
    axis2_om_node_t *svcs = NULL;
    axis2_om_element_t *svcs_element = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(arch_reader, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, file_path, AXIS2_FAILURE);    
    AXIS2_PARAM_CHECK((*env)->error, dep_engine, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);
    
    desc_builder = axis2_desc_builder_create_with_file_and_dep_engine(env, 
        file_path, dep_engine);
    if(!desc_builder)
    {
        return AXIS2_FAILURE;
    }
    svcs = AXIS2_DESC_BUILDER_BUILD_OM(desc_builder, env);
    svcs_element = AXIS2_OM_NODE_GET_DATA_ELEMENT(svcs, env);
    root_element_name = AXIS2_OM_ELEMENT_GET_LOCALNAME(svcs_element, env);
    if(0 == AXIS2_STRCMP(AXIS2_SVC_ELEMENT, root_element_name))
    {
        axis2_svc_t *svc = NULL;
        struct axis2_arch_file_data *file_data = NULL;
        axis2_char_t *name = NULL;
        axis2_char_t *short_file_name = NULL;
        
        file_data = AXIS2_DEP_ENGINE_GET_CURRENT_FILE_ITEM(dep_engine, env);
        name = AXIS2_ARCH_FILE_DATA_GET_NAME(file_data, env);
        short_file_name = AXIS2_DESC_BUILDER_GET_SHORT_FILE_NAME(desc_builder, 
            env, name);
        svc = AXIS2_ARCH_FILE_DATA_GET_SVC(file_data, env, short_file_name);
        if(NULL == svc)
        {
            axis2_qname_t *svc_qname = NULL;
            
            svc_qname = axis2_qname_create(env, short_file_name, NULL, NULL);
            svc = axis2_svc_create_with_qname(env, svc_qname);
            status = AXIS2_ARCH_FILE_DATA_ADD_SVC(file_data, env, svc);
            if(AXIS2_FAILURE == status)
            {
                AXIS2_QNAME_FREE(svc_qname, env);
                svc_qname = NULL;
                return AXIS2_FAILURE;
            }
        }
        AXIS2_SVC_SET_PARENT(svc, env, svc_grp);
        /*axisService.setClassLoader(engine.getCurrentFileItem().getClassLoader());
        ServiceBuilder serviceBuilder = new ServiceBuilder(engine,axisService);
        serviceBuilder.populateService(services);
        engine.getCurrentFileItem().getDeploybleServices().add(axisService);
        */
    }
    else if(0 == AXIS2_STRCMP(AXIS2_SVC_GRP_ELEMENT, root_element_name))
    {
        /*ServiceGroupBuilder groupBuilder = new ServiceGroupBuilder(services,engine);
        groupBuilder.populateServiceGroup(axisServiceGroup);*/
    }
    return status;
}

axis2_status_t AXIS2_CALL
axis2_arch_reader_read_module_arch(axis2_arch_reader_t *arch_reader,
                                    axis2_env_t **env,
                                    axis2_char_t *file_path,
                                    struct axis2_dep_engine *dep_engine,
                                    axis2_module_desc_t *module)
{
    axis2_char_t *file_name = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(arch_reader, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, file_path, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, dep_engine, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module, AXIS2_FAILURE);
    
    file_name = AXIS2_STRACAT(file_path, AXIS2_MODULE_XML, env);
    if(!file_name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    status = axis2_file_handler_access(file_name, AXIS2_F_OK);
    if(AXIS2_SUCCESS == status)
    {
        /*
            ModuleBuilder builder = new ModuleBuilder(in, engine, module);
            builder.populateModule();
        */
    }
    else
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_MODULE_XML_NOT_FOUND_FOR_THE_MODULE, AXIS2_FAILURE);
        status = AXIS2_FAILURE;
    }
    return status;
}
