#include <iostream>
#include <fstream>
#include "Header.h"

int main()
{
	HouseOfElves elf;
	elf.readLetters();
	elf.work();
	elf.countCandies();
	elf.print();
	HouseOfTrolls troll;
	troll ^ elf;
	troll.work();
	troll.print();
	MrsSanta mrs_santa;
	mrs_santa^ elf;
	mrs_santa^ troll;
	mrs_santa.work();
	mrs_santa.print();
	Santa santa;
	santa^ elf;
	santa.work();
	santa.print();
	mrs_santa.saveTheList(santa.letMrsSantaKnow());
	mrs_santa.printTowns();

	return 0;
}

