#include "Header.h"


/******************************************************************************************
*			House of Elves																  *
*******************************************************************************************/

HouseOfElves::HouseOfElves() {
	nr_of_candies = 0;
	letters.clear();
	towns.clear();
}
void HouseOfElves::readLetters() {
	std::string line;
	std::string temp;
	std::string word;

	std::ifstream imbox;
    try {
        imbox.open("Imbox.csv");
    }

    catch (const std::ofstream::failure& e) {
        std::cout << " This file can't be open or doesn't exist!";
        
    }
    
    int i = 0;
    while (imbox >> temp) {
        if (temp == "")
            break;
        std::stringstream ss(temp);
        std::vector<std::string> row;

        while (std::getline(ss, word, ';')) {
            row.push_back(word);
        }
        Letter newLetter;
        newLetter.last_name = row[0];
        newLetter.first_name = row[1];
        std::string str = row[2];
        std::stringstream s(str);
        int x;
        s >> x;
        newLetter.town = x;
        towns.push_back(row[2]);
        newLetter.age = std::stoi(row[3]);
        newLetter.color = row[4];

        for (int j = 5; j < row.size(); j++) {
            newLetter.wish_list.push_back(row[j]);
        }
        i++;
        this->letters.push_back(newLetter);
    }  
}

void HouseOfElves::work() {
    std::string line;
    std::string temp;
    std::string word;
    std::vector<std::pair<Letter, GiftForChildren<float> > > pair ;
    std::ifstream list;

    try {
        list.open("NaughtyList.csv");
    }

    catch (const std::ofstream::failure& e) {
        std::cout << " This file can't be open or doesn't exist!";

    }

    while (list >> temp) {
        std::stringstream ss(temp);
        std::vector<std::string> row;

        GiftForChildren<float> gift;

        while (std::getline(ss, word, ';')) {
            row.push_back(word);
        }

        gift.last_name = row[0];
        gift.first_name =row[1];

        if (row[2] == "naughty") {
            gift.budget = 10;
            gift.naughty = true;
        }
        else {
            gift.budget = 100;
            gift.naughty = false;
        }

        for (Letter letter : letters) {
            if (letter.last_name == gift.last_name &&
                letter.first_name == gift.first_name) {
                gift.colour = letter.color;
                pair.push_back(std::make_pair(letter, gift));
            }
        }
        
    }
    list.close();

    std::ifstream workshop;
    workshop.open("Workshop.csv");
    std::vector<Gift> workshopGifts;
    while (workshop >> temp) {

        std::vector<std::string> row;
        std::stringstream ss(temp);

        Gift gift;

        while (std::getline(ss, word, ';')) {
            row.push_back(word);
        }

        gift.gift_name = row[0];
        gift.stock = std::stoi(row[1]);
        gift.price = std::stoi(row[2]);
        workshopGifts.push_back(gift);
    }

    //checks every presents from the wishlists and decide if the child has
    // enough money to get that present or if the present is in the workshop at all
    for (auto it = pair.begin(); it != pair.end(); it++) {
        for (int i = 0; i < it->first.wish_list.size(); i++){
            for (int j = 0; j < workshopGifts.size(); j++) {
                if (it->first.wish_list[i] == workshopGifts[j].gift_name) {
                    if (it->second.budget - workshopGifts[j].price >= 0 && workshopGifts[j].stock > 0) {
                        it->second.budget -= workshopGifts[j].price;
                        it->second.presents.push_back(workshopGifts[j].gift_name);
                        workshopGifts[j].stock--;
                    }
                }
            }
          
        }
        // default gift
        if (it->second.presents.size() == 0) {
            if (it->second.budget == 100) {
                it->second.presents.push_back("defaultGift$100");
                it->second.budget = 0;
            }

            else {
                it->second.presents.push_back("defaultGift$10");
                it->second.budget = 0;
            }
        }
        gifts.push_back(it->second);
    }
}

void HouseOfElves::countCandies() {
    for (GiftForChildren<float> gift : gifts) {
        this->nr_of_candies += gift.budget;
    }
}

int HouseOfElves::sendCandies() {
    return nr_of_candies;
}

std::vector<std::string> HouseOfElves::sendTowns() {
    return towns;
}

