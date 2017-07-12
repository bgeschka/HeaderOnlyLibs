#ifndef __JSONC_HELPERS__
#define __JSONC_HELPERS__

#include <json-c/json.h>
#include <string.h>

static json_object* getJsonObjectByKey(const json_object* o, const char* key)
{
    if (o == NULL || key == NULL)
        return NULL;
    const char* pos = NULL;
    if ((pos = strstr(key, ".")) == NULL)
        return json_object_object_get(o, key);

    int nlen = pos - key;
    char ikey[nlen];
    strncpy(ikey, key, nlen);
    json_object* _o = json_object_object_get(o, ikey);
    return getJsonObjectByKey(_o, pos + 1);
}

#define jsonarrayforeach(list,cur) \
    for(int __i=0;(cur=json_object_array_get_idx(list,__i))!=NULL;__i++) 


#endif /* ifndef __JSONC_HELPERS__ */
