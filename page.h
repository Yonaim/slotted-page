#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>

#define PAGE_SIZE 4096 // 4KB

// A page is a node of B-tree
enum pageType
{
	ROOT = 1,
	INTERIOR,
	LEAF
};

typedef struct Page
{
	uint32_t id;
	uint8_t  buf[PAGE_SIZE];
} Page;

// location: start of allocated space
typedef struct PageHeader
{
	uint16_t free_start;
	uint16_t free_end;
	uint8_t  flag;
} PageHeader;

typedef struct RecordPointer
{
	uint16_t location;
	uint16_t size;
} RecordPointer;

typedef struct RecordPointerList
{
	RecordPointer *list; // e.g list[2]
	uint16_t       size;
} RecordPointerList;

#define PAGE_HEADER(page) (pageHeader *)page;

//------------------------------------------------------------------------------

// In-memory page management (volatile operations)
Page              *page_create(enum pageType type, uint32_t id);
void               page_add_record(void *page, void *record, uint16_t size);
void              *page_get_record(void *page, uint16_t idx);
void               page_remove_record(void *page, uint16_t idx);
void               page_compact(void *page);
RecordPointerList *page_record_pointer_list(void *page);

// Persistent disk I/O (non-volatile storage)
void  save_page(int fd, void *page);
Page *load_page(int fd, uint32_t id);

#endif
