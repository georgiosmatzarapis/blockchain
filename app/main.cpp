// author: georgiosmatzarapis

#include <iostream>

#include "../blockchain/include/Block.hpp"
#include "../blockchain/include/User.hpp"

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

  std::vector<std::unique_ptr<transaction::Payload>> aData{};
  aData.push_back(
      std::make_unique<transaction::Payload>(aOwner, aReceiver, aAmount));

  block::Block aBlock{"cd12daBlockHash0aa21", "previousHash", 1,
                      std::move(aData), 2};

  aBlock.display();

  return 0;
}
