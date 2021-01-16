#pragma once
#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#define INT_MAX 999999


//-------------------Letter containing children information ------------------------------------
struct Letter {
	std::string last_name;
	std::string first_name;
	int age;
	std::string town;
	std::vector<std::string> wish_list;
	std::string color; // blue - boy; pink - girl
};

//------a structure which helps us to store the gifts available in the workshop
struct Gift {
	std::string gift_name;
	int stock;
	float price;
};
//------a struct that helps elves to organise each present taking in consideration if that child is naughty or not
//------use template because the budget can be int or float 
template<typename T>
struct GiftForChildren {
	std::string last_name;
	std::string first_name;
	std::string colour;
	bool naughty;
	T budget;
	std::vector<std::string> presents; //this vector stores all the presents from the whishlist
	int number_of_candies;
	bool coal; //false if nice(no coal for this children) ; true otherwise (naughty child)
	std::string wrap; //pink-girls ; blue-boys;
};

//abstract class
class SantaCrew { 
public:
	virtual void work() = 0;
	virtual void print() = 0;
};

class HouseOfElves : SantaCrew {
private:
	std::vector<Letter> letters;
	int nr_of_candies;
	std::vector<std::string> towns;
	std::vector<GiftForChildren<float>> gifts;
public:
	HouseOfElves();
	void print() override; // prints the work that the elves have done
	void readLetters(); // reads the information from the csv file and stores it in a vector of letters
	void work() override; // this method is called in order to process the information stored in letters
	void countCandies(); // this method is called to count the candies that Mrs Santa has to bake
	int sendCandies(); // this method is called from Mrs Santa class to retrieve the number of candies
	std::vector<std::string> sendTowns(); // called by Santa to store the towns that he has to visit
	std::vector<GiftForChildren<float>> sendGifts() const { return gifts; } // called by the trolls to get the gifts
	
	~HouseOfElves() {
		std::cout << "The Elves went to sleep ...";
	}
};

class HouseOfTrolls : SantaCrew {
private:
	std::vector<GiftForChildren<float>> gifts;
	int nr_of_coals;
	int nr_of_blue_wraps;
	int nr_of_pink_wraps;
public:
	HouseOfTrolls() {
		gifts.clear();
		nr_of_blue_wraps = 0;
		nr_of_pink_wraps = 0;
		nr_of_coals = 0;
	}
	void print() override; // prints the work done by the trolls
	void work() override; // method called to process information taken from the elves
	int sendNrOfCoals();
	void operator ^ (HouseOfElves& elf); // operator used to retrieve the gifts from the elves

	~HouseOfTrolls() {
		std::cout << "The Trolls went to sleep ...";
	}
};

class MrsSanta : SantaCrew {
private:
	float extra_budget;
	int nr_of_candies;
	int nr_of_coals;
	std::vector<std::string>towns;
public:
	void work() override;
	void print() override;
	void operator ^ (HouseOfTrolls& troll); // operator used to retrieve the number of coals from trolls
	void operator ^ (HouseOfElves& elf); // operator used to retrieve the number of candies from elves 
	void saveTheList(std::vector<std::string> towns); // method called to ask for the list of towns
	void printTowns();

	~MrsSanta() {
		std::cout << "Mrs Santa went to sleep ...";
	}
};

class Santa : SantaCrew {
private:
	std::vector<std::string> towns;
	float add_matrix[10][10];
	std::map<int, std::string >  pair;
	float dp[64][6];
	int n = 6;
	int VISITED_ALL = (1 << n) - 1;
public:
	Santa();
	void operator ^ (HouseOfElves& elf); // operator used to retrieve the list of towns from the elves
	void work() override;
	void print() override;
	void  path(int mask, int pos); // prints the shortest path santa has to fallow
	float tsp(int mask, int pos); // algorithm used for calculating the total km
	std::vector<std::string>letMrsSantaKnow(); // sends towns to Mrs Santa

	~Santa() {
		std::cout << "Santa went to sleep ...";
	}
};

