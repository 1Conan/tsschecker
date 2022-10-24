/*
 * common.h
 * Misc functions used in idevicerestore
 *
 * Copyright (c) 2012-2019 Nikias Bassen. All Rights Reserved.
 * Copyright (c) 2012 Martin Szulecki. All Rights Reserved.
 * Copyright (c) 2010 Joshua Hill. All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef IDEVICERESTORE_COMMON_H
#define IDEVICERESTORE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <libfragmentzip/libfragmentzip.h>
#include <stdint.h>
#include <string.h>
#include <plist/plist.h>
#include "debug.h"
#include "endianness.h"

#define USER_AGENT_STRING "InetURL/1.0"
#ifdef WIN32
#include <windows.h>
#define __mkdir(path, mode) mkdir(path)
#define FMT_qu "%I64u"
#ifndef sleep
#define sleep(x) Sleep(x*1000)
#endif
#else
#include <sys/stat.h>
#define __mkdir(path, mode) mkdir(path, mode)
#define FMT_qu "%llu"
#endif

void debug_plist(plist_t plist);
void debug_plist2(plist_t plist);
char *generate_guid(void);
uint8_t _plist_dict_get_bool(plist_t dict, const char *key);
uint64_t _plist_dict_get_uint(plist_t dict, const char *key);
int _plist_dict_copy_uint(plist_t target_dict, plist_t source_dict, const char *key, const char *alt_source_key);
int _plist_dict_copy_bool(plist_t target_dict, plist_t source_dict, const char *key, const char *alt_source_key);
int _plist_dict_copy_data(plist_t target_dict, plist_t source_dict, const char *key, const char *alt_source_key);
int _plist_dict_copy_string(plist_t target_dict, plist_t source_dict, const char *key, const char *alt_source_key);
int _plist_dict_copy_item(plist_t target_dict, plist_t source_dict, const char *key, const char *alt_source_key);

#ifdef __cplusplus
}
#endif

#endif
