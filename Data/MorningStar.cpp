#include "MorningStar.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const MorningStar*> MorningStar::_morning_stars;

/* protected constructors */
MorningStar::MorningStar(const string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
MorningStar::~MorningStar() {
}

/* public static methods */
void MorningStar::init() {
	/* morning stars */
	create("morning star", 10, 120, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* artifact morning stars */
	create("Trollsbane", 200, 120, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const MorningStar*>& MorningStar::morningStars() {
	return _morning_stars;
}

/* protected static methods */
void MorningStar::addToMap(const string& name, const MorningStar* morning_star) {
	_morning_stars[name] = morning_star;
	Weapon::addToMap(name, morning_star);
}

/* private static methods */
void MorningStar::create(const string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, bool one_handed, const int& alignment) {
	addToMap(name, new MorningStar(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}