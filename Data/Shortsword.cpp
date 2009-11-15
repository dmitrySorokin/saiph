#include "Shortsword.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Shortsword*> Shortsword::_shortswords;

/* protected constructors */
Shortsword::Shortsword(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Shortsword::~Shortsword() {
}

/* public static methods */
void Shortsword::init() {
	/* short swords */
	create("orcish short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("dwarvish short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("elven short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_WOOD, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("crude short sword", _shortswords["orcish short sword"]);
	addToMap("broad short sword", _shortswords["dwarvish short sword"]);
	addToMap("runed short sword", _shortswords["elven short sword"]);
}

const map<const string, const Shortsword*>& Shortsword::shortswords() {
	return _shortswords;
}

/* protected static methods */
void Shortsword::addToMap(const string& name, const Shortsword* short_sword) {
	_shortswords[name] = short_sword;
	Weapon::addToMap(name, short_sword);
}

/* private static methods */
void Shortsword::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Shortsword(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
