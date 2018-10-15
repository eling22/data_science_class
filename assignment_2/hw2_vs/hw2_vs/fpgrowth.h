#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct Item
{
	int num;
	int times;
};

class NodePtr;
class Node {
public:
	int num_;
	int level_;
	int times_;
	vector<NodePtr> childs_;
	Node* parent_;
	Node() {}
	Node(int num, int level, Node* parent) : num_(num), level_(level), times_(1), parent_(parent) {}
};
class NodePtr
{
public:
	Node* tptr_;
	NodePtr() {}
	NodePtr(Node* tptr) : tptr_(tptr) {}
};

class Tree
{
public:
	Tree();
	Node* GetRoot() { return root; }
	void Show();
protected:
	Node* root;
	int getIndexByValue(vector<NodePtr>& arr, int value);
	void ShowLoop(Node* ptr);
	
private:

};

class VerticalTree : public Tree
{
public:
	void Add(vector<int> data);
private:
};

class HorizontalList 
{
public:
	void Set(map<int, int> num_index_map, Node* vtree_root);
	void Show_htable_map();
	void Show_htable(int num);
	vector<NodePtr> GetList(int num);
	int GetIndex(int num) { return htable_map_[num]; }
private:
	vector<vector<NodePtr>> htable_;
	map<int, int> htable_map_;

	void SetLoop(Node * ptr);
	void ShowParent(Node* ptr);
};

class NumVerticalTree : public Tree
{
public:
	NumVerticalTree(vector<NodePtr> list, double support_min);
	void Set(vector<NodePtr> list);
	void ShowTreeList();

private:
	double support_min;
	vector<Item> temp;
	vector<vector<Item>> pre_set;

	Node* SetLoop(Node* ptr, Node* ft_ptr, int times);
	void Set_pre_set(Node* ptr);
	void ShowVector(vector<Item> arr);
	void ClearUnderSup();
	void ClearUnderSup_loop(Node* ptr);
};

class FPGrowth
{
public:
	FPGrowth(vector<vector<int>> data, int line_num, double support_min);
	~FPGrowth();

	void ShowOriginData();
	void ShowOneNumFrequence();

	void Show(int num);

private:
	vector<vector<int>> data_;
	map<int, int> num_frequence_map_;
	double support_min_num_;

	VerticalTree vtree_;
	HorizontalList hlist_;
	vector<NumVerticalTree> num_vtreelist_;


	void Set_num_frequence_map();
	void SortDataByFrequenceForEachList();
	void ClearUnderSup();
	void SetVerticalTree();
	void SetHorizontalList();
	void SetNumVerticalTreeList();

	map<int, int> Get_num_index_map();

};

