#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>

#define PAGE_SIZE 4048 // 4KB

enum pageType
{
	ROOT = 1,
	INTERIOR,
	LEAF
};

// location: start of allocated space
struct PageHeader
{
	uint32_t id;
	uint16_t free_start;
	uint16_t free_end;
	uint8_t  flag;
};

struct RecordPointer
{
	uint16_t location;
	uint16_t size;
};

struct RecordPointerList
{
	RecordPointer *start;
	uint16_t       size;
};

#define PAGE_HEADER(page) (pageHeader *)page;

//------------------------------------------------------------------------------

// In-memory page management (volatile operations)
void              *page_new(enum pageType type, uint32_t id);
void               page_add_cell(uint16_t size);
void              *page_get_cell(uint16_t idx);
void               page_remove_cell(void *page, uint16_t idx);
void               page_compact(void *page);
RecordPointerList *page_record_pointer_list(void *page);

// Persistent disk I/O (non-volatile storage)
void  save_page(int fd, void *page);
void *load_page(int fd, uint32_t id);

#endif
