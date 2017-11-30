#include "Compresser.h"
#include <fstream>
#include <queue>
#include <bitset>
using namespace std;

Compresser::~Compresser()
{
    // delete cnt;
}

void Compresser::compress(const string& filename)
{
    count(filename);
    root = buildHuffTreeByCnt();
    dfs(root, "");
    CreatedCompressedFile(filename);
}

void Compresser::count(const string& filename)
{  
    for(int i=0;i<256;++i)
        cnt[i] = 0;

    ifstream in;
    in.open(filename, ios::binary);
    uchar temp;
    while( in.read((char*)&temp,1) )
    {
        buf.push_back(temp);
        ++cnt[int(temp)];
    }
    in.close();
}

Node* Compresser::buildHuffTreeByCnt()
{
    Node* cur;
    Node* f;
    Node* s;

    priority_queue<Node*, vector<Node*>, cmp> q;

    for(int i=0;i<256;++i)
    {
        if(cnt[i]!=0)
        {
            cur = new Node(uchar(i), cnt[i]);
            q.push(cur);
        }
    }

    while(q.size() > 1)
    {
        f = q.top();
        q.pop();
        s = q.top();
        q.pop();
        cur = new Node(uchar(0), f->num+ s->num);
        cur->left = f;
        cur->right = s;
        q.push(cur);
    }

    if(!q.empty())
        return q.top();
    else
        return nullptr;
}


void Compresser::dfs(Node* root, const string& code)
{
    if(root == nullptr)
        return;
    
    if(root->left == nullptr && root->right== nullptr)
    {
        codes[int(root->ch)] = code;
        return;        
    }
    dfs(root->left, code+"0");
    dfs(root->right, code+"1");
}

void Compresser::CreatedCompressedFile(const string& filename)
{
    bitset<8> byt(0);
    ofstream out;
    out.open(filename + ".comp", ios::binary);
    int n=0;
    for(auto& b:buf)
    {
        for(auto& bit: codes[int(b)])
        {
            byt[n] = (bit=='0'? 0:1);
            ++n;
            if(n==8)
            {
                n=0;
                out.write((char*)&byt, 1);
            }
        }
    }
    out.close();

    out.open(filename + ".comp.huff", ios::out);
    for(int i=0;i<256;++i)
    {
        if(cnt[i]!=0)
            out<<i<<" "<<codes[i]<<endl;
    }
    out.close();
}

void  Compresser::depress(const string& filename)
{
    ifstream in;
    // in.open(filename)
}
