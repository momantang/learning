// Leetcode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>

using namespace std;
class Solution1
{
public:
	vector<int> towSum(vector<int> &nums, int target) {
		for (int i=0;i<nums.size();i++)
		{
			for (int j=i+1;j<nums.size();j++)
			{
				if ((nums[i]+nums[j])==target)
				{
					return vector<int>{i, j};
				}
			}
		}
		
	}

};

/**
 * Definition for singly-linked list.
 
*/
struct ListNode {
      int val;
      ListNode *next;
      ListNode() : val(0), next(nullptr) {}
      ListNode(int x) : val(x), next(nullptr) {}
      ListNode(int x, ListNode *next) : val(x), next(next) {}
  };
class Solution2 {
public:
	ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
		cout << l1->val << endl;
		cout << l2->val << endl;
		int l1n=0, l2n=0,l3n=0;
		int i = 1;
		do 
		{
			l1n += l1->val*i;
			i = i * 10;

		} while ((l1=l1->next)!=NULL);
		i = 1;
		do
		{
			l2n += l2->val*i;
			i = i * 10;

		} while ((l2 = l2->next) != NULL);
		l3n = l1n + l2n;
		ListNode* root = nullptr;
		do 
		{
			int num = l3n % i;
			if (root==nullptr&&num>0)
			{
				root = new ListNode(num);
				l3n = l3n / 10;
			}
			else if (root!=nullptr)
			{
				ListNode* next = new ListNode(num);
				l3n = l3n / 10;
			}
			i = i / 10;
		} while (l3n>0);
		return root;
	}
};

int main()
{
	Solution1 s;
	Solution2 s2;

	ListNode *root1 =new  ListNode(2 );
	ListNode *root2 =new  ListNode(5);
	s2.addTwoNumbers(root1, root2);
	/*vector<int> nums{ 2,5,5,11 };
	int taget = 10;
	vector<int> result = s.towSum(nums, taget);
	for (auto i:result)
	{
		cout << i << endl;
	}
	*/
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
