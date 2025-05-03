/**
 *				 			[STRUCTURE OF A 'PAGE']
 *
 *                             free_start			  free_end
 *                                 |--->grows     grows<---|
 *                                 V                       V
 *  +--------+----------+----------+-----------------------+---------+---------+
 *  | header | rec_ptr1 | rec_ptr2 |     unused space...   | record2 | record1 |
 *  +--------+----------+----------+-----------------------+---------+---------+
 *
 */

#include "page.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

void *page_create(enum pageType type, uint32_t id)
{
	void       *page   = malloc(PAGE_SIZE);
	PageHeader *header = PAGE_HEADER(page);

	memset(page, 0, PAGE_SIZE);
	header->id         = id;
	header->n_records  = 0;
	header->type       = type;
	header->free_start = sizeof(PageHeader);
	header->free_end   = PAGE_SIZE - 1;
	header->free_size  = header->free_end - header->free_start;
	return (page);
}

void page_add_record(void *page, void *record, uint16_t size)
{
	PageHeader *header = PAGE_HEADER(page);

	assert(header->free_size >= size);

	RecordPointer add_ptr;
	add_ptr.location = (header->free_end) - size;
	add_ptr.size     = size;

	memcpy(&(page[header->free_start]), &add_ptr, sizeof(RecordPointer));
	memcpy(&(page[add_ptr.location]), record, add_ptr.size);


	header->n_records++;
	header->free_start += sizeof(RecordPointer);
	header->free_end -= size;
	header->free_size -= size;
}

void *page_get_record(void *page, uint16_t idx)
{
	RecordPointer *ptr = RECORD_POINTER_LIST(page) + idx;

	if (ptr->location == 0)
		return (NULL);
	else
		return (page + ptr->location);
}

// Marks the record at 'idx' as deleted
// The actual space reclaimed during compaction
void page_remove_record(void *page, uint16_t idx)
{
	RecordPointer *ptr = (RECORD_POINTER_LIST(page)) + (idx * sizeof(RecordPointer));

	ptr->location = 0;
	PAGE_HEADER(page)->flags |= PAGE_CAN_COMPACT;
}

// TODO
void page_compact(void *page)
{
}

// -------------- Persistent disk I/O (non-volatile storage) -------------------

// Find this page as id and sync to disk right now
// Assume that pages are placed in the order of id in a file
void save_page(int fd, void *page)
{
	uint32_t id = PAGE_HEADER(page)->id;

	lseek(fd, PAGE_SIZE * id, SEEK_SET);
	assert(write(fd, page, PAGE_SIZE) != -1);
	fsync(fd);
}

void *load_page(int fd, uint32_t id)
{
	void *page = page_create(LEAF, id);

	lseek(fd, PAGE_SIZE * id, SEEK_SET);
	assert(read(fd, page, PAGE_SIZE) != -1);
	return (page);
}
