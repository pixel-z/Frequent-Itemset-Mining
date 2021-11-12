#include <bits/stdc++.h>
#include <chrono>
using namespace std::chrono;

using namespace std;
typedef long long int ll;
vector<vector<int>> frequents_global;
struct FP_TREESINGLENODE
{
    bool mark;
    int num_time;
    FP_TREESINGLENODE *parent;
    int node_value;
    map<int, FP_TREESINGLENODE *> childrens;
};

FP_TREESINGLENODE *Create_TreeNode(FP_TREESINGLENODE *par)
{
    FP_TREESINGLENODE *newnode = new FP_TREESINGLENODE;
    newnode->mark = false;
    newnode->num_time = 0;
    newnode->parent = par;
    newnode->node_value = -1;

    return newnode;
}

void FP_MERGING_STRAGY(int minsup, vector<pair<set<int>, int>> data, vector<int> alpha)
{
    map<int, int> mp;
    for (int i = 0; i < data.size(); i++)
    {
        for (auto it : data[i].first)
        {
            mp[it] += data[i].second;
        }
    }
    vector<pair<int, int>> order;
    for (auto it : mp)
    {
        if (it.second >= minsup)
            order.push_back({it.second, it.first});
    }
    if (order.empty())
    {
        return;
    }
    // cerr << "enter1"
        //  << "\n";
    sort(order.begin(), order.end(), greater<pair<int, int>>());
    map<int, int> position;

    FP_TREESINGLENODE *root = Create_TreeNode(nullptr);
    // cerr << "sdhjsdk"
        //  << "\n";
    vector<set<FP_TREESINGLENODE *>> Trie_struct(order.size());
    FP_TREESINGLENODE *craw = root;
    for (int i = 0; i < order.size(); i++)
    {
        position[order[i].second] = i;
    }
    for (int i = 0; i < data.size(); i++)
    {
        craw = root;
        for (int j = 0; j < order.size(); j++)
        {
            if (data[i].first.find(order[j].second) != data[i].first.end())
            {
                if (craw->childrens.find(order[j].second) != craw->childrens.end())
                {
                    craw->childrens[order[j].second]->num_time += data[i].second;
                    craw = craw->childrens[order[j].second];
                }
                else
                {

                    FP_TREESINGLENODE *temp = Create_TreeNode(craw);
                    Trie_struct[position[order[j].second]].insert(temp);
                    temp->parent = craw;
                    temp->node_value = order[j].second;
                    craw->childrens[order[j].second] = temp;
                    craw->childrens[order[j].second]->num_time += data[i].second;
                    craw = craw->childrens[order[j].second];
                }
            }
        }
    }
    // cerr << "tqwufdqkd"
    //      << "\n";
    if (root->childrens.size() == 0)
    {
        return;
    }
    craw = root;
    bool is_single = true;
    while (craw != nullptr)
    {
        if (craw->childrens.size() == 0)
        {
            break;
        }
        else
        {
            if (craw->childrens.size() > 1)
            {
                is_single = false;
                break;
            }
            else
            {
                auto it = craw->childrens.begin();
                craw = it->second;
            }
        }
    }
    vector<vector<pair<set<int>, int>>> dataset_arr(Trie_struct.size());
    if (!is_single)
    {

        for (int i = Trie_struct.size() - 1; i >= 0; i--)
        {
            vector<int> b;
            auto it = Trie_struct[i].begin();
            while (it != Trie_struct[i].end())
            {
                if ((*it)->mark)
                {
                    it++;
                    continue;
                }
                vector<FP_TREESINGLENODE *> add;
                FP_TREESINGLENODE *craw = *it;
                set<int> trans;
                vector<int> path;
                vector<int> freq;

                while (craw->parent->node_value != -1)
                {

                    freq.push_back(craw->parent->num_time);
                    add.push_back(craw->parent);
                    path.push_back(craw->parent->node_value);
                    craw = craw->parent;
                }
                if (!path.empty())
                {
                    trans.insert(path.back());
                    add.back()->mark = 1;
                }
                for (int k = path.size() - 2; k >= 0; k--)
                {
                    if (!add[k]->mark)
                    {
                        add[k]->mark = 1;
                        dataset_arr[position[path[k]]].push_back({trans, freq[k]});
                    }
                    trans.insert(path[k]);
                }
                if (trans.size() > 0 && (*it)->mark == 0)
                {
                    dataset_arr[i].push_back({trans, (*it)->num_time});
                }
                (*it)->mark = 1;
                it++;
            }
            b = alpha;
            // for (int j = 0; j < alpha.size(); j++)
            // {
            //     b.push_back(alpha[j]);
            // }
            b.push_back(order[i].second);
            frequents_global.push_back(b);
            FP_MERGING_STRAGY(minsup, dataset_arr[i], b);
        }
    }
    else
    {
        vector<int> path;
        int val = order.size();
        for (int i = 0; i < order.size(); i++)
        {
            path.push_back(order[i].second);
        }

        for (int i = 1; i < (1 << val); i++)
        {
            vector<int> items = alpha;
            string sub = "";
            int lp = i;
            for (int j = 0; j < path.size(); j++)
            {
                if (lp % 2)
                {
                    sub += "1";
                }
                else
                {
                    sub += "0";
                }
                lp = lp >> 2;
            }

            // print("Manoj")
            // cout << "Manoj" << endl;
            // cout << sub.length
            for (int j = 0; j < path.size(); j++)
            {

                if (sub[j] != '0')
                {
                    items.push_back(path[j]);
                }
            }
            frequents_global.push_back(items);
        }
        return;
    }
}

