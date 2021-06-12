#ifndef RM_LIB_CUSTOM_FILE_H
#define RM_LIB_CUSTOM_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 文件结构体
 */
typedef struct __FILE {
    volatile int handle;
}__FILE;

#ifdef __cplusplus
}
#endif

#endif //RM_LIB_CUSTOM_FILE_H
