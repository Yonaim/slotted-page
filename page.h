#ifndef PAGE_H
#define PAGE_H

#include <stdint.h>

#define PAGE_SIZE        4096 // 4KB
#define PAGE_CAN_COMPACT 0x01
#define PAGE_DIRTY       0x02
#define PAGE_FULL        0x04

// A page is a node of B-tree
enum pageType
{
	ROOT = 1,
	INTERIOR,
	LEAF
};

typedef struct Page
{
	uint8_t  buf[PAGE_SIZE];
} Page;

// metadata of a page
// location: start of allocated space
typedef struct PageHeader
{
	uint32_t id;
	enum pageType type;
	uint16_t      free_start;
	uint16_t      free_end;
	uint16_t      free_size; // free_end - free_start
	uint8_t       flags;
} PageHeader;

typedef struct RecordPointer
{
	uint16_t location; // start offset of record
	uint16_t size;     // size of record
} RecordPointer;

typedef struct RecordPointerList
{
	RecordPointer *list; // e.g list[2]
	uint16_t       size; // size of pointer list
} RecordPointerList;

#define PAGE_HEADER(page) ((PageHeader *)(page))
#define RECORD_POINTER_LIST(page) ((RecordPointerList *)((u_int8_t *)page + sizeof(PageHeader)))

//------------------------------------------------------------------------------

// In-memory page management (volatile operations)
void *page_create(enum pageType type, uint32_t id);
void  page_add_record(void *page, void *record, uint16_t size);
void *page_get_record(void *page, uint16_t idx);
void  page_remove_record(void *page, uint16_t idx);
void  page_compact(void *page);

// Persistent disk I/O (non-volatile storage)
void  save_page(int fd, void *page);
void *load_page(int fd, uint32_t id);

#endif
