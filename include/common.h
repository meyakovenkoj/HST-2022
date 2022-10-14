#ifndef __COMMON_H__
#define __COMMON_H__

#define CHECK_ERROR_CODE(error_code) \
    if (error_code) { \
        printf("File: %s, Line: %d, error code: %d\n", __FILE__, __LINE__, error_code); \
        goto done; \
    }

struct MpiConfig
{
    int num_procs;
    int proc_id;
};

#endif