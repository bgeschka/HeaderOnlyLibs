#ifndef __DBG_H__
#define __DBG_H__
#include <string.h>
#include <stdio.h>

/*
 * some glorified printfs - prints filename,linenumber and function name,
 * supports printf formatting
 *
 *
 *      1	#include "dbg.h"
 *      2
 *      3	int main(int argc, char *argv[])
 *      4	{
 *      5	        DBG("sometest");
 *      6	        return 0;
 *      7	}
 *      ---
 *      $ ./test
 *      test.c:5:main()=[sometest]
 *
 */

#define __FILENAME__							\
	(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifndef DBG
#define DBG(form,...) \
do {									\
	fprintf(stdout,							\
	"%s:%d:%s()=[" form "]\n" ,					\
	__FILENAME__,							\
	 __LINE__,							\
	__func__,							\
	##__VA_ARGS__  );						\
} while (0)
#endif

#endif /* ifndef __DBG_H__ */
