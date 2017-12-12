#include "Compresser.h"
#include <fstream>
#include <queue>
#include <bitset>
#include <map>
#include <iostream>
#include <stack>
using namespace std;

Compresser::Compresser():root(nullptr),old_size(0),charNum(0){}
Compresser::~Compresser()
{
    if(root != nullptr)
        delete root;
}

void Compresser::clear()
{
    buf.clear();
    buf2.clear();
    old_size = 0;
    new_size = 0;
    charNum = 0;

    stack<Node*> s;

    //二叉树的非递归遍历 回收内存
    Node* cur = root;
    while(cur != nullptr || !s.empty())
    {
        while(cur != nullptr)
        {
            s.push(cur);
            cur = cur->left;
        }

    if(s.empty())
        break;
    cur = s.top();
    cur = cur ->right;
    delete s.top();
    s.pop();
    }
    root = nullptr;
}

void Compresser::compress(const string& filename)
{
    clear();

    count(filename);
    root = buildHuffTreeByCnt();
    dfs(root, "");
    CreatCompressedFile(filename + ".comp");
    
    cout<<"****************"<<endl;
    cout<<"源文件大小："<<old_size<<"bits"<<endl;
    cout<<"压缩文件大小："<<new_size<<"bits"<<endl;
    cout<<"压缩率:"<<double(new_size)/old_size*100<<"%"<<endl;
    cout<<"****************"<<endl;
}

void Compresser::count(const string& filename)
{  
    for(int i=0;i<256;++i)
        cnt[i] = 0;

    ifstream in;
    in.open(filename, ios::binary);
    uchar temp;
    old_size = 0;
    while( in.read((char*)&temp,1) )
    {
        ++old_size;
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

void Compresser::CreatCompressedFile(const string& filename)
{
    bitset<8> byt(0);
    byt.reset();
    ofstream out;
    out.open(filename, ios::binary);
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
                ++new_size;
                byt.reset();                
            }
        }
        
    }
    if(n!=0)
        out.write((char*)&byt, 1);
    out.close();

    out.open(filename + ".huff", ios::out);
    out<<old_size<<"bits"<<endl;
    for(int i=0;i<256;++i)
    {
        if(cnt[i]!=0)
            out<<i<<" "<<codes[i]<<endl;
    }
    out.close();
}

void  Compresser::depress(const string& filename)
{
    clear();
    
    if(root != nullptr)
        delete root;
    root = buildHuffTreeByFile(filename + ".huff");

    ifstream in;
    in.open(filename, ios::binary);

    uchar temp;
    while( in.read((char*)&temp, 1) )
    {
        ++new_size;        
        buf.push_back(temp);
    }

    // bitset<8> b;

    for(auto& byt:buf)
    {
        bitset<8> b(byt);
        run(b);
    }

    CreatDepressedFile(filename + ".dpred");

    cout<<"****************"<<endl;
    cout<<"源文件大小："<<old_size<<"bits"<<endl;
    cout<<"压缩文件大小："<<new_size<<"bits"<<endl;
    cout<<"压缩率:"<<double(new_size)/old_size*100<<"%"<<endl;
    cout<<"****************"<<endl;
}

Node* Compresser::buildHuffTreeByFile(const string& filename)
{
    ifstream in;
    in.open(filename);
    int ch;
    string code;
    Node* root = new Node(uchar(0), 0);

    auto build = [](Node* root, uchar ch, const string& code)
    {
        for(auto& dir: code)
        {
            if(dir == '0' )
            {
                if(root->left == nullptr)
                    root->left = new Node(uchar(0), 0);
                root = root->left;
            }
            else
            {
                if(root->right == nullptr)
                    root->right = new Node(uchar(0), 0);
                root = root->right;
            }
        }
        root->ch = ch;
    };

    in>>old_size;
    while(in>>ch)
    {
        in>>code;
        build(root, uchar(ch), code);
    }
    in.close();

    return root;
}

void Compresser::run(bitset<8>& b)
{
    // for(int i=0;i<8;++i)
    //     cout<<b[i];
    // cout<<endl;

    static Node* cur = root;
    for(int i=0;i<8;++i)
    {
        if(b[i] == 0)
            cur = cur->left;
        else
            cur = cur->right;
        if(cur->left == nullptr && cur->right == nullptr)
        {
            buf2.push_back(cur->ch);
            // ++new_size;
            // cout<<cur->ch;
            cur = root;            
        }
    }
}

void Compresser::CreatDepressedFile(const string& filename)
{
    ofstream out;
    out.open(filename, ios::binary);
    out.write((char*)&buf2[0], old_size);
    // for(int i=0;i<old_size;++i)
    // {
        
    // }
}

