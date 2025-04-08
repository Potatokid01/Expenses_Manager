#include <bits/stdc++.h>

using namespace std;

// class Solution {
//     public:
//         int minimumOperations(vector<int>& nums) {
//             if (nums.size() == 0) return 0;
//             set <int> num(100);
//             for (int i = nums.end() - 1; i >= 0; i--){
//                 if (num.find(nums[i]) != num.end()) return i/3 + 1;
//                 else num.insert(nums[i]);
//             }
//         }
//     };
int main(){
    map <string, double> typeAmounts = {
        {"An uong", 0}
    };
    cout << typeAmounts.begin()->first << " " << typeAmounts.begin()->second << endl;
}