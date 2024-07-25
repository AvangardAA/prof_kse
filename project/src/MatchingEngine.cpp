#include "MatchingEngine.h"

auto MatchingEngine::process_limit(const LimitOrder& order) -> void 
{
    if (order.isBid) { bids.push(order); } 
    else { asks.push(order); }
}

auto MatchingEngine::process_market(MarketOrder order) -> void
{
    match_market(order);
}

auto MatchingEngine::match_market(MarketOrder order) -> void
{
    // std::println("DEBUG: {}, {}, {}, {}", order.owner, order.id, order.quantity, order.isBid);

    book_queue& match_book = order.isBid ? asks : bids;

    while (!match_book.empty() && order.quantity > 0) 
    {
        LimitOrder best_ord = match_book.top();

        int match_qty = std::min(order.quantity, best_ord.quantity);
        std::println("TRADE LOG: Matched market order with {}{} for quantity {} at price {}", \
            (order.isBid ? "Ask ID " : "Bid ID "), best_ord.id, match_qty, best_ord.price);

        best_ord.quantity -= match_qty;
        order.quantity -= match_qty;

        if (best_ord.quantity <= 0) { match_book.pop(); } 
        else
        {
            match_book.pop();
            match_book.push(best_ord);
        }
    }

    if (order.quantity > 0)
    {
        std::println("TRADE LOG: Unmatched quantity for market order: {}", order.quantity);
    }

    order.isBid ? asks : bids = match_book;
}

auto MatchingEngine::print_LOB() -> std::string
{
    std::stringstream ss;
    ss << "------- Limit Order Book for " << instrument << " -------\n";
    ss << "Asks:\n";
    ss << print_books(asks);
    ss << "Bids:\n";
    ss << print_books(bids);
    ss << "---------------------------------------\n";
    return ss.str();
}

auto MatchingEngine::print_books(book_queue& queue) -> std::string
{
    std::stringstream ss;
    book_queue temp_book = queue;

    while (!temp_book.empty())
    {
        LimitOrder order = temp_book.top();
        ss << "Price: " << std::setw(8) << order.price
            << ", Quantity: " << std::setw(8) << order.quantity
            << ", Owner: " << std::setw(10) << order.owner
            << ", ID: " << std::setw(4) << order.id
            << ", Side: " << (order.isBid ? "Bid" : "Ask") << "\n";
        temp_book.pop();
    }

    return ss.str();
}

auto MatchingEngine::dummy_init() -> void
{
    int id = 1;
    double price = 50;
    double qty = 15.00;
    for (;;) 
    {
        if (id == 11) { break; }

        process_limit(LimitOrder("system", id, price, qty, (id % 2 == 0 ? true : false)));
        id % 2 == 0 ? price += id : price -= id;
        id % 2 == 0 ? qty += id : qty -= id;
        id++;
    }

    // print_LOB();
}