//
//  ipswme.h
//  tsschecker
//
//  Created by tihmstar on 07.01.16.
//  Copyright © 2016 tihmstar. All rights reserved.
//

#ifndef tsscheker_h
#define tsscheker_h

#include <stdio.h>
#include "jsmn.h"
#include <plist/plist.h>
#include "tss.h"
#include "all.h"
#define noncelen 20


extern int print_tss_response;
extern int nocache;
extern int save_shshblobs;
extern const char *shshSavePath;


typedef struct{
    uint64_t ecid;
    char *apnonce;
    char *sepnonce;
}t_devicevals;

typedef struct{
    const char *version;
    int isOta       : 1;
    int useBeta     : 1;
    int noBaseband  : 1;
    int isBuildid   : 1;
}t_iosVersion;

char *getFirmwareJson();
char *getOtaJson();
int parseTokens(char *json, jsmntok_t **tokens);
char **getListOfiOSForDevice(char *firmwarejson, jsmntok_t *tokens, char *device, int isOTA, int *versionCntt);
int printListOfDevices(char *firmwarejson, jsmntok_t *tokens);
int printListOfiOSForDevice(char *firmwarejson, jsmntok_t *tokens, char *device, int isOTA);

char *getFirmwareUrl(char *device, t_iosVersion versVals,char *firmwarejson, jsmntok_t *tokens);
char *getBuildManifest(char *url, const char *device, const char *version, int isOta);
int64_t getBBGCIDForDevice(char *deviceModel);

int tssrequest(plist_t *tssrequest, char *buildManifest, char *device, t_devicevals devVals, int checkBaseband);
int isManifestSignedForDevice(char *buildManifestPath, char **device, t_devicevals devVals, t_iosVersion versVals);
int isManifestBufSignedForDevice(char *buildManifestBuffer, char *device, t_devicevals devVals, int checkBaseband);
int isVersionSignedForDevice(char *firmwareJson, jsmntok_t *firmwareTokens, t_iosVersion versVals, char *device, t_devicevals devVals);

int checkDeviceExists(char *device, char *firmwareJson, jsmntok_t *tokens, int isOta);
int checkFirmwareForDeviceExists(char *device, t_iosVersion version, char *firmwareJson, jsmntok_t *tokens);

#endif /* tsscheker_h */