void HouseOfElves::print() {
    std::cout << "The Elves counted..." << std::endl;
    for (GiftForChildren<float> gift : gifts) {
        std::cout << gift.last_name + " " + gift.first_name << ": ";
        for (int i = 0; i < gift.presents.size(); i++) {
            std::cout << gift.presents[i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "The Elves are done printing!" << std::endl;
}

/******************************************************************************************
*			House of Trolls															      *
*******************************************************************************************/


void HouseOfTrolls::operator ^ (HouseOfElves& elf) {
    gifts = elf.sendGifts();
}

void HouseOfTrolls::work() {
    for (GiftForChildren<float> gift : gifts) {
        if (gift.naughty) {
            gift.coal = true;
            nr_of_coals++;
        }
        else
            gift.coal = false;
        if (gift.colour == "pink") {
            gift.wrap = "pink";
            nr_of_pink_wraps++;
        }
        else {
            gift.wrap = "blue";
            nr_of_blue_wraps++;
        }
    }
}

void HouseOfTrolls::print() {
    std::cout << "The Trolls counted..." << std::endl;
    std::cout << "Blue wraps: " << nr_of_blue_wraps << std::endl;
    std::cout << "Pink wraps: " << nr_of_pink_wraps << std::endl;
    std::cout << "The Trolls are done printing!" << std::endl;
}

int HouseOfTrolls::sendNrOfCoals() {
    return nr_of_coals;
}


/******************************************************************************************
*			Mrs Sanata														              *
*******************************************************************************************/

void MrsSanta::operator^(HouseOfElves& elf) {
    nr_of_candies = elf.sendCandies();
}

void MrsSanta::operator^(HouseOfTrolls& trol) {
    nr_of_coals = trol.sendNrOfCoals();
}

void MrsSanta::work() {
    extra_budget = nr_of_candies + 0.5 * nr_of_coals;
}

void MrsSanta::print() {
    std::cout << extra_budget << std::endl;
}

void MrsSanta::saveTheList(std::vector<std::string> towns) {
    this->towns = towns;
}

void MrsSanta::printTowns() {
    std::cout << "I know Santa's list of towns! " << std::endl;
    for (int i = 0; i < towns.size();i++)
        std::cout << towns[i] << " ";
    std::cout << std::endl;
}

/******************************************************************************************
*			                         Sanata	     							              *
*******************************************************************************************/

Santa::Santa() {
    for (int i = 0;i < (1 << n);i++) {
        for (int j = 0;j < n;j++) {
            dp[i][j] = -1;
        }
    }
    towns.clear();
}
void Santa::operator^(HouseOfElves& elf) {
    towns = elf.sendTowns();
    //n = towns.size() + 1; // plus 1 means north pole is included 
}

void Santa::work() {
    std::string line;
    std::string temp;
    std::string word;
    std::ifstream list;
    list.open("Towns.csv");
    int ct = 0;
    int begin = 1;
    int x = 1;

    //insert in a matrix every town from csv
    while (list >> temp) {
        std::stringstream ss(temp);
        std::vector<std::string> row;
        ct++;

        while (std::getline(ss, word, ';')) {
            row.push_back(word);
            
        }

        if (ct == 1)
            continue;

        if (ct == 2) {
            pair.insert(std::make_pair(ct - 2,row[0]));
            for (int i = 1; i < row.size(); i++)
                add_matrix[0][i - 1] = std::stof(row[i]);
            continue;
        }
            
        
        for (int i = 0; i < towns.size();i++) {
            if (row[0] == towns[i]) {
                pair.insert(std::make_pair(x, towns[i]));
                for (int i = 1; i < row.size(); i++)
                    add_matrix[begin][i - 1] = std::stof(row[i]);
            }
           
        }
        begin++;
        x++;
                   
    }
    
    list.close();
}

// path uses the a similar algorithm that tsp method uses, but prints the order of towns
// uses a mask with nr of towns bits. bit 0 means not visited, 1 means visited

void  Santa::path(int mask, int pos) {

    if (mask == VISITED_ALL) return; // if all towns were visited we stop
    int ans = INT_MAX, chosenCity;

    for (int city = 0;city < n;city++) {

        if ((mask & (1 << city)) == 0) { // if not visited

            int newAns = add_matrix[pos][city] + dp[mask | (1 << city)][city];
            if (newAns < ans) {
                ans = newAns;
                chosenCity = city;
            }
        }

    }
    towns.push_back(pair[chosenCity]);
    
    path(mask | (1 << chosenCity), chosenCity);
}

float Santa::tsp(int mask, int pos) {

    if (mask == VISITED_ALL) {
        return 0;
    }
    if (dp[mask][pos] != -1) {

        return dp[mask][pos];
    }

    //Now from current node, algorithm will try to go to every other node and take the min ans
    float ans = INT_MAX;

    //Visit all the unvisited cities and take the best route
    for (int city = 0;city < n;city++) {

        if ((mask & (1 << city)) == 0) { // if not visited

            float newAns = add_matrix[pos][city] + tsp(mask | (1 << city), city);
            ans = std::min(ans, newAns);
        }

    }

    return dp[mask][pos] = ans;

}

std::vector<std::string> Santa::letMrsSantaKnow() {
    return towns;
}

void Santa::print() {
    towns.clear();
    std::cout << "Total Km: ";
    std::cout << tsp(1, 0) << std::endl;
    towns.push_back("Rovaniemi");
    path(1, 0);
    for (int i = 0; i < towns.size(); i++)
        std::cout << towns[i] << " ";
    std::cout << std::endl;
}