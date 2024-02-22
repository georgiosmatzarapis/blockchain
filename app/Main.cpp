// author: georgiosmatzarapis

#include <iostream>

#include "Block.hpp"
#include "User.hpp"

int main() {
  std::string aOwner{};
  std::string aReceiver{};
  double aAmount{};

  std::cout << "Please fill in your transaction details:" << std::endl;
  std::cout << "Owner: ";
  std::cin >> aOwner;
  std::cout << "Receiver: ";
  std::cin >> aReceiver;
  std::cout << "Amount: ";
  std::cin >> aAmount;

  std::vector<std::unique_ptr<transaction::Payload>> aPayloads{};
  aPayloads.push_back(
      std::make_unique<transaction::Payload>(aOwner, aReceiver, aAmount));

  block::Block aBlock{"previousHash", 1, std::move(aPayloads)};

  aBlock.display();

  return 0;
}