#pragma once
#include <string>
#include <vector>
#include <utility>
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
        ~Compresser();
        void compress(const string& filename);
        void depress(const string& filename);
        void count(const string& filename);
        void CreatedCompressedFile(const string& filename);
        Node* buildHuffTreeByCnt();
        void dfs(Node* root, const string& code);
        string codes[300];
        int cnt[300];
        
    private:
        // typedef pair<char,int> Node;
        typedef unsigned char uchar;
        vector<uchar> buf;
        Node* root;
};