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
    axis2_bool_t found_svc = AXIS2_FALSE;
    
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
                                    axis2_char_t *file_path
                                    struct axis2_dep_engine *engine,
                                    struct axis2_svc_grp *svc_grp)
{
    axis2_char_t *file_name = NULL;
    int len = 0;
    file_name = AXIS2_STRACAT(file_path, AXIS2_SVC_XML, env);
    if(NULL != file_name)
    {
        InputStream in ;
            in = new FileInputStream(file);
            buildServiceGroup(in,engine,axisServiceGroup);
            axisServiceGroup.setServiceGroupName(engine.getCurrentFileItem().getName());
        
        }
    } else 
    {
        AXIS2_ERROR_SET((*env)->error, 
        throw new DeploymentException(
                Messages.getMessage(DeploymentErrorMsgs.SERVICE_XML_NOT_FOUND));
    }
}

private void buildServiceGroup(InputStream zin, DeploymentEngine engine,
                               AxisServiceGroup axisServiceGroup)
        throws XMLStreamException, DeploymentException {
    DescriptionBuilder builder;
    String rootelementName;
    builder = new DescriptionBuilder(zin, engine);
    OMElement services = builder.buildOM();
    rootelementName = services.getLocalName();
    if(SERVICE_ELEMENT.equals(rootelementName)){
        AxisService axisService = engine.getCurrentFileItem().
                getService(DescriptionBuilder.getShortFileName(
                        engine.getCurrentFileItem().getName()));
        if(axisService == null){
            axisService = new AxisService(
                    new QName(DescriptionBuilder.getShortFileName(
                            engine.getCurrentFileItem().getName())));
            engine.getCurrentFileItem().addService(axisService);
        }
        axisService.setParent(axisServiceGroup);
        axisService.setClassLoader(engine.getCurrentFileItem().getClassLoader());
        ServiceBuilder serviceBuilder = new ServiceBuilder(engine,axisService);
        serviceBuilder.populateService(services);
        engine.getCurrentFileItem().getDeploybleServices().add(axisService);
    } else if(SERVICE_GROUP_ELEMENT.equals(rootelementName)){
        ServiceGroupBuilder groupBuilder = new ServiceGroupBuilder(services,engine);
        groupBuilder.populateServiceGroup(axisServiceGroup);
    }
}

public void readModuleArchive(String filename,
                              DeploymentEngine engine,
                              ModuleDescription module , boolean explodedDir)
        throws DeploymentException {
    // get attribute values
    boolean foundmoduleXML = false;
    if (!explodedDir) {
        ZipInputStream zin;
        try {
            zin = new ZipInputStream(new FileInputStream(filename));
            ZipEntry entry;
            while ((entry = zin.getNextEntry()) != null) {
                if (entry.getName().equals(MODULEXML)) {
                    foundmoduleXML = true;
                    ModuleBuilder builder = new ModuleBuilder(zin, engine, module);
                    builder.populateModule();
                    break;
                }
            }
            zin.close();
            if (!foundmoduleXML) {
                throw new DeploymentException(Messages.getMessage(
                        DeploymentErrorMsgs.MODULEXML_NOT_FOUND_FOR_THE_MODULE, filename));
            }
        } catch (Exception e) {
            throw new DeploymentException(e);
        }
    } else {
        File file = new File(filename, MODULEXML);
        if (file.exists()) {
            InputStream in;
            try {
                in = new FileInputStream(file);
                ModuleBuilder builder = new ModuleBuilder(in, engine, module);
                builder.populateModule();
            } catch (FileNotFoundException e) {
                throw new DeploymentException(Messages.getMessage(DeploymentErrorMsgs.FNF_WITH_E
                        ,e.getMessage()));
            }
        } else {
            throw new DeploymentException(Messages.getMessage(
                    DeploymentErrorMsgs.MODULEXML_NOT_FOUND_FOR_THE_MODULE, filename));
        }

    }
}


/**
 * This method first check whether the given module is there in the user home dirctory if so return
 * that , else try to read the given module form classpath (from resources ) if found first get the module.mar
 * file from the resourceStream and write that into user home/axis2home/nodule directory
 *
 * @param moduleName
 * @return
 * @throws DeploymentException
 */
public File creatModuleArchivefromResource(String moduleName,
                                           String axis2repository) throws DeploymentException {
    File modulearchiveFile;
    File modules;
    try {
        int BUFFER = 2048;
        if (axis2repository == null) {
            String userHome = System.getProperty("user.home");
            File userHomedir = new File(userHome);
            File repository = new File(userHomedir, ".axis2home");
            modules = new File(repository, "modules");
        } else {
            modules = new File(axis2repository, "modules");
        }
        String modulearchiveName = moduleName + ".mar";
        modulearchiveFile = new File(modules, modulearchiveName);
        if (modulearchiveFile.exists()) {
            return modulearchiveFile;
        }
        ClassLoader cl = Thread.currentThread().getContextClassLoader();
        InputStream in = cl.getResourceAsStream("modules/" + moduleName + ".mar");
        if (in == null) {
            in = cl.getResourceAsStream("modules/" + moduleName + ".jar");
        }
        if (in == null) {
            throw new DeploymentException(Messages.getMessage(
                        DeploymentErrorMsgs.MODULEXML_NOT_FOUND_FOR_THE_MODULE, moduleName));
        } else {
            if(!modules.exists()) {
                modules.mkdirs();
            }
            modulearchiveFile.createNewFile();
            FileOutputStream dest = new
                    FileOutputStream(modulearchiveFile);
            ZipOutputStream out = new ZipOutputStream(new
                    BufferedOutputStream(dest));
            byte data[] = new byte[BUFFER];
            ZipInputStream zin;
            zin = new ZipInputStream(in);
            ZipEntry entry;
            while ((entry = zin.getNextEntry()) != null) {
                ZipEntry zip = new ZipEntry(entry);
                out.putNextEntry(zip);
                int count;
                while ((count = zin.read(data, 0, BUFFER)) != -1) {
                    out.write(data, 0, count);
                }
            }
            out.close();
            zin.close();
        }

    } catch (Exception e) {
        throw new DeploymentException(e);
    }
    return modulearchiveFile;
}
