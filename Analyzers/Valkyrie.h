#ifndef VALKYRIE_H
/* defines */
#define VALKYRIE_H

/* forward declare */
class Valkyrie;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* sets up things for a player of class valkyrie */
class Valkyrie : public Analyzer {
	public:
		/* constructors */
		Valkyrie(Saiph *saiph);

		/* methods */
		void init();

	private:
		/* variables */
		Saiph *saiph;
		Request req;

		/* methods */
		void setupFood();
		void setupWand();
		void setupWeapon();
};
#endif