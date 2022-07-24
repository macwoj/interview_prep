#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <queue>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    int depth(TreeNode* root) {
        if (root==nullptr)
            return 0;
        int count = 1;
        count += depth(root->left);
        return count;
    }
    bool exists(TreeNode* root,int idx,int d) { //idx=1 , d=3
        int left = 0;
        int right = (int)pow(2,d-1)-1; //3
        TreeNode* node = root;
        while(d>1) { //2
            auto piv = (left+right)/2; //piv=2+3/2=1
            if (idx <= piv) {
                right = piv; //
                node = node->left;
            } else {
                left = piv+1; //2
                node = node->right;
            }
            d--;
        }
        cout << "idx=" << idx << " ";
        if (node)
            cout << "node=" << node->val;
        cout << endl;
        return node!=nullptr;
    }
    int countNodes(TreeNode* root) {
        int d = depth(root); //3
        if (d < 2) {
            return d;
        }
        int left = 0;
        int right = (int)pow(2,d-1)-1; //3
        while (left<=right) {
            int piv = (left+right)/2; //piv=0+3/2=1
            cout << "left=" << left << " right=" << right  << " piv=" << piv << endl;
            if (exists(root,piv,d)) {
                left = piv+1;
            } else {
                right = piv-1;
            }
        }
        int count = (int)pow(2,d-1) - 1;
        cout << "count=" << count << endl;
        cout << "left=" << left << endl;
        count += left;
        return count;
    }
};
class Solution2 {
public:
    //[[7,10],[2,4]]
    int minMeetingRooms(vector<vector<int>> intervals) {
        //sort by start time
        sort(intervals.begin(),intervals.end(),[](auto a,auto b) {
            return a[0] < b[0];
        });
        priority_queue<int> heap;
        int rooms = 0;
        for (auto i:intervals) {
            heap.push(-i[1]); //11,16
            while (-heap.top() <= i[0]) {
                heap.pop();
            }
            cout << i[0] << " " << i[1] << " " << heap.size() << endl;
            if (heap.size() > rooms)
                rooms = heap.size();
        }
        return rooms;
    }
};
class Solution3 {
public:
    //4
    //0 1 2 3
    int get(int i,vector<int>& nums1, vector<int>& nums2) {
        if (i < nums1.size())
            return nums1[i];
        return nums2[i-nums1.size()];
    }
    double findMedianNoOverlap(vector<int>& nums1, vector<int>& nums2) {
        int n = nums1.size() + nums2.size();
        int med = n/2;
        double result = 0;
        if (n%2==0) {
            result += get(med,nums1,nums2);
            result += get(med-1,nums1,nums2);
            result /= 2.0;
            return result;  
        } else {
            return get(med,nums1,nums2);
        }
    }
    
    //1,3,5,6,7,8
    //1,3,6
    //5,7,8
    //[-1,3]
    //[1,2]
    double findMedian(vector<int>& nums1, vector<int>& nums2) {
        int i1 = 0;
        int i2 = 0;
        int n = nums1.size() + nums2.size();
        while (i1+i2 < n/2) {//2,1<3
            cout << i1 << " " << i2 << endl;
            if (i1<(nums1.size()-2) && nums1[i1]<=nums2[i2]) {
                i1++;
            } else if (i2<(nums2.size()-2)) {
                i2++;
            }
        }
        cout << "nums1[i1]=" << nums1[i1] << " nums2[i2]=" << nums2[i2] << endl;
        if (n%2==1) {
            if ((i2>=nums2.size() && i1<nums1.size()) || nums1[i1]<nums2[i2])
                return nums1[i1];
            else
                return nums2[i2];
        } else {
            if (i2>=nums2.size()) {
                cout << ">i2 " << i1 << " " << i2 << endl;
                return (nums1[i1]+nums1[i1-1])/2.0;
            }
            else if (i1>=nums1.size()) {
                cout << ">i1 " << i1 << " " << i2 << endl;
                return (nums2[i2]+nums2[i2-1])/2.0;
            }
            else {
                if (nums1[i1]==nums2[i2]) {
                    cout << "== " << i1 << " " << i2 << endl;
                    return nums1[i1];
                }
                else if (nums1[i1]>nums2[i2] && nums1[i1-1]<nums2[i2-1]) {
                    cout << "i2 i2-1 " << i1 << " " << i2 << endl;
                    return (nums2[i2]+nums2[i2-1])/2.0;
                }
                else if (nums1[i1]<nums2[i2] && nums1[i1-1]>nums2[i2-1]) {
                    cout << "i1 i1-1 " << i1 << " " << i2 << endl;
                    return (nums1[i1]+nums1[i1-1])/2.0;
                }
                cout << " " << i1 << " " << i2 << endl;
                return (nums1[i1]+nums2[i2-1])/2.0;//6+5/2
            }
        }
    }
    //1,5,6,8
    //4,9
    double median(vector<int>& nums) {
        int n = nums.size()/2;
        if (nums.empty())
            return 0.0;
        if (nums.size()==1)
            return nums.front();
        if (nums.size()%2==0) {
            return (nums[n]+nums[n-1])/2.0;
        } else {
            return nums[n];
        }
    }
    //1,3,5,6,7,8,9
    //1,3,6,9
    //5,7,8
    double findMedianSortedArrays(vector<int> nums1, vector<int> nums2) {
        if (nums1.empty()) {
            return median(nums2);
        } else if (nums2.empty()) {
            return median(nums1);
        }
        else if (nums1.back() <= nums2.front()) {
            return findMedianNoOverlap(nums1,nums2);
        } else if (nums2.back() <= nums1.front()) {
            return findMedianNoOverlap(nums2,nums1);
        } else if (nums1.front() <= nums2.front()) {
            return findMedian(nums1,nums2);
        } else {
            return findMedian(nums2,nums1);
        }
    }
    //1,3,5,6,7,8
    //1,3,6
    //5,7,8
};

int main() {
    // TreeNode* root = new TreeNode(1,
    //     new TreeNode(2,new TreeNode(4),new TreeNode(5)),
    //     new TreeNode(3,new TreeNode(6),nullptr)
    // );
    // cout << Solution().countNodes(root) << endl;
    // cout << Solution().backspaceCompare("xywrrmp","xywrrmu#p") << endl;
    // cout << Solution().backspaceCompare("nzp#o#g","b#nzp#o#g") << endl;
    // cout << Solution2().minMeetingRooms({{2,11},{6,16},{11,16}}) << endl;
    // cout << Solution3().findMedianSortedArrays({1,3,6},{5,7,8}) << endl;
    //cout << Solution3().findMedianSortedArrays({0,0},{0,0}) << endl;
    cout << Solution3().findMedianSortedArrays({0,0,0,0,0},{-1,0,0,0,0,0,1}) << endl;
}