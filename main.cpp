// author: georgiosmatzarapis

#include <iostream>

int main() {
	std::string aMyString;
	do {
		std::cout << "Press 'Q' or 'q' to exit " << std::endl;
	} while (std::getline(std::cin, aMyString) && aMyString != "Q" && aMyString != "q");
	return 0;
}
