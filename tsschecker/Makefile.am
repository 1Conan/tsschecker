AM_CFLAGS = $(libplist_CFLAGS) $(libplistmm_CFLAGS) $(libpartialzip_CFLAGS) $(libcurl_CFLAGS) $(libcrypto_CFLAGS) $(libxml_CFLAGS)
AM_LDFLAGS = $(libplist_LIBS) $(libplistmm_LDFLAGS) $(libpartialzip_LIBS) $(libcurl_LIBS) $(libcrypto_LIBS) $(libxml_LIBS)

bin_PROGRAMS	= tsschecker_tool
tsschecker_tool_CFLAGS = $(AM_CFLAGS)
tsschecker_tool_LDADD = $(AM_LDFLAGS)
tsschecker_tool_SOURCES	= tsschecker.c tss.c jsmn.c download.c main.c
