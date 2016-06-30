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

typedef struct{
    uint64_t ecid;
    char *apnonce;
    char *sepnonce;
}t_devicevals;

char *getFirmwareJson();
char *getOtaJson();
int parseTokens(char *json, jsmntok_t **tokens);
char **getListOfiOSForDevice(char *firmwarejson, jsmntok_t *tokens, char *device, int isOTA, int *versionCntt);
int printListOfDevices(char *firmwarejson, jsmntok_t *tokens);
int printListOfiOSForDevice(char *firmwarejson, jsmntok_t *tokens, char *device, int isOTA);

char *getFirmwareUrl(char *device, char *version,char *firmwarejson, jsmntok_t *tokens, int isOta, int useBeta);
char *getBuildManifest(char *url, int isOta);
int64_t getBBGCIDForDevice(char *deviceModel);

int tssrequest(plist_t *tssrequest, char *buildManifest, char *device, t_devicevals devVals, int checkBaseband);
int isManifestSignedForDevice(char *buildbManifestPath, char **device, t_devicevals devVals, int checkBaseband, char **version);
int isManifestBufSignedForDevice(char *buildManifestBuffer, char *device, t_devicevals devVals, int checkBaseband);
int isVersionSignedForDevice(char *firmwareJson, jsmntok_t *firmwareTokens, char *version, char *device, t_devicevals devVals, int otaFirmware, int checkBaseband, int useBeta);

int checkDeviceExists(char *device, char *firmwareJson, jsmntok_t *tokens, int isOta);
int checkFirmwareForDeviceExists(char *device, char *version, char *firmwareJson, jsmntok_t *tokens, int isOta);

#endif /* tsscheker_h */
