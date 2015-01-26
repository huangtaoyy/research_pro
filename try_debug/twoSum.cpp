/*
 * twoSum.cpp
 *
 *  Created on: 2014-6-5
 *      Author: huangtao
 */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
	int idx;
	int val;
	Node() {}
	Node(int idx, int val) {
		this->idx = idx;
		this->val = val;
	}
};

bool compare(const Node& a, const Node& b){
	return a.val < b.val;
}

class Solution {
public:
    vector<int> twoSum1(vector<int> &numbers, int target) {
    	vector<int> ret(2, 0);
    	int length = numbers.size();
    	for (int i = 0; i < length; i ++) {
    		int an = target - numbers[i];
    		for(int j = i + 1; j < length; j ++) {
    			if (an == numbers[j]) {
    				ret[0] = (i + 1);
    				ret[1] = (j + 1);
    				return ret;
    			}
    		}
    	}
    	return ret;
    }

    vector<int> twoSum2(vector<int> &numbers, int target) {
    	vector<int> ret(2, 0);
    	int length = numbers.size();
    	vector<Node> nums(length);

    	for(int i = 0; i < length ; i ++) {
    		nums[i] = Node(i + 1, numbers[i]);
    	}

    	sort(nums.begin(), nums.end(), compare);

    	int b = 0, e = length - 1;
    	while (b < e) {
    		int sum = nums[b].val + nums[e].val;
    		if(sum == target) {
    			ret[0] = min(nums[b].idx, nums[e].idx);
    			ret[1] = max(nums[b].idx, nums[e].idx);
    			return ret;
    		} else if (sum < target) {
    			b ++;
    		} else {
    			e --;
    		}
    	}
    	return ret;
    }
};

void print_vector(vector<int> &v) {
	int length = v.size();
	for(int i = 0; i < length; i ++) {
		cout << v[i] << ", " ;
	}
	cout << endl;
}

int main() {
	int arr[] = {7, 11, 15, 8, 2};
	vector<int> numbers(arr, arr + 5);

	Solution so;
	vector<int> v1 = so.twoSum1(numbers, 9);
	vector<int> v2 = so.twoSum2(numbers, 9);
	print_vector(v1);
	print_vector(v2);
	return 0;
}

