#pragma once
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <bitset>
using namespace std;

struct Node
{
    unsigned char ch;
    int num;
    Node* left;
    Node* right;
    Node(unsigned char ch, int num):ch(ch),num(num),left(nullptr),right(nullptr){}
};

struct cmp
{
    bool operator()(const Node* a, const Node* b) const
    {
        return a->num>b->num;
    }
};

class Compresser
{
    public:
        Compresser();
        ~Compresser();
        void compress(const string& filename);
        void depress(const string& filename);
        void count(const string& filename);
        void CreatCompressedFile(const string& filename);
        void CreatDepressedFile(const string& filename);
        Node* buildHuffTreeByCnt();
        Node* buildHuffTreeByFile(const string& filename);
        void run(bitset<8>& b);
        void dfs(Node* root, const string& code);
        void clear();
        string codes[300];
        int cnt[300];
        
    private:
        typedef unsigned char uchar;
        vector<uchar> buf;
        vector<uchar> buf2;
        Node* root;
        unsigned long long old_size;
        unsigned long long new_size;        
        char charNum;
};