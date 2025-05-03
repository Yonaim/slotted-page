#ifndef RECORD_H
#define RECORD_H

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

#endif
