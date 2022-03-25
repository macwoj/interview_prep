#include <unordered_map>
#include <map>
#include <list>
#include <iostream>
#include <memory>

using namespace std;

struct Order {
    int orderID_;
    int size_;
    double price_;
    bool sellSide;
};

using OrderListT = list<Order>;

struct Level {
    OrderListT orders_;
    int size_;
};

using LevelPtr = shared_ptr<Level>;
using LimitMap = map<double,LevelPtr>;
using OrderBook = unordered_map<int,list<Order>::iterator>;
using LimitPriceBook = unordered_map<double,LimitMap::iterator>;

struct LimitBook {
    LimitMap buyBook_;
    LimitMap sellBook_;
    LimitMap::iterator bestBuy_;
    LimitMap::reverse_iterator bestSell_;

    LimitBook(): bestBuy_(buyBook_.end()),bestSell_(sellBook_.rend()) {}
};

class LimitOrderBook {
private:
    LimitBook limitBook_;
    OrderBook orders_;
    LimitPriceBook buyPrice_; //price to map iterator
    LimitPriceBook sellPrice_;

    void newOrder(
            const Order& order,
            OrderBook& orders,
            LimitPriceBook& price,
            LimitMap& limitBook
        ) {
        auto it_price = price.find(order.price_);
        if (it_price == price.end()) {
            auto level =  make_shared<Level>();
            level->size_ = order.size_;
            auto [it_limit,_] = limitBook.insert({order.price_,level});
            it_limit->second->orders_.push_front(order);
            price.insert({order.price_,it_limit});
            orders.insert({order.orderID_,it_limit->second->orders_.begin()});
        } else {
            auto it_limit = it_price->second;
            auto& level = it_limit->second;
            level->size_ += order.size_;
            level->orders_.push_front(order);
            orders.insert({order.orderID_,level->orders_.begin()});
        }
    }
public:


    void newOrder(const Order& order) {
        if (order.sellSide) {
            newOrder(order,orders_,sellPrice_,limitBook_.sellBook_);
            if (limitBook_.bestSell_ == limitBook_.sellBook_.rend() ||
                order.price_ < limitBook_.bestSell_->first) {
                limitBook_.bestSell_ = limitBook_.sellBook_.rbegin();
            }
        } else {
            newOrder(order,orders_,buyPrice_,limitBook_.buyBook_);
            if (limitBook_.bestBuy_ == limitBook_.buyBook_.end() ||
                order.price_ > limitBook_.bestBuy_->first) {
                limitBook_.bestBuy_ = limitBook_.buyBook_.begin();
            }
        }
    }

    void cancelOrder(int orderID) {
        auto it = orders_.find(orderID);
        if (it!=orders_.end()) {
            auto [_,order] = *it;
            auto& book = order->sellSide ? limitBook_.sellBook_ : limitBook_.buyBook_;
            auto& level = order->sellSide ? sellPrice_[order->price_] : buyPrice_[order->price_];
            auto price = level->first;
            level->second->size_ -= order->size_;
            level->second->orders_.erase(order);
            orders_.erase(orderID);
            if (level->second->orders_.empty()) {
                book.erase(level);
                if (order->sellSide) {
                    if (price == limitBook_.bestSell_->first) {
                        limitBook_.bestSell_ = limitBook_.sellBook_.rbegin();
                    }
                } else {
                    if (price == limitBook_.bestBuy_->first) {
                        limitBook_.bestBuy_ = limitBook_.buyBook_.begin();
                    }
                }
            }
        }
    }

    void print() { 
        auto printimpl = [](auto begin,auto end) {
            for (auto it = begin;it!=end;++it) {
                auto [price,level] = *it;
                cout << price << ":" << level->size_ << " ";
                for (auto order:level->orders_) {
                    cout << "[" << order.orderID_ << "," << order.size_ << "] ";
                }
                cout << endl;
            }
        };
        cout << " Sell:" << limitBook_.bestSell_->first << endl;
        printimpl(limitBook_.sellBook_.rbegin(),limitBook_.sellBook_.rend());
        cout << " Buy:" << limitBook_.bestBuy_->first << endl;
        printimpl(limitBook_.buyBook_.begin(),limitBook_.buyBook_.end());
    }

};

int main() {

    LimitOrderBook book;

    book.newOrder({0,10,100.0,true});
    book.newOrder({1,15,100.0,true});
    book.newOrder({2,9,101.0,true});
    book.newOrder({3,15,100.0,true});

    book.newOrder({4,9,99.0,false});
    book.newOrder({5,8,99.1,false});
    book.newOrder({6,10,99.2,false});

    book.print();

    book.cancelOrder(4);
    book.print();
}