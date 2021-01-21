
/*
为了做这2道题

序列化二叉树：
https://www.lintcode.com/problem/serialize-and-deserialize-binary-tree/

二叉树的线性化
https://www.lintcode.com/problem/search-range-in-binary-search-tree/description

在做线性化的时候，参考了下面页面代码：
https://www.cnblogs.com/Blog-day/p/My_Blog_Days_17.html
*/

#include <stdio.h>
#include <string>
#include <sstream>
#include <exception>
#include <queue>
#include <iostream>

using std::string;
using std::stringstream;
using std::exception;
using std::queue;


class TreeNode {
public:
    enum pointertag{ LINK, THREAD };

    int val;
    TreeNode *left, *right;
    pointertag lefttag, righttag;

    TreeNode(int val)
    {
        this->val = val;
        left = right = NULL;
        lefttag = righttag = LINK;
    }
};

void   InOrderThding(TreeNode* cur, TreeNode* prev)
{
    if (cur == NULL)
        return;

    InOrderThding(cur->left, prev);

    if (prev && prev->right == NULL)
    {
        prev->right = cur;
        prev->righttag = TreeNode::THREAD;
    }

     if (cur->left == NULL)
     {
         cur->left = prev;
         cur->lefttag = TreeNode::THREAD;
     }

     InOrderThding(cur->right, cur);
}

class iterator
{
    TreeNode *pos;
public:
    iterator(TreeNode* node)
    {
        pos = node;
    }
    void operator++()  // 运算符 ++a;
    {
        if (!pos->right)
            throw exception("Iterator end reached");

        if (pos->righttag == TreeNode::THREAD)
            pos = pos->right;
        else if (pos->righttag == TreeNode::LINK)
        {
            TreeNode* rightChild = pos->right;

            TreeNode *rightThenLeftMost = rightChild; // 右子树中的最左节点
            while (rightThenLeftMost->left && rightThenLeftMost->lefttag==TreeNode::LINK)
            {
                rightThenLeftMost = rightThenLeftMost->left;
            }
            pos = rightThenLeftMost;

        }
    }

    void operator++(int)   // 运算符 a++;
    {
        operator++();
    }

    void operator--()  // 运算符 --a;
    {
        if (!pos->left)
            throw exception("Iterator very start reached");

        if (pos->lefttag == TreeNode::THREAD)
            pos = pos->left;
        else if (pos->lefttag == TreeNode::LINK)
        {
            TreeNode* leftChild = pos->left;
            TreeNode *leftThenRightMost = leftChild; // 右子树中的最左节点
            while (leftThenRightMost->right && leftThenRightMost->righttag==TreeNode::LINK)
            {
                leftThenRightMost = leftThenRightMost->right;
            }
            pos = leftThenRightMost;
        }
    }

    void operator--(int)
    {
        operator--();
    }
};

 TreeNode* deserialize(string &data)
 {
     TreeNode *root = NULL;

     queue<TreeNode*> bfvQueue;  // bfvStack == broad-first-visit Stack,   广度优先遍历 的工作队列
     
     stringstream ss(data);
     int value;
     char ch;
     bool createNullNode = false;
     bool leftChildDone = false; // 右孩子节点已经设置完毕
     TreeNode *parent = NULL;
     while (ss.get(ch))   // 这个接口  用到了将stringstream.bool()强制转换，当flags包含EOF位的时候，会返回false
     {
         switch (ch)
         {
         case ',':
         case ' ':
         case '}':
             continue;

         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
             ss.putback(ch);
         case '{':
             createNullNode = false;
             ss >> value;
             break;
         case '#':
             createNullNode = true;
             break;
         }

         if (!root)
         {
             root = new TreeNode(value);
             bfvQueue.push(root);
             continue;
         }

         if (!leftChildDone)
         {
             parent = bfvQueue.front();
             bfvQueue.pop();
         }

         TreeNode *child = (createNullNode ? NULL : new TreeNode(value));
         if (!leftChildDone)
         {
             parent->left = child;
             leftChildDone = true;
         }
         else
         {
             parent->right = child;
             leftChildDone = false;
         }

         if (child)
             bfvQueue.push(child);
     }

     return root;
 }

 string serialize(TreeNode * root)
 {
     stringstream ss;
     queue<TreeNode*> bfvQueue;  // bfvStack == broad-first-visit Stack,   广度优先遍历 的工作队列

     bfvQueue.push(root);

     while (!bfvQueue.empty())
     {
         TreeNode *node = bfvQueue.front();
         bfvQueue.pop();
         if (node)
         {
             bfvQueue.push(node->left);
             bfvQueue.push(node->right);
         }
         if (node)
             ss << node->val << ", ";
         else
             ss << '#' << ", ";
     }
     return ss.str();
 }

 int main()
 {
     TreeNode *root = deserialize(string("{20,8,22,4,12}"));
     string str = serialize(root);
     std::cout << "serialize:  " << str << std::endl;

     root = deserialize(string("{3,9,20,#,#,15,7}"));
     str = serialize(root);
     std::cout << "serialize:  " << str << std::endl;
     getchar();
 }

