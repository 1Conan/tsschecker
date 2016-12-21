//
//  all.h
//  tsschecker
//
//  Created by tihmstar on 26.01.16.
//  Copyright © 2016 tihmstar. All rights reserved.
//

#ifndef all_h
#define all_h

extern int idevicerestore_debug;

#define info(a ...) printf(a)
#define log(a ...) if (dbglog) printf(a)
#define warning(a ...) if (dbglog) printf(a)
#define debug(a ...) if (idevicerestore_debug) printf(a)
#define error(a ...) printf("[Error] "),printf(a)

#define VERSION_COMMIT_COUNT "undefined version number" //"126"
#define VERSION_COMMIT_SHA "undefined version commit"   //"726c69695fe5337b9f9cf62046310c829908b058"

#endif /* all_h */
