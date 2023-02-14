//
//  all.h
//  tsschecker
//
//  Created by tihmstar on 26.01.16.
//  Copyright © 2016 tihmstar. All rights reserved.
//

#ifndef all_h
#define all_h

#define TSSCHECKER_VERSION_MAJOR "0"
#define TSSCHECKER_VERSION_PATCH "0"

#ifdef DEBUG // this is for developing with Xcode
#define TSSCHECKER_BUILD_TYPE "DEBUG"
#define TSSCHECKER_VERSION_SHA "Build: " __DATE__ " " __TIME__ ""
#else
#define TSSCHECKER_BUILD_TYPE "RELEASE"
#endif

#endif /* all_h */
