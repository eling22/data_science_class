#include "fpgrowth.h"

FPGrowth::FPGrowth(vector<vector<int>> data, int line_num, double support_min)
{
	data_ = data;
	support_min_num_ = line_num * support_min;

	Set_num_frequence_map();
	SortDataByFrequenceForEachList();
	ClearUnderSup();
	SetVerticalTree();
	SetHorizontalList();
	SetNumVerticalTreeList();
}

FPGrowth::~FPGrowth()
{
}

void FPGrowth::ShowOriginData()
{
	for (int i = 0; i < data_.size(); i++)
	{
		for (int j = 0; j < data_[i].size(); j++)
		{
			cout << data_[i][j] << " ";
		}
		cout << endl;
	}
}

void FPGrowth::Set_num_frequence_map()
{
	for (int i = 0; i < data_.size(); i++)
	{
		for (int j = 0; j < data_[i].size(); j++)
		{
			num_frequence_map_[data_[i][j]]++;
		}
	}
}

void FPGrowth::ShowOneNumFrequence()
{
	for (map<int, int>::iterator it = num_frequence_map_.begin(); it != num_frequence_map_.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
}

void FPGrowth::SortDataByFrequenceForEachList()
{
	auto comp = [=](int a, int b) { return (num_frequence_map_[a] > num_frequence_map_[b]); };
	
	for (int i = 0; i < data_.size(); i++)
	{
		sort(data_[i].begin(), data_[i].end(), comp );
	}
}

void FPGrowth::ClearUnderSup()
{
	for (int i = 0; i < data_.size(); i++)
	{
		while (num_frequence_map_[data_[i].back()] < support_min_num_) {
			data_[i].pop_back();
			if (data_[i].empty()) {
				break;
			}
		}
	}
}

void FPGrowth::SetVerticalTree()
{
	for (int i = 0; i < data_.size(); i++)
	{
		vtree_.Add(data_[i]);
	}
}

void FPGrowth::SetHorizontalList()
{
	hlist_.Set(Get_num_index_map(), vtree_.GetRoot());
}

void FPGrowth::SetNumVerticalTreeList()
{
	for (map<int, int>::iterator it = num_frequence_map_.begin(); it != num_frequence_map_.end(); ++it) {
		int frequence = it->second;
		if (frequence > support_min_num_) {
			int num = it->first;
			num_vtreelist_.push_back(NumVerticalTree(hlist_.GetList(num), support_min_num_));
		}
	}
}

map<int, int> FPGrowth::Get_num_index_map()
{
	map<int, int> num_index_map;
	for (map<int, int>::iterator it = num_frequence_map_.begin(); it != num_frequence_map_.end(); ++it) {
		int frequence = it->second;
		if (frequence > support_min_num_) {
			int num = it->first;
			num_index_map[num] = num_index_map.size();

		}
	}
	return num_index_map;
}

void FPGrowth::Show(int num)
{
	hlist_.Show_htable(num);
	num_vtreelist_[hlist_.GetIndex(num)].ShowTreeList();
}

Tree::Tree()
{
	root = new Node();
	root->level_ = 0;
}

void Tree::Show()
{
	ShowLoop(root);
}

int Tree::getIndexByValue(vector<NodePtr>& arr, int value)
{
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i].tptr_->num_ == value) {
			return i;
		}
	}
	return -1;
}

void Tree::ShowLoop(Node * ptr)
{
	for (int i = 0; i < ptr->childs_.size(); i++) {
		for (int j = 0; j < ptr->level_; j++) {
			cout << " ";
		}
		cout << ptr->childs_[i].tptr_->num_ << ":" << ptr->childs_[i].tptr_->times_ << endl;
		ShowLoop(ptr->childs_[i].tptr_);
	}
}

void VerticalTree::Add(vector<int> data)
{
	Node* ptr = root;
	for (int i = 0; i < data.size(); i++) {
		int index = getIndexByValue(ptr->childs_, data[i]);
		if (index == -1) {
			NodePtr newnode(new Node(data[i], i + 1, ptr));
			ptr->childs_.push_back(newnode);
			ptr = ptr->childs_.back().tptr_;
		} else {
			ptr->childs_[index].tptr_->times_++;
			ptr = ptr->childs_[index].tptr_;
		}
	}
}

