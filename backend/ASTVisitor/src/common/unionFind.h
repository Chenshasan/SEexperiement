#pragma once
#include <vector>
class UnionFind
{
private:
    std::vector<int> parent;
    int count;
    std::vector<int> rank;

public:
    UnionFind() = delete;
    UnionFind(const UnionFind &) = delete;
    UnionFind(int count)
    {
        parent.resize(count);
        rank.resize(count);
        this->count = count;
        for (int i = 0; i < count; ++i)
        {
            parent[i] = i;
            rank[i] = 1;
        }
    }
    ~UnionFind()
    {
        parent.clear();
        rank.clear();
    }
    //路径压缩
    int find(int p)
    {
        if (p != parent[p])
            parent[p] = find(parent[p]);
        return parent[p];
    }
    bool isConnected(int p, int q)
    {
        return find(p) == find(q);
    }
    void unionElement(int p, int q)
    {
        int pRoot = find(p), qRoot = find(q);
        if (pRoot == qRoot)
            return;
        if (rank[pRoot] < rank[qRoot])
            parent[pRoot] = qRoot;
        else if (rank[qRoot] < rank[pRoot])
            parent[qRoot] = pRoot;
        else
        {
            parent[pRoot] = qRoot;
            rank[qRoot] += 1;
        }
    }
    void reset(int p)
    {
        parent[p] = p;
    }
};