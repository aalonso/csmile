bin_PROGRAMS = csmile

csmile_CFLAGS =              \
    -I. -I$(top_srcdir)     \
    -I$(top_srcdir)/src     \
    $(GOBJECT_CFLAGS)		\
	$(GTHREAD_CFLAGS)		\
	$(GSTREAMER_CFLAGS)
 
csmile_LDADD =         \
    $(GOBJECT_LIBS)	  \
	$(GTHREAD_LIBS)		\
	$(GSTREAMER_LIBS)

csmile_SOURCES = csmile-gst-cam.h	\
	csmile-gst-cam.c	\
	csmile-gtk-ui.h     \
    csmile-gtk-ui.c     \
    main.c    

