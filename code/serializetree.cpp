#include <string>
#include <deque>
#include <sstream>

using namespace std;
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Codec {
public:
    
    void comma(string& str) {
        if (str.back() != '[')
            str += ",";
    }

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        deque<TreeNode*> nodes;
        nodes.push_front(root);
        string result = "[";
        while (!nodes.empty()) {
            auto node = nodes.back();
            nodes.pop_back();
            comma(result);
            if (node) {
                result += to_string(node->val);
                nodes.push_front(node->left);
                nodes.push_front(node->right);
            }
            else
                result += "null";
        }
        result += "]";
        // cout << result << endl;
        return result;
    }
    
    int to_int(string s) {
        return stoi(s.c_str());
    }
    
    bool isValid(string s) {
        if (s.empty())
            return false;
        if (s.compare("null") == 0)
            return false;
        return true;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        data = data.substr(1,data.length()-2);
        // cout << data << endl;
        stringstream s(data);
        string tmp;
        deque<TreeNode*> nodes;
        TreeNode* root = nullptr;
        if (getline(s,tmp,',')) {
            if (isValid(tmp)) {
                root = new TreeNode(to_int(tmp));
                nodes.push_front(root);
            }
        }
        while (!nodes.empty()) {
            TreeNode* node = nodes.back();
            nodes.pop_back();
            if (getline(s,tmp,',')) {
                if (isValid(tmp)) {
                    nodes.push_front(new TreeNode(to_int(tmp)));
                    node->left = nodes.front();
                }
            }
            if (getline(s,tmp,',')) {
                if (isValid(tmp)) {
                    nodes.push_front(new TreeNode(to_int(tmp)));
                    node->right = nodes.front();
                }
            }
        }
        return root;
    }
};

int main() {

}