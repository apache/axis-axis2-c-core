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

#include <axis2_utils.h>
#include <stdlib.h>
#include <axis2_string.h>
#include <string.h>
#include <axis2_conf.h>
#include <axis2_relates_to.h>
#include <axis2_addr.h>
#include <axis2_http_transport.h>

AXIS2_DECLARE(axis2_char_t**)
axis2_parse_request_url_for_svc_and_op(axis2_env_t **env, 
                                                axis2_char_t *request)

{
    axis2_char_t **ret = NULL;
    axis2_char_t *service_str = NULL;
    axis2_char_t *tmp = NULL;
    int i = 0;
    AXIS2_FUNC_PARAM_CHECK(request, env, NULL);
    ret  = AXIS2_MALLOC((*env)->allocator, 
                                        2*(sizeof(axis2_char_t *)));
    memset(ret, 0, 2*sizeof(axis2_char_t*)); 
    tmp = request;

    
    while(1)
    {
        tmp = strstr(tmp, AXIS2_REQUEST_URL_PREFIX);
        if(NULL == tmp)
            break;
        else
        {
            service_str = tmp;
            tmp += AXIS2_STRLEN(AXIS2_REQUEST_URL_PREFIX);
        }
    }
    if(NULL != service_str)
    {
        service_str += AXIS2_STRLEN(AXIS2_REQUEST_URL_PREFIX);
        if('\0' != *service_str)
        {
            service_str++; /*to remove the leading '/' */
            tmp = strchr(service_str, '/');
            if(NULL != tmp)
            {
                i = tmp - service_str;
                ret[0] = AXIS2_MALLOC((*env)->allocator, i*sizeof(char)+1);
                strncpy(ret[0], service_str,i);
                ret[0][i] = '\0';

                /* Now search for the op */
                service_str = tmp;
                if('\0' != *service_str)
                {
                    service_str++;
                    tmp = strchr(service_str, '?');
                    if(NULL != tmp)
                    {
                        i = tmp - service_str;
                        ret[1] = AXIS2_MALLOC((*env)->allocator, 
                                                i*sizeof(char)+1);
                        strncpy(ret[1], service_str,i);
                        ret[1][i] = '\0';
                    }
                    else
                    {
                        ret[1] = AXIS2_STRDUP(service_str, env);
                    }
                }
            }
            else
            {
                ret[0] = AXIS2_STRDUP(service_str, env);
            }
        }
    }
    return ret;
}

/*
public static void addHandler(Flow flow,
                              Handler handler,
                              String phaseName) {
    HandlerDescription handlerDesc = new HandlerDescription();
    PhaseRule rule = new PhaseRule(phaseName);
    handlerDesc.setRules(rule);
    handler.init(handlerDesc);
    handlerDesc.setHandler(handler);
    flow.addHandler(handlerDesc);
}
*/

