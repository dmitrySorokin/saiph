#ifndef DUNGEON_H
/* defines */
#define DUNGEON_H
/* buffer for various stuff */
#define BUFFER_SIZE 65536
/* dungeon limits */
#define ROWS 24
#define COLS 80
#define MAP_ROW_START 1 // where the actual map start
#define MAP_ROW_END 21 // and end
#define ATTRIBUTES_ROW 22
#define STATUS_ROW 23
/* colours & attributes */
#define INVERSE 7
#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37
/* map symbols */
#define SOLID_ROCK ' '
#define VERTICAL_WALL '|'
#define HORIZONTAL_WALL '-'
#define FLOOR '.'
#define OPEN_DOOR 128
#define CLOSED_DOOR '7'
#define IRON_BARS 129
#define TREE 130
#define CORRIDOR '#'
#define STAIRS_UP '<'
#define STAIRS_DOWN '>'
#define ALTAR '_'
#define GRAVE '\\'
#define THRONE 131
#define SINK 132
#define FOUNTAIN '{'
#define WATER '}'
#define ICE 133
#define LAVA 134
#define LOWERED_DRAWBRIDGE 135
#define RAISED_DRAWBRIDGE '8'
#define TRAP '^'
/* items */
#define NOITEM 0
#define ILLEGAL 'm'
#define WEAPON ')'
#define ARMOR '['
#define RING '='
#define AMULET '"'
#define TOOL '('
#define FOOD '%'
#define POTION '!'
#define SCROLL '?'
#define SPELLBOOK '+'
#define WAND '/'
#define GOLD '$'
#define GEM '*'
#define STATUE '`'
#define BOULDER '0'
#define IRON_BALL '`'
#define CHAINS '`'
#define VENOM '`'
/* special "monsters" */
#define NOMONSTER 0
#define PET 136
#define PLAYER 137


/* forward declare */
class Dungeon;

/* includes */
#include "Player.h"

/* namespace */
using namespace std;

/* a dungeon represents a map, including attribute/status rows */
class Dungeon {
	public:
		/* variables */
		char map[ROWS][COLS + 1];
		unsigned char colour[ROWS][COLS];
		char messages[BUFFER_SIZE];
		bool menu;
		bool question;
		Player player;

		/* constructors */
		Dungeon();
		Dungeon(const Dungeon &dungeon);
};
#endif
