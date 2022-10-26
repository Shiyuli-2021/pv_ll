#include "pv_ll.h"
#include <stdio.h>

pv_ll_t ll_test;

int main()
{
    pv_ll_pool_init();
    
	printf(" ll_test push & free value: \r\n");
    for (int i = 0; i < 10; i++) {
		pv_ll_pool_insert(&ll_test, i + 1);
    }
	pv_ll_pool_insert_unique(&ll_test, 1);

	pv_ll_for_each (node, (&ll_test)) {
		printf("%d \r\n", node->user_data);
	}

	pv_ll_pool_free_value(&ll_test, 1);
    
	pv_ll_for_each (node, (&ll_test)) {
		printf("%d \r\n", node->user_data);
	}
	printf("ll_test = %d \r\n", ll_test.size);
	printf("ll_pool = %d \r\n", pv_ll_poll_get_size());

	printf(" ll_copy: \r\n");
	pv_ll_t ll_copy;

	pv_ll_init(&ll_copy);
	pv_ll_pool_copy(&ll_test, &ll_copy);
	pv_ll_for_each (node, (&ll_copy)) {
		printf("%d \r\n", node->user_data);
	}
	printf("ll_test = %d \r\n", ll_copy.size);
	printf("ll_pool = %d \r\n", pv_ll_poll_get_size());


	pv_ll_pool_free_all(&ll_test);
	pv_ll_pool_free_all(&ll_copy);

	printf("ll_test = %d \r\n", ll_test.size);
	printf("ll_pool = %d \r\n", pv_ll_poll_get_size());

    while(1);
}