axis2_msg_ctx_t *AXIS2_CALL
axis2_utils_create_out_msg_ctx(axis2_env_t **env,
                                axis2_msg_ctx_t *in_msg_ctx)
{
    axis2_msg_ctx_t *new_msg_ctx = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_transport_in_desc_t *transport_in = NULL;
    axis2_transport_out_desc_t *transport_out = NULL;
    axis2_msg_info_headers_t *old_msg_info_headers = NULL;
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_endpoint_ref_t *reply_to = NULL;
    axis2_endpoint_ref_t *fault_to = NULL;
    axis2_endpoint_ref_t *to = NULL;
    axis2_char_t *msg_id = NULL;
    axis2_relates_to_t *relates_to = NULL;
    axis2_char_t *action = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_svc_ctx_t *svc_ctx = NULL;
    void *trt_property = NULL;
    void *trt_out_info_property = NULL;
    void *char_set_encoding_property = NULL;
    axis2_bool_t doing_rest = AXIS2_FALSE;
    axis2_bool_t doing_mtom = AXIS2_FALSE;
    axis2_bool_t server_side = AXIS2_FALSE;
    axis2_svc_grp_ctx_t *svc_grp_ctx = NULL;
    
    AXIS2_PARAM_CHECK((*env)->error, in_msg_ctx, NULL);
    
    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(in_msg_ctx, env);
    transport_in = AXIS2_MSG_CTX_GET_TRANSPORT_IN_DESC(in_msg_ctx, env);
    transport_out = AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(in_msg_ctx, env);
    
    new_msg_ctx = axis2_msg_ctx_create(env, conf_ctx, transport_in, transport_out);
    if(!new_msg_ctx)
    {
        return NULL;
    }
    old_msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(in_msg_ctx, env);
    if(!old_msg_info_headers)
    {
        return NULL;
    }
    msg_info_headers = axis2_msg_info_headers_create(env, NULL, NULL);
    if(!msg_info_headers)
    {
        return NULL;
    }
    /* TODO UUID is hard coded until it is generated */
    AXIS2_MSG_INFO_HEADERS_SET_MESSAGE_ID(msg_info_headers, env, "UUID");
    /*messageInformationHeaders.setMessageId(UUIDGenerator.getUUID());*/
    reply_to = AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, env, reply_to);
    
    fault_to = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(msg_info_headers, env, fault_to);
    
    to = AXIS2_MSG_INFO_HEADERS_GET_TO(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_FROM(msg_info_headers, env, to);
    
    msg_id = AXIS2_MSG_INFO_HEADERS_GET_MESSAGE_ID(old_msg_info_headers, env);
    relates_to = axis2_relates_to_create(env, msg_id, 
        AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE);
    AXIS2_MSG_INFO_HEADERS_SET_RELATES_TO(msg_info_headers, env, relates_to);
    
    action = AXIS2_MSG_INFO_HEADERS_GET_ACTION(old_msg_info_headers, env);
    AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_info_headers, env, action);
    AXIS2_MSG_CTX_SET_MSG_INFO_HEADERS(new_msg_ctx, env, msg_info_headers);
    
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_OP_CTX(new_msg_ctx, env, op_ctx);
    
    svc_ctx = AXIS2_MSG_CTX_GET_SVC_CTX(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_SVC_CTX(new_msg_ctx, env, svc_ctx);
    
    trt_property = AXIS2_MSG_CTX_GET_PROPERTY(in_msg_ctx, env, 
            AXIS2_TRANSPORT_OUT, AXIS2_FALSE);
            
    AXIS2_MSG_CTX_SET_PROPERTY(new_msg_ctx, env, AXIS2_TRANSPORT_OUT, trt_property,
        AXIS2_FALSE);
    
    trt_out_info_property = AXIS2_MSG_CTX_GET_PROPERTY(in_msg_ctx, env, 
            AXIS2_HTTP_OUT_TRANSPORT_INFO, AXIS2_FALSE);
            
    AXIS2_MSG_CTX_SET_PROPERTY(new_msg_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO, 
            trt_out_info_property, AXIS2_FALSE);

    /* Setting the charater set encoding */
    
    char_set_encoding_property = AXIS2_MSG_CTX_GET_PROPERTY(in_msg_ctx, env, 
            AXIS2_CHARACTER_SET_ENCODING, AXIS2_FALSE);         
    AXIS2_MSG_CTX_SET_PROPERTY(new_msg_ctx, env, AXIS2_CHARACTER_SET_ENCODING, 
            char_set_encoding_property, AXIS2_FALSE);
            
    doing_rest = AXIS2_MSG_CTX_GET_DOING_REST(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_DOING_REST(new_msg_ctx, env, doing_rest);
    
    doing_mtom = AXIS2_MSG_CTX_GET_DOING_MTOM(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_DOING_MTOM(new_msg_ctx, env, doing_mtom);
    
    server_side = AXIS2_MSG_CTX_GET_SERVER_SIDE(in_msg_ctx, env);
    AXIS2_MSG_CTX_SET_SERVER_SIDE(new_msg_ctx, env, server_side);
    
    svc_grp_ctx = AXIS2_MSG_CTX_GET_SVC_GRP_CTX(new_msg_ctx, env);
    AXIS2_MSG_CTX_SET_SVC_GRP_CTX(new_msg_ctx, env, svc_grp_ctx);
    
    return new_msg_ctx;
}
/*
public static AxisService createSimpleService(QName serviceName,
                                                     MessageReceiver messageReceiver,
                                                     String className,
                                                     QName opName) throws AxisFault {
    AxisService service = new AxisService(serviceName);
    service.setClassLoader(Thread.currentThread().getContextClassLoader());
    service.addParameter(
            new ParameterImpl(AbstractMessageReceiver.SERVICE_CLASS,
                    className));

    //todo I assumed in-out mep , this has to be imroved : Deepal
    AxisOperation axisOp = new InOutAxisOperation(opName);
    axisOp.setMessageReceiver(messageReceiver);
    axisOp.setStyle(WSDLService.STYLE_RPC);
    service.addOperation(axisOp);
    return service;
}

//    public static ServiceContext createServiceContext(
//        AxisService service,
//        ConfigurationContext engineContext)
//        throws AxisFault {
//        ServiceContext serviceContext = new ServiceContext(service, engineContext);
//        createExecutionChains(serviceContext);
//        return serviceContext;
//    }

public static AxisService createSimpleService(QName serviceName,
                                                     String className,
                                                     QName opName) throws AxisFault {
    return createSimpleService(serviceName,
            new RawXMLINOutMessageReceiver(),
            className,
            opName);
}

//    public static void addHandlers(Flow flow, Phase phase) throws AxisFault {
//        if (flow != null) {
//            int handlerCount = flow.getHandlerCount();
//            for (int i = 0; i < handlerCount; i++) {
//                phase.addHandler(flow.getHandler(i).getHandler());
//            }
//        }
//    }
public static void resolvePhases(AxisConfiguration axisconfig,
                                 AxisService axisService)
        throws AxisFault, PhaseException {
    //todo we do not need this
//        PhaseResolver pr = new PhaseResolver(axisconfig, axisService);
//        pr.buildchains();
    // fixing the BUG AXIS2-278
    // we do not need to  do this , since when adding a service this automatically done
}

public static String getParameterValue(Parameter param) {
    if (param == null) {
        return null;
    } else {
        return (String) param.getValue();
    }
}


public static String[] parseRequestURLForServiceAndOperation(
        String filePart) {
    String[] values = new String[2];

    int index = filePart.lastIndexOf(Constants.REQUEST_URL_PREFIX);
    String serviceStr = null;
    if (-1 != index) {
        serviceStr =
                filePart.substring(
                        index + Constants.REQUEST_URL_PREFIX.length() + 1);
        if ((index = serviceStr.indexOf('/')) > 0) {
            values[0] = serviceStr.substring(0, index);
            int lastIndex = serviceStr.indexOf('?');
            if (lastIndex >= 0) {
                values[1] = serviceStr.substring(index + 1, lastIndex);
            } else {
                values[1] = serviceStr.substring(index + 1);
            }
        } else {
            values[0] = serviceStr;
        }
    }
    return values;
}

public static void extractServiceGroupAndServiceInfo(String filePart, MessageContext messageContext) throws AxisFault {
    String[] values = parseRequestURLForServiceAndOperation(
            filePart);
    String serviceNameAndGroup = values[0];
    if (serviceNameAndGroup != null) {
        String[] serviceNameAndGroupStrings = serviceNameAndGroup.split(":");
        AxisConfiguration registry =
                messageContext.getSystemContext().getAxisConfiguration();
        if (serviceNameAndGroupStrings[0] != null) {
            AxisServiceGroup axisServiceGroup = registry.getServiceGroup(serviceNameAndGroupStrings[0]);
            String serviceNameStr = "";
            if (serviceNameAndGroupStrings.length == 1) {
                // This means user has not given a service name.
                // the notations is ...../axis2/services/<ServiceGroupName>
                serviceNameStr = serviceNameAndGroupStrings[0];
            }
            AxisService axisService = registry.getService(serviceNameStr);
            if (axisServiceGroup != null && axisService != null) {
                messageContext.setAxisServiceGroup(axisServiceGroup);
                messageContext.setAxisService(axisService);
            }
        }
    }
}

public static ServiceContext fillContextInformation(AxisOperation axisOperation, AxisService axisService, ConfigurationContext configurationContext) throws AxisFault {
    MessageContext msgContext;
    //  2. if null, create new opCtxt
    OperationContext operationContext = new OperationContext(axisOperation);
//        OperationContext operationContext = OperationContextFactory.createOperationContext(axisOperation.getAxisSpecifMEPConstant(), axisOperation);

    //  fill the service group context and service context info
    return fillServiceContextAndServiceGroupContext(axisService, configurationContext);

}

private static ServiceContext fillServiceContextAndServiceGroupContext(AxisService axisService, ConfigurationContext configurationContext) throws AxisFault {
    String serviceGroupContextId = UUIDGenerator.getUUID();
    ServiceGroupContext serviceGroupContext = new ServiceGroupContext(configurationContext, axisService.getParent());
    serviceGroupContext.setId(serviceGroupContextId);
    configurationContext.registerServiceGroupContext(serviceGroupContext);
    return new ServiceContext(axisService, serviceGroupContext);
}

public static ConfigurationContext getNewConfigurationContext(String repositry) throws Exception {
    ConfigurationContextFactory erfac = new ConfigurationContextFactory();
    File file = new File(repositry);
    if (!file.exists()) {
        throw new Exception(
                "repository directory " + file.getAbsolutePath() +
                        " does not exists");
    }
    return erfac.buildConfigurationContext(
            file.getAbsolutePath());
}
*/
