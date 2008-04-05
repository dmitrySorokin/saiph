#include "Saiph.h"

/* constructors */
Saiph::Saiph(bool remote) {
	connection = new Connection(remote);
	world = new World(connection);

	/* next command */
	command.analyzer = -1;
	command.priority = 0;

	/* pathing */
	pathcost = new unsigned int*[ROWS];
	for (int r = 0; r < ROWS; ++r) {
		pathcost[r] = new unsigned int[COLS];
		for (int c = 0; c < COLS; ++c)
			pathcost[r][c] = 0xffff;
	}
	pathpos = new char*[ROWS * COLS];
	for (int r = 0; r < ROWS * COLS; ++r) {
		pathpos[r] = new char[2];
		pathpos[r][0] = 0;
		pathpos[r][1] = 0;
	}
	passable = new char[MAX_PASSABLE];
	passable_count = 0;
	/* most frequent features should be first */
	passable[passable_count++] = FLOOR;
	passable[passable_count++] = CORRIDOR;
	passable[passable_count++] = WATER;
	passable[passable_count++] = OPEN_DOOR;
	passable[passable_count++] = STAIRS_UP;
	passable[passable_count++] = STAIRS_DOWN;
	passable[passable_count++] = FOUNTAIN;
	passable[passable_count++] = ALTAR;
	passable[passable_count++] = GRAVE;
	passable[passable_count++] = TRAP;
	passable[passable_count++] = SINK;
	passable[passable_count++] = THRONE;
	passable[passable_count++] = ICE;
	passable[passable_count++] = LAVA;
	passable[passable_count++] = LOWERED_DRAWBRIDGE;
	passable[passable_count++] = WEAPON;
	passable[passable_count++] = ARMOR;
	passable[passable_count++] = RING;
	passable[passable_count++] = AMULET;
	passable[passable_count++] = TOOL;
	passable[passable_count++] = FOOD;
	passable[passable_count++] = POTION;
	passable[passable_count++] = SCROLL;
	passable[passable_count++] = SPELLBOOK;
	passable[passable_count++] = WAND;
	passable[passable_count++] = GOLD;
	passable[passable_count++] = GEM;
	passable[passable_count++] = STATUE;
	passable[passable_count++] = IRON_BALL;
	passable[passable_count++] = CHAINS;
	passable[passable_count++] = VENOM;

	/* branches */
	branches = new Branch*[MAX_BRANCHES];
	current_branch = BRANCH_MAIN;
	for (int b = 0; b < MAX_BRANCHES; ++b) {
		branches[b] = new Branch;
		memset(branches[b]->map, ' ', MAX_DUNGEON_DEPTH * ROWS * COLS);
		memset(branches[b]->search, '\0', MAX_DUNGEON_DEPTH * ROWS * COLS);
		memset(branches[b]->unpassable, 1, MAX_DUNGEON_DEPTH * ROWS * COLS);
		memset(branches[b]->diagonally_unpassable, 1, MAX_DUNGEON_DEPTH * ROWS * COLS);
	}

	/* messages */
	messages = "";

	/* Analyzers */
	analyzers = new Analyzer*[MAX_ANALYZERS];
	analyzer_count = 0;
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new DoorAnalyzer(this));
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new ExploreAnalyzer(this));
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new HealthAnalyzer(this));
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new LevelAnalyzer(this));
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new MonsterAnalyzer(this));
}

/* destructors */
Saiph::~Saiph() {
	for (int a = 0; a < analyzer_count; ++a)
		delete analyzers[a];
	delete [] analyzers;
	for (int b = 0; b < MAX_BRANCHES; ++b)
		delete branches[b];
	delete [] branches;
	for (int p = 0; p < ROWS; ++p)
		delete [] pathcost[p];
	delete [] pathcost;
	for (int p = 0; p < (ROWS * COLS); ++p)
		delete [] pathpos[p];
	delete [] pathpos;
	delete [] passable;
	delete world;
	delete connection;
}

/* methods */
void Saiph::dumpMaps() {
	/* search map */
	cout << (char) 27 << "[26;1H";
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (branches[current_branch]->search[world->player.dungeon][r][c] % 96 + 32);
			//cout << (int) branches[current_branch]->unpassable[world->player.dungeon][r][c];
			//cout << (int) branches[current_branch]->diagonally_unpassable[world->player.dungeon][r][c];
		}
		cout << endl;
	}
	/* world map as the bot sees it */
	for (int r = 0; r < ROWS; ++r) {
		cout << (char) 27 << "[" << r + 1 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (branches[current_branch]->map[world->player.dungeon][r][c]);
		}
	}
	/* path map */
	for (int r = 0; r < ROWS; ++r) {
		cout << (char) 27 << "[" << r + 26 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (pathcost[r][c] % 96 + 32);
		}
	}
	/* return cursor back to player */
	cout << (char) 27 << "[" << world->player.row + 1 << ";" << world->player.col + 1 << "H";
}

