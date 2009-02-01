#include "Dig.h"
#include "../World.h"
#include "../Level.h"
#include "../Globals.h"

using namespace std;

Dig::Dig(Saiph* saiph) : Analyzer("Dig"), saiph(saiph), dig_direction(0) {
}

void Dig::parseMessages(const string& messages) {
	if (dig_direction && messages.find(MESSAGE_WHAT_TO_APPLY) != string::npos) {
		command = findDiggingTool();
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (dig_direction && messages.find(MESSAGE_DIG_DIRECTION) != string::npos) {
		command = dig_direction;
		priority = PRIORITY_CONTINUE_ACTION;
		dig_direction = 0;
	}
	if (!findDiggingTool())
		return;
	if (directionIsFloor(DOWN) && directionIsWall(N) && ((directionIsWall(W) && directionIsFloor(NW)) || (directionIsWall(E) && directionIsFloor(NE))))
		dig_direction = N;
	else if (directionIsFloor(DOWN) && directionIsWall(S) && ((directionIsWall(W) && directionIsFloor(SW)) || (directionIsWall(E) && directionIsFloor(SE))))
		dig_direction = S;
	else if (saiph->levels[saiph->position.level].branch != BRANCH_SOKOBAN && boulderInDirection() != 0)
		dig_direction = boulderInDirection();
	else
		dig_direction = 0;
}

void Dig::analyze() {
	if (dig_direction) {
		command = APPLY;
		priority = PRIORITY_DIG_PATH;
	}
}

int Dig::directionIs(int direction) {
	saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col];
	int rowOffset = 0, colOffset = 0;
	switch(direction) {
		case NOWHERE:
		case DOWN:
			break;
		case NW:
			rowOffset = -1;
			colOffset = -1;
			break;
		case N:
			rowOffset = -1;
			break;
		case NE:
			rowOffset = -1;
			colOffset = 1;
			break;
		case W:
			colOffset = -1;
			break;
		case E:
			colOffset = 1;
			break;
		case SW:
			rowOffset = 1;
			colOffset = -1;
			break;
		case SE:
			rowOffset = 1;
			colOffset = 1;
			break;
	}
	return saiph->levels[saiph->position.level].dungeonmap[saiph->position.row+rowOffset][saiph->position.col+colOffset];
}

bool Dig::directionIsWall(int direction) {
	int type = directionIs(direction);
	return type == VERTICAL_WALL || type == HORIZONTAL_WALL;
}

bool Dig::directionIsFloor(int direction) {
	return directionIs(direction) == FLOOR;
}

unsigned char Dig::findDiggingTool() {
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); i++)
		if ((i->second.name == "pick-axe" || i->second.name == "dwarvish mattock") && i->second.beatitude != CURSED)
			return i->first;
	return 0;
}

int Dig::boulderInDirection() {
	Level curlev = saiph->levels[saiph->position.level];
	int row = saiph->position.row, col = saiph->position.col;
	if (curlev.dungeonmap[row-1][col-1] == BOULDER)
		return NW;
	if (curlev.dungeonmap[row-1][col] == BOULDER)
		return N;
	if (curlev.dungeonmap[row-1][col+1] == BOULDER)
		return NE;
	if (curlev.dungeonmap[row][col-1] == BOULDER)
		return W;
	if (curlev.dungeonmap[row][col+1] == BOULDER)
		return E;
	if (curlev.dungeonmap[row+1][col-1] == BOULDER)
		return SW;
	if (curlev.dungeonmap[row+1][col] == BOULDER)
		return S;
	if (curlev.dungeonmap[row+1][col+1] == BOULDER)
		return SE;
	return 0;
}
