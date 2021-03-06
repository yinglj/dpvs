/*
 * DPVS is a software load balancer (Virtual Server) based on DPDK.
 *
 * Copyright (C) 2017 iQIYI (www.iqiyi.com).
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __DPVS_MEMPOOL_H__
#define __DPVS_MEMPOOL_H__

#include <rte_mempool.h>

#define MP_NAMSIZ               32
#define MP_OBJ_COOKIE_OFFSET    8
#define MP_OBJ_COOKIE_MARK      0xA55AC33C
#define MP_SIZE_MIN             16
#define MP_OBJ_SIZE_MIN         32
#define MP_OBJ_SIZE_MAX         65536   /* 64KB */

#define RTE_LOGTYPE_DPVS_MPOOL     RTE_LOGTYPE_USER1

enum dpvs_mp_obj_flag {
    MEM_OBJ_FROM_POOL = 1,
    MEM_OBJ_FROM_HEAP
};

struct dpvs_mp_obj_cookie {
    uint32_t mark;
    uint16_t flag;
    uint16_t pool_idx;
};

struct dpvs_mp_elem {
    char name[MP_NAMSIZ];
    uint32_t obj_size;
    uint16_t obj_num;
    uint16_t cache_size;
    struct rte_mempool *pool;
};

struct dpvs_mempool {
    char name[MP_NAMSIZ];
    uint32_t obj_size_min;              /* minimum object size =  obj_size_min - MEM_OBJ_COOKIE_OFFSET */
    uint32_t obj_size_max;              /* maximum object size =  obj_size_max - MEM_OBJ_COOKIE_OFFSET */
    uint32_t pool_mem;                  /* memory size for each pool, in bytes */
    uint16_t pool_arr_size;
    struct dpvs_mp_elem pool_array[0];
};

/*
 * dpvs_mempool consists of an array of rte_mempool, with object size varies from 'obj_size_min'
 * to 'obj_size_min' in 2-exponential step, each rte_mempool of the same memory 'pool_mem_kbytes'.
 * Mempool with large object size populates less objects, and the total memory of dpvs_mempool is 
 * (log2(obj_size_max)-log2(obj_size_min))*pool_mem_kbytes KB.
 */
struct dpvs_mempool* dpvs_mempool_create(char *name,
        int obj_size_min, int obj_size_max, int pool_mem_kbytes);

void dpvs_mempool_destroy(struct dpvs_mempool *mp);

void *dpvs_mempool_get(struct dpvs_mempool *mp, int size);

void dpvs_mempool_put(struct dpvs_mempool *mp, void *obj);

#endif /* __DPVS_MEMPOOL_H__ */