void Saiph::farlook(int row, int col) {
	/* look at something, eg. monster */
	char command[ROWS + COLS + 3];
	int pos = 0;
	command[pos++] = ';';
	int cursor_row = world->player.row;
	int cursor_col = world->player.col;
	while (cursor_row != row && cursor_col != col)
		command[pos++] = findNextDirection(row, col, cursor_row, cursor_col);
	command[pos++] = ',';
	command[pos] = '\0';
	cerr << command << endl;
	world->command(command);
}

char Saiph::findNextDirection(const int to_row, const int to_col, int &from_row, int &from_col) {
	/* finds the next key we must press to move the cursor.
	 * this ignores walls & such!
	 * this will also change from_row/from_col! */
	if (from_row < to_row && from_col < to_col) {
		from_row++;
		from_col++;
		return MOVE_SE;
	}
	if (from_row < to_row && from_col > to_col) {
		from_row++;
		from_col--;
		return MOVE_SW;
	}
	if (from_row > to_row && from_col < to_col) {
		from_row--;
		from_col++;
		return MOVE_NE;
	}
	if (from_row > to_row && from_col > to_col) {
		from_row--;
		from_col--;
		return MOVE_NW;
	}
	if (from_row < to_row) {
		from_row++;
		return MOVE_S;
	}
	if (from_row > to_row) {
		from_row--;
		return MOVE_N;
	}
	if (from_col < to_col) {
		from_col++;
		return MOVE_E;
	}
	if (from_col > to_col) {
		from_col--;
		return MOVE_W;
	}
	return -1;
}

bool Saiph::run() {
	/* update maps */
	updateMaps();

	/* print stuff so we see what we're doing */
	dumpMaps();

	/* reset command */
	command.analyzer = -1;
	command.priority = 0;

	/* save dungeon in history */

	/* deal with messages */
	messages = world->messages;
	for (int a = 0; a < analyzer_count; ++a) {
		int priority = analyzers[a]->parseMessages();
		if (priority > command.priority) {
			command.analyzer = a;
			command.priority = priority;
		}
	}

	/* call start() in analyzers */
	for (int a = 0; a < analyzer_count; ++a) {
		int priority = analyzers[a]->start();
		if (priority > command.priority) {
			command.analyzer = a;
			command.priority = priority;
		}
	}

	/* inspect the dungeon */
	inspect();

	/* call finish() in analyzers */
	for (int a = 0; a < analyzer_count; ++a) {
		int priority = analyzers[a]->finish();
		if (priority > command.priority) {
			command.analyzer = a;
			command.priority = priority;
		}
	}

	/* check if we got a command */
	if (command.analyzers == -1)
		return false;

	/* let an analyzer do its command */
	analyzers[command.analyzer]->command();
	return true;
}

