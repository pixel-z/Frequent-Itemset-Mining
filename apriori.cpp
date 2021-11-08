#include <bits/stdc++.h>
using namespace std;

vector<set<int>> dataset;               // all preprocessed transactions
vector<vector<int>> Candidates_k;       // potential candidates
vector<vector<int>> L_k;                // frequent itemsets in kth lvl
set<int> all_items;                     // all items in whole dataset
map<int, int> cnt;                      // item count in dataset
vector<vector<vector<int>>> freq_items; // Final list of items which are frequent

void apriori()
{

}

int main()
{
    int minsup;
    cout << "Enter the minimum support value (frequency): ";
    cin >> minsup;
    cout << "Enter Transactional Dataset filename: ";
    string input_file = "SIGN.txt";
    // cin >> input_file;
    cout << "----------------------------------------------------------------\n";

    // cout to output.txt
    // freopen("output.txt", "w", stdout);

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
                int item_int = 0;
                item_stream >> item_int;
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

    // NORMAL APRIORI without any optimization
    auto start_time = chrono::high_resolution_clock::now();
    cout << "NORMAL APRIORI:\n";
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
    auto stop = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start_time);
    cout << "Time taken: " << duration.count() / 1000000.0 << " secs\n";
    cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n";

    return 0;
}