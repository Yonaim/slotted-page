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

void *page_create(enum pageType type, uint32_t id)
{
	void       *page   = malloc(sizeof(Page));
	PageHeader *header = PAGE_HEADER(page);

	memset(page, 0, sizeof(Page));
	header->id         = id;
	header->type       = type;
	header->free_start = sizeof(PageHeader);
	header->free_end   = PAGE_SIZE - 1;
	header->free_size  = header->free_end - header->free_start;
	return (page);
}

void page_add_record(void *page, void *record, uint16_t size)
{
	PageHeader        *header = PAGE_HEADER(page);
	RecordPointerList *ptrs   = RECORD_POINTER_LIST(page);

	assert(header->free_size < size);

	RecordPointer add_record;
	add_record.location = (header->free_end) - size;
	add_record.size     = size;

	memcpy(&(page[header->free_start]), &add_record, sizeof(RecordPointer));
	memcpy(&(page[add_record.location]), record, add_record.size);

	header->free_start += sizeof(RecordPointer);
	header->free_end -= size;
	header->free_size -= size;
}

void *page_get_record(void *page, uint16_t idx)
{
	RecordPointer *ptr = &((RECORD_POINTER_LIST(page))->list[idx]);

	return (page + ptr->location);
}

// Marks the record at 'idx' as deleted
// The actual space reclaimed during compaction
void page_remove_record(void *page, uint16_t idx)
{
	RecordPointer *ptr = &((RECORD_POINTER_LIST(page))->list[idx]);

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
