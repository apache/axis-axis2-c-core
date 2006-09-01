<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text"/>

      <!--Template for in out message receiver -->
      <xsl:template match="/interface">
        <xsl:variable name="skeletonname"><xsl:value-of select="@name"/></xsl:variable>
        <xsl:variable name="qname"><xsl:value-of select="@qname"/></xsl:variable>
        <xsl:variable name="method-prefix"><xsl:value-of select="@prefix"/></xsl:variable>
        <xsl:variable name="svcop-prefix"><xsl:value-of select="@svcop_prefix"/></xsl:variable>
        <xsl:variable name="svcname"><xsl:value-of select="@svcname"/></xsl:variable>
        <xsl:variable name="soapVersion"><xsl:value-of select="@soap-version"/></xsl:variable>

        /**
         * <xsl:value-of select="@name"/>.c
         *
         * This file was auto-generated from WSDL for "<xsl:value-of select="$qname"/>" service
         * by the Apache Axis2 version: #axisVersion# #today#
         *  <xsl:value-of select="$skeletonname"/>
         */

        #include "<xsl:value-of select="$svcop-prefix"/>.h"
        #include &lt;axis2_svc_skeleton.h&gt;
        #include &lt;axis2_array_list.h&gt;
        #include &lt;stdio.h&gt;

        /**
         * functions prototypes
         */

        /* On fault, handle the fault */
        axiom_node_t* AXIS2_CALL
        <xsl:value-of select="$method-prefix"/>_on_fault(axis2_svc_skeleton_t *svc_skeleton,
                  const axis2_env_t *env, axiom_node_t *node);

        /* Free the service */
        int AXIS2_CALL
        <xsl:value-of select="$method-prefix"/>_free(axis2_svc_skeleton_t *svc_skeleton,
                  const axis2_env_t *env);

        /* This method invokes the right service method */
        axiom_node_t* AXIS2_CALL
        <xsl:value-of select="$method-prefix"/>_invoke(axis2_svc_skeleton_t *svc_skeleton,
                    const axis2_env_t *env,
                    axiom_node_t *node,
                    axis2_msg_ctx_t *msg_ctx);

        /* Initializing the environment  */
        int AXIS2_CALL
        <xsl:value-of select="$method-prefix"/>_init(axis2_svc_skeleton_t *svc_skeleton,
                        const axis2_env_t *env);

        /* Create the service  */
        axis2_svc_skeleton_t* AXIS2_CALL
        <xsl:value-of select="$method-prefix"/>_create(const axis2_env_t *env);

        /**
         * Implementations for the functions
         */

	axis2_svc_skeleton_t* AXIS2_CALL
	<xsl:value-of select="$method-prefix"/>_create(const axis2_env_t *env)
	{
	    axis2_svc_skeleton_t *svc_skeleton = NULL;
	    svc_skeleton = AXIS2_MALLOC(env->allocator,
	        sizeof(axis2_svc_skeleton_t));


	    svc_skeleton->ops = AXIS2_MALLOC(
	        env->allocator, sizeof(axis2_svc_skeleton_ops_t));

	    svc_skeleton->func_array = NULL;

	    svc_skeleton->ops->free = <xsl:value-of select="$method-prefix"/>_free;
	    svc_skeleton->ops->init = <xsl:value-of select="$method-prefix"/>_init;
	    svc_skeleton->ops->invoke = <xsl:value-of select="$method-prefix"/>_invoke;
	    svc_skeleton->ops->on_fault = <xsl:value-of select="$method-prefix"/>_on_fault;

	    return svc_skeleton;
	}


	int AXIS2_CALL
	<xsl:value-of select="$method-prefix"/>_init(axis2_svc_skeleton_t *svc_skeleton,
	                        const axis2_env_t *env)
	{
	    svc_skeleton->func_array = axis2_array_list_create(env, 10);
        <xsl:for-each select="method">
	      AXIS2_ARRAY_LIST_ADD(svc_skeleton->func_array, env, "<xsl:value-of select="@localpart"/>");
        </xsl:for-each>

	    /* Any initialization stuff of <xsl:value-of select="$svcname"/> goes here */
	    return AXIS2_SUCCESS;
	}

	int AXIS2_CALL
	<xsl:value-of select="$method-prefix"/>_free(axis2_svc_skeleton_t *svc_skeleton,
				 const axis2_env_t *env)
	{
          if(svc_skeleton->func_array)
          {
            AXIS2_ARRAY_LIST_FREE(svc_skeleton->func_array, env);
            svc_skeleton->func_array = NULL;
          }

          if(svc_skeleton->ops)
          {
            AXIS2_FREE(env->allocator, svc_skeleton->ops);
            svc_skeleton->ops = NULL;
          }

          if(svc_skeleton)
          {
            AXIS2_FREE(env->allocator, svc_skeleton);
            svc_skeleton = NULL;
          }
          return AXIS2_SUCCESS;
	}


	/*
	 * This method invokes the right service method
	 */
	axiom_node_t* AXIS2_CALL
	<xsl:value-of select="$method-prefix"/>_invoke(axis2_svc_skeleton_t *svc_skeleton,
				const axis2_env_t *env,
				axiom_node_t *content_node,
				axis2_msg_ctx_t *msg_ctx)
	{
         /* depending on the function name invoke the
          * corresponding  method
          */
          <!--here the databinding support code is removed-->
          axiom_element_t *element = NULL;

          if (content_node)
	      {
            if (AXIOM_NODE_GET_NODE_TYPE(content_node, env) == AXIOM_ELEMENT)
            {
               element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(content_node, env);
               if (element)
               {
                  axis2_char_t *op_name = AXIOM_ELEMENT_GET_LOCALNAME(element, env);
                  if (op_name)
                  {
                    <xsl:for-each select="method">

                    <xsl:variable name="soapAction"><xsl:value-of select="@soapaction"></xsl:value-of></xsl:variable>
                    <xsl:variable name="name"><xsl:value-of select="@name"/></xsl:variable>
                    <xsl:variable name="style"><xsl:value-of select="@style"/></xsl:variable>
                    <xsl:variable name="method-name"><xsl:value-of select="@name"/></xsl:variable>
                    <xsl:variable name="method-ns"><xsl:value-of select="@namespace"/> </xsl:variable>
                    <xsl:variable name="outputtype"><xsl:value-of select="output/param/@type"></xsl:value-of></xsl:variable>

                    if ( AXIS2_STRCMP(op_name, "<xsl:value-of select="@localpart"/>") == 0 )
                    {
                         <!--TODO:remove the hard coded parameters -->
                         <xsl:if test="$outputtype!=''">return </xsl:if>
                         <xsl:text> </xsl:text>
                         <xsl:value-of select="$svcop-prefix"/>_<xsl:value-of select="$method-name"/>(env <xsl:for-each select="input/param[@type!='']"> ,
                                              content_node </xsl:for-each>);
                         <xsl:if test="$outputtype=''">return NULL;</xsl:if>
                    
                    }
                    </xsl:for-each>
                  }
                }
             }
          }
          printf("<xsl:value-of select="$skeletonname"/> service ERROR: invalid OM parameters in request\n");
          return content_node;
        }

    axiom_node_t* AXIS2_CALL
    <xsl:value-of select="$method-prefix"/>_on_fault(axis2_svc_skeleton_t *svc_skeleton,
                  const axis2_env_t *env, axiom_node_t *node)
	{
		axiom_node_t *error_node = NULL;
		    axiom_node_t* text_node = NULL;
		axiom_element_t *error_ele = NULL;
		error_ele = axiom_element_create(env, node, "<xsl:value-of select="$skeletonname"/>Error", NULL,
    					&amp;error_node);
		AXIOM_ELEMENT_SET_TEXT(error_ele, env, "<xsl:value-of select="$skeletonname"/>failed",
    					text_node);
		return error_node;
	}


	/**
	 * Following block distinguish the exposed part of the dll.
 	 */

    AXIS2_EXTERN int AXIS2_CALL
    axis2_get_instance(struct axis2_svc_skeleton **inst,
	                        const axis2_env_t *env)
	{
		*inst = <xsl:value-of select="$method-prefix"/>_create(env);
		
        if(!(*inst))
        {
            return AXIS2_FAILURE;
        }

  		return AXIS2_SUCCESS;
	}

	AXIS2_EXTERN int AXIS2_CALL
    axis2_remove_instance(axis2_svc_skeleton_t *inst,
                            const axis2_env_t *env)
	{
        axis2_status_t status = AXIS2_FAILURE;
       	if (inst)
        {
            status = AXIS2_SVC_SKELETON_FREE(inst, env);
        }
    	return status;
	}


    </xsl:template>

</xsl:stylesheet>
