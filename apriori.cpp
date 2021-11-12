#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds; // find_by_order(),order_of_key()
typedef tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> ordered_set;

vector<set<int>> dataset;               // all preprocessed transactions
vector<vector<int>> Candidates_k;       // potential candidates
vector<vector<int>> L_k;                // frequent itemsets in kth lvl
vector<vector<vector<int>>> freq_items; // Final list of items which are frequent
int minsup;                             // minimum support

// check if candidate support is > minsup
vector<vector<int>> candidates_greater_than_minsup(vector<set<int>> &dataset)
{
    vector<vector<int>> new_candidates;

    for (auto candidate : Candidates_k)
    {
        int sup = 0;
        for (auto transaction : dataset)
        {
            bool supportive_transaction = true;
            for (auto item : candidate)
            {
                if (transaction.find(item) == transaction.end())
                {
                    supportive_transaction = false;
                    break;
                }
            }
            if (supportive_transaction)
                sup++;
        }
        if (sup >= minsup)
            new_candidates.push_back(candidate);
    }
    return new_candidates;
}

void generate_freq_items()
{
    L_k.clear();
    vector<vector<int>> freq_itemsets = candidates_greater_than_minsup(dataset);
    for (auto i : freq_itemsets)
    {
        L_k.push_back(i);
    }
}

void next_candidates()
{
    Candidates_k.clear();
    for (int i = 0; i < L_k.size(); i++)
    {
        vector<int> v1 = L_k[i];
        int c1 = v1.back();
        v1.pop_back();
        for (int j = i + 1; j < L_k.size(); j++)
        {
            // all items same except last
            vector<int> v2 = L_k[j];
            int c2 = v2.back();
            v2.pop_back();
            if (v1 == v2)
            {
                v2.push_back(min(c1, c2));
                v2.push_back(max(c1, c2));
                Candidates_k.push_back(v2);
            }
            // else
            //     break;
        }
    }
    // new lvl potential candidates are found
    // Pruning
    set<vector<int>> set_L_k;
    for (auto i : L_k)
        set_L_k.insert(i);

    vector<vector<int>> new_candidates;
    for (auto candidate : Candidates_k)
    {
        bool flag = true; // true = all parents freq
        vector<int> par = candidate;
        for (int i = 0; i < candidate.size(); i++)
        {
            // candidate = ABCD
            // removing B and check ACD freq
            par.erase(par.begin() + i);
            if (set_L_k.find(par) == set_L_k.end())
            {
                flag = false;
                break;
            }
            par.insert(par.begin() + i, candidate[i]);
        }
        if (flag)
            new_candidates.push_back(candidate);
    }
    Candidates_k = new_candidates;
}

void apriori()
{
    while (1)
    {
        next_candidates();
        if (Candidates_k.empty())
            break;

        // we pruned the candidates, now remove wrt minsup
        generate_freq_items();
        if (!L_k.empty())
            freq_items.push_back(L_k);
        // if(L_k[0].size() == 2)
        // {
        //     for(auto i : L_k)
        //     {
        //         for(auto j : i)
        //             cout<<j<<" ";
        //         cout<<endl;
        //     }
        // }
    }
}

void print_freq_items()
{
    if (freq_items.empty())
    {
        cout << "No frequent itemsets found\n";
        return;
    }
    for (auto i : freq_items)
    {
        for (auto j : i)
        {
            cout << "[ ";
            for (auto k : j)
                cout << k << ",";
            cout << "]\n";
        }
    }
    cout << "Total freq items found: " << freq_items.size() << "\n";
}

void display_frequent_itemsets(vector<vector<vector<int>>> &frequent_itemsets_mined)
{
    int z = 0, j, k, total_itemsets = 0;
    if (frequent_itemsets_mined.size())
    {
        for (auto i : frequent_itemsets_mined)
        {

            cout << "\nFrequent Itemsets for k = " << ++z << "\n";
            cout << "Number of Itemsets formed are : " << i.size() << endl;
            cout << "\n[";
            total_itemsets += i.size();
            for (j = 0; j < i.size() - 1; j++)
            {
                cout << "[";
                for (k = 0; k < i[j].size(); k++)
                {
                    cout << i[j][k];
                    if (i[j].size() - 1 != k)
                    {
                        cout << ",";
                    }
                }
                cout << "],";
            }
            cout << "[";
            for (k = 0; k < i[j].size(); k++)
            {
                cout << i[j][k];
                if (i[j].size() - 1 != k)
                {
                    cout << ",";
                }
            }
            cout << "]";
            cout << "]\n";
        }
        cout << endl;
        cout << "Total Number of Itemsets Generated are : " << total_itemsets << endl;
        cout << endl;
    }
    else
        cout << "No Frequent Itemsets Found" << endl;
}