int main(void)
{
    int max_single_item = INT_MIN;
    int minsup;
    string input_file;
    cout << "Enter the Minimum Support Value You Want For Dataset : ";
    cin >> minsup;
    cout << "------------------------------------------------------------"
         << "\n";

    cout << "Enter the  Dataset Filename You Want To Run FP-GROWTH: ";
    cin >> input_file;
    cout << "------------------------------------------------------------"
         << "\n";

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    // CONVERSION OF ORIGINAL TEXT DATA INTO THE TRANSACTIONAL DATASET
    // vector<set<int> >dataset;

    // Error message for not opening a file
    ifstream file_data(input_file);
    if (!file_data)
    {
        cout << endl
             << "The given file does not exist!" << endl;
        return -1;
    }
    set<int> transaction;

    vector<int> sigleton_frequency;
    vector<pair<set<int>, int>> dataset;
    string single_transaction;
    while (getline(file_data, single_transaction))
    {
        string item = "";

        for (int i = 0; i < single_transaction.size(); i++)
        {
            if ((single_transaction[i] == '-'))
            {
                i += 2;
            }
            else if (single_transaction[i] == ' ')
            {
                stringstream item_i(item);
                int item_ii = -1;
                item_i >> item_ii;
                if (item_ii != -1)
                {
                    max_single_item = max(max_single_item, item_ii);
                    transaction.insert(item_ii);
                    item = "";
                }
            }
            else
            {
                item += single_transaction[i];
            }
        }
        // for (auto it : transaction)
        // {
        //     cout << it << " ";
        // }
        // cout << "\n";
        dataset.push_back(make_pair(transaction, 1));
        transaction.clear();
    }
    vector<int> alpha;
    file_data.close();
    // cerr << "enter "
    //      << "\n";
    cout << "Running the FP-GROWTH With Merging Techenic";
    auto start = high_resolution_clock::now();
    FP_MERGING_STRAGY(minsup, dataset, alpha);

    vector<pair<int, vector<int>>> freq_item_mined;

    for (int i = 0; i < frequents_global.size(); i++)
    {
        freq_item_mined.push_back({frequents_global[i].size(), frequents_global[i]});
    }
    sort(freq_item_mined.begin(), freq_item_mined.end());
    int Size, cs;

    vector<int> counts;
    for (int i = 0; i < freq_item_mined.size();)
    {
        cs = 0;
        Size = freq_item_mined[i].first;

        while (i < freq_item_mined.size())
        {
            if (freq_item_mined[i].first == Size)
            {
                cs++;
                i++;
            }
            else
            {
                break;
            }
        }
        counts.push_back(cs);
    }
    reverse(counts.begin(), counts.end());
    for (int i = 0; i < freq_item_mined.size();)
    {

        cout << "Frequent Itemsets for k = " << freq_item_mined[i].first << endl;
        cout << "Number of Itemsets formed for this value are : " << counts.back() << endl;
        cout << endl;
        cout << "[";
        counts.pop_back();
        Size = freq_item_mined[i].first;

        while (i < freq_item_mined.size())
        {
            if (freq_item_mined[i].first == Size)
            {
                cout << "[";
                int size = freq_item_mined[i].second.size();
                for (int j = 0; j < size; j++)
                {
                    int val = freq_item_mined[i].second[j];
                    cout << val;
                    if (j != size - 1)
                    {
                        cout << ",";
                    }
                }
                cout << "]";
                i++;
                if (i < freq_item_mined.size())
                    if (freq_item_mined[i].first == Size)
                        cout << ",";
            }
            else
            {
                break;
            }
        }
        cout << "]" << endl;
        cout << endl;
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Total number of Itemsets Generated are : " << freq_item_mined.size() << endl;
    cout << endl;
    cout
        << "-----------------------------------------------------------------------"
        << "\n";
    cout << "TIME TAKEN : " << double(duration.count()) / 1e6 << " sec" << endl;
    cout << endl;
    cout << "~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~" << endl;
    return 0;
}