char Saiph::shortestPath(int row, int col, bool allow_illegal_last_move, int &distance, bool &direct_line) {
	/* attempt to find the shortest path to destination.
	 * returns move or -1 if unable to find a path */
	/* allow_illegal_last_move:
	 * if this is true we'll "move" diagonally through doors if it's the last move.
	 * why? because monsters may stand in a door way, making it possible to attack them */
	distance = 0;
	direct_line = true;
	if (row < MAP_ROW_START || row >= MAP_ROW_END || col < 1 || col > COLS - 1)
		return -1;
	unsigned int curcost = pathcost[row][col];
	unsigned int antiloop = curcost - 666; // any other value but curcost :)
	char move = -1;
	char prevmove = -1;
	while (curcost > 0 && antiloop != curcost) {
		int r = row;
		int c = col;
		char s = branches[current_branch]->map[world->player.dungeon][row][col];
		antiloop = curcost; // if curcost doesn't change the loop will end
		if (pathcost[row - 1][col - 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || (s != OPEN_DOOR && branches[current_branch]->map[world->player.dungeon][row - 1][col - 1] != OPEN_DOOR && (branches[current_branch]->diagonally_unpassable[world->player.dungeon][row - 1][col] == 0 || branches[current_branch]->diagonally_unpassable[world->player.dungeon][row][col - 1] == 0)))) {
			move = MOVE_SE;
			r = row - 1;
			c = col - 1;
			curcost = pathcost[r][c];
		}
		if (pathcost[row - 1][col] < curcost) {
			move = MOVE_S;
			r = row - 1;
			c = col;
			curcost = pathcost[r][c];
		}
		if (pathcost[row - 1][col + 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || (s != OPEN_DOOR && branches[current_branch]->map[world->player.dungeon][row - 1][col + 1] != OPEN_DOOR && (branches[current_branch]->diagonally_unpassable[world->player.dungeon][row - 1][col] == 0 || branches[current_branch]->diagonally_unpassable[world->player.dungeon][row][col + 1] == 0)))) {
			move = MOVE_SW;
			r = row - 1;
			c = col + 1;
			curcost = pathcost[r][c];
		}
		if (pathcost[row][col - 1] < curcost) {
			move = MOVE_E;
			r = row;
			c = col - 1;
			curcost = pathcost[r][c];
		}
		if (pathcost[row][col + 1] < curcost) {
			move = MOVE_W;
			r = row;
			c = col + 1;
			curcost = pathcost[r][c];
		}
		if (pathcost[row + 1][col - 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || (s != OPEN_DOOR && branches[current_branch]->map[world->player.dungeon][row + 1][col - 1] != OPEN_DOOR && (branches[current_branch]->diagonally_unpassable[world->player.dungeon][row + 1][col] == 0 || branches[current_branch]->diagonally_unpassable[world->player.dungeon][row][col - 1] == 0)))) {
			move = MOVE_NE;
			r = row + 1;
			c = col - 1;
			curcost = pathcost[r][c];
		}
		if (pathcost[row + 1][col] < curcost) {
			move = MOVE_N;
			r = row + 1;
			c = col;
			curcost = pathcost[r][c];
		}
		if (pathcost[row + 1][col + 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || (s != OPEN_DOOR && branches[current_branch]->map[world->player.dungeon][row + 1][col + 1] != OPEN_DOOR && (branches[current_branch]->diagonally_unpassable[world->player.dungeon][row + 1][col] == 0 || branches[current_branch]->diagonally_unpassable[world->player.dungeon][row][col + 1] == 0)))) {
			move = MOVE_NW;
			r = row + 1;
			c = col + 1;
			curcost = pathcost[r][c];
		}
		if (distance > 0 && prevmove != move)
			direct_line = false;
		prevmove = move;
		++distance;
		row = r;
		col = c;
	}
	if (curcost > 0)
		return -1;
	return move;
}

/* private methods */
void Saiph::inspect() {
	/* inspect the dungeon for interesting monsters/objects/places
	 * iterations:
	 * r: 20 ->      20
	 * c: 80 ->    1600
	 * a: 32 ->   51200
	 * s: 64 -> 3276800
	 * so try not to track unneeded symbols.
	 */
	char symbol;
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			symbol = world->map[r][c];
			for (int a = 0; a < analyzer_count; ++a) {
				for (int s = 0; s < analyzers[a]->symbol_count; ++s) {
					if (analyzers[a]->symbols[s] == symbol)
						analyzers[a]->analyze(r, c, symbol);
				}
			}
		}
	}
}

void Saiph::updateMaps() {
	/* update the various maps */
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			char s = world->map[r][c];
			if (s == SOLID_ROCK)
				continue;
			char hs = branches[current_branch]->map[world->player.dungeon][r][c];
			if (s == VERTICAL_WALL || s == HORIZONTAL_WALL || s == FLOOR || s == OPEN_DOOR || s == CLOSED_DOOR || s == IRON_BARS || s == TREE || s == CORRIDOR || s == STAIRS_UP || s == STAIRS_DOWN || s == ALTAR || s == GRAVE || s == THRONE || s == SINK || s == FOUNTAIN || s == WATER || s == ICE || s == LAVA || s == LOWERED_DRAWBRIDGE || s == RAISED_DRAWBRIDGE || s == TRAP) {
				/* "static" dungeon features (doors may be destroyed, though).
				 * update the map showing static stuff */
				branches[current_branch]->map[world->player.dungeon][r][c] = s;
			} else if (hs == SOLID_ROCK) {
				/* "dynamic" stuff that can disappear on a spot we've never seen before.
				 * pretend there's an open door here until we see otherwise */
				branches[current_branch]->map[world->player.dungeon][r][c] = OPEN_DOOR;
			} else if (hs == CLOSED_DOOR) {
				/* there used to be a door here, but now something else is here.
				 * it's quite possible a monster opened the door */
				branches[current_branch]->map[world->player.dungeon][r][c] = OPEN_DOOR;
			} else if (hs == HORIZONTAL_WALL || hs == VERTICAL_WALL) {
				/* there used to be a wall here, but isn't no longer.
				 * make it an open door for the time being */
				branches[current_branch]->map[world->player.dungeon][r][c] = OPEN_DOOR;
			}
			/* unpassable map */
			branches[current_branch]->unpassable[world->player.dungeon][r][c] = (s == VERTICAL_WALL || s == HORIZONTAL_WALL || s == CLOSED_DOOR || s == IRON_BARS || s == TREE || s == RAISED_DRAWBRIDGE || s == BOULDER) ? 1 : 0;
			/* diagonally unpassable map */
			branches[current_branch]->diagonally_unpassable[world->player.dungeon][r][c] = (s == VERTICAL_WALL || s == HORIZONTAL_WALL || s == CLOSED_DOOR || s == IRON_BARS || s == TREE || s == RAISED_DRAWBRIDGE || s == OPEN_DOOR) ? 1 : 0;
		}
	}

	updatePathMap();
}

