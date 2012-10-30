#ifndef _TESTKERNELLIB_EXPORTS_H_
#define _TESTKERNELLIB_EXPORTS_H_

#ifdef TESTKERNELLIB_EXPORTS
#define TESTKERNELLIB_API __attribute__ ((visibility ("default")))
#else
#define TESTKERNELLIB_API
#endif

#endif
