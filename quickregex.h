#ifndef __QUICK_REGEX_H__
#define __QUICK_REGEX_H__

/*
 * see documentation at:
 * https://www.codeproject.com/Questions/275223/Regular-expressions-using-regex-h 
 *
 * in a nutshell:
 *      One of         [AB]    one of A or B
 *      Not one of     [^AB]   anything but A and B (new line may or may not be included, implementation or option specific)
 *      Zero or more   A*      any number of A's including 0
 *      Group          (A)     Used for grouping things like OR
 *      Any character  .       Any single character (not including new line)
 *
 *      match anything starting with a number:          ^[0-9]
 *      match anything ending with a number:            .*[0-9]$
 *      match anything starting with blub:              ^blub
 *      match anything starting with blub or blah :     ^(blub|blah)
 *      match anything ending with blub or blah :       (blub|blah)$
 *      match anything contain "substr" :               .*substr.*
 *
 *
 */

#include <regex.h>
#include <stdlib.h>
#include <string.h>

#define MAXMATCHES 32

regmatch_t *getLongestMatch(regmatch_t *matches)
{
    regmatch_t *longest = &matches[0]; //just use first element as longest to start with

    int i;
    for (i = 0; i < (MAXMATCHES-1); ++i) {
        int curlen = matches[i].rm_eo - matches[i].rm_so;
        int longestlen = longest->rm_eo - longest->rm_so;
        if (curlen > longestlen) {
            longest=&matches[i];
        }
    }
    return longest;
}

/*
 *  returns start of matched longest matching portion on success
 *  NULL on nothing found or ERROR
 *
 *  this function recompiles the regex every call,
 *  but it is easy to use
 */
static inline char* match(const char* regx, char* str)
{
	static regex_t exp;
	int rv = regcomp(&exp, regx, REG_EXTENDED);
	if (rv != REG_NOERROR) {
		char err[BUFSIZ];
		regerror(rv, &exp, err, BUFSIZ);
		fprintf(stderr, "failed to compile regex with %d : %s\n", rv, err);
		return NULL;
	}
	regmatch_t matches[1];

	if (regexec(&exp, str, 1, matches, 0) == REG_NOERROR)
		return str + matches[0].rm_so;
	return NULL;
}

/*
 * similar to match, but extracts the longest matching portion
 * and returns a newly allocated string containing it
 */
static inline char *matchExtract(char *regx, char *str)
{
    static regex_t exp;
    if (regcomp(&exp, regx, REG_EXTENDED) != REG_NOERROR) return NULL;
    regmatch_t matches[MAXMATCHES];
    int i;
    for (i = 0; i < MAXMATCHES-1; ++i)
        memset((void*)&matches[i],0,sizeof(regmatch_t) );

    if (regexec(&exp, str, MAXMATCHES-1, matches, 0) == REG_NOERROR)
    {
        regmatch_t *lm = getLongestMatch(matches);
        int len = ( lm->rm_eo - lm->rm_so);
        char *ret = (char *)calloc(len+1, 1);
        strncpy(ret, str+lm->rm_so, len);

        return ret;
    }

    return NULL;
}

#endif /* ifndef __QUICK_REGEX_H__ */
