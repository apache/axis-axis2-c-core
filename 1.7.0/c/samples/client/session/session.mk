session:
	@cl.exe /nologo /D "WIN32" /D "_WINDOWS" /D "AXIS2_DECLARE_EXPORT" /D "_MBCS" *.C /I.\..\..\..\include /c
	@link.exe /nologo session.obj /LIBPATH:.\..\..\..\lib axiom.lib axutil.lib axis2_engine.lib axis2_parser.lib /OUT:session.exe

	



