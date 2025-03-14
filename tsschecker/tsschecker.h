//
//  tsschecker.h
//  tsschecker
//
//  Created by tihmstar on 07.01.16.
//  Copyright © 2016 tihmstar. All rights reserved.
//

#ifndef tsschecker_h
#define tsschecker_h

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TSSCHECKER_VERSION_MAJOR
#define TSSCHECKER_VERSION_MAJOR "0"
#endif
#ifndef TSSCHECKER_VERSION_COUNT
#define TSSCHECKER_VERSION_COUNT "0"
#endif
#ifndef TSSCHECKER_VERSION_PATCH
#define TSSCHECKER_VERSION_PATCH "0"
#endif
#ifndef TSSCHECKER_VERSION_SHA
#define TSSCHECKER_VERSION_SHA "0"
#endif
#ifndef TSSCHECKER_BUILD_TYPE
#define TSSCHECKER_BUILD_TYPE "0"
#endif

#include <stdio.h>
#include <plist/plist.h>
#include <libtatsu/tss.h>

#include "jssy.h"
#include "debug.h"
    
extern int dbglog;
extern int nocache;
extern int save_shshblobs;
extern int update_install;
extern int erase_install;
extern int save_bplist;
extern const char *shshSavePath;

struct bbdevice{
    const char *deviceModel;
    uint64_t bbgcid;
    size_t bbsnumSize;
};

typedef struct bbdevice* t_bbdevice;

typedef enum{
    kInstallTypeDefault = 0, //default is always erase install
    kInstallTypeUpdate = 1,
    kInstallTypeErase = 1<<1
} t_installType;

typedef struct{
    char *deviceModel; //either model, or boardconfig
    char *deviceBoard;
    char *apnonce;
    char *sepnonce;
    char *cryptexnonce;
    uint64_t ecid;
    uint64_t bbgcid;
    size_t parsedApnonceLen;
    size_t parsedSepnonceLen;
    size_t parsedCryptexnonceLen;
    uint8_t *bbsnum;
    size_t bbsnumSize;
    char generator[19];
    char cryptexseed[36];
    union{
        t_installType installType : 2;
        union{
            int ___pading : 1;
            int isUpdateInstall : 1; //take only last bit of installType into account
        };
    };
    
}t_devicevals;

typedef enum{
    kBasebandModeWithBaseband = 0,
    kBasebandModeWithoutBaseband = 1,
    kBasebandModeOnlyBaseband = 2,
}t_basebandMode;

typedef struct{
    char *version;
    char *buildID;
    t_basebandMode basebandMode;
    int isOta       : 1;
    int useBeta     : 1;
}t_iosVersion;
    
typedef struct{
    char *url;
    char *version;
    char *buildID;
    int isDupulicate;
}t_versionURL;

int parseHex(const char *nonce, size_t *parsedLen, char *ret, size_t *retSize);

char *getFirmwareJson(void);
char *getBetaFirmwareJson2(const char *type, const char *buildid);
char *getBetaFirmwareJson(const char *device);
char *getOtaJson(void);
long parseTokens(const char *json, jssytok_t **tokens);
char **getListOfiOSForDevice(jssytok_t *tokens, const char *device, int isOTA, int *versionCntt, bool beta);
char **getListOfiOSForDevice2(jssytok_t *tokens, const char *device, int isOTA, int *versionCntt, int buildid, bool beta);
char *getBetaURLForDevice2(jssytok_t *tokens, const char *buildid);
char *getBetaURLForDevice(jssytok_t *tokens, const char *buildid);
int printListOfDevices(jssytok_t *tokens);
int printListOfiOSForDevice(jssytok_t *tokens, char *device, int isOTA);

char *getFirmwareUrl(const char *deviceModel, t_iosVersion *versVals, jssytok_t *tokens, bool beta, bool ota);
t_versionURL *getFirmwareUrls(const char *deviceModel, t_iosVersion *versVals, jssytok_t *tokens, bool beta, bool ota);
char *getBuildManifest(char *url, const char *device, const char *version, const char *buildID, int isOta);
t_bbdevice getBBDeviceInfo(const char *deviceModel);

int tssrequest(plist_t *tssrequest, char *buildManifest, t_devicevals *devVals, t_basebandMode basebandMode);
int isManifestSignedForDevice(const char *buildManifestPath, t_devicevals *devVals, t_iosVersion *versVals, const char* server_url_string);
int isManifestBufSignedForDevice(char *buildManifestBuffer, t_devicevals *devVals, t_basebandMode basebandMode, const char* server_url_string);
int isVersionSignedForDevice(jssytok_t *firmwareTokens, t_iosVersion *versVals, t_devicevals *devVals, const char* server_url_string);

jssytok_t *getFirmwaresForDevice(const char *device, jssytok_t *tokens, int isOta);

int checkFirmwareForDeviceExists(t_devicevals *devVals, t_iosVersion *versVals, jssytok_t *tokens);

int downloadPartialzip(const char *url, const char *file, const char *dst);

const char *getBoardconfigFromModel(const char *model);
const char *getModelFromBoardconfig(const char *boardconfig);
plist_t getBuildidentity(plist_t buildManifest, const char *model, int isUpdateInstall);
plist_t getBuildidentityWithBoardconfig(plist_t buildManifest, const char *boardconfig, int isUpdateInstall);

#ifdef __cplusplus
}
#endif
    
#endif /* tsscheker_h */
