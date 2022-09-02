#include <unordered_map>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <set>
#include <iostream>

using namespace std;

class Column {
    map<int,list<int>> vals_;
public:
    void insert(int id,int val) {
        auto [it,isNew] = vals_.insert({val,{id}});
        if (!isNew)
            it->second.push_back(id);
    }

    set<int> equal(int val) {
        auto it = vals_.find(val);
        if (it!=vals_.end()) {
            return set<int>(it->second.begin(),it->second.end());
        }
        return {};
    }

    set<int> greatereq(int val) {
        auto it = vals_.lower_bound(val);
        set<int> result;
        while (it!=vals_.end()) {
            if (it->first>=val)
                for (auto d:it->second)
                    result.insert(d);
            ++it;
        }
        return result;
    }
    set<int> lesseq(int val) {
        auto it = vals_.upper_bound(val);
        if (it==vals_.begin()) {
            return {};
        }
        set<int> result;
        do {
            --it;
            if (it->first<=val)
                for (auto d:it->second)
                    result.insert(d);
        } while (it!=vals_.begin());
        return result;
    }
};

class Table {
    unordered_map<string,Column> columns_;
    vector<string> column_order_;
    int next_val_;
    string name_;
    vector<tuple<int,vector<int>>> data_;
public:
    using Command = tuple<string,string,int>;
    Table(string name,const vector<string>& columns):column_order_(columns), next_val_(0), name_(name) {
        for (auto c:columns) {
            columns_[c] = {};
        }
    }
    int insert(vector<int> data) {
        data_.push_back({next_val_,data});
        for (int i=0;i<column_order_.size();i++) {
            auto it = columns_.find(column_order_[i]);
            if (it!=columns_.end()) {
                it->second.insert(next_val_,data[i]);
            }
        }
        next_val_++;
        return next_val_ - 1;
    }

    set<int> intersect(const set<int>& a,const set<int>& b) {
        set<int> intersect;
        set_intersection(a.begin(),a.end(),b.begin(),b.end(),std::inserter(intersect, intersect.begin()));
        return intersect;
    }

    vector<int> select(vector<Command> commands) {
        for (auto [column_name,op,val]:commands) {
            cout << "[" << column_name << " " << op << " " << val << "] ";
        }
        cout << endl;
        set<int> result;
        for(int i=0;i<commands.size();i++) {
            auto [column_name,op,val] = commands[i];
            auto col = columns_.find(column_name);
            set<int> res;
            if (op=="=") {
                res = col->second.equal(val);
            } else if (op=="<=") {
                res = col->second.lesseq(val);
            } else if (op==">=") {
                res = col->second.greatereq(val);
            }
            if (i==0)
                result = res;
            else
                result = intersect(result,res);
        }
        return vector<int>(result.begin(),result.end());
    }

    void print() {
        cout << name_ << " ";
        for (auto c:column_order_) {
            cout << c << " ";
        }
        cout << endl;
        for (auto [id,d]:data_) {
            cout << id << " - ";
            for (auto i:d)
                cout << i << " ";
            cout << endl;
        }
    }
};

class Database {
    unordered_map<string,Table> tables_;
public:
    using Command = Table::Command;
    bool create_table(string name,vector<string> columns) {
        tables_.insert({name,Table(name,columns)});
        return false;
    }
    int insert(string table,vector<int> data) {
        auto it = tables_.find(table);
        if (it!=tables_.end()) {
            return it->second.insert(data);
        }
        return -1;
    }
    vector<int> select(string table,vector<Command> commands) {
        auto it = tables_.find(table);
        if (it!=tables_.end()) {
            return it->second.select(commands);
        }
        return {};
    }

    void print() {
        for (auto t:tables_)
            t.second.print();
    }
};

int main() {
    Database db;
    db.create_table("test0",{"a","b","c","d","e"});
    for (int i=0;i<20;i++)
        db.insert("test0",{i+1,i+2,i+3,i+4,i+5});
    db.print();
    auto printres = [](auto res) {
        for (auto r:res)
            cout << r << " ";
        cout << endl;
    };
    // printres(db.select("test0",{{"a","=",5}}));
    printres(db.select("test0",{{"a","<=",5}}));
    printres(db.select("test0",{{"a","<=",5},{"a","=",5}}));
    printres(db.select("test0",{{"a",">=",5},{"b","<=",16}}));
}