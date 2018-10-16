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

void FPGrowth::ShowTree()
{
	vtree_.Show();
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
	int i = 0;
	for (map<int, int>::iterator it = num_frequence_map_.begin(); it != num_frequence_map_.end(); ++it) {
		cout << i << "/" << num_frequence_map_.size() << " " << it->first << endl;
		i++;

		int frequence = it->second;
		if (frequence > support_min_num_) {
			int num = it->first;
			num_vtreelist_.push_back(NumVerticalTree(hlist_.GetList(num), support_min_num_));
		}
	}
	cout << "finish" << endl;
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
	num_vtreelist_[hlist_.GetIndex(num)].Show();
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
	cout << endl;
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

void NumVerticalTree::Set(vector<NodePtr> list)
{
	Node* root_ptr = root;
	int num_times = 0;
	for (int i = 0; i < list.size(); i++) {
		num_times += list[i].tptr_->times_;
		SetLoop(list[i].tptr_->parent_, root_ptr, list[i].tptr_->times_);
	}
}

NumVerticalTree::NumVerticalTree(vector<NodePtr> list, double support_min)
{
	this->support_min = support_min;
	Set(list);
	ClearUnderSup();
	Set_pre_set(this->root);
	//Set_combine(memo);

	Show();
	/*time_t t1, t2;
	t1 = time(NULL);
	Set_combine();
	t2 = time(NULL);
	cout << "time" << t2 - t1 << endl;*/
}

void NumVerticalTree::ShowTreeList()
{
	temp.clear();
	for (int i = 0; i < pre_set.size(); i++) {
		ShowVector(pre_set[i]);
	}
	cout << endl;

	time_t t1, t2;

	t1 = time(NULL);
	Set_combine();
	cout << "without memo" << endl;
	//ShowCombine();
	t2 = time(NULL);
	cout << "time" << t2 - t1 << endl;
	t1 = time(NULL);
	this->combine.resize(0);
	Set_combine(memo);
	cout << "memo" << endl;
	//ShowCombine();
	t2 = time(NULL);
	cout << "time" << t2 - t1 << endl;

}

void NumVerticalTree::ShowCombine()
{
	for (int i = 0; i < combine.size(); i++) {
		combine[i].Show();
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
	for (int i = 0; i < ptr->childs_.size(); i++) {
		Item item{ ptr->childs_[i].tptr_->num_ ,ptr->childs_[i].tptr_->times_ };
		temp.push_back(item);
		Set_pre_set(ptr->childs_[i].tptr_);
		pre_set.push_back(temp);
		temp.pop_back();
	}
}

void NumVerticalTree::Set_combine()
{

	for (int i = 0; i < pre_set.size(); i++) {
		//ShowVector(pre_set[i]);
		
		// 2^(pre_set.size()-1) 除了尾巴以外的組合
		int size = (1 << (pre_set[i].size() - 1));
		for (int j = 0; j < size; j++) {
			vector<int> itemlist;
			if ((pre_set[i].size() - 1) < 20) {
				bitset<20> foo(j);
				int min_times = pre_set[i].back().times;
				for (int k = 0; k < pre_set[i].size() - 1; k++) {
					if (foo[k] == 1) {
						itemlist.push_back(pre_set[i][k].num);
						if (min_times > pre_set[i][k].times) {
							min_times = pre_set[i][k].times;
						}
					}
				}
				itemlist.push_back(pre_set[i].back().num);
				ItemList it = ItemList(itemlist, min_times);
				Add_combine(it);
			} else if ((pre_set[i].size() - 1) < 100) {
				bitset<100> foo(j);
				int min_times = pre_set[i].back().times;
				for (int k = 0; k < pre_set[i].size() - 1; k++) {
					if (foo[k] == 1) {
						itemlist.push_back(pre_set[i][k].num);
						if (min_times > pre_set[i][k].times) {
							min_times = pre_set[i][k].times;
						}
					}
				}
				itemlist.push_back(pre_set[i].back().num);
				ItemList it = ItemList(itemlist, min_times);
				Add_combine(it);
			} else {
				bitset<1000> foo(j);
				int min_times = pre_set[i].back().times;
				for (int k = 0; k < pre_set[i].size() - 1; k++) {
					if (foo[k] == 1) {
						itemlist.push_back(pre_set[i][k].num);
						if (min_times > pre_set[i][k].times) {
							min_times = pre_set[i][k].times;
						}
					}
				}
				itemlist.push_back(pre_set[i].back().num);
				ItemList it = ItemList(itemlist, min_times);
				Add_combine(it);
			}
			
			//bitset<1000> foo(j);
			//int min_times = pre_set[i].back().times;
			//for (int k = 0; k < pre_set[i].size() - 1; k++) {
			//	if (foo[k] == 1) {
			//		itemlist.push_back(pre_set[i][k].num);
			//		if (min_times > pre_set[i][k].times) {
			//			min_times = pre_set[i][k].times;
			//		}
			//	}
			//}
			//itemlist.push_back(pre_set[i].back().num);
			//ItemList it = ItemList(itemlist, min_times);
			//Add_combine(it);
		}
		
	}
}

void NumVerticalTree::Set_combine(Memory & memo)
{
	for (int i = 0; i < pre_set.size(); i++) {
		Add_combine(memo.GetCombine(pre_set[i]), pre_set[i].back().times);
	}
}

void NumVerticalTree::Add_combine(ItemList& itemlist)
{
	for (int i = 0; i < combine.size(); i++) {
		if (combine[i].isEqual(itemlist)) {
			combine[i].AddTimes(itemlist.times());
			return;
		}
	}
	combine.push_back(itemlist);
}

void NumVerticalTree::Add_combine(vector<ItemList> list, int times)
{
	for (int i = 0; i < list.size(); i++) {
		list[i].Set_times(times);
		Add_combine(list[i]);
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

ItemList::ItemList(vector<int> nums, int times)
{
	this->nums_ = nums;
	this->times_ = times;
}

bool ItemList::isEqual(const ItemList & other)
{
	bool aaa = (this->nums_ == other.nums_);
	int a = 0;
	return aaa;
}

void ItemList::AddValue(Item it)
{
	nums_.push_back(it.num);
	times_ = min(times_, it.times);
}

void ItemList::Show()
{
	for (int i = 0; i < nums_.size(); i++) {
		cout << nums_[i] << " ";
	}
	cout << ": " << times_ << endl;
}

vector<ItemList> Memory::GetCombine(vector<Item> arr)
{
	if (FindCombine(arr)) {
		return GetOnlyWithTail(memo_[hash(arr)]);
	} else {
		Item tail = arr.back();
		arr.pop_back();
		return GetOnlyWithTail(GetCombine_loop(arr, tail));
	}
}

vector<ItemList> Memory::GetCombine_loop(vector<Item> arr, Item it)
{
	if (arr.size() == 0) {
		return AddCombine(it);
	} else if (FindCombine(arr)) {
		return AddCombine(arr,memo_[hash(arr)], it);
	} else{
		Item tail = arr.back();
		//arr.pop_back();
		return AddCombine(arr, GetCombine_loop(vector<Item>(arr.begin(),arr.end()-1), tail), it);
	}
}

bool Memory::FindCombine(vector<Item> arr)
{
	return (memo_.count(hash(arr)) != 0);
}

vector<ItemList> Memory::AddCombine(Item it)
{
	vector<Item> items;
	items.push_back(it);

	vector<int> nums;
	nums.push_back(it.num);
	vector<ItemList> lists;
	lists.push_back(ItemList(nums, it.times));
	memo_[hash(items)] = lists;

	return lists;
}

vector<ItemList> Memory::AddCombine(vector<Item> arr, vector<ItemList> list, Item it)
{
	arr.push_back(it);

	vector<int> nums;
	nums.push_back(it.num);
	//for (int i = 0; i < arr.size(); i++) {
	//	nums.push_back(arr[i].num);
	//}
	

	int length = list.size();
	list.resize((length * 2) + 1);
	for (int i = 0; i < length; i++) {
		ItemList itlist = list[i];
		itlist.AddValue(it);
		list[length + i] = itlist;
	}
	list[list.size() - 1] = ItemList(nums, it.times);
	memo_[hash(arr)] = list;

	return list;
}

string Memory::hash(vector<Item> list)
{
	if (list.empty()) {
		return "0";
	}
	string str = to_string(list.size());
	str = str + to_string(list.front().num);
	str = str + to_string(list.back().num);
	return str;
}

vector<ItemList> Memory::GetOnlyWithTail(vector<ItemList> arr)
{
	int size = arr.size();
	int head = size / 2;
	return vector<ItemList>(arr.begin()+head,arr.end());
}
