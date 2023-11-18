// author: georgiosmatzarapis

#pragma once

#include <chrono>
#include <cstdint>
#include <string>

namespace transaction {
class Coinbase {
 public:
  /**
   * @brief Indicate first transaction in the network.
   * Can also be used for mining rewards.
   */
  Coinbase(std::string owner, const double& amount,
           const std::chrono::system_clock::time_point& timestamp);
  Coinbase(Coinbase&& coinbase) noexcept;
  Coinbase(const Coinbase& coinbase);
  ~Coinbase();

  [[nodiscard]] std::string getOwner() const;
  [[nodiscard]] double getAmount() const;
  [[nodiscard]] const std::chrono::system_clock::time_point&
  getTimestamp() const;

 private:
  std::string _owner{};
  double _bitcoinAmount{};
  std::chrono::system_clock::time_point _timestamp{};
};

class Payload {
 public:
  Payload(std::string sender, std::string receiver, const double& amount,
          const std::chrono::system_clock::time_point& timestamp);
  Payload(Payload&& payload) noexcept;
  Payload(const Payload& payload);
  ~Payload();

  [[nodiscard]] std::string getSender() const;
  [[nodiscard]] std::string getReceiver() const;
  [[nodiscard]] std::uint64_t getSatoshiAmount() const;
  [[nodiscard]] double getBitcoinAmount() const;
  [[nodiscard]] const std::chrono::system_clock::time_point&
  getTimestamp() const;
  [[nodiscard]] std::string getBitcoinRepresentation() const;

 private:
  static const std::string RemoveTrailingZeros(const std::string& iAmount);

  /**
   * @brief Convert the Bitcoin value to satoshi.
   * fyi: Bitcoins are typically represented in satoshis, where 1 Bitcoin is
   * equal to 100,000,000 satoshis.
   * @return Satoshi value.
   */
  std::uint64_t bitcoinToSatoshi() const;

  /**
   * @brief Represent Bitcoin in std::string for display purposes.
   * @return Bitcoin representation in std::string.
   */
  std::string bitcoinRepresentation() const;

  std::string _sender{};
  std::string _receiver{};
  std::uint64_t _satoshiAmount{};
  double _bitcoinAmount{};
  std::chrono::system_clock::time_point _timestamp{};
  std::string _bitcoinRepresentation{};
};
} // namespace transaction
