if COND_XML
  MAYBE_XML = xml
endif
if COND_CORE
  MAYBE_CORE = core
endif
if COND_TEST
  MAYBE_TEST = test
endif

#SUBDIRS = util common xml core wsdl test
SUBDIRS = util common $(MAYBE_XML) wsdl $(MAYBE_CORE) $(MAYBE_TEST)
#SUBDIRS = util common test
