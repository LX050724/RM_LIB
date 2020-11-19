#include "RMQueue.h"

RM_Status RMQueueInit(RMQueue_Handle *handle, uint32_t typeSize, uint32_t depth) {
    handle->dataPtr = (void **) RMLIB_MALLOC(sizeof(void **) * depth);
    if (handle->dataPtr == NULL) return RM_ERROR;
    for (int i = 0; i < depth; i++) {
        handle->dataPtr[i] = RMLIB_MALLOC(typeSize);
        if (handle->dataPtr[i] == NULL) {
            for (int j = 0; j < i; j++)
                RMLIB_FREE(handle->dataPtr[j]);
            return RM_ERROR;
        }
    }
    handle->typeSize = typeSize;
    handle->fifoSize = depth;
    handle->head = 0;
    handle->end = 0;
    handle->size = 0;
    handle->Lock = 0;
    return RM_SUCCESS;
}

RM_Status RMQueuePush(RMQueue_Handle *handle, void *dataPtr) {
    if (handle->size < handle->fifoSize && handle->Lock == 0) {
        memcpy(handle->dataPtr[handle->end], dataPtr, handle->typeSize);
        handle->end = (handle->end + 1) % handle->fifoSize;
        handle->size++;
        return RM_SUCCESS;
    } else return RM_ERROR;
}

void *RMQueueTop(RMQueue_Handle *handle) {
    if (handle->size != 0)
        return handle->dataPtr[handle->head];
    else return NULL;
}

void *RMQueuePop(RMQueue_Handle *handle) {
    if (handle->size != 0) {
        void *tmp = handle->dataPtr[handle->head];
        handle->head = (handle->head + 1) % handle->fifoSize;
        handle->size--;
        return tmp;
    } else return NULL;
}

void RMQueueDelete(RMQueue_Handle *handle) {
    for (int i = 0; i < handle->fifoSize; i++)
        RMLIB_FREE(handle->dataPtr[i]);
    RMLIB_FREE(handle->dataPtr);
}

void *RMQueueGetEndPtr(RMQueue_Handle *handle) {
    if (handle->size < handle->fifoSize && handle->Lock == 0) {
        handle->Lock = 1;
        return handle->dataPtr[handle->end];
    } else return NULL;
}

RM_Status RMQueuePushEndPtr(RMQueue_Handle *handle) {
    if (handle->Lock == 1) {
        handle->Lock = 0;
        handle->end = (handle->end + 1) % handle->fifoSize;
        handle->size++;
        return RM_SUCCESS;
    } else return RM_ERROR;
}
