//
//  ipswme.h
//  tsschecker
//
//  Created by tihmstar on 07.01.16.
//  Copyright © 2016 tihmstar. All rights reserved.
//

#ifndef tsscheker_h
#define tsscheker_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "jsmn.h"
#include <plist/plist.h>
#include "tss.h"
#include "all_tsschecker.h"
#define noncelen 20

extern int dbglog;
extern int print_tss_response;
extern int nocache;
extern int save_shshblobs;
extern const char *shshSavePath;


typedef struct{
    uint64_t ecid;
    uint64_t bbgcid;
    char *apnonce;
    char *sepnonce;
    char generator[19];
}t_devicevals;

typedef enum{
    kBasebandModeWithBaseband = 0,
    kBasebandModeWithoutBaseband = 1,
    kBasebandModeOnlyBaseband = 2,
}t_basebandMode;

typedef struct{
    const char *version;
    t_basebandMode basebandMode;
    int isOta       : 1;
    int useBeta     : 1;
    int isBuildid   : 1;
}t_iosVersion;
    
char *getFirmwareJson();
char *getOtaJson();
int parseTokens(const char *json, jsmntok_t **tokens);
char **getListOfiOSForDevice(char *firmwarejson, jsmntok_t *tokens, const char *device, int isOTA, int *versionCntt);
int printListOfDevices(char *firmwarejson, jsmntok_t *tokens);
int printListOfiOSForDevice(char *firmwarejson, jsmntok_t *tokens, char *device, int isOTA);

char *getFirmwareUrl(const char *device, t_iosVersion versVals, const char *firmwarejson, jsmntok_t *tokens);
char *getBuildManifest(char *url, const char *device, const char *version, int isOta);
int64_t getBBGCIDForDevice(char *deviceModel);

int tssrequest(plist_t *tssrequest, char *buildManifest, char *device, t_devicevals *devVals, t_basebandMode basebandMode);
int isManifestSignedForDevice(const char *buildManifestPath, char **device, t_devicevals *devVals, t_iosVersion *versVals);
int isManifestBufSignedForDevice(char *buildManifestBuffer, char *device, t_devicevals devVals, t_basebandMode basebandMode);
int isVersionSignedForDevice(char *firmwareJson, jsmntok_t *firmwareTokens, t_iosVersion versVals, char *device, t_devicevals devVals);


int checkDeviceExists(char *device, char *firmwareJson, jsmntok_t *tokens, int isOta);
int checkFirmwareForDeviceExists(char *device, t_iosVersion version, char *firmwareJson, jsmntok_t *tokens);

int downloadPartialzip(const char *url, const char *file, const char *dst);

    
#ifdef __cplusplus
}
#endif
    
#endif /* tsscheker_h */
