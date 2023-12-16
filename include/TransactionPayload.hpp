// author: georgiosmatzarapis

#pragma once

#include <chrono>
#include <cstdint>
#include <string>

namespace transaction {
static const std::string RemoveTrailingZeros(const std::string& iAmount);

/**
 * @brief Convert the Bitcoin value to satoshi.
 * fyi: Bitcoins are typically represented in satoshis, where 1 Bitcoin is
 * equal to 100,000,000 satoshis.
 * @return Satoshi value.
 */
std::uint64_t BitcoinToSatoshi(const double& iBitcoinAmount);

/**
 * @brief Represent Bitcoin in string for display purposes.
 * @return Bitcoin representation in std::string.
 */
std::string BitcoinRepresentation(const double& iBitcoinAmount);

class Coinbase {
 public:
  /**
   * @brief Indicate first transaction in the network.
   * Can also be used for mining rewards.
   */
  Coinbase(std::string owner, const double& bitcoinAmount);
  Coinbase(const Coinbase& coinbase);
  Coinbase& operator=(const Coinbase& coinbase);
  Coinbase(Coinbase&& coinbase) noexcept;
  Coinbase& operator=(Coinbase&& coinbase) noexcept;
  virtual ~Coinbase();

  [[nodiscard]] std::string getOwner() const;
  [[nodiscard]] double getBitcoinAmount() const;
  [[nodiscard]] const std::chrono::system_clock::time_point
  getTimestamp() const;
  [[nodiscard]] std::uint64_t getSatoshiAmount() const;
  [[nodiscard]] std::string getBitcoinRepresentation() const;

 private:
  std::string _owner{};
  double _bitcoinAmount{};
  std::chrono::system_clock::time_point _timestamp{};
  std::uint64_t _satoshiAmount{};
  std::string _bitcoinRepresentation{};
};

class Payload : public Coinbase {
 public:
  using Coinbase::Coinbase;

  Payload(std::string owner, std::string receiver, const double& bitcoinAmount);
  Payload(const Payload& payload);
  Payload& operator=(const Payload& payload);
  Payload(Payload&& payload) noexcept;
  Payload& operator=(Payload&& payload) noexcept;
  ~Payload() override;

  [[nodiscard]] std::string getReceiver() const;

 private:
  std::string _receiver{};
};
} // namespace transaction