void HorizontalList::Set(map<int, int> num_index_map, Node * vtree_root)
{
	htable_map_ = num_index_map;
	htable_.resize(htable_map_.size());
	SetLoop(vtree_root);
}

void HorizontalList::Show_htable_map()
{
	for (map<int, int>::iterator it = htable_map_.begin(); it != htable_map_.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
}

void HorizontalList::Show_htable(int num)
{
	int index = htable_map_[num];
	for (int i = 0; i < htable_[index].size(); i++) {
		ShowParent(htable_[index][i].tptr_->parent_);
		cout << num << ":" << htable_[index][i].tptr_->times_ << endl;
	}
	cout << endl;
}

vector<NodePtr> HorizontalList::GetList(int num)
{
	int index = htable_map_[num];
	return htable_[index];
}

void HorizontalList::SetLoop(Node * ptr)
{
	for (int i = 0; i < ptr->childs_.size(); i++) {
		int num = ptr->childs_[i].tptr_->num_;
		int index = htable_map_[num];
		htable_[index].push_back(ptr->childs_[i]);

		SetLoop(ptr->childs_[i].tptr_);
	}
}

void HorizontalList::ShowParent(Node * ptr)
{
	if (ptr->level_ != 0) {
		ShowParent(ptr->parent_);
		cout << ptr->num_ << ",";
	}
}

NumVerticalTree::NumVerticalTree(vector<NodePtr> list, double support_min)
{
	this->support_min = support_min;
	Set(list);
	ClearUnderSup();
	Set_pre_set(this->root);
}

void NumVerticalTree::Set(vector<NodePtr> list)
{
	Node* root_ptr = root;
	int num_times = 0;
	for (int i = 0; i < list.size(); i++) {
		num_times += list[i].tptr_->times_;
		SetLoop(list[i].tptr_->parent_, root_ptr, list[i].tptr_->times_);
	}
}

void NumVerticalTree::ShowTreeList()
{
	temp.clear();
	for (int i = 0; i < pre_set.size(); i++) {
		ShowVector(pre_set[i]);
	}
}

Node * NumVerticalTree::SetLoop(Node * list_ptr, Node * tree_ptr, int times)
{
	//tree在向下走，list在向上走

	if (list_ptr->level_ == 0) {
		return NULL;
	}
	if (list_ptr->level_ != 1) {
		tree_ptr = SetLoop(list_ptr->parent_, tree_ptr, times);
	}

	int index = getIndexByValue(tree_ptr->childs_, list_ptr->num_);
	if (index == -1) {
		NodePtr newnode(new Node(list_ptr->num_, tree_ptr->level_ + 1, tree_ptr));
		newnode.tptr_->times_ = times;
		tree_ptr->childs_.push_back(newnode);
		tree_ptr = tree_ptr->childs_.back().tptr_;
	} else {
		tree_ptr->childs_[index].tptr_->times_ += times;
		tree_ptr = tree_ptr->childs_[index].tptr_;
	}
	return tree_ptr;
	
}

void NumVerticalTree::Set_pre_set(Node* ptr)
{
	if (ptr->childs_.size() == 0) {
		pre_set.push_back(temp);
	}
	for (int i = 0; i < ptr->childs_.size(); i++) {
		Item item{ ptr->childs_[i].tptr_->num_ ,ptr->childs_[i].tptr_->times_ };
		temp.push_back(item);
		Set_pre_set(ptr->childs_[i].tptr_);
		temp.pop_back();
	}
}

void NumVerticalTree::ShowVector(vector<Item> arr)
{
	for (int i = 0; i < arr.size(); i++) {
		cout << arr[i].num << ":" << arr[i].times << " ";
	}
	cout << endl;
}

void NumVerticalTree::ClearUnderSup()
{
	ClearUnderSup_loop(this->root);
}

void NumVerticalTree::ClearUnderSup_loop(Node * ptr)
{
	for (int i = 0; i < ptr->childs_.size(); i++) {
		ClearUnderSup_loop(ptr->childs_[i].tptr_);
		if (ptr->childs_[i].tptr_->times_ < support_min) {
			ptr->childs_.erase(ptr->childs_.begin() + i);
			i--;
		}
	}
}
