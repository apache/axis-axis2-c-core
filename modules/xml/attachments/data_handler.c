/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *data_handler
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_data_handler.h>

/** 
 * @brief Flow struct impl
 *	Axis2 Flow impl  
 */
typedef struct axis2_data_handler_impl
{
	axis2_data_handler_t data_handler;
    /**
     * Field list
     */
    /*   
	axis2_char_t type;
    *void handler;
	*/
} axis2_data_handler_impl_t;



#define AXIS2_INTF_TO_IMPL(data_handler) ((axis2_data_handler_impl_t *)(data_handler))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_data_handler_free (axis2_data_handler_t *data_handler, axis2_env_t **env);

axis2_char_t * AXIS2_CALL
axis2_data_handler_get_content_type (axis2_data_handler_t *data_handler, axis2_env_t **env);

axis2_byte_t * AXIS2_CALL
axis2_data_handler_get_input_stream (axis2_data_handler_t *data_handler, axis2_env_t **env); 

axis2_status_t AXIS2_CALL
axis2_data_handler_write_to(axis2_data_handler_t *data_handler, axis2_env_t **env, \
							axis2_byte_t output_stream);

/************************** End of Function headers ************************/

AXIS2_DECLARE(axis2_data_handler_t *)
axis2_data_handler_create (axis2_env_t **env, void *data, axis2_char_t *data_type)
{
    axis2_data_handler_impl_t *data_handler_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	data_handler_impl = (axis2_data_handler_impl_t *) AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_data_handler_impl_t));
		
	if(NULL == data_handler_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	
    data_handler_impl->data_handler.ops = NULL;
    
	data_handler_impl->data_handler.ops = AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_data_handler_ops_t));
	if(NULL == data_handler_impl->data_handler.ops)
    {
        axis2_data_handler_free(&(data_handler_impl->data_handler), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

	data_handler_impl->data_handler.ops->free =  axis2_data_handler_free;
    data_handler_impl->data_handler.ops->get_content_type = axis2_data_handler_get_content_type;
    data_handler_impl->data_handler.ops->get_input_stream = axis2_data_handler_get_input_stream;
    data_handler_impl->data_handler.ops->write_to = axis2_data_handler_write_to;
	return &(data_handler_impl->data_handler);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL
axis2_data_handler_free (axis2_data_handler_t *data_handler, axis2_env_t **env)
{
    axis2_data_handler_impl_t *data_handler_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    data_handler_impl = AXIS2_INTF_TO_IMPL(data_handler);
    
    if(NULL != data_handler->ops)
    {
        AXIS2_FREE((*env)->allocator, data_handler->ops);
        data_handler->ops = NULL;
    }
    
    if(data_handler_impl)
    {
        AXIS2_FREE((*env)->allocator, data_handler_impl);
        data_handler_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_data_handler_get_content_type (axis2_data_handler_t *data_handler, axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);       
	
	/*@TODO implement the following*/
	return "";
}

axis2_byte_t * AXIS2_CALL
axis2_data_handler_get_input_stream (axis2_data_handler_t *data_handler, axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);       
	
	/*@TODO implement the following*/
	/**
	InputStream getInputStream()
	final DataContentHandler dch = getDataContentHandler();
             if (dch == null) {
                 throw new UnsupportedDataTypeException();
           }
             final PipedInputStream is = new PipedInputStream();
             Thread thread = new Thread() {
                 public void run() {
                     try {
                         PipedOutputStream os = new PipedOutputStream(is);
                        is.connect(os);
                         try {
                             dch.writeTo(data, mimeType, os);
                         } finally {
                            os.close();
                         }
                     } catch (IOException e) {
                         // ignore, per spec - doh!
                     }
                 }
             };
             thread.start();
             return is;
	*/		
	return "";
}

axis2_status_t AXIS2_CALL
axis2_data_handler_write_to(axis2_data_handler_t *data_handler, axis2_env_t **env, \
							axis2_byte_t output_stream)
{
	/**
          if (ds instanceof ObjectDataSource) {
              ObjectDataSource ods = (ObjectDataSource) ds;
              DataContentHandler dch = getDataContentHandler();
              if (dch == null) {
                  throw new UnsupportedDataTypeException();
              }
              dch.writeTo(ods.data, ods.mimeType, os);
          } else {
              byte[] buffer = new byte[1024];
              InputStream is = getInputStream();
              try {
                  int count;
                  while ((count = is.read(buffer)) != -1) {
                      os.write(buffer, 0, count);
                  }
              } finally {
                  is.close();
              }
          }
	*/  
    return AXIS2_SUCCESS;
}

/* Private Functions */


/* ========================================================================= */
/*
55      public DataSource getDataSource() {
56          return ds;
57      }
58  
59      public String getName() {
60          return ds.getName();
61      }
62  

	
66  
70  

92  
93      public OutputStream getOutputStream() throws IOException {
94          return ds.getOutputStream();
95      }
96  
97      public synchronized DataFlavor[] getTransferDataFlavors() {
98          return getDataContentHandler().getTransferDataFlavors();
99      }
100 
101     public boolean isDataFlavorSupported(DataFlavor flavor) {
102         DataFlavor[] flavors = getTransferDataFlavors();
103         for (int i = 0; i < flavors.length; i++) {
104             DataFlavor dataFlavor = flavors[i];
105             if (dataFlavor.equals(flavor)) {
106                 return true;
107             }
108         }
109         return false;
110     }
111 
112     public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException, IOException {
113         DataContentHandler dch = getDataContentHandler();
114         if (dch != null) {
115             return dch.getTransferData(flavor, ds);
116         } else if (this.flavor.match(flavor)) {
117             if (ds instanceof ObjectDataSource) {
118                 return ((ObjectDataSource) ds).data;
119             } else {
120                 return ds.getInputStream();
121             }
122         } else {
123             throw new UnsupportedFlavorException(flavor);
124         }
125     }
126 
127     public CommandInfo[] getPreferredCommands() {
128         return getCommandMap().getPreferredCommands(ds.getContentType());
129     }
130 
131     public CommandInfo[] getAllCommands() {
132         return getCommandMap().getAllCommands(ds.getContentType());
133     }
134 
135     public CommandInfo getCommand(String cmdName) {
136         return getCommandMap().getCommand(ds.getContentType(), cmdName);
137     }
138 
139     public Object getContent() throws IOException {
140         if (ds instanceof ObjectDataSource) {
141             return ((ObjectDataSource) ds).data;
142         } else {
143             DataContentHandler dch = getDataContentHandler();
144             if (dch != null) {
145                 return dch.getContent(ds);
146             } else {
147                 return ds.getInputStream();
148             }
149         }
150     }
151 
152     public Object getBean(CommandInfo cmdinfo) {
153         try {
154             return cmdinfo.getCommandObject(this, this.getClass().getClassLoader());
155         } catch (IOException e) {
156             return null;
157         } catch (ClassNotFoundException e) {
158             return null;
159         }
160     }
161
*/
     /**
163      * A local implementation of DataSouce used to wrap an Object and mime-type.
164      */
/*
165     private class ObjectDataSource implements DataSource {
166         private final Object data;
167         private final String mimeType;
168 
169         public ObjectDataSource(Object data, String mimeType) {
170             this.data = data;
171             this.mimeType = mimeType;
172         }
173 
174         public String getName() {
175             return null;
176         }
177 
178         public String getContentType() {
179             return mimeType;
180         }
181 
182         public InputStream getInputStream() throws IOException {
183             final DataContentHandler dch = getDataContentHandler();
184             if (dch == null) {
185                 throw new UnsupportedDataTypeException();
186             }
187             final PipedInputStream is = new PipedInputStream();
188             Thread thread = new Thread() {
189                 public void run() {
190                     try {
191                         PipedOutputStream os = new PipedOutputStream(is);
192                         is.connect(os);
193                         try {
194                             dch.writeTo(data, mimeType, os);
195                         } finally {
196                             os.close();
197                         }
198                     } catch (IOException e) {
199                         // ignore, per spec - doh!
200                     }
201                 }
202             };
203             thread.start();
204             return is;
205         }
206 
207         public OutputStream getOutputStream() throws IOException {
208             return null;
209         }
210     }
211 
212     public synchronized void setCommandMap(CommandMap commandMap) {
213         this.commandMap = commandMap;
214         this.dch = null;
215     }
216 
217     private synchronized CommandMap getCommandMap() {
218         return commandMap != null ? commandMap : CommandMap.getDefaultCommandMap();
219     }
220
*/
     /**
222      * Search for a DataContentHandler for our mime type.
223      * The search is performed by first checking if a global factory has been set using
224      * {@link #setDataContentHandlerFactory(DataContentHandlerFactory)};
225      * if found then it is called to attempt to create a handler.
226      * If this attempt fails, we then call the command map set using {@link #setCommandMap(CommandMap)}
227      * (or if that has not been set, the default map returned by {@link CommandMap#getDefaultCommandMap()})
228      * to create the handler.
229      *
230      * The resulting handler is cached until the global factory is changed.
231      *
232      * @return
233      */
/*
234     private synchronized DataContentHandler getDataContentHandler() {
235         DataContentHandlerFactory localFactory;
236         synchronized (DataHandler.class) {
237             if (factory != originalFactory) {
238                 // setDCHF was called - clear our cached copy of the DCH and DCHF
239                 dch = null;
240                 originalFactory = factory;
241             }
242             localFactory = originalFactory;
243         }
244         if (dch == null) {
245             String contentType = ds.getContentType();
246             if (localFactory != null) {
247                 dch = localFactory.createDataContentHandler(contentType);
248             }
249             if (dch == null) {
250                 if (commandMap != null) {
251                     dch = commandMap.createDataContentHandler(contentType);
252                 } else {
253                     dch = CommandMap.getDefaultCommandMap().createDataContentHandler(contentType);
254                 }
255             }
256         }
257         return dch;
258     }
259 
261      * This is used to check if the DataContentHandlerFactory has been changed.
262      * This is not specified behaviour but this check is required to make this work like the RI.
264     private DataContentHandlerFactory originalFactory;
265 
266     {
267         synchronized (DataHandler.class) {
268             originalFactory = factory;
269         }
270     }
271 
272     private static DataContentHandlerFactory factory;
273 
275      * Set the DataContentHandlerFactory to use.
276      * If this method has already been called then an Error is raised.
277      *
278      * @param newFactory the new factory
279      * @throws SecurityException if the caller does not have "SetFactory" RuntimePermission
281     public static synchronized void setDataContentHandlerFactory(DataContentHandlerFactory newFactory) {
282         if (factory != null) {
283             throw new Error("javax.activation.DataHandler.setDataContentHandlerFactory has already been defined");
284         }
285         SecurityManager sm = System.getSecurityManager();
286         if (sm != null) {
287             sm.checkSetFactory();
288         }
289         factory = newFactory;
290     }	
*/
