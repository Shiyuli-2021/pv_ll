#include "pv_ll.h"

void pv_ll_init(pv_ll_t * ll_p)
{
    ll_p->head = NULL;
    ll_p->size = 0;
}

void pv_ll_node_init(pv_ll_node_t * node_p, ll_base_t user_data)
{
	node_p->user_data = user_data;
    node_p->next = NULL;
    
#ifdef PV_LL_CB_ENABLE
	node_p->callback = NULL;
	node_p->cb_data = NULL;
#endif
}

void pv_ll_insert(pv_ll_t * ll_p, pv_ll_node_t * node_new)
{
	pv_ll_node_t * node_p = ll_p->head;
	pv_ll_node_t * node_temp = ll_p->head;

	while( (node_temp != NULL) ) {
		if(node_temp == node_new) {
			return;
		}
		node_temp = (node_temp->next);
	}

	ll_p->head = node_new;
	node_new->next = node_p;
	
	ll_p->size += 1;
}

void pv_ll_insert_no_check(pv_ll_t * ll_p, pv_ll_node_t * node_new)
{
	pv_ll_node_t * node_p = ll_p->head;

	ll_p->head = node_new;
	node_new->next = node_p;
	
	ll_p->size += 1;
}

void pv_ll_push_back(pv_ll_t * ll_p, pv_ll_node_t * node_new)
{
    pv_ll_node_t ** node_temp = &ll_p->head;

	while( ((*node_temp) != NULL) ) {
		if(*node_temp == node_new) {
			return;
		}
		node_temp = &((*node_temp)->next);
	}

    *node_temp = node_new;
	node_new->next = NULL;
	ll_p->size += 1;
}

void pv_ll_delete_node(pv_ll_t * ll_p, pv_ll_node_t * node_new)
{
	pv_ll_node_t ** node_temp = &ll_p->head;

	if(node_new == NULL) {
		return;
	}

	if(*node_temp == node_new) {
		ll_p->head = ll_p->head->next;
		ll_p->size -= 1;
		return;
	}

	while( ((*node_temp)->next) != node_new ) {
		if((*node_temp)->next == NULL) {
			return;
		}
		node_temp = &((*node_temp)->next);
	}
	(*node_temp)->next = (*node_temp)->next->next;
	ll_p->size -= 1;
}

void pv_ll_clear(pv_ll_t * ll_p)
{
	ll_p->size = 0;
	ll_p->head = NULL;
}

bool pv_ll_is_value_exist(pv_ll_t* ll_p, ll_base_t user_data)
{
	uint32_t i;
	pv_ll_node_t* node = ll_p->head;

	for(i = 0; i < ll_p->size; i++) {
		if (node->user_data == user_data) {
			return true;
		} else {
			node = node->next;
		}
	}
	return false;
}

uint32_t pv_ll_get_node_size(pv_ll_t * ll_p)
{
    return ll_p->size;
}

#ifdef PV_LL_CB_ENABLE
void pv_ll_node_set_cb(pv_ll_node_t * node_p, pv_ll_callback_t callback, void * cb_data)
{
	node_p->callback = callback;
	node_p->cb_data = cb_data;
}

void pv_ll_cb_for_each(pv_ll_t * ll_p)
{
	pv_ll_node_t * node_temp = ll_p->head;

	for(int i = 0; i < ll_p->size; i++) {
		if(node_temp->callback != NULL) {
			node_temp->callback(node_temp->cb_data);
		}
		node_temp = node_temp->next;
	}
}
#endif




static pv_ll_t			ll_pool;
static pv_ll_node_t 	ll_node_pool[PV_NODE_POOL_SIZE];

void pv_ll_pool_init(void)
{
	pv_ll_init(&ll_pool);
	
	for (int i = 0; i < PV_NODE_POOL_SIZE; i++) {
		pv_ll_node_init(&ll_node_pool[i], 0);
		pv_ll_push_back(&ll_pool, &ll_node_pool[i]);
	}
}

bool pv_ll_pool_insert(pv_ll_t* ll, ll_base_t user_data)
{
	pv_ll_node_t* node = ll_pool.head;
	
	if (ll == NULL) {
		return false;
	}
	
	if (ll_pool.size == 0) {
		return false;
	}
	
	pv_ll_delete_node(&ll_pool, node);
	pv_ll_node_init(node, user_data);
	pv_ll_insert_no_check(ll, node);
	
	return true;
}

bool pv_ll_pool_insert_unique(pv_ll_t* ll, ll_base_t user_data)
{
	pv_ll_node_t* node = ll_pool.head;
	
	if (ll == NULL) {
		return false;
	}
	
	if (ll_pool.size == 0) {
		return false;
	}

	if (pv_ll_is_value_exist(ll, user_data)) {
		return true;
	}
	
	pv_ll_delete_node(&ll_pool, node);
	pv_ll_node_init(node, user_data);
	pv_ll_insert_no_check(ll, node);
	
	return true;
}

bool pv_ll_pool_push_back(pv_ll_t* ll, ll_base_t user_data)
{
	pv_ll_node_t* node = ll_pool.head;
	
	if (ll == NULL) {
		return false;
	}
	
	if (ll_pool.size == 0) {
		return false;
	}
	
	pv_ll_delete_node(&ll_pool, node);
	pv_ll_node_init(node, user_data);
	pv_ll_push_back(ll, node);
	
	return true;
}

void pv_ll_pool_free_all(pv_ll_t* ll)
{
	int i;
	int size = ll->size;
	pv_ll_node_t* node = NULL;

	if (ll == NULL) {
		return;
	}
	
	for (i = 0; i < size; i++) {
		node = ll->head;
		pv_ll_delete_node(ll, node);
		pv_ll_push_back(&ll_pool, node);
	}
}

void pv_ll_pool_free_node(pv_ll_t* ll, pv_ll_node_t* node)
{
	if (ll == NULL || node == NULL) {
		return;
	}
	
	pv_ll_delete_node(ll, node);
	pv_ll_push_back(&ll_pool, node);
}

void pv_ll_pool_free_value(pv_ll_t* ll, ll_base_t value)
{
	int i;
	pv_ll_node_t* node = NULL;
	pv_ll_node_t* node_next = NULL;

	if (ll == NULL || ll->size == 0) {
		return;
	}
	
	node = ll->head;

	for (i = 0; i < (int)(ll->size); i++) {
		node_next = node->next;

		if (node->user_data == value) {
			pv_ll_delete_node(ll, node);
			pv_ll_push_back(&ll_pool, node);
			i -= 1;
		}

		if (node_next != NULL) {
			node = node_next;
		} else {
			return;
		}
		
	}
}

bool pv_ll_pool_copy(pv_ll_t* ll_src, pv_ll_t* ll_dst)
{
	if (ll_src == NULL || ll_dst == NULL) {
		return false;
	}
	
	pv_ll_clear(ll_dst);
	
	pv_ll_for_each(node, ll_src) {
		if (pv_ll_pool_push_back(ll_dst, node->user_data) == false) {
			return false;
		}
	}
	
	return true;
}

uint32_t pv_ll_poll_get_size(void)
{
    return ll_pool.size;
}



























