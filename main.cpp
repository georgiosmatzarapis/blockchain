// author: georgiosmatzarapis

#include <iostream>
#include <memory>

#include "include/Block.hpp"

int main() {
  std::string aSender{};
  std::string aReceiver{};
  double aAmount{};

  std::cout << "Please fill in your transaction details:" << std::endl;
  std::cout << "Sender: ";
  std::cin >> aSender;
  std::cout << "Receiver: ";
  std::cin >> aReceiver;
  std::cout << "Amount: ";
  std::cin >> aAmount;

  transaction::Payload aPayload{aSender, aReceiver, aAmount};

  auto aData{std::make_unique<std::vector<transaction::Payload>>(
      std::initializer_list<transaction::Payload>{aPayload})};
  block::Block aBlock{"cd12daBlockHash0aa21", "previousHash", 1,
                      std::move(aData), 2};

  aBlock.display();

  return 0;
}
