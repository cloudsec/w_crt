//#define WMALLOC_DEBUG

#ifdef WMALLOC_DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

#include "w_crt.h"

struct list_head {
	struct list_head *prev, *next;
};

#define INIT_LIST_HEAD(name_ptr)	do { 	(name_ptr)->next = (name_ptr);	\
						(name_ptr)->prev = (name_ptr);	\
					}while (0)

#define OFFSET(type, member)            (char *)&(((type *)0x0)->member)
#define container_of(ptr, type, member) ({					\
			(type *)((char * )ptr - OFFSET(type, member)); });
	
#define list_for_each(pos, head)        for (pos = head->next; pos != head; pos = pos->next)
#define list_for_each_prev(pos, head)	for (pos = head->prev; pos != head; pos = pos->prev)
#define list_entry(ptr, type, member)   container_of(ptr, type, member)

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	head->prev->next = new;
	new->prev = head->prev;
	new->next = head;
	head->prev = new;
}

static inline void list_del(struct list_head *p)
{
	p->prev->next = p->next;
	p->next->prev = p->prev;
}

static inline int list_empty(struct list_head *head)
{
	return head->next == head;
}

struct heap_chunk {
	char *chunk_pos;
	int inuse;
	struct list_head list;
};

#define HEAP_SIZE			(1024 * 1024 * 4)
#define HEAP_CHUNK_STRUCT_SIZE		sizeof(struct heap_chunk)
#define DEFAULT_CHUNK_SIZE		128

#define CHUNK_SIZE			DEFAULT_CHUNK_SIZE + HEAP_CHUNK_STRUCT_SIZE
#define CHUNK_NUM			(HEAP_SIZE / (DEFAULT_CHUNK_SIZE + \
					HEAP_CHUNK_STRUCT_SIZE))

struct list_head heap_inactive_list;
struct list_head heap_active_list;

void *heap_base = NULL;

#ifdef WMALLOC_DEBUG
void debug_print_list(struct list_head *list_head)
{
	struct heap_chunk *s = NULL;
	struct list_head *p = NULL;

	if (list_empty(list_head))
		return NULL;

	list_for_each(p, list_head) {
		s = list_entry(p, struct heap_chunk, list);
		if (s)
			printf("chunk_pos: %p\n", s->chunk_pos);
	}	
}
#endif

void *malloc(unsigned int size)
{
	struct heap_chunk *s = NULL;
	struct list_head *p = NULL;

	if (!size || size > DEFAULT_CHUNK_SIZE)
		return NULL;

	if (list_empty(&heap_inactive_list))
		return NULL;

	list_for_each(p, (&heap_inactive_list)) {
		s = list_entry(p, struct heap_chunk, list);
		if (s && !s->inuse) {
			s->inuse = 1;
			list_del(p);
			list_add_tail(p, &heap_active_list);
			return s->chunk_pos;
		}
	}

	return NULL;
}

void free(void *addr)
{
	struct heap_chunk *s = NULL;

	s = container_of(addr + DEFAULT_CHUNK_SIZE, struct heap_chunk, chunk_pos);
	s->inuse = 0;

	list_del(&(s->list));
	list_add_tail(&(s->list), &heap_inactive_list);
}

void heap_chunk_init(void *heap_base)
{
	int idx;

	for (idx = 0; idx < CHUNK_NUM; idx += CHUNK_SIZE) {
		struct heap_chunk *heap_chunk;

		heap_chunk = (struct heap_chunk *)(heap_base + 
			DEFAULT_CHUNK_SIZE + idx);
		heap_chunk->inuse = 0;
		heap_chunk->chunk_pos = heap_base + idx;
		list_add_tail(&(heap_chunk->list), &heap_inactive_list);
	}
}

static int brk(void *end_data_segment)
{
	int ret;

	asm("movl $45, %%eax\n\t"
	    "movl %1, %%ebx\n\t"
	    "int $0x80\n\t"
	    "movl %%eax, %0"
	    :"=r"(ret):"m"(end_data_segment)); 
}

int w_crt_heap_init(void)
{
	void *heap_end;

	INIT_LIST_HEAD(&heap_inactive_list);
	INIT_LIST_HEAD(&heap_active_list);

	heap_base = (void *)brk(0);
	heap_end = (void *)brk(heap_base + HEAP_SIZE);
	
	if (!heap_end)
		return -1;

	heap_chunk_init(heap_base);

	return 0;
}

#ifdef WMALLOC_DEBUG
int main(void)
{
	int i;
	char *s;

	w_crt_heap_init();

	for (i = 0; i < 8; i++) {
		s = malloc(64);
		printf("wmalloc at %p\n", s);
	}
	debug_print_list(&heap_active_list);
	free(s);
	debug_print_list(&heap_active_list);
	debug_print_list(&heap_inactive_list);
}
#endif
