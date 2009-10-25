#include "Dagger.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Dagger*> Dagger::_daggers;

/* protected constructors */
Dagger::Dagger(const string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Dagger::~Dagger() {
}

/* public static methods */
void Dagger::init() {
	/* daggers */
	create("orcish dagger", 4, 10, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("dagger", 4, 10, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("silver dagger", 40, 12, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_SILVER, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("athame", 4, 10, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("elven dagger", 4, 10, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_WOOD, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("crude dagger", _daggers["orcish dagger"]);
	addToMap("runed dagger", _daggers["elven dagger"]);

	/* artifact daggers */
	create("Grimtooth", 300, 10, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, CHAOTIC);
	create("Magicbane", 3500, 10, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_MAGICRES | PROPERTY_ARTIFACT, true, NEUTRAL);
	create("Sting", 800, 10, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_WOOD, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, CHAOTIC);
}

const map<const string, const Dagger*>& Dagger::daggers() {
	return _daggers;
}

/* protected static methods */
void Dagger::addToMap(const string& name, const Dagger* dagger) {
	_daggers[name] = dagger;
	Weapon::addToMap(name, dagger);
}

/* private static methods */
void Dagger::create(const string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, bool one_handed, const int& alignment) {
	addToMap(name, new Dagger(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
