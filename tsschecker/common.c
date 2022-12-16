/*
 * common.c
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "common.h"

int idevicerestore_debug;

#define MAX_PRINT_LEN 64*1024
#ifndef NO_DEBUG_PLIST
void debug_plist(plist_t plist) {
    uint32_t size = 0;
    char* data = NULL;
    if(plist_to_xml(plist, &data, &size) != PLIST_ERR_SUCCESS) {
        debug("Failed to convert plist data to xml!\n");
        return;
    }
    if (size <= MAX_PRINT_LEN)
        debug("%s:printing %i bytes plist:\n%s", __FILE__, size, data);
    else
        debug("%s:supressed printing %i bytes plist...\n", __FILE__, size);
    free(data);
}
void debug_plist2(plist_t plist) {
	uint32_t size = 0;
	char* data = NULL;
	if(plist_to_xml(plist, &data, &size) != PLIST_ERR_SUCCESS) {
		info("Failed to convert plist data to xml!\n");
		return;
	}
	if (size <= MAX_PRINT_LEN)
		info("%s:printing %i bytes plist:\n%s", __FILE__, size, data);
	else
		info("%s:supressed printing %i bytes plist...\n", __FILE__, size);
	free(data);
}
#endif

#define GET_RAND(min, max) ((rand() % (max - min)) + min)

char *generate_guid(void)
{
    char *guid = (char *) malloc(sizeof(char) * 37);
    const char *chars = "ABCDEF0123456789";
    srand(time(NULL));
    int i = 0;

    for (i = 0; i < 36; i++) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            guid[i] = '-';
            continue;
        } else {
            guid[i] = chars[GET_RAND(0, 16)];
        }
    }
    guid[36] = '\0';
    return guid;
}

uint64_t _plist_dict_get_uint(plist_t dict, const char *key)
{
	uint64_t uintval = 0;
	char *strval = NULL;
	uint64_t strsz = 0;
	plist_t node = plist_dict_get_item(dict, key);
	if (!node) {
		return uintval;
	}
	switch (plist_get_node_type(node)) {
	case PLIST_UINT:
		plist_get_uint_val(node, &uintval);
		break;
	case PLIST_STRING:
		plist_get_string_val(node, &strval);
		if (strval) {
			uintval = strtoull(strval, NULL, 0);
			free(strval);
		}
		break;
	case PLIST_DATA:
		plist_get_data_val(node, &strval, &strsz);
		if (strval) {
			if (strsz == 8) {
				uintval = le64toh(*(uint64_t*)strval);
			} else if (strsz == 4) {
				uintval = le32toh(*(uint32_t*)strval);
			} else if (strsz == 2) {
				uintval = le16toh(*(uint16_t*)strval);
			} else if (strsz == 1) {
				uintval = strval[0];
			} else {
				error("%s: ERROR: invalid size " FMT_qu " for data to integer conversion\n", __func__, strsz);
			}
			free(strval);
		}
		break;
	default:
		break;
	}
	return uintval;
}

uint8_t _plist_dict_get_bool(plist_t dict, const char *key)
{
	uint8_t bval = 0;
	uint64_t uintval = 0;
	char *strval = NULL;
	uint64_t strsz = 0;
	plist_t node = plist_dict_get_item(dict, key);
	if (!node) {
		return 0;
	}
	switch (plist_get_node_type(node)) {
	case PLIST_BOOLEAN:
		plist_get_bool_val(node, &bval);
		break;
	case PLIST_UINT:
		plist_get_uint_val(node, &uintval);
		bval = (uint8_t)uintval;
		break;
	case PLIST_STRING:
		plist_get_string_val(node, &strval);
		if (strval) {
			if (strcmp(strval, "true")) {
				bval = 1;
			} else if (strcmp(strval, "false")) {
				bval = 0;
			}
			free(strval);
		}
		break;
	case PLIST_DATA:
		plist_get_data_val(node, &strval, &strsz);
		if (strval) {
			if (strsz == 1) {
				bval = strval[0];
			} else {
				error("%s: ERROR: invalid size " FMT_qu " for data to boolean conversion\n", __func__, strsz);
			}
			free(strval);
		}
		break;
	default:
		break;
	}
	return bval;
}

int _plist_dict_copy_uint(plist_t target_dict, plist_t source_dict, const char *key, const char *alt_source_key)
{
	if (plist_dict_get_item(source_dict, (alt_source_key) ? alt_source_key : key) == NULL) {
		return -1;
	}
	uint64_t u64val = _plist_dict_get_uint(source_dict, (alt_source_key) ? alt_source_key : key);
	plist_dict_set_item(target_dict, key, plist_new_uint(u64val));
	return 0;
}

int _plist_dict_copy_bool(plist_t target_dict, plist_t source_dict, const char *key, const char *alt_source_key)
{
	if (plist_dict_get_item(source_dict, (alt_source_key) ? alt_source_key : key) == NULL) {
		return -1;
	}
	uint64_t bval = _plist_dict_get_bool(source_dict, (alt_source_key) ? alt_source_key : key);
	plist_dict_set_item(target_dict, key, plist_new_bool(bval));
	return 0;
}

int _plist_dict_copy_data(plist_t target_dict, plist_t source_dict, const char *key, const char *alt_source_key)
{
	plist_t node = plist_dict_get_item(source_dict, (alt_source_key) ? alt_source_key : key);
	if (!PLIST_IS_DATA(node)) {
		return -1;
	}
	plist_dict_set_item(target_dict, key, plist_copy(node));
	return 0;
}

int _plist_dict_copy_string(plist_t target_dict, plist_t source_dict, const char *key, const char *alt_source_key)
{
	plist_t node = plist_dict_get_item(source_dict, (alt_source_key) ? alt_source_key : key);
	if (!PLIST_IS_STRING(node)) {
		return -1;
	}
	plist_dict_set_item(target_dict, key, plist_copy(node));
	return 0;
}

int _plist_dict_copy_item(plist_t target_dict, plist_t source_dict, const char *key, const char *alt_source_key)
{
	plist_t node = plist_dict_get_item(source_dict, (alt_source_key) ? alt_source_key : key);
	if (!node) {
		return -1;
	}
	plist_dict_set_item(target_dict, key, plist_copy(node));
	return 0;
}
