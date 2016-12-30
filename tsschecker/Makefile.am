AM_CFLAGS = $(libplist_CFLAGS) $(libfragmentzip_CFLAGS) $(libcurl_CFLAGS) $(libcrypto_CFLAGS) $(libirecovery_CFLAGS)
AM_LDFLAGS = $(libplist_LIBS) $(libfragmentzip_LIBS) $(libcurl_LIBS) $(libcrypto_LIBS) $(libirecovery_LIBS) -lm

if NOMAIN
noinst_LTLIBRARIES = libtsschecker.la
libtsschecker_la_SOURCES = tsschecker.c tss.c jsmn.c download.c
libtsschecker_la_CFLAGS = $(AM_CFLAGS)
libtsschecker_la_LIBADD = $(AM_LDFLAGS)
else
bin_PROGRAMS	= tsschecker
tsschecker_CFLAGS = $(AM_CFLAGS)
tsschecker_LDADD = $(AM_LDFLAGS)
tsschecker_SOURCES	= tsschecker.c tss.c jsmn.c download.c main.c
endif
