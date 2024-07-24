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
    book_queue& match_book = order.isBid ? asks : bids;

    while (!match_book.empty() && order.quantity > 0) 
    {
        LimitOrder best_ord = match_book.top();

        int match_qty = std::min(order.quantity, best_ord.quantity);
        std::println("Matched market order with {}{} for quantity {} at price {}", \
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
        std::println("Unmatched quantity for market order: {}", order.quantity);
    }

    order.isBid ? asks : bids = match_book;
}

auto MatchingEngine::print_LOB() -> void
{
    std::println("------- Limit Order Book for {} -------", instrument);
    std::println("Asks:");
    print_books(asks);
    std::println("Bids:");
    print_books(bids);
    std::println("---------------------------------------");
}

auto MatchingEngine::print_books(book_queue& queue) -> void
{
    book_queue temp_book = queue;

    while (!temp_book.empty())
    {
        LimitOrder order = temp_book.top();
        std::println("Price: {}, Quantity: {}, Owner: {}, ID: {}, Side: {}", order.price, order.quantity, order.owner, order.id, (order.isBid ? "Bid" : "Ask"));
        temp_book.pop();
    }
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