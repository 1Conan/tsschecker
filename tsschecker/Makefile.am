AM_CFLAGS = $(libplist_CFLAGS) $(libfragmentzip_CFLAGS) $(libcurl_CFLAGS) $(libcrypto_CFLAGS)
AM_LDFLAGS = $(libplist_LIBS) $(libfragmentzip_LIBS) $(libcurl_LIBS) $(libcrypto_LIBS) -lm

bin_PROGRAMS	= tsschecker
tsschecker_CFLAGS = $(AM_CFLAGS)
tsschecker_LDADD = $(AM_LDFLAGS)
tsschecker_SOURCES	= tsschecker.c tss.c jsmn.c download.c main.c
