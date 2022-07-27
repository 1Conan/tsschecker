//
//  debug.h
//  tsschecker
//
//  Created by s0uthwest on 08.01.19.
//  Copyright © 2016 tihmstar. All rights reserved.
//

#ifndef debug_h
#define debug_h

// idevicerestore flags
extern int idevicerestore_debug;
#ifndef log
#define log(a ...) if (dbglog) printf(a)
#endif
#ifndef info
#define info(a ...) printf(a)
#endif
#ifndef warning
#define warning(a ...) if (dbglog) printf("[WARNING] "), printf(a)
#endif
#ifndef debug
#define debug(a ...) if (idevicerestore_debug) printf(a)
#endif
#ifndef error
#define error(a ...) printf("[Error] "),printf(a)
#endif

// statis assert
#define CASSERT(predicate, file) _impl_CASSERT_LINE(predicate,__LINE__,file)
#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];

#endif /* debug_h */
