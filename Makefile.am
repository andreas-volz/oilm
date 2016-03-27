MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess config.h.in \
		       config.sub configure depcomp install-sh ltmain.sh \
		       missing module.desktop

SUBDIRS = src

ACLOCAL_AMFLAGS = -I m4

EDJE_FLAGS = -v \
	   -id $(top_srcdir)/images \
	   -fd $(top_srcdir)/fonts

# data files for the module
filesdir = $(libdir)/enlightenment/modules/$(MODULE)

files_DATA = module.desktop \
	     e-module-$(MODULE).edj

EXTRA_DIST = $(files_DATA) \
	     images/module_icon.png \
	     fonts/VeraBd.ttf \
	     module.desktop

%.edj:  %.edc
	$(EDJE_CC) $(EDJE_FLAGS) $< $@

clean-local:
	 rm -rf e-module-oilm.edj module.desktop *~
uninstall:
	 rm -rf $(DESTDIR)$(datadir)
	 
