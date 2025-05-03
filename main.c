//------------------------------------------------------------------------------

enum POKEMON_TYPE
{
	NORMAL = 1,
	FIRE,
	WATER,
	ELECTRIC,
	GRASS,
	ICE,
	FAIRY
};

typedef struct PokemonRecord
{
	unsigned int  id;
	unsigned int  level;
	unsigned char type;
	char          name[10];
} PokemonRecord;

typedef struct TrainerRecord
{
	unsigned int id;
	unsigned int money;
	char         name[10];
} TrainerRecord;

typedef void (*display_record_t)(void *);

//------------------------------------------------------------------------------

#include "page.h"
#include <fcntl.h>
#include <stdio.h>

void display_pokemon_record(void *record)
{
	PokemonRecord *pokemon = (PokemonRecord *)record;

	printf("Pokemon id: %d, level: %d, type: %d, name: %s\n", pokemon->id,
		   pokemon->level, pokemon->type, pokemon->name);
}

void display_trainer_record(void *record)
{
	TrainerRecord *trainer = (TrainerRecord *)record;

	printf("Trainer id: %d, money: %d, name: %s\n", trainer->id, trainer->money,
		   trainer->name);
}

void add_pokemon_records(void *page)
{
	PokemonRecord record_pokemon[3];
	record_pokemon[0] = (PokemonRecord){
		.id = 393, .level = 12, .type = WATER, .name = "Piplup"};
	record_pokemon[1] = (PokemonRecord){
		.id = 35, .level = 7, .type = FAIRY, .name = "Clefairy"};
	record_pokemon[2] = (PokemonRecord){
		.id = 25, .level = 1, .type = ELECTRIC, .name = "Pikachu"};

	page_add_record(page, &record_pokemon[0], sizeof(PokemonRecord));
	page_add_record(page, &record_pokemon[1], sizeof(PokemonRecord));
	page_add_record(page, &record_pokemon[2], sizeof(PokemonRecord));
}

void add_trainer_records(void *page)
{
	TrainerRecord record_trainer[3];
	record_trainer[0] = (TrainerRecord){.id = 1, .money = 1000, .name = "Dawn"};
	record_trainer[0] = (TrainerRecord){.id = 2, .money = 500, .name = "Lucas"};
	record_trainer[0] = (TrainerRecord){.id = 3, .money = 300, .name = "Barry"};

	page_add_record(page, &record_trainer[0], sizeof(TrainerRecord));
	page_add_record(page, &record_trainer[1], sizeof(TrainerRecord));
	page_add_record(page, &record_trainer[2], sizeof(TrainerRecord));
}

void display_loaded_page(void *page, display_record_t display_func)
{
	PageHeader    *header = PAGE_HEADER(page);
	RecordPointer *ptr    = (RECORD_POINTER_LIST(page));
	void          *record;

	for (int i = 0; i < header->n_records; i++)
	{
		record = page_get_record(page, i);
		if (record == NULL)
			continue;
		display_func(record);
	}
}

int main()
{
	int   fd;
	void *page[2];
	void *loaded;

	fd = open("pokemon_world.db", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (1);

	page[0] = page_create(LEAF, 0);
	page[1] = page_create(LEAF, 1);

	add_pokemon_records(page[0]);
	add_trainer_records(page[1]);

	save_page(fd, page[0]);
	save_page(fd, page[1]);

	loaded = load_page(fd, 0);
	display_loaded_page(loaded, display_pokemon_record);

	loaded = load_page(fd, 1);
	display_loaded_page(loaded, display_trainer_record);

	return (0);
}
