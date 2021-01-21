/*
刷这个题目
https://www.lintcode.com/problem/maximum-subarray-iii/description
*/


#include <vector>
#include <algorithm>
#include <iostream>
#include <assert.h>
using std::vector;
using std::max;
using std::min;

class Solution {
    
private:

int maxSubArrayRecursive(vector<int> &nums, vector<vector<vector<int>>> &wk,vector<vector<vector<bool>>> &done, int from, int to, int k)
{
    if (done[from][to-from+1-1][k-1])
       return  wk[from][to-from+1-1][k-1];
    
    int result;

#if 0
    // 先对问题进行化简: 如果数组首、或者尾，有一系列的负数，那么可以将数列这些负数酌情舍弃，但是舍弃的前提条件是，舍弃后，数组长度依然是大于等于k的
    int lightFrom = from;  // lightFrom： 轻量级的from
    int lightTo = to; // lightTo:  轻量级的to
    while (lightFrom<=lightTo && nums[lightFrom] < 0 && (lightTo - lightFrom + 1 - 1) >= k)
        lightFrom++;
    while (lightTo >= lightFrom &&  nums[lightTo] < 0 && (lightTo - lightFrom + 1 - 1) >= k)
        lightTo--;
    if (lightFrom <= lightTo && (lightTo - lightFrom + 1) >= k && (lightTo - lightFrom) < (to - from))
    {
        if (done[lightFrom][lightTo - lightFrom + 1 - 1][k - 1])
        {
            done[from][to - from + 1 - 1][k - 1] = true;
            wk[from][to - from + 1 - 1][k - 1] = wk[lightFrom][lightTo - lightFrom + 1 - 1][k - 1];
            return  wk[lightFrom][lightTo - lightFrom + 1 - 1][k - 1];
        }
        else
        {
            result = maxSubArrayRecursive(nums, wk, done, lightFrom, lightTo, k);
            done[lightFrom][lightTo - lightFrom + 1 - 1][k - 1] = true;
            wk[lightFrom][lightTo - lightFrom + 1 - 1][k - 1] = result;
            done[from][to - from + 1 - 1][k - 1] = true;
            wk[from][to - from + 1 - 1][k - 1] = result;
            return result;
        }
    }
#endif

    if (k == 1)
    {
        result = 0;
        for (int i = from; i <=to; i++)
           result+=nums[i];
        
        for (int i = from; i <to; i++)
        {
            int leftMax = maxSubArrayRecursive(nums, wk, done, from, i, 1);
            int rightMax = maxSubArrayRecursive(nums, wk, done, i+1, to, 1);
            result = max(result, max(leftMax, rightMax));
        }
    } 
    else
    {
        result = INT_MIN;
        for (int i = from; i < to; i++)
        {
            int left_max_partition = i - from +1;
            int right_max_partition = to - i;
            
            for (int j = min(0, k); j <= min(left_max_partition, k); j++)
            {
                if (j > min(left_max_partition, k) || (k - j) > min(right_max_partition, k))
                    continue;

                if (j == 0)
                { 
                    int rightMax = maxSubArrayRecursive(nums, wk, done, i + 1, to, k);
                    result = max(result, rightMax);
                }
                else
                { // j > 0
                    if (k - j > 0)
                    {
                        int leftMax = maxSubArrayRecursive(nums, wk, done, from, i, j);
                        int rightMax = maxSubArrayRecursive(nums, wk, done, i + 1, to, k - j);
                        result = max(result, leftMax + rightMax);
                    }
                    else // k-j == 0
                    {
                        assert(k - j == 0);
                        int leftMax = maxSubArrayRecursive(nums, wk, done, from, i, k);
                        result = max(result, leftMax);
                    }
                }



            }
        }       
    }
    done[from][to-from+1-1][k-1] = true;
    wk[from][to-from+1-1][k-1] = result;
    return result;
}

public:
    /**
     * @param nums: A list of integers
     * @param k: An integer denote to find k non-overlapping subarrays
     * @return: An integer denote the sum of max k non-overlapping subarrays
     */
    int maxSubArray(vector<int> &nums, int k) {
        // write your code here
        
        vector<vector<vector<bool>>>  done(nums.size());
        vector<vector<vector<int>>>  wk(nums.size());
        for (int i = 0; i < nums.size(); i++) // 每个数组起始元素下标
        {
          wk[i].resize(nums.size()-i); // 相当于初始化数组大小
          done[i].resize(nums.size()-i); // 相当于初始化数组大小
          
          for (int j = 1; j <= nums.size() - i; j++) // 每个 起始下标开始的各个串的长度范围
          {
            int size = j; 
            wk[i][j-1].resize(min(k, size)); // 初始化数组大小
            done[i][j-1].resize(min(k, size)); // 初始化数组大小
           
            for (int partition = 1; partition <= min(k, size); partition++) // 各个子串 可以分成段的数量范围
            {
               wk[i][j-1][partition-1] = 0;
               done[i][j-1][partition-1] = false;
            }
          }
        }
        
        return maxSubArrayRecursive(nums, wk, done, 0, nums.size()-1, k);

    }
    
    
};

void main()
{
    Solution *s = new Solution();
    vector<int> arr = { 1,2,3 };
    vector<int> arr2 = { -45,74,-14,65,-12,73,-54,8,-8,2,-4,91,-48,8,-94,98,-13,66,-36,24,-36,21,-82,86,-79,85,-90,28,-93,78,-12,65,-79,4,-50,24,-96,89,-9,80,-5,49,-54,3,-68,74,-36,43,-63,51,-97,87,-45,27,-2,96,-97,36,-7,43,-48,90,-67,80,-43,33,-68,76,-4,61,-73,34,-59,6,-96,72,-40,58,-92,44,-69,84,-94,64,-22,46,-10,88,-34,11,-15,38,-62,59,-100,50,-38,20,-45,25,-9,42,-24,94,-19,69,-86,100,-30,78,-27,82,-79,40,-77,90,-7,1,-59,67,-56,80,-99,71,-4,83,-60,9,-3,9,-1,4,-66,22,-23,70,-36,44,-58,17 };
    std::cout << s->maxSubArray(arr, 1)<<std::endl;
    std::cout << s->maxSubArray(arr2, 102) << std::endl;

    getchar();
}