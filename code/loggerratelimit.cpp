#include <unordered_map>
#include <string>

using namespace std;
class Logger {
    unordered_map<string,int> messages_;
public:
    Logger() {
        
    }
    
    bool shouldPrintMessage(int timestamp, string message) {
        auto [it,isNew] = messages_.insert({message,timestamp});
        if (!isNew) {
            if (timestamp - it->second >= 10) {
                it->second = timestamp;
                return true;
            } else {
                return false;
            }
        }
        return true;
    }
};

int main() {

}