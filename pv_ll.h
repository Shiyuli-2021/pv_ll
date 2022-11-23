#ifndef __PV_LL_H__
#define __PV_LL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* set node pool size */
#define PV_NODE_POOL_SIZE	100

/* set user_data type */
typedef uint32_t    ll_base_t;

typedef struct _pv_ll_node_t pv_ll_node_t;

typedef void (*pv_ll_callback_t)(void* cb_data);

struct _pv_ll_node_t{
    ll_base_t user_data;
    struct _pv_ll_node_t * next;
    
#ifdef PV_LL_CB_ENABLE
    void * cb_data;
    pv_ll_callback_t callback;
#endif
};

typedef struct _pv_ll_t{
    uint32_t size;
    struct _pv_ll_node_t * head;
}pv_ll_t;


#define pv_ll_for_each(node, ll_p)   for (pv_ll_node_t* node = (ll_p)->head; node != NULL; node = node->next)

void pv_ll_init(pv_ll_t * ll_p);

void pv_ll_node_init(pv_ll_node_t * node_p, ll_base_t user_data);

void pv_ll_insert(pv_ll_t * ll_p, pv_ll_node_t * node_new);

void pv_ll_push_back(pv_ll_t * ll_p, pv_ll_node_t * node_new);

void pv_ll_delete_node(pv_ll_t * ll_p, pv_ll_node_t * node_new);

void pv_ll_clear(pv_ll_t * ll_p);

bool pv_ll_is_value_exist(pv_ll_t* ll_p, ll_base_t user_data);

uint32_t pv_ll_get_node_size(pv_ll_t * ll_p);

#ifdef PV_LL_CB_ENABLE
void pv_ll_node_set_cb(pv_ll_node_t * node_p, pv_ll_callback_t callback, void * cb_data);

void pv_ll_cb_for_each(pv_ll_t * ll_p);
#endif

void pv_ll_pool_init(void);

bool pv_ll_pool_insert(pv_ll_t* ll, ll_base_t user_data);

bool pv_ll_pool_insert_unique(pv_ll_t* ll, ll_base_t user_data);

bool pv_ll_pool_push_back(pv_ll_t* ll, ll_base_t user_data);

void pv_ll_pool_free_all(pv_ll_t* ll);

void pv_ll_pool_free_node(pv_ll_t* ll, pv_ll_node_t* node);

void pv_ll_pool_free_value(pv_ll_t* ll, ll_base_t value);

bool pv_ll_pool_copy(pv_ll_t* ll_src, pv_ll_t* ll_dst);

uint32_t pv_ll_poll_get_size(void);

#ifdef __cplusplus
}   /* extern "C" */ 
#endif

#endif

