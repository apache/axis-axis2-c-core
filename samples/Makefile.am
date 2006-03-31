samplesdir=$(prefix)/samples
samples_DATA=AUTHORS autogen.sh ChangeLog configure.ac Makefile.am NEWS README configure Makefile.in config.h.in INSTALL COPYING
SUBDIRS = server client
install-data-hook:
	chmod 755 $(samplesdir)/configure $(samplesdir)/autogen.sh