void Saiph::updatePathMap() {
	/* create a path map used for finding shortest path */
	/* is this dijkstra? */
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c)
			pathcost[r][c] = UINT_MAX;
	}
	int row = world->player.row;
	int col = world->player.col;
	pathcost[row][col] = 0;
	int nextnode = 0;
	int nodes = 1;
	pathpos[nextnode][0] = row;
	pathpos[nextnode][1] = col;
	int curcost = 0;
	while (nextnode < nodes) {
		row = pathpos[nextnode][0];
		col = pathpos[nextnode][1];
		curcost = pathcost[row][col];
		char ds = branches[current_branch]->map[world->player.dungeon][row][col];
		for (int r = row - 1; r <= row + 1; ++r) {
			if (r < MAP_ROW_START || r >= MAP_ROW_END)
				continue;
			for (int c = col - 1; c <= col + 1; ++c) {
				if (c < 0 || c >= COLS)
					continue;
				char s = world->map[r][c];
				char dt = branches[current_branch]->map[world->player.dungeon][r][c];
				if (branches[current_branch]->unpassable[world->player.dungeon][r][c] == 1)
					continue;
				/* doors are special, no diagonal entrance/exit.
				 * even if it seems this isn't needed, it is!
				 * when there's a trap in front of a door the trap cost will make
				 * it impossible to move any further. as an example, let's say we're
				 * in the corridor leading to this room and we want that wand:
				 *  -----|
				 *  |.../|
				 * #-^...|
				 *  |....|
				 *  -----|
				 * in path map without this if:
				 *  ------
				 *  |579b|
				 * 0298ad|
				 *  |579b|
				 *  ------
				 * the pathing works by looking at the cost on the square of the wand,
				 * in this case the cost is "b" (11 in decimal (duh)). from that point it
				 * will go towards the lowest cost, in this case "9" straight to the left.
				 * further from "9" it will go to "7", to the left again, being the lowest cost.
				 * then it will go to "5". however, on square with "2" there's a door,
				 * and we can't move diagonally into that door, and every other direction cost
				 * more. thus, it'll stop here.
				 * if we make the cost map look like this instead:
				 *  ------
				 *  |bceg|
				 * 029bdf|
				 *  |bceg|
				 *  ------
				 * then it will move like this: g, d, b, 9, 2, 0
				 * we'll still need to check for doors when backtracing, though:
				 * let's say we're standing on either of the "b" instead, the cost for moving
				 * diagonally into the door is lower than the cost of moving on the trap.
				 */
				else if ((ds == OPEN_DOOR || dt == OPEN_DOOR) && r != row && c != col)
					continue;
				else if ((branches[current_branch]->diagonally_unpassable[world->player.dungeon][row][c] == 1 && branches[current_branch]->diagonally_unpassable[world->player.dungeon][r][col] == 1))
					continue;
				unsigned int newpathcost = curcost + ((r == row || c == col) ? COST_CARDINAL : COST_DIAGONAL);
				if (s == LAVA)
					newpathcost += COST_LAVA;
				else if (s == TRAP)
					newpathcost += COST_TRAP;
				else if (s == WATER)
					newpathcost += COST_WATER;
				if (newpathcost < pathcost[r][c]) {
					pathcost[r][c] = newpathcost;
					pathpos[nodes][0] = r;
					pathpos[nodes][1] = c;
					++nodes;
				}
			}
		}
		++nextnode;
	}
}

/* main */
int main() {
	Saiph saiph(false);
	//for (int a = 0; a < 5 && saiph.run(); ++a)
	//	;
	while (saiph.run())
		;
}
