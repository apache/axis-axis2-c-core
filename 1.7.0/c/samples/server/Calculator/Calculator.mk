echo:
	@cl.exe /nologo /D "WIN32" /D "AXIS2_DECLARE_EXPORT" /D "_WINDOWS" /D "_MBCS" *.C /I.\..\..\..\include /c
	@link.exe /nologo *.obj /LIBPATH:.\..\..\..\lib axiom.lib axutil.lib axis2_engine.lib axis2_parser.lib /DLL /OUT:Calculator.dll
	



