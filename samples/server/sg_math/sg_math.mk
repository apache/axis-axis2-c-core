# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
# 
#   http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
echo:
	@cl.exe /nologo /D "WIN32" /D "AXIS2_DECLARE_EXPORT" /D "_WINDOWS" /D "_MBCS" add.c add_skeleton.c /I.\..\..\..\..\include /c
	@link.exe /nologo add.obj /LIBPATH:.\..\..\..\..\lib axiom.lib axutil.lib axis2_engine.lib axis2_parser.lib /DLL /OUT:add.dll

	
	@cl.exe /nologo /D "WIN32" /D "AXIS2_DECLARE_EXPORT" /D "_WINDOWS" /D "_MBCS" div.c div_skeleton.c /I.\..\..\..\..\include /c
	@link.exe /nologo div.obj /LIBPATH:.\..\..\..\..\lib axiom.lib axis2_util.lib axis2_engine.lib axis2_parser.lib /DLL /OUT:div.dll


	@cl.exe /nologo /D "WIN32" /D "AXIS2_DECLARE_EXPORT" /D "_WINDOWS" /D "_MBCS" sub.c sub_skeleton.c /I.\..\..\..\..\include /c
	@link.exe /nologo sub.obj /LIBPATH:.\..\..\..\..\lib axiom.lib axutil.lib axis2_engine.lib axis2_parser.lib /DLL /OUT:sub.dll


	@cl.exe /nologo /D "WIN32" /D "AXIS2_DECLARE_EXPORT" /D "_WINDOWS" /D "_MBCS" mul.c mul_skeleton.c /I.\..\..\..\..\include /c
	@link.exe /nologo mul.obj /LIBPATH:.\..\..\..\..\lib axiom.lib axutil.lib axis2_engine.lib axis2_parser.lib /DLL /OUT:mul.dll