void process_partition(vector<set<int>> &short_dataset, vector<vector<vector<int>>> &potential_itemsets)
{
    Candidates_k.clear();
    L_k.clear();
    freq_items.clear();
    set<int> all_items; // all items in short_dataset
    map<int, int> cnt;  // item count in short_dataset

    for (auto i : short_dataset)
    {
        for (auto j : i)
        {
            all_items.insert(j);
            cnt[j]++;
        }
    }

    for (auto it : all_items)
    {
        if (cnt[it] >= minsup)
        {
            // pushing singletons in form of vector
            vector<int> v = {it};
            L_k.push_back(v);
        }
    }
    freq_items.push_back(L_k);

    apriori();
    for (auto i : freq_items)
    {
        potential_itemsets.push_back(i);
    }
}

int main()
{
    int type = 1;
    cout << "Choose the type of optimization strategy:\n";
    cout << "1. Normal Apriori\n2. Hash Based Technique\n3. Partitioning\n";
    cout << "Enter No.: ";
    cin >> type;

    cout << "Enter the minimum support: ";
    cin >> minsup;
    cout << "Enter Transactional Dataset filename: ";
    string input_file = "pfden.dat";
    // cin >> input_file;
    cout << "----------------------------------------------------------------\n";

    // cout to output.txt
    freopen("output.txt", "w", stdout);

    // ----------------------------------------------------------------------------
    // Preprocessing the txt file

    ifstream file(input_file);
    if (!file)
    {
        cout << "File not found.\n";
        return -1;
    }
    string line;
    while (getline(file, line))
    {
        set<int> transaction;
        string item = "";
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == '-')
            {
                i += 2;
            }
            else if (line[i] == ' ')
            {
                stringstream item_stream(item);
                int item_int = -1;
                item_stream >> item_int;
                if (item_int >= 0)
                    transaction.insert(item_int);
                item = "";
            }
            else
            {
                item += line[i];
            }
        }
        // for(auto i : transaction) cerr<<i<<" ";
        // cerr<<endl;
        dataset.push_back(transaction);
    }
    file.close();
    // ----------------------------------------------------------------------------
    // MAIN code

    if (type == 1)
    {
        // NORMAL APRIORI with no optimization
        cout << "OUTPUT BY NORMAL APRIORI:\n";
        set<int> all_items; // all items in whole dataset
        map<int, int> cnt;  // item count in dataset

        auto start_time = chrono::high_resolution_clock::now();
        for (int i = 0; i < dataset.size(); i++)
        {
            for (auto j : dataset[i])
            {
                all_items.insert(j);
                cnt[j]++;
            }
        }

        for (auto it : all_items)
        {
            if (cnt[it] >= minsup)
            {
                // pushing singletons in form of vector
                vector<int> v = {it};
                L_k.push_back(v);
            }
        }
        freq_items.push_back(L_k);

        apriori();

        display_frequent_itemsets(freq_items);
        // print_freq_items();
        auto stop = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start_time);
        cout << "Time taken: " << duration.count() / 1000000.0 << " secs\n";
        cout << "---------------------------------------------------------------\n";
    }
    else if (type == 2)
    {
        // HASH APRIORI
        cout << "OUTPUT BY APRIORI OPTIMIZED BY HASH BASED TECHNIQUES:\n";
        auto start_time = chrono::high_resolution_clock::now();

        ordered_set all_items; // all items in whole dataset
        map<int, int> cnt;     // item count in dataset
        for (int i = 0; i < dataset.size(); i++)
        {
            for (auto j : dataset[i])
            {
                all_items.insert(j);
                cnt[j]++;
            }
        }

        set<pair<int, int>> bucket[7];
        int bucket_count[7] = {0};
        for (auto transaction : dataset)
        {
            auto itr = transaction.begin();

            while (itr != transaction.end())
            {
                auto itr2 = next(itr, 1);
                while (itr2 != transaction.end())
                {
                    // all possible pairs in transaction set
                    int order1 = all_items.order_of_key(*itr);
                    int order2 = all_items.order_of_key(*itr2);

                    // calculate hash and put into bucket
                    int hash = (10 * order1 + order2) % 7;
                    bucket_count[hash]++;
                    bucket[hash].insert({*itr, *itr2});
                    itr2++;
                }
                itr++;
            }
        }

        // all freq single items are pushed
        for (auto item : all_items)
        {
            if (cnt[item] >= minsup)
            {
                vector<int> v = {item};
                L_k.push_back(v);
            }
        }
        freq_items.push_back(L_k);

        // 2 size itemsets
        for (int i = 0; i < 7; i++)
        {
            if (bucket_count[i] >= minsup)
            {
                for (auto p : bucket[i])
                {
                    vector<int> v = {p.first, p.second};
                    Candidates_k.push_back(v);
                }
            }
        }
        // new lvl potential candidates are found
        // Pruning
        set<vector<int>> set_L_k;
        for (auto i : L_k)
            set_L_k.insert(i);

        vector<vector<int>> new_candidates;
        for (auto candidate : Candidates_k)
        {
            bool flag = true; // true = all parents freq
            vector<int> par = candidate;
            for (int i = 0; i < candidate.size(); i++)
            {
                // candidate = ABCD
                // removing B and check ACD freq
                par.erase(par.begin() + i);
                if (set_L_k.find(par) == set_L_k.end())
                {
                    flag = false;
                    break;
                }
                par.insert(par.begin() + i, candidate[i]);
            }
            if (flag)
                new_candidates.push_back(candidate);
        }
        Candidates_k = new_candidates;

        Candidates_k = candidates_greater_than_minsup(dataset);
        L_k.clear();
        for (auto i : Candidates_k)
            L_k.push_back(i);
        if (!L_k.empty())
            freq_items.push_back(L_k);
        // for(auto i : L_k)
        // {
        //     for(auto j : i)
        //         cout<<j<<" ";
        //     cout<<endl;
        // }
        apriori();

        display_frequent_itemsets(freq_items);
        auto stop = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start_time);
        cout << "Time taken: " << duration.count() / 1000000.0 << " secs\n";
        cout << "---------------------------------------------------------------\n";
    }
    else
    {
        // PARTITION APRIORI
        cout << "OUTPUT BY APRIORI OPTIMIZED BY PARTITIONING:\n";
        auto start_time = chrono::high_resolution_clock::now();

        vector<vector<vector<int>>> potential_itemsets;
        // here freq_items = potential freq
        // potential_itemsets = final freq items

        // we are just reading 10-10 transactions at a time
        vector<set<int>> short_dataset;

        ifstream file3(input_file);
        if (!file3)
        {
            cout << "File not found.\n";
            return -1;
        }
        string line3;
        while (getline(file3, line3))
        {
            if (short_dataset.size() >= 10)
            {
                process_partition(short_dataset, potential_itemsets);
                short_dataset.clear();
            }

            set<int> transaction;
            string item = "";
            for (int i = 0; i < line3.size(); i++)
            {
                if (line3[i] == '-')
                {
                    i += 2;
                }
                else if (line3[i] == ' ')
                {
                    stringstream item_stream(item);
                    int item_int = -1;
                    item_stream >> item_int;
                    if (item_int >= 0)
                        transaction.insert(item_int);
                    item = "";
                }
                else
                {
                    item += line3[i];
                }
            }
            // for(auto i : transaction) cerr<<i<<" ";
            // cerr<<endl;
            short_dataset.push_back(transaction);
        }
        file3.close();

        if (!short_dataset.empty())
            process_partition(short_dataset, potential_itemsets);

        set<int> all_items; // all items in whole dataset
        map<int, int> cnt;  // item count in dataset

        for (int i = 0; i < dataset.size(); i++)
        {
            for (auto j : dataset[i])
            {
                all_items.insert(j);
                cnt[j]++;
            }
        }

        // we have all potential itemsets, we just need to check their minsup
        map<vector<int>, int> counter;
        set<vector<int>> final;

        // 2nd scan over database
        ifstream file2(input_file);
        if (!file2)
        {
            cout << "File not found.\n";
            return -1;
        }
        string line2;
        while (getline(file2, line2))
        {
            set<int> transaction;
            string iteem = "";
            for (int i = 0; i < line2.size(); i++)
            {
                if (line2[i] == '-')
                {
                    i += 2;
                }
                else if (line2[i] == ' ')
                {
                    stringstream item_stream(iteem);
                    int item_int = -1;
                    item_stream >> item_int;
                    if (item_int >= 0)
                        transaction.insert(item_int);
                    iteem = "";
                }
                else
                {
                    iteem += line2[i];
                }
            }
            // for(auto i : transaction) cerr<<i<<" ";
            // cerr<<endl;

            // process transaction
            for (auto i : potential_itemsets)
            {
                for (auto itemset : i)
                {
                    bool f = 0;
                    for (auto item : itemset)
                    {
                        if (transaction.find(item) == transaction.end())
                        {
                            f = 1;
                            break;
                        }
                    }
                    if (f == 0)
                    {
                        // itemset is there in this transaction
                        counter[itemset]++;
                        if (counter[itemset] == minsup)
                        {
                            final.insert(itemset);
                        }
                    }
                }
            }
        }
        file2.close();
        Candidates_k.clear();
        L_k.clear();
        freq_items.clear();

        for (auto it : all_items)
        {
            if (cnt[it] >= minsup)
            {
                // pushing singletons in form of vector
                vector<int> v = {it};
                L_k.push_back(v);
            }
        }
        freq_items.push_back(L_k);

        apriori();

        display_frequent_itemsets(freq_items);
        auto stop = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start_time);
        cout << "Time taken: " << duration.count() / 1000000.0 << " secs\n";
        cout << "---------------------------------------------------------------\n";
    }

    return 0;
}