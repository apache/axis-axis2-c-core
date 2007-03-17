<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="text"/>

     <!-- cater for the multiple classes - wrappped mode - currently not well supported.-->
    <xsl:template match="/classs">
        <xsl:variable name="name"><xsl:value-of select="@name"/></xsl:variable>
        <xsl:variable name="axis2_name">axis2_<xsl:value-of select="@name"/></xsl:variable>
        <xsl:variable name="axis2_capsname">AXIS2_<xsl:value-of select="@caps-name"/></xsl:variable>
       /**
        * <xsl:value-of select="$axis2_name"/>.c
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/C version: #axisVersion# #today#
        */

       /**
        *  <xsl:value-of select="$axis2_name"/>
        */
        #include "<xsl:value-of select="$axis2_name"/>.h"

        <xsl:apply-templates/>

    </xsl:template>

    <!--cater for the multiple classes - unwrappped mode -->
    <xsl:template match="/">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="class">
        <xsl:variable name="name"><xsl:value-of select="@name"/></xsl:variable>
        <xsl:variable name="axis2_name">axis2_<xsl:value-of select="@name"/></xsl:variable>
        <xsl:variable name="istype"><xsl:value-of select="@type"/></xsl:variable>
        <xsl:variable name="axis2_capsname">AXIS2_<xsl:value-of select="@caps-name"/></xsl:variable>

        <xsl:variable name="originalName"><xsl:value-of select="@originalName"/></xsl:variable>
        <xsl:variable name="nsuri"><xsl:value-of select="@nsuri"/></xsl:variable>
        <xsl:variable name="nsprefix"><xsl:value-of select="@nsprefix"/></xsl:variable>
       /**
        * <xsl:value-of select="$axis2_name"/>.h
        *
        * This file was auto-generated from WSDL
        * by the Apache Axis2/C version: #axisVersion# #today#
        *
        */

        #include "<xsl:value-of select="$axis2_name"/>.h"
          <xsl:choose>
             <xsl:when test="@type">
              /**
               * This type was generated from the piece of schema that had
               * name = <xsl:value-of select="@originalName"/>
               * Namespace URI = <xsl:value-of select="@nsuri"/>
               * Namespace Prefix = <xsl:value-of select="@nsprefix"/>
               */
             </xsl:when>
             <xsl:otherwise>
              /**
               * implmentation of the <xsl:value-of select="@originalName"/>|<xsl:value-of select="@nsuri"/> element
               */
             </xsl:otherwise>
          </xsl:choose>


        typedef struct <xsl:value-of select="$axis2_name"/><xsl:text>_impl </xsl:text><xsl:value-of select="$axis2_name"/>_impl_t;

        struct <xsl:value-of select="$axis2_name"/>_impl
        {
            <xsl:value-of select="$axis2_name"/>_t <xsl:value-of select="$name"/>;

            <xsl:if test="not(@type)">
                axis2_qname_t* qname;
            </xsl:if>

            <xsl:for-each select="property">
                <xsl:variable name="propertyType">
                   <xsl:choose>
                     <xsl:when test="@isarray">axis2_array_list_t*</xsl:when>
                     <xsl:when test="not(@type)">axiom_node_t*</xsl:when> <!-- these are anonymous -->
                     <xsl:when test="@ours">axis2_<xsl:value-of select="@type"/>_t*</xsl:when>
                     <xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
                   </xsl:choose>
                </xsl:variable>
                <xsl:variable name="propertyBaseType">
                   <xsl:choose>
                     <xsl:when test="@ours">axis2_<xsl:value-of select="@type"/>_t*</xsl:when>
                     <xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
                   </xsl:choose>
                </xsl:variable>
                <xsl:variable name="propertyName"><xsl:value-of select="@name"></xsl:value-of></xsl:variable>
                <xsl:variable name="CName"><xsl:value-of select="@cname"></xsl:value-of></xsl:variable>

                <xsl:value-of select="$propertyType"/><xsl:text> </xsl:text>attrib_<xsl:value-of select="$CName"/>;
            </xsl:for-each>
        };

        #define AXIS2_INTF_TO_IMPL(<xsl:value-of select="$name"/>) \
            ((<xsl:value-of select="$axis2_name"/>_impl_t *) <xsl:value-of select="$name"/>)

        /************************* Function prototypes ********************************/

        axis2_status_t AXIS2_CALL
        <xsl:value-of select="$axis2_name"/>_free (
                <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                const axis2_env_t *env);

        <xsl:if test="not(@type)">
          axis2_qname_t* AXIS2_CALL
          <xsl:value-of select="$axis2_name"/>_get_qname (
                <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                const axis2_env_t *env);
        </xsl:if>

        axiom_node_t* AXIS2_CALL
        <xsl:value-of select="$axis2_name"/>_parse_om (
                <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                const axis2_env_t *env,
                axiom_node_t* <xsl:value-of select="$name"/>_om_node);

        axis2_status_t AXIS2_CALL
        <xsl:value-of select="$axis2_name"/>_build_om (
                <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                const axis2_env_t *env, axiom_node_t* parent);

        <xsl:for-each select="property">
            <xsl:variable name="propertyType">
               <xsl:choose>
                    <xsl:when test="@isarray">axis2_array_list_t*</xsl:when>
                    <xsl:when test="not(@type)">axiom_node_t*</xsl:when> <!-- these are anonymous -->
                    <xsl:when test="@ours">axis2_<xsl:value-of select="@type"/>_t*</xsl:when>
                    <xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="propertyName"><xsl:value-of select="@name"></xsl:value-of></xsl:variable>
            <xsl:variable name="CName"><xsl:value-of select="@cname"></xsl:value-of></xsl:variable>

            /**
             * getter for <xsl:value-of select="$propertyName"/>.
             */
            <xsl:value-of select="$propertyType"/> AXIS2_CALL
            <xsl:value-of select="$axis2_name"/>_get_<xsl:value-of select="$CName"/>(
                    <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                    const axis2_env_t *env);

            /**
             * setter for <xsl:value-of select="$propertyName"/>
             */
            axis2_status_t AXIS2_CALL
            <xsl:value-of select="$axis2_name"/>_set_<xsl:value-of select="$CName"/>(
                    <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                    const axis2_env_t *env,
                    <xsl:value-of select="$propertyType"/><xsl:text> </xsl:text> param_<xsl:value-of select="$CName"/>);
        </xsl:for-each>

       /************************* Function Implmentations ********************************/
        AXIS2_EXTERN <xsl:value-of select="$axis2_name"/>_t* AXIS2_CALL
        <xsl:value-of select="$axis2_name"/>_create(
            const axis2_env_t *env )
        {
            <xsl:value-of select="$axis2_name"/>_impl_t* <xsl:value-of select="$name"/>_impl = NULL;
            <xsl:if test="not(@type)">
              axis2_qname_t* qname = NULL;
            </xsl:if>
            AXIS2_ENV_CHECK(env, NULL);

            <xsl:value-of select="$name"/>_impl = (<xsl:value-of select="$axis2_name"/>_impl_t *) AXIS2_MALLOC(env->
                allocator, sizeof(<xsl:value-of select="$axis2_name"/>_impl_t));

            if(NULL == <xsl:value-of select="$name"/>_impl)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                return NULL;
            }

            <xsl:value-of select="$name"/>_impl-> <xsl:value-of select="$name"/>.ops =
                            AXIS2_MALLOC (env->allocator, sizeof(<xsl:value-of select="$axis2_name"/>_ops_t));
            if(NULL == <xsl:value-of select="$name"/>_impl-> <xsl:value-of select="$name"/>.ops)
            {
                axis2_<xsl:value-of select="$name"/>_free(&amp;(<xsl:value-of select="$name"/>_impl-><xsl:value-of select="$name"/>), env);
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                return NULL;
            }

            <xsl:for-each select="property">
                <xsl:variable name="CName"><xsl:value-of select="@cname"></xsl:value-of></xsl:variable>
                <xsl:choose>
                  <xsl:when test="@ours">
                    <xsl:value-of select="$name"/>_impl-> attrib_<xsl:value-of select="$CName"/>  = NULL;
                  </xsl:when>
                  <!-- todo for others -->
                </xsl:choose>
            </xsl:for-each>

            <xsl:value-of select="$name"/>_impl-><xsl:value-of select="$name"/>.ops->free = <xsl:value-of select="$axis2_name"/>_free;
            <xsl:if test="not(@type)">
              <xsl:value-of select="$name"/>_impl-><xsl:value-of select="$name"/>.ops->get_qname = <xsl:value-of select="$axis2_name"/>_get_qname;
            </xsl:if>
            <xsl:value-of select="$name"/>_impl-><xsl:value-of select="$name"/>.ops->parse_om = <xsl:value-of select="$axis2_name"/>_parse_om;
            <xsl:value-of select="$name"/>_impl-><xsl:value-of select="$name"/>.ops->build_om = <xsl:value-of select="$axis2_name"/>_build_om;
            <xsl:for-each select="property">
                <xsl:variable name="propertyName"><xsl:value-of select="@name"></xsl:value-of></xsl:variable>
                <xsl:variable name="CName"><xsl:value-of select="@cname"></xsl:value-of></xsl:variable>
                <xsl:value-of select="$name"/>_impl-><xsl:value-of select="$name"/>.ops->get_<xsl:value-of select="$CName"/> = <xsl:value-of select="$axis2_name"/>_get_<xsl:value-of select="$CName"/>;
                <xsl:value-of select="$name"/>_impl-><xsl:value-of select="$name"/>.ops->set_<xsl:value-of select="$CName"/> = <xsl:value-of select="$axis2_name"/>_set_<xsl:value-of select="$CName"/>;
            </xsl:for-each>
            <xsl:if test="not(@type)">
              qname =  axis2_qname_create (env,
                        "<xsl:value-of select="@originalName"/>",
                        "<xsl:value-of select="@nsuri"/>",
                        "<xsl:value-of select="@nsprefix"/>");

              <xsl:value-of select="$name"/>_impl->qname = qname;
            </xsl:if>

            return &amp;(<xsl:value-of select="$name"/>_impl-><xsl:value-of select="$name"/>);
         }

        axis2_status_t AXIS2_CALL
        <xsl:value-of select="$axis2_name"/>_free (
                <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                const axis2_env_t *env)
        {
            <xsl:value-of select="$axis2_name"/>_impl_t *<xsl:value-of select="$name"/>_impl = NULL;
            <!--<xsl:if test="property/@isarray and (property/@ours or property/@type='axis2_char_t*' or property/@type='axis2_date_time_t*' or property/@type='axis2_base64_binary_t*' or property/@type='axiom_node_t*')">-->
            <xsl:if test="property/@isarray">
                  int i = 0;
                  int count = 0;
                  void *element = NULL;
            </xsl:if>

            AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

            <xsl:value-of select="$name"/>_impl = AXIS2_INTF_TO_IMPL(<xsl:value-of select="$name"/>);

            <xsl:for-each select="property">
             <!-- please check all the freeing thing below -->
             <!--<xsl:if test="@isarray or not(@type='short' or @type='int' or @type='char' or @type='long' or @type='double' or @type='float' or @type='axis2_bool_t')">-->
             <xsl:if test="@isarray or @ours or @type='axis2_char_t*' or @type='axis2_qname_t*' or @type='axis2_uri_t*' or @type='axis2_date_time_t*' or @type='axis2_base64_binary_t*'">
              <xsl:variable name="propertyType">
                 <xsl:choose>
                   <xsl:when test="@isarray">axis2_array_list_t*</xsl:when>
                   <xsl:when test="not(@type)">axiom_node_t*</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">axis2_<xsl:value-of select="@type"/>_t*</xsl:when>
                   <xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="capspropertyType">
                 <xsl:choose>
                   <xsl:when test="@isarray">AXIS2_ARRAY_LIST</xsl:when>
                   <xsl:when test="not(@type)">AXIOM_NODE</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">AXIS2_<xsl:value-of select="@caps-type"/></xsl:when>
                   <xsl:otherwise><xsl:value-of select="@caps-type"/></xsl:otherwise> <!-- this will not be used -->
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="nativePropertyType"> <!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="not(@type)">axiom_node_t*</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">axis2_<xsl:value-of select="@type"/>_t*</xsl:when>
                   <xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="nativeCapspropertyType"><!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="not(@type)">AXIOM_NODE</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">AXIS2_<xsl:value-of select="@caps-type"/></xsl:when>
                   <xsl:otherwise><xsl:value-of select="@caps-type"/></xsl:otherwise> <!-- this will not be used -->
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="propertyName"><xsl:value-of select="@name"></xsl:value-of></xsl:variable>
              <xsl:variable name="CName"><xsl:value-of select="@cname"></xsl:value-of></xsl:variable>

              <xsl:variable name="attriName"><!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="@isarray">(<xsl:value-of select="$nativePropertyType"/>)element</xsl:when>
                   <xsl:otherwise><xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="justAttriName"><!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="@isarray">element</xsl:when>
                   <xsl:otherwise><xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>



              <!-- handles arrays -->
              <xsl:if test="@isarray">
                if ( <xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/> != NULL)
                {
                    count = AXIS2_ARRAY_LIST_SIZE( <xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/>, env);
                    for( i = 0; i &lt; count; i ++)
                    {
                       element = AXIS2_ARRAY_LIST_GET( <xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/>, env, i);
              </xsl:if>

              <!-- the following element can be inside array or independent one -->
              if( <xsl:value-of select="$justAttriName"/> != NULL)
              {
                 <!-- how to free all the ours things -->
                 <xsl:choose>
                   <xsl:when test="@ours">
                      <xsl:value-of select="$nativeCapspropertyType"/>_FREE( <xsl:value-of select="$attriName"/>, env);
                   </xsl:when>

                   <xsl:when test="$nativePropertyType='short' or $nativePropertyType='axis2_bool_t' or $nativePropertyType='char' or $nativePropertyType='int' or $nativePropertyType='float' or $nativePropertyType='double' or $nativePropertyType='long'">
                     <xsl:if test="@isarray">
                      <!-- free ints, longs, float in array-->
                      /** we keep primtives as pointers in arrasy, so need to free them */
                      AXIS2_FREE( env-> allocator, element);
                     </xsl:if>
                   </xsl:when>

                   <!-- free axis2_char_t s -->
                   <xsl:when test="$nativePropertyType='axis2_char_t*'">
                      AXIS2_FREE( env-> allocator, <xsl:value-of select="$attriName"/>);
                   </xsl:when>

                   <!-- free nodes -->
                   <xsl:when test="$nativePropertyType='axiom_node_t*'">
                    AXIOM_NODE_FREE_TREE ( <xsl:value-of select="$attriName"/>, env );
                   </xsl:when>

                   <xsl:when test="$nativePropertyType='axis2_qname_t*'">
                    axis2_qname_free( <xsl:value-of select="$attriName"/>, env );
                   </xsl:when>

                   <xsl:when test="$nativePropertyType='axis2_uri_t*'">
                    axis2_uri_free( <xsl:value-of select="$attriName"/>, env );
                   </xsl:when>

                   <xsl:when test="$nativePropertyType='axis2_date_time_t*'">
                    AXIS2_DATE_TIME_FREE( <xsl:value-of select="$attriName"/>, env );
                   </xsl:when>

                   <xsl:when test="$propertyType='axis2_base64_binary_t*'">
                    AXIS2_BASE64_BINARY_FREE ( <xsl:value-of select="$attriName"/>, env );
                   </xsl:when>

                   <!--TODO: This should be extended for all the types that should be freed.. -->
                   <xsl:otherwise>
                     /** This is an unknown type or a primitive. Free this manually for unknown type */
                   </xsl:otherwise>
                 </xsl:choose>
                 <xsl:value-of select="$justAttriName"/> = NULL;
              }

              <!-- close tags arrays -->
              <xsl:if test="@isarray">
                    }
                    AXIS2_ARRAY_LIST_FREE( <xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/>, env);
                }
              </xsl:if>
             </xsl:if> <!--close for test of primitive types -->
            </xsl:for-each>

            <xsl:if test="not(@type)">
              if(<xsl:value-of select="$name"/>_impl->qname )
              {
                  axis2_qname_free (<xsl:value-of select="$name"/>_impl->qname, env);
                  <xsl:value-of select="$name"/>_impl->qname = NULL;
              }
            </xsl:if>

            if(<xsl:value-of select="$name"/>->ops)
            {
                AXIS2_FREE(env->allocator, <xsl:value-of select="$name"/>->ops);
                <xsl:value-of select="$name"/>->ops = NULL;
            }

            if(<xsl:value-of select="$name"/>_impl)
            {
                AXIS2_FREE( env->allocator, <xsl:value-of select="$name"/>_impl);
                <xsl:value-of select="$name"/>_impl = NULL;
            }
            return AXIS2_SUCCESS;
        }

        <xsl:if test="not(@type)">  <!-- this would only generate for elements -->
           axis2_qname_t* AXIS2_CALL
           <xsl:value-of select="$axis2_name"/>_get_qname (
                   <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                   const axis2_env_t *env)
           {
               <xsl:value-of select="$axis2_name"/>_impl_t *<xsl:value-of select="$name"/>_impl = NULL;

               AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

               <xsl:value-of select="$name"/>_impl = AXIS2_INTF_TO_IMPL(<xsl:value-of select="$name"/>);

               return <xsl:value-of select="$name"/>_impl-> qname;
           }
        </xsl:if>

        axis2_status_t AXIS2_CALL
        <xsl:value-of select="$axis2_name"/>_build_om (
                <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                const axis2_env_t *env,
                axiom_node_t* parent)
        {
            <xsl:value-of select="$axis2_name"/>_impl_t *<xsl:value-of select="$name"/>_impl = NULL;

            axis2_status_t status = AXIS2_SUCCESS;
            axiom_namespace_t *ns1 = NULL;
            <xsl:if test="property/@attribute">
               axiom_attribute_t *parent_attri = NULL;
               axiom_element_t *parent_element = NULL;
               axis2_char_t *attrib_text = NULL;
            </xsl:if>
            <xsl:if test="property/@isarray">
               int i = 0;
               int element_found = 0;
               axis2_array_list_t *arr_list = NULL;
            </xsl:if>
            <xsl:if test="@ordered and property/@isarray">
               int sequence_broken = 0;
               axiom_node_t *tmp_node = NULL;
            </xsl:if>
            <!-- these two are requried -->
            <xsl:if test="property/@ours or (property/@isarray and (property/@type='short' or property/@type='int' or property/@type='char' or property/@type='long' or property/@type='float' or property/@type='double')) or property/@type='axis2_date_time_t*' or property/@type='axis2_base64_binary_t*'">
               void *element = NULL;
            </xsl:if>
            <xsl:if test="property">
              <xsl:if test="not(property/@attribute) ">
                <!-- this effort is to find atleast onething without @ours -->
                <xsl:for-each select="property[not(@ours) or @ours='']">
                  <xsl:if test="position()=1">
                     axis2_char_t* text_value = NULL;
                  </xsl:if>
                </xsl:for-each>
              </xsl:if>
            </xsl:if>
            <xsl:if test="property and (property/@isarray or not(@type) or @type='' or property/@attribute)">
               axis2_qname_t *qname = NULL;
            </xsl:if>
            <xsl:if test="not(property/@attribute) and (not(@ordered) or @ordered='' or property/@isarray)">
              axis2_qname_t *element_qname = NULL;
            </xsl:if>
            <xsl:for-each select="property">
             <xsl:if test="position()=1">
               axiom_node_t *first_node = NULL;
               <!--<xsl:if test="not(../@type) or ../@type='' or property/@attribute">
                 axis2_qname_t *qname = NULL;
               </xsl:if>
               axiom_node_t *current_node = NULL;
               axiom_element_t *current_element = NULL;-->
             </xsl:if>
            </xsl:for-each>
            <xsl:if test="property and (not(property/@attribute) or property/@attribute='')">
               axiom_node_t *current_node = NULL;
               axiom_element_t *current_element = NULL;
            </xsl:if>
            AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
            <xsl:value-of select="$name"/>_impl = AXIS2_INTF_TO_IMPL(<xsl:value-of select="$name"/>);

            ns1 = axiom_namespace_create (env,
                                         "<xsl:value-of select="$nsuri"/>",
                                         "<xsl:value-of select="$nsprefix"/>");
            <xsl:if test="property">
              if ( NULL == parent )
              {
                /** This should be checked above */
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "NULL elemenet for <xsl:value-of select="$originalName"/>"
                                              " %d :: %s", env->error->error_number,
                                              AXIS2_ERROR_GET_MESSAGE(env->error));
                return AXIS2_FAILURE;
              }
            </xsl:if>
            <xsl:choose>
              <xsl:when test="not(@type)">
                <xsl:for-each select="property">
                 <xsl:if test="position()=1">
                    current_element = AXIOM_NODE_GET_DATA_ELEMENT( parent, env);
                    qname = AXIOM_ELEMENT_GET_QNAME( current_element, env, parent);
                    if ( axis2_qname_equals( qname, env, <xsl:value-of select="$name"/>_impl-> qname ) )
                    {
                        first_node = AXIOM_NODE_GET_FIRST_CHILD( parent, env);
                    }
                    else
                    {
                        first_node = parent;
                    }
                 </xsl:if>
                </xsl:for-each>
              </xsl:when>
              <xsl:otherwise>
                first_node = parent;
              </xsl:otherwise>
             </xsl:choose>

            <xsl:for-each select="property/@attribute">
             <xsl:if test="position()=1">
                 parent_element = AXIOM_NODE_GET_DATA_ELEMENT( parent, env);
             </xsl:if>
            </xsl:for-each>

            <xsl:for-each select="property">
              <xsl:variable name="propertyType">
                 <xsl:choose>
                   <xsl:when test="@isarray">axis2_array_list_t*</xsl:when>
                   <xsl:when test="not(@type)">axiom_node_t*</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">axis2_<xsl:value-of select="@type"/>_t*</xsl:when>
                   <xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="capspropertyType">
                 <xsl:choose>
                   <xsl:when test="@isarray">AXIS2_ARRAY_LIST</xsl:when>
                   <xsl:when test="not(@type)">AXIOM_NODE</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">AXIS2_<xsl:value-of select="@caps-type"/></xsl:when>
                   <xsl:otherwise><xsl:value-of select="@caps-type"/></xsl:otherwise> <!-- this will not be used -->
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="nativePropertyType"> <!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="not(@type)">axiom_node_t*</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">axis2_<xsl:value-of select="@type"/>_t*</xsl:when>
                   <xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="nativeCapspropertyType"><!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="not(@type)">AXIOM_NODE</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">AXIS2_<xsl:value-of select="@caps-type"/></xsl:when>
                   <xsl:otherwise><xsl:value-of select="@caps-type"/></xsl:otherwise> <!-- this will not be used -->
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="propertyName"><xsl:value-of select="@name"></xsl:value-of></xsl:variable>
              <xsl:variable name="CName"><xsl:value-of select="@cname"></xsl:value-of></xsl:variable>
              <xsl:variable name="CapsCName"><xsl:value-of select="@caps-cname"></xsl:value-of></xsl:variable>

              <xsl:variable name="attriName"><!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="@isarray">(<xsl:value-of select="$nativePropertyType"/>)element</xsl:when>
                   <xsl:otherwise><xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="justAttriName"><!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="@isarray">element</xsl:when>
                   <xsl:otherwise><xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>
              <xsl:choose>
                <xsl:when test="@attribute">
                  qname = axis2_qname_create( env, "<xsl:value-of select="$propertyName"/>", "<xsl:value-of select="@nsuri"/>","<xsl:choose>
                                                                   <xsl:when test="@prefix!=''"><xsl:value-of select="@prefix"/></xsl:when>
                                                                   <xsl:when test="@nsuri=../@nsuri"><xsl:value-of select="../@nsprefix"/></xsl:when></xsl:choose>");
                  parent_attri = AXIOM_ELEMENT_GET_ATTRIBUTE( parent_element, env, qname);
                  if( parent_attri != NULL)
                  {
                    attrib_text = AXIOM_ATTRIBUTE_GET_VALUE( parent_attri, env);
                  }
                  else
                  {
                    /** hope this work */
                    attrib_text = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME( parent_element, env, "<xsl:value-of select="$propertyName"/>");
                  }
                  if( attrib_text != NULL)
                  {
                      <!-- here only simple type possible -->
                      <xsl:choose>
                        <!-- add int s -->
                        <xsl:when test="$nativePropertyType='int'">
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, atoi( attrib_text));
                        </xsl:when>

                        <!-- add axis2_char_t s -->
                        <xsl:when test="$nativePropertyType='char'">
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, (char)atoi( attrib_text));
                        </xsl:when>

                        <!-- add short s -->
                        <xsl:when test="$nativePropertyType='short'">
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, atoi( attrib_text));
                        </xsl:when>

                        <!-- add long s -->
                        <xsl:when test="$nativePropertyType='long'">
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, atol( attrib_text));
                        </xsl:when>

                        <!-- add float s -->
                        <xsl:when test="$nativePropertyType='float'">
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, atof( attrib_text));
                        </xsl:when>
                        <!-- add double s -->
                        <xsl:when test="$nativePropertyType='double'">
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, atof( attrib_text));
                        </xsl:when>

                        <!-- add axis2_char_t s -->
                        <xsl:when test="$nativePropertyType='axis2_char_t*'">
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, axis2_strdup( attrib_text, env));
                        </xsl:when>

                        <!-- add axis2_qname_t s -->
                        <xsl:when test="$nativePropertyType='axis2_qname_t*'">
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, axis2_qname_create_from_string( env, attrib_text));
                        </xsl:when>

                        <!-- add axis2_uri_t s -->
                        <xsl:when test="$nativePropertyType='axis2_uri_t*'">
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, axis2_uri_parse_string( env, attrib_text));
                        </xsl:when>
                        <!-- add axis2_bool_t s -->
                        <xsl:when test="$nativePropertyType='axis2_bool_t'">
                           if ( !axis2_strcmp( attrib_text, "TRUE") || !axis2_strcmp( attrib_text, "true") )
                           {
                               <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, AXIS2_TRUE);
                           }
                           else
                           {
                               <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, AXIS2_FALSE);
                           }
                        </xsl:when>
                        <!-- add date_time_t* s -->
                        <xsl:when test="$nativePropertyType='axis2_date_time_t*'">
                           element = (void*)axis2_date_time_create( env);
                           AXIS2_DATE_TIME_DESERIALIZE_DATE_TIME( (axis2_date_time_t*)element, env,
                                                                      attrib_text);
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, ( <xsl:value-of select="$nativePropertyType"/>)element);
                        </xsl:when>
                        <!-- add hex_binary_t* s -->
                        <xsl:when test="$nativePropertyType='axis2_base64_binary_t*'">
                           element = (void*)axis2_base64_binary_create( env);
                           AXIS2_BASE64_BINARY_SET_ENCODED_BINARY( <xsl:value-of select="$name"/>_impl-> attrib_<xsl:value-of select="$CName"/>, env,
                                                                      attrib_text);
                           <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>,
                                                          env, ( <xsl:value-of select="$nativePropertyType"/>)element);
                        </xsl:when>
                        <xsl:otherwise>
                          <!--TODO: add new attributes types -->
                          /** can not handle the attribute type <xsl:value-of select="$nativePropertyType"/>*/
                        </xsl:otherwise>
                      </xsl:choose>
                    }
                    <xsl:if test="not(@nillable)">
                    else
                    {
                        /** this is not a nillable attribute*/
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "non nillable attribute <xsl:value-of select="$propertyName"/> missing"
                                              " %d :: %s", env->error->error_number,
                                              AXIS2_ERROR_GET_MESSAGE(env->error));
                        return AXIS2_FAILURE;
                    }
                    </xsl:if>
                </xsl:when>
                <xsl:otherwise> <!-- when it is an element not(@attribute) -->
                  <!-- handles arrays -->
                   <xsl:if test="@isarray">
                    /**
                     * building <xsl:value-of select="$CName"/> array
                     */
                     <xsl:if test="position()=1">
                       arr_list = axis2_array_list_create( env, 10);
                     </xsl:if>
                   </xsl:if>

                     <!-- for each non attribute properties there will always be an element-->
                     /**
                      * building <xsl:value-of select="$propertyName"/> element
                      */
                     <!-- array and non array build is so different so big choose, when is requried-->
                     <!-- the method of picking the element is depend on the ../@ordered -->
                     <xsl:choose>
                       <xsl:when test="not(@isarray)">  <!--not an array so continue normal -->
                           <xsl:choose>
                             <xsl:when test="../@ordered">
                               <xsl:choose>
                                 <xsl:when test="position()=1">
                                   current_node = first_node;
                                 </xsl:when>
                                 <xsl:otherwise>
                                   /**
                                     * because elements are ordered this works fine
                                     */
                                   <!-- current node should contain the ordered value -->
                                   if( current_node != NULL)
                                   {
                                       current_node = AXIOM_NODE_GET_NEXT_SIBLING( current_node, env);
                                   }
                                 </xsl:otherwise>
                               </xsl:choose>
                             </xsl:when>
                             <xsl:otherwise>
                                   /**
                                     * because elements are not ordered we should surf all the sibling to pick the right one
                                     */
                               for ( current_node = first_node; current_node != NULL;
                                             current_node = AXIOM_NODE_GET_NEXT_SIBLING( current_node, env))
                               {
                                  current_element = AXIOM_NODE_GET_DATA_ELEMENT( current_node, env);
                                  qname = AXIOM_ELEMENT_GET_QNAME( current_element, env, current_node);
                                  element_qname = axis2_qname_create( env, "<xsl:value-of select="$propertyName"/>", "<xsl:value-of select="@nsuri"/>", "<xsl:choose>
                                                                   <xsl:when test="@prefix!=''"><xsl:value-of select="@prefix"/></xsl:when>
                                                                   <xsl:when test="@nsuri=../@nsuri"><xsl:value-of select="../@nsprefix"/></xsl:when></xsl:choose>");
                                  if ( axis2_qname_equals( element_qname, env, qname))
                                  {
                                       /** found the requried element */
                                       break;
                                  }
                               }
                             </xsl:otherwise>
                           </xsl:choose>
                           if ( current_node != NULL)
                           {
                              <xsl:if test="../@ordered">current_element = AXIOM_NODE_GET_DATA_ELEMENT( current_node, env);</xsl:if>
                              <!-- changes to following choose tag should be changed in another 2 places -->
                                 <xsl:choose>
                                    <xsl:when test="@ours">
                                      element = (void*)axis2_<xsl:value-of select="@type"/>_create( env);
                                      status =  <xsl:value-of select="$nativeCapspropertyType"/>_BUILD_OM( ( <xsl:value-of select="$nativePropertyType"/>)element, env,
                                                                             AXIOM_NODE_GET_FIRST_CHILD(current_node, env)==NULL?current_node:AXIOM_NODE_GET_FIRST_CHILD(current_node, env));
                                      if( AXIS2_FAILURE ==  status)
                                      {
                                          AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in building element <xsl:value-of select="$propertyName"/> "
                                                              " %d :: %s", env->error->error_number,
                                                              AXIS2_ERROR_GET_MESSAGE(env->error));
                                          return AXIS2_FAILURE;
                                      }
                                      status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                   ( <xsl:value-of select="$nativePropertyType"/>)element);
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='axis2_char_t*'">
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      if ( NULL == text_value )
                                      {
                                         status = AXIS2_FAILURE;
                                      }
                                      else
                                      {
                                          status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                   text_value);
                                      }
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='axis2_uri_t*'">
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      if ( NULL == text_value )
                                      {
                                         status = AXIS2_FAILURE;
                                      }
                                      else
                                      {
                                          status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                   axis2_uri_parse_string( env, text_value));
                                      }
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='axis2_qname_t*'">
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      if ( NULL == text_value )
                                      {
                                         status = AXIS2_FAILURE;
                                      }
                                      else
                                      {
                                          status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                   axis2_qname_create_from_string( env, text_value));
                                      }
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='char'">
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      if ( NULL == text_value )
                                      {
                                         status = AXIS2_FAILURE;
                                      }
                                      else
                                      {
                                          status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                    (char)atoi( text_value));
                                      }
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='int'">
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      if ( NULL == text_value )
                                      {
                                         status = AXIS2_FAILURE;
                                      }
                                      else
                                      {
                                          status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                    atoi( text_value));
                                      }
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='short'">
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      if ( NULL == text_value )
                                      {
                                         status = AXIS2_FAILURE;
                                      }
                                      else
                                      {
                                          status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                    atoi( text_value));
                                      }
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='float'">
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      if ( NULL == text_value )
                                      {
                                         status = AXIS2_FAILURE;
                                      }
                                      else
                                      {
                                          status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                     atof( text_value));
                                      }
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='double'">
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      if ( NULL == text_value )
                                      {
                                         status = AXIS2_FAILURE;
                                      }
                                      else
                                      {
                                          status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                     atof( text_value));
                                      }
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='long'">
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      if ( NULL == text_value )
                                      {
                                         status = AXIS2_FAILURE;
                                      }
                                      else
                                      {
                                          status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                      atol( text_value));
                                      }
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='axiom_node_t*'">
                                      text_value = NULL; /** just to avoid warning */
                                      status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                      current_node);
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='axis2_bool_t'">
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      if ( NULL == text_value )
                                      {
                                         status = AXIS2_FAILURE;
                                      }
                                      else if ( !strcmp ( text_value , "true" ) || !strcmp ( text_value, "TRUE") )
                                      {
                                         status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                      AXIS2_TRUE);
                                      }
                                      else
                                      {
                                         status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                      AXIS2_FALSE);
                                      }
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='axis2_date_time_t*'">
                                      element = (void*)axis2_date_time_create( env);
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      status = AXIS2_DATE_TIME_DESERIALIZE_DATE_TIME( (axis2_date_time_t*)element, env,
                                                                      text_value);
                                      if( AXIS2_FAILURE ==  status)
                                      {
                                          AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in building element <xsl:value-of select="$propertyName"/> "
                                                              " %d :: %s", env->error->error_number,
                                                              AXIS2_ERROR_GET_MESSAGE(env->error));
                                          return AXIS2_FAILURE;
                                      }
                                      status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                   ( <xsl:value-of select="$nativePropertyType"/>)element);
                                    </xsl:when>
                                    <xsl:when test="$nativePropertyType='axis2_base64_binary_t*'">
                                      element = (void*)axis2_base64_binary_create( env);
                                      text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                      status = AXIS2_BASE64_BINARY_SET_ENCODED_BINARY( <xsl:value-of select="$name"/>_impl-> attrib_<xsl:value-of select="$CName"/>, env,
                                                                      text_value);
                                      if( AXIS2_FAILURE ==  status)
                                      {
                                          AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in building element <xsl:value-of select="$propertyName"/> "
                                                              " %d :: %s", env->error->error_number,
                                                              AXIS2_ERROR_GET_MESSAGE(env->error));
                                          return AXIS2_FAILURE;
                                      }
                                      status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                   ( <xsl:value-of select="$nativePropertyType"/>)element);
                                    </xsl:when>
                                    <xsl:otherwise>
                                      <!-- TODO: add other types here -->
                                      /** imposible to handle the request type - so please do it manually */
                                      text_value = NULL;
                                    </xsl:otherwise>
                                 </xsl:choose>
                                 if( AXIS2_FAILURE ==  status)
                                 {
                                     AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in setting the value for <xsl:value-of select="$propertyName"/> "
                                                         " %d :: %s", env->error->error_number,
                                                         AXIS2_ERROR_GET_MESSAGE(env->error));
                                     return AXIS2_FAILURE;
                                 }
                           }
                           <xsl:if test="not(@nillable) and not(@minOccurs=0)">
                               else
                               {
                                   /** this is not a nillable element*/
                                   AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "non nillable or minOuccrs != 0 element <xsl:value-of select="$propertyName"/> missing"
                                                         " %d :: %s", env->error->error_number,
                                                         AXIS2_ERROR_GET_MESSAGE(env->error));
                                   return AXIS2_FAILURE;
                               }
                           </xsl:if>
                        </xsl:when>
                        <xsl:otherwise> <!-- when it is all the way an array -->
                           <xsl:choose>
                             <xsl:when test="../@ordered"> <!-- all the elements should follow this -->
                               for ( i = 0, sequence_broken = 0, tmp_node = current_node = <xsl:choose>
                                             <xsl:when test="position()=1">first_node</xsl:when>
                                             <xsl:otherwise>AXIOM_NODE_GET_NEXT_SIBLING( current_node, env)</xsl:otherwise></xsl:choose>; current_node != NULL; current_node = AXIOM_NODE_GET_NEXT_SIBLING( current_node, env))
                               {
                                  current_element = AXIOM_NODE_GET_DATA_ELEMENT( current_node, env);
                                  qname = AXIOM_ELEMENT_GET_QNAME( current_element, env, current_node);
                                  element_qname = axis2_qname_create( env, "<xsl:value-of select="$propertyName"/>", "<xsl:value-of select="@nsuri"/>","<xsl:choose>
                                                                   <xsl:when test="@prefix!=''"><xsl:value-of select="@prefix"/></xsl:when>
                                                                   <xsl:when test="@nsuri=../@nsuri"><xsl:value-of select="../@nsprefix"/></xsl:when></xsl:choose>");
                                  if ( axis2_qname_equals( element_qname, env, qname) )
                                  {
                                      if( sequence_broken)
                                      {
                                        /** found element out of order */
                                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "elements found out of order for array<xsl:value-of select="$propertyName"/> missing"
                                                         " %d :: %s", env->error->error_number,
                                                         AXIS2_ERROR_GET_MESSAGE(env->error));
                                        return AXIS2_FAILURE;
                                      }
                                      tmp_node = current_node; /** always update the current node */
                                      element_found = 1;
                                      <!-- changes to following choose tag should be changed in another 2 places -->
                                     <xsl:choose>
                                        <xsl:when test="@ours">
                                          element = (void*)axis2_<xsl:value-of select="@type"/>_create( env);
                                          status =  <xsl:value-of select="$nativeCapspropertyType"/>_BUILD_OM( ( <xsl:value-of select="$nativePropertyType"/>)element, env,
                                                                             AXIOM_NODE_GET_FIRST_CHILD(current_node, env)==NULL?current_node:AXIOM_NODE_GET_FIRST_CHILD(current_node, env));
                                          if( AXIS2_FAILURE ==  status)
                                          {
                                              AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in building element <xsl:value-of select="$propertyName"/> "
                                                                  " %d :: %s", env->error->error_number,
                                                                  AXIS2_ERROR_GET_MESSAGE(env->error));
                                              return AXIS2_FAILURE;
                                          }
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_char_t*'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)text_value);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_qname_t*'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)axis2_qname_create_from_string(env, text_value));
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_uri_t*'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)axis2_uri_parse_string(env, text_value));
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='char'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          /** we keeps ints in arrays from their pointers */
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = (char)atoi(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='int'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          /** we keeps ints in arrays from their pointers */
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = atoi(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='short'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          /** we keeps ints in arrays from their pointers */
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = atoi(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='float'">
                                          /** we keeps float in arrays from their pointers */
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = atof(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='double'">
                                          /** we keeps float in arrays from their pointers */
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = atof(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='long'">
                                          /** we keeps long in arrays from their pointers */
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = atol(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axiom_node_t*'">
                                          text_value = NULL; /** just to avoid warning */
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)current_node);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_bool_t'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          if ( !strcmp ( text_value , "true" ) || !strcmp ( text_value, "TRUE") )
                                          {
                                             AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)AXIS2_TRUE);
                                          }
                                          else
                                          {
                                             AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)AXIS2_FALSE);
                                          }
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_date_time_t*'">
                                          element = (void*)axis2_date_time_create( env);
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          status = AXIS2_DATE_TIME_DESERIALIZE_DATE_TIME( (axis2_date_time_t*)element, env,
                                                                          text_value);
                                          if( AXIS2_FAILURE ==  status)
                                          {
                                              AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in building element <xsl:value-of select="$propertyName"/> "
                                                                  " %d :: %s", env->error->error_number,
                                                                  AXIS2_ERROR_GET_MESSAGE(env->error));
                                              return AXIS2_FAILURE;
                                          }
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_base64_binary_t*'">
                                          element = (void*)axis2_base64_binary_create( env);
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          status = AXIS2_BASE64_BINARY_SET_ENCODED_BINARY( <xsl:value-of select="$name"/>_impl-> attrib_<xsl:value-of select="$CName"/>, env,
                                                                          text_value);
                                          if( AXIS2_FAILURE ==  status)
                                          {
                                              AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in building element <xsl:value-of select="$propertyName"/> "
                                                                  " %d :: %s", env->error->error_number,
                                                                  AXIS2_ERROR_GET_MESSAGE(env->error));
                                              return AXIS2_FAILURE;
                                          }
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:otherwise>
                                          <!-- TODO: add other types here -->
                                          /** imposible to handle the request type - so please do it manually */
                                          text_value = NULL;
                                        </xsl:otherwise>
                                     </xsl:choose>
                                     status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                   arr_list);
                                     if( AXIS2_FAILURE ==  status)
                                     {
                                         AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in setting the value for <xsl:value-of select="$propertyName"/> "
                                                             " %d :: %s", env->error->error_number,
                                                             AXIS2_ERROR_GET_MESSAGE(env->error));
                                         return AXIS2_FAILURE;
                                     }

                                     i ++;
                                  }
                                  else
                                  {
                                      sequence_broken = 1;
                                  }
                               }
                               current_node = tmp_node;

                             </xsl:when>
                             <xsl:otherwise> <!-- otherwse for "../@ordered" -->

                               /**
                                 * because elements are not ordered we should surf all the sibling to pick the right one
                                 */
                               for ( i = 0, current_node = first_node; current_node != NULL; current_node = AXIOM_NODE_GET_NEXT_SIBLING( current_node, env))
                               {
                                  current_element = AXIOM_NODE_GET_DATA_ELEMENT( current_node, env);
                                  qname = AXIOM_ELEMENT_GET_QNAME( current_element, env, current_node);
                                  element_qname = axis2_qname_create( env, "<xsl:value-of select="$propertyName"/>", "<xsl:value-of select="@nsuri"/>","<xsl:choose>
                                                                   <xsl:when test="@prefix!=''"><xsl:value-of select="@prefix"/></xsl:when>
                                                                   <xsl:when test="@nsuri=../@nsuri"><xsl:value-of select="../@nsprefix"/></xsl:when></xsl:choose>");
                                  if ( axis2_qname_equals( element_qname, env, qname)
                                  {
                                       /** found the requried element */
                                       element_found = 1;
                                      <!-- changes to following choose tag should be changed in another 2 places -->
                                     <xsl:choose>
                                        <xsl:when test="@ours">
                                          element = (void*)axis2_<xsl:value-of select="@type"/>_create( env);
                                          status =  <xsl:value-of select="$nativeCapspropertyType"/>_BUILD_OM( ( <xsl:value-of select="$nativePropertyType"/>)element, env,
                                                                             AXIOM_NODE_GET_FIRST_CHILD(current_node, env)==NULL?current_node:AXIOM_NODE_GET_FIRST_CHILD(current_node, env));
                                          if( AXIS2_FAILURE ==  status)
                                          {
                                              AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in building element <xsl:value-of select="$propertyName"/> "
                                                                  " %d :: %s", env->error->error_number,
                                                                  AXIS2_ERROR_GET_MESSAGE(env->error));
                                              return AXIS2_FAILURE;
                                          }
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_char_t*'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)text_value);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_qname_t*'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)axis2_qname_create_from_string(env, text_value));
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_uri_t*'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)axis2_uri_parse_string(env, text_value));
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='char'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          /** we keeps ints in arrays from their pointers */
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = (char)atoi(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>>
                                        <xsl:when test="$nativePropertyType='int'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          /** we keeps ints in arrays from their pointers */
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = atoi(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='short'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          /** we keeps ints in arrays from their pointers */
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = atoi(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='float'">
                                          /** we keeps float in arrays from their pointers */
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = atof(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='double'">
                                          /** we keeps float in arrays from their pointers */
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = atof(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='long'">
                                          /** we keeps long in arrays from their pointers */
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          element = AXIS2_MALLOC( env-> allocator, 64);
                                          (*(<xsl:value-of select="$nativePropertyType"/>*)element) = atol(text_value);
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axiom_node_t*'">
                                          text_value = NULL; /** just to avoid warning */
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)current_node);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_bool_t'">
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          if ( !strcmp ( text_value , "true" ) || !strcmp ( text_value, "TRUE") )
                                          {
                                             AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)AXIS2_TRUE);
                                          }
                                          else
                                          {
                                             AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, (void*)AXIS2_FALSE);
                                          }
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_date_time_t*'">
                                          element = (void*)axis2_date_time_create( env);
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          status = AXIS2_DATE_TIME_DESERIALIZE_DATE_TIME( (axis2_date_time_t*)element, env,
                                                                          text_value);
                                          if( AXIS2_FAILURE ==  status)
                                          {
                                              AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in building element <xsl:value-of select="$propertyName"/> "
                                                                  " %d :: %s", env->error->error_number,
                                                                  AXIS2_ERROR_GET_MESSAGE(env->error));
                                              return AXIS2_FAILURE;
                                          }
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:when test="$nativePropertyType='axis2_base64_binary_t*'">
                                          element = (void*)axis2_base64_binary_create( env);
                                          text_value = AXIOM_ELEMENT_GET_TEXT(current_element, env, current_node );
                                          status = AXIS2_BASE64_BINARY_SET_ENCODED_BINARY( <xsl:value-of select="$name"/>_impl-> attrib_<xsl:value-of select="$CName"/>, env,
                                                                          text_value);
                                          if( AXIS2_FAILURE ==  status)
                                          {
                                              AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in building element <xsl:value-of select="$propertyName"/> "
                                                                  " %d :: %s", env->error->error_number,
                                                                  AXIS2_ERROR_GET_MESSAGE(env->error));
                                              return AXIS2_FAILURE;
                                          }
                                          AXIS2_ARRAY_LIST_ADD_AT( arr_list, env, i, element);
                                        </xsl:when>
                                        <xsl:otherwise>
                                          <!-- TODO: add other types here -->
                                          /** imposible to handle the request type - so please do it manually */
                                          text_value = NULL;
                                        </xsl:otherwise>
                                     </xsl:choose>
                                     status = <xsl:value-of select="$axis2_capsname"/>_SET_<xsl:value-of select="$CapsCName"/>( <xsl:value-of select="$name"/>, env,
                                                                   arr_list);
                                     if( AXIS2_FAILURE ==  status)
                                     {
                                         AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "failed in setting the value for <xsl:value-of select="$propertyName"/> "
                                                             " %d :: %s", env->error->error_number,
                                                             AXIS2_ERROR_GET_MESSAGE(env->error));
                                         return AXIS2_FAILURE;
                                     }

                                     i ++;
                                  }
                               }
                             </xsl:otherwise> <!--closing otherwise for "../@ordered" -->
                           </xsl:choose> <!-- chooses for ordered or not ordered -->
                        </xsl:otherwise> <!-- closing when it is all the way an array -->
                      </xsl:choose> <!-- check array or not -->
                   </xsl:otherwise> <!-- closing when it is an element not(@attribute) -->
                 </xsl:choose> <!--- chooosing for element or attribute -->
            </xsl:for-each> <!-- closing for each property -->

          return status;
       }

        axiom_node_t* AXIS2_CALL
        <xsl:value-of select="$axis2_name"/>_parse_om (
                <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                const axis2_env_t *env, axiom_node_t* parent)
        {
            <xsl:value-of select="$axis2_name"/>_impl_t *<xsl:value-of select="$name"/>_impl = NULL;

            <!-- first declration part -->
            axiom_namespace_t *ns1 = NULL;
            <xsl:for-each select="property/@attribute">
             <xsl:if test="position()=1">
               axiom_attribute_t *text_attri = NULL;
               axiom_element_t *parent_element = NULL;
             </xsl:if>
            </xsl:for-each>
            <xsl:for-each select="property/@isarray">
             <xsl:if test="position()=1">
               long i = 0;
               long count = 0;
               void *element = NULL;
             </xsl:if>
            </xsl:for-each>

            <xsl:if test="property">
              <xsl:if test="not(property/@attribute) ">
                <!-- this effort is to find atleast onething without @ours -->
                <xsl:for-each select="property[not(@ours) or @ours='']">
                  <xsl:if test="position()=1">
                     axis2_char_t* text_value = NULL;
                  </xsl:if>
                </xsl:for-each>
              </xsl:if>
            </xsl:if>
            <xsl:for-each select="property">
             <xsl:if test="position()=1">
               <!--axiom_node_t *current_node = NULL;
               axiom_element_t *current_element = NULL;-->
             </xsl:if>
            </xsl:for-each>
            <xsl:if test="property and (not(property/@attribute) or property/@attribute='')">
               axiom_node_t *current_node = NULL;
               axiom_element_t *current_element = NULL;
            </xsl:if>
            AXIS2_ENV_CHECK(env, NULL);
            <xsl:value-of select="$name"/>_impl = AXIS2_INTF_TO_IMPL(<xsl:value-of select="$name"/>);

            ns1 = axiom_namespace_create (env,
                                         "<xsl:value-of select="$nsuri"/>",
                                         "<xsl:value-of select="$nsprefix"/>");
            <xsl:if test="not(@type)">
              <xsl:for-each select="property">
               <xsl:if test="position()=1">
                if( parent == NULL)
                {
                    current_element = axiom_element_create (env, parent, "<xsl:value-of select="$originalName"/>", ns1 , &amp;current_node);
                    AXIOM_ELEMENT_SET_NAMESPACE ( current_element, env, ns1, current_node);
                    parent = current_node;
                }
               </xsl:if>
              </xsl:for-each>
            </xsl:if>
            <xsl:for-each select="property/@attribute">
             <xsl:if test="position()=1">
                 parent_element = AXIOM_NODE_GET_DATA_ELEMENT( parent, env);
             </xsl:if>
            </xsl:for-each>


            <xsl:for-each select="property">
              <xsl:variable name="propertyType">
                 <xsl:choose>
                   <xsl:when test="@isarray">axis2_array_list_t*</xsl:when>
                   <xsl:when test="not(@type)">axiom_node_t*</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">axis2_<xsl:value-of select="@type"/>_t*</xsl:when>
                   <xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="capspropertyType">
                 <xsl:choose>
                   <xsl:when test="@isarray">AXIS2_ARRAY_LIST</xsl:when>
                   <xsl:when test="not(@type)">AXIOM_NODE</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">AXIS2_<xsl:value-of select="@caps-type"/></xsl:when>
                   <xsl:otherwise><xsl:value-of select="@caps-type"/></xsl:otherwise> <!-- this will not be used -->
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="nativePropertyType"> <!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="not(@type)">axiom_node_t*</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">axis2_<xsl:value-of select="@type"/>_t*</xsl:when>
                   <xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="nativeCapspropertyType"><!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="not(@type)">AXIOM_NODE</xsl:when> <!-- these are anonymous -->
                   <xsl:when test="@ours">AXIS2_<xsl:value-of select="@caps-type"/></xsl:when>
                   <xsl:otherwise><xsl:value-of select="@caps-type"/></xsl:otherwise> <!-- this will not be used -->
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="propertyName"><xsl:value-of select="@name"></xsl:value-of></xsl:variable>
              <xsl:variable name="CName"><xsl:value-of select="@cname"></xsl:value-of></xsl:variable>

              <xsl:variable name="attriName"><!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="@isarray">(<xsl:value-of select="$nativePropertyType"/>)element</xsl:when>
                   <xsl:otherwise><xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>
              <xsl:variable name="justAttriName"><!--these are used in arrays to take the native type-->
                 <xsl:choose>
                   <xsl:when test="@isarray">element</xsl:when>
                   <xsl:otherwise><xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/></xsl:otherwise>
                 </xsl:choose>
              </xsl:variable>

              <xsl:choose>
                <xsl:when test="@attribute">
                      <!-- here only simple type possible -->
                      <xsl:choose>
                        <!-- add int s -->
                        <xsl:when test="$nativePropertyType='int'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           sprintf ( text_value, "%d", <xsl:value-of select="$attriName"/> );
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add int s -->
                        <xsl:when test="$nativePropertyType='char'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           sprintf ( text_value, "%c", <xsl:value-of select="$attriName"/> );
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add short s -->
                        <xsl:when test="$nativePropertyType='short'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           sprintf ( text_value, "%d", <xsl:value-of select="$attriName"/> );
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add long s -->
                        <xsl:when test="$nativePropertyType='long'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           sprintf ( text_value, "%d", (int)<xsl:value-of select="$attriName"/> );
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add float s -->
                        <xsl:when test="$nativePropertyType='float'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           sprintf ( text_value, "%f", <xsl:value-of select="$attriName"/> );
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add double s -->
                        <xsl:when test="$nativePropertyType='double'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           sprintf ( text_value, "%f", <xsl:value-of select="$attriName"/> );
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add axis2_char_t s -->
                        <xsl:when test="$nativePropertyType='axis2_char_t*'">
                           text_value = <xsl:value-of select="$attriName"/>;
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                        </xsl:when>

                        <!-- add axis2_uri_t s -->
                        <xsl:when test="$nativePropertyType='axis2_uri_t*'">
                           text_value = axis2_uri_to_string(<xsl:value-of select="$attriName"/>, env, AXIS2_URI_UNP_OMITUSERINFO);
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                        </xsl:when>

                        <!-- add axis2_qname_t s -->
                        <xsl:when test="$nativePropertyType='axis2_qname_t*'">
                           text_value = axis2_qname_to_string(<xsl:value-of select="$attriName"/>, env);
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                        </xsl:when>

                        <!-- add axis2_bool_t s -->
                        <xsl:when test="$nativePropertyType='axis2_bool_t'">
                           text_value = (<xsl:value-of select="$attriName"/>)?"true":"false";
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                        </xsl:when>
                        <!-- add axis2_date_time_t s -->
                        <xsl:when test="$nativePropertyType='axis2_date_time_t*'">
                           text_value =  AXIS2_DATE_TIME_SERIALIZE_DATE_TIME( <xsl:value-of select="$attriName"/>, env);
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                        </xsl:when>
                        <!-- add axis2_base64_binary_t s -->
                        <xsl:when test="$nativePropertyType='axis2_base64_binary_t*'">
                           text_value =  AXIS2_BASE64_BINARY_GET_ENCODED_BINARY( <xsl:value-of select="$attriName"/>, env);
                           text_attri = axiom_attribute_create (env, "<xsl:value-of select="$propertyName"/>", text_value, ns1);
                           AXIOM_ELEMENT_ADD_ATTRIBUTE (parent_element, env, text_attri, parent);
                        </xsl:when>
                        <xsl:otherwise>
                          <!--TODO: add new attributes types -->
                          /** can not handle the attribute type <xsl:value-of select="$nativePropertyType"/>*/
                          text_value = NULL;
                          parent_element = NULL;
                          text_attri = NULL;
                        </xsl:otherwise>
                      </xsl:choose>
                </xsl:when>
                <xsl:otherwise>
                   <!-- handles arrays -->
                   <xsl:if test="@isarray">
                     /**
                      * parsing <xsl:value-of select="$CName"/> array
                      */
                     if ( <xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/> != NULL)
                     {
                         count = AXIS2_ARRAY_LIST_SIZE( <xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/>, env);
                         for( i = 0; i &lt; count; i ++)
                         {
                            element = AXIS2_ARRAY_LIST_GET( <xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/>, env, i);
                    </xsl:if>
                     <!-- for each non attribute properties there will always be an element-->
                     /**
                      * parsing <xsl:value-of select="$propertyName"/> element
                      */
                     current_element = axiom_element_create (env, parent, "<xsl:value-of select="$propertyName"/>", ns1 , &amp;current_node);
                     AXIOM_ELEMENT_SET_NAMESPACE ( current_element, env, ns1, current_node);


                      <!-- how to build all the ours things -->
                      <xsl:choose>
                        <xsl:when test="@ours">
                           <xsl:value-of select="$nativeCapspropertyType"/>_PARSE_OM( <xsl:value-of select="$attriName"/>, env, current_node);
                        </xsl:when>

                        <!-- add int s -->
                        <xsl:when test="$nativePropertyType='int'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           <xsl:choose>
                             <xsl:when test="@isarray">
                               sprintf ( text_value, "%d", *((<xsl:value-of select="$nativePropertyType"/>*)element) );
                             </xsl:when>
                             <xsl:otherwise>
                               sprintf ( text_value, "%d", <xsl:value-of select="$attriName"/> );
                             </xsl:otherwise>
                           </xsl:choose>
                           AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add char s -->
                        <xsl:when test="$nativePropertyType='char'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           <xsl:choose>
                             <xsl:when test="@isarray">
                               sprintf ( text_value, "%c", *((<xsl:value-of select="$nativePropertyType"/>*)element) );
                             </xsl:when>
                             <xsl:otherwise>
                               sprintf ( text_value, "%c", <xsl:value-of select="$attriName"/> );
                             </xsl:otherwise>
                           </xsl:choose>
                           AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add short s -->
                        <xsl:when test="$nativePropertyType='short'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           <xsl:choose>
                             <xsl:when test="@isarray">
                               sprintf ( text_value, "%d", *((<xsl:value-of select="$nativePropertyType"/>*)element) );
                             </xsl:when>
                             <xsl:otherwise>
                               sprintf ( text_value, "%d", <xsl:value-of select="$attriName"/> );
                             </xsl:otherwise>
                           </xsl:choose>
                           AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>


                        <!-- NOTE: arrays for long, float, int are handled differently. they are stored in pointers -->
                        <!-- add long s -->
                        <xsl:when test="$nativePropertyType='long'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           <xsl:choose>
                             <xsl:when test="@isarray">
                               sprintf ( text_value, "%d", (int)*((<xsl:value-of select="$nativePropertyType"/>*)element) );
                             </xsl:when>
                             <xsl:otherwise>
                               sprintf ( text_value, "%d", (int)<xsl:value-of select="$attriName"/> );
                             </xsl:otherwise>
                           </xsl:choose>
                           AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add float s -->
                        <xsl:when test="$nativePropertyType='float'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           <xsl:choose>
                             <xsl:when test="@isarray">
                               sprintf ( text_value, "%f", *((<xsl:value-of select="$nativePropertyType"/>*)element) );
                             </xsl:when>
                             <xsl:otherwise>
                               sprintf ( text_value, "%f", <xsl:value-of select="$attriName"/> );
                             </xsl:otherwise>
                           </xsl:choose>
                           AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add double s -->
                        <xsl:when test="$nativePropertyType='double'">
                           text_value = (axis2_char_t*) AXIS2_MALLOC ( env-> allocator, sizeof ( axis2_char_t) * 64);
                           <xsl:choose>
                             <xsl:when test="@isarray">
                               sprintf ( text_value, "%f", *((<xsl:value-of select="$nativePropertyType"/>*)element) );
                             </xsl:when>
                             <xsl:otherwise>
                               sprintf ( text_value, "%f", <xsl:value-of select="$attriName"/> );
                             </xsl:otherwise>
                           </xsl:choose>
                           AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                           AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!-- add axis2_char_t s -->
                        <xsl:when test="$nativePropertyType='axis2_char_t*'">
                           text_value = <xsl:value-of select="$attriName"/>;
                           AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                        </xsl:when>

                        <!-- add axis2_uri_t s -->
                        <xsl:when test="$nativePropertyType='axis2_uri_t*'">
                           text_value = axis2_uri_to_string(<xsl:value-of select="$attriName"/>, env, AXIS2_URI_UNP_OMITUSERINFO);
                           AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                        </xsl:when>

                        <!-- add axis2_qname_t s -->
                        <xsl:when test="$nativePropertyType='axis2_qname_t*'">
                           text_value = axis2_qname_to_string(<xsl:value-of select="$attriName"/>, env);
                           AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                        </xsl:when>

                        <!-- add nodes -->
                        <xsl:when test="$nativePropertyType='axiom_node_t*'">
                           text_value = NULL; /** just to bypass the warning unused variable */
                           AXIOM_NODE_ADD_CHILD( current_node, env, <xsl:value-of select="$attriName"/>);
                        </xsl:when>

                        <xsl:when test="$nativePropertyType='axis2_date_time_t*'">
                          text_value = AXIS2_DATE_TIME_SERIALIZE_DATE_TIME( <xsl:value-of select="$attriName"/>, env);
                          AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                          AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <xsl:when test="$propertyType='axis2_base64_binary_t*'">
                          text_value = AXIS2_BASE64_BINARY_GET_ENCODED_BINARY(<xsl:value-of select="$attriName"/>, env);
                          AXIOM_ELEMENT_SET_TEXT(current_element, env, text_value, current_node);
                          AXIS2_FREE( env-> allocator, text_value);
                        </xsl:when>

                        <!--TODO: This should be extended for all the types that should be freed.. -->
                        <xsl:otherwise>
                          /** This is an unknown type or a primitive. handle this manually for unknown type */
                          text_value = NULL;
                        </xsl:otherwise>
                      </xsl:choose>

                   <!-- close tags arrays -->
                   <xsl:if test="@isarray">
                         }
                         AXIS2_ARRAY_LIST_FREE( <xsl:value-of select="$name"/>_impl->attrib_<xsl:value-of select="$CName"/>, env);
                     }
                   </xsl:if>
                </xsl:otherwise> <!-- othewise for non attributes -->
              </xsl:choose>
            </xsl:for-each>
            return parent;
        }

        <xsl:for-each select="property">
            <xsl:variable name="propertyType">
               <xsl:choose>
                    <xsl:when test="@isarray">axis2_array_list_t*</xsl:when>
                    <xsl:when test="not(@type)">axiom_node_t*</xsl:when> <!-- these are anonymous -->
                    <xsl:when test="@ours">axis2_<xsl:value-of select="@type"/>_t*</xsl:when>
                    <xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
               </xsl:choose>
            </xsl:variable>
            <xsl:variable name="propertyName"><xsl:value-of select="@name"></xsl:value-of></xsl:variable>
            <xsl:variable name="CName"><xsl:value-of select="@cname"></xsl:value-of></xsl:variable>

            /**
             * getter for <xsl:value-of select="$propertyName"/>.
             */
            <xsl:value-of select="$propertyType"/> AXIS2_CALL
            <xsl:value-of select="$axis2_name"/>_get_<xsl:value-of select="$CName"/>(
                    <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                    const axis2_env_t *env)
             {
                <xsl:value-of select="$axis2_name"/>_impl_t *<xsl:value-of select="$name"/>_impl = NULL;

                AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

                <xsl:value-of select="$name"/>_impl = AXIS2_INTF_TO_IMPL(<xsl:value-of select="$name"/>);

                return <xsl:value-of select="$name"/>_impl-> attrib_<xsl:value-of select="$CName"/>;
             }

            /**
             * setter for <xsl:value-of select="$propertyName"/>
             */
            axis2_status_t AXIS2_CALL
            <xsl:value-of select="$axis2_name"/>_set_<xsl:value-of select="$CName"/>(
                    <xsl:value-of select="$axis2_name"/>_t*<xsl:text> </xsl:text><xsl:value-of select="$name"/>,
                    const axis2_env_t *env,
                    <xsl:value-of select="$propertyType"/><xsl:text> </xsl:text> param_<xsl:value-of select="$CName"/>)
             {
                <xsl:value-of select="$axis2_name"/>_impl_t *<xsl:value-of select="$name"/>_impl = NULL;
                <xsl:if test="@isarray">
                 int size = 0;
                </xsl:if>

                AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

                <xsl:value-of select="$name"/>_impl = AXIS2_INTF_TO_IMPL(<xsl:value-of select="$name"/>);

                <xsl:if test="@isarray">
                  size = AXIS2_ARRAY_LIST_SIZE( param_<xsl:value-of select="$CName"/>, env);
                  <xsl:if test="not(@unbound)">
                      if ( size &gt; <xsl:value-of select="@maxOccurs"/> )
                      {
                          AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "<xsl:value-of select="$propertyName"/> has exceed the maxOccurs(<xsl:value-of select="@maxOccurs"/>)"
                                                     " %d :: %s", env->error->error_number,
                                                     AXIS2_ERROR_GET_MESSAGE(env->error));
                          return AXIS2_FAILURE;
                      }
                  </xsl:if>
                  if ( size &lt; <xsl:value-of select="@minOccurs"/> )
                  {
                      AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "<xsl:value-of select="$propertyName"/> has less than minOccurs(<xsl:value-of select="@minOccurs"/>)"
                                                 " %d :: %s", env->error->error_number,
                                                 AXIS2_ERROR_GET_MESSAGE(env->error));
                      return AXIS2_FAILURE;
                  }
                </xsl:if>
                <xsl:if test="not(@nillable) and @ours">
                  if( NULL == param_<xsl:value-of select="$CName"/> )
                  {
                      AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "<xsl:value-of select="$propertyName"/> is NULL, but not a nullable element"
                                                 " %d :: %s", env->error->error_number,
                                                 AXIS2_ERROR_GET_MESSAGE(env->error));
                      return AXIS2_FAILURE;
                  }
                </xsl:if>
                <xsl:value-of select="$name"/>_impl-> attrib_<xsl:value-of select="$CName"/> = param_<xsl:value-of select="$CName"/>;
                return AXIS2_SUCCESS;
             }
        </xsl:for-each>

    </xsl:template>

</xsl:stylesheet>
