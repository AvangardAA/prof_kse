#pragma once
#include <queue>
#include <string>
#include <vector>
#include <algorithm>
#include <print>

class BaseOrder 
{
public:
    int id;
    int quantity;
    bool isBid;
    std::string owner;

    BaseOrder(std::string owner, int id, int quantity, bool isBid) : owner(std::move(owner)), id(id), quantity(quantity), isBid(isBid) {}
    BaseOrder() = delete;
};

struct LimitOrder : public BaseOrder 
{
    double price;

    LimitOrder(std::string owner, int id, double price, int quantity, bool isBid) : BaseOrder(std::move(owner), id, quantity, isBid), price(price) {}
    LimitOrder() = delete;
};

struct MarketOrder : public BaseOrder 
{
    MarketOrder(std::string owner, int id, int quantity, bool isBid) : BaseOrder(std::move(owner), id, quantity, isBid) {}
    MarketOrder() = delete;
};

struct CompareOrders 
{
    auto operator()(const LimitOrder& a, const LimitOrder& b) -> bool 
    {
        if (a.isBid != b.isBid)
        {
            return a.isBid;
        }

        if (a.isBid)
        {
            return a.price < b.price;
        } 
         
        return a.price < b.price;
    }
};

using book_queue = std::priority_queue<LimitOrder, std::vector<LimitOrder>, CompareOrders>;

// P0: Matching engine
class MatchingEngine 
{
public:
    explicit MatchingEngine(std::string  inst) : instrument(std::move(inst)) { dummy_init(); }
    MatchingEngine() = delete;

    auto process_limit(const LimitOrder& order) -> void;

    auto process_market(MarketOrder order) -> void;

    auto match_market(MarketOrder order) -> void;

    auto print_LOB() -> void;

private:
    book_queue bids;
    book_queue asks;

    auto dummy_init() -> void;
    auto print_books(book_queue& queue) -> void;

    std::string instrument;
};
