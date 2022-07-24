#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

int read4(char *buf) {
    return {};
}
/**
 * The read4 API is defined in the parent class Reader4.
 *     int read4(char *buf4);
 */

//check if we have data in buffer
// if buffer to buf
//else copy data to buffer
// if buffer to buf

class Solution {
public:
    vector<char> buf_;
    int idx_,end_;
    Solution():buf_(4,0),idx_(0),end_(0) {
        
    }
    bool haveData() {
        return idx_ < end_;
    }
    
    int copyBuffer(char *buf, int n) {
        int toCopy = min(n,end_ - idx_);
        memcpy(buf,&buf_[idx_],toCopy);
        idx_ += toCopy;
        return toCopy;
    }
    /**
     * @param buf Destination buffer
     * @param n   Number of characters to read
     * @return    The number of actual characters read
     */
    int read(char *buf, int n) {
        char* start = buf;
        while (n > 0) {
            if (!haveData()) {
                int tmp = read4(&buf_[0]);
                if (tmp == 0)
                    break;
                idx_ = 0;
                end_ = tmp;
            }
            int copied = copyBuffer(buf,n);
            n -= copied;
            buf = buf + copied;
        }
        return buf-start;
    }
};

/*
File file("abcde"); // File is "abcde", initially file pointer (fp) points to 'a'
char[] buf4 = new char[4]; // Create buffer with enough space to store characters
read4(buf4); // read4 returns 4. Now buf4 = "abcd", fp points to 'e'
read4(buf4); // read4 returns 1. Now buf4 = "e", fp points to end of file
read4(buf4); // read4 returns 0. Now buf4 = "", fp points to end of file
*/
int main() {

}