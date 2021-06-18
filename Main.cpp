#include <bits/stdc++.h>
using namespace std;
int maxWeight = 4000000;
class transInfo
{
public:
    string id;
    int fee;
    int weight;
    vector<string> parent;
};
 // Unordered maps O(1) will map cur_TransId to iterator.
unordered_map<string, transInfo *> unoMapTrans; 
set<string> CompletedTrans;
//This set will always maintain highest fee at top.
set<pair<double, transInfo *>, greater<pair<double, transInfo *>>> transSet;
vector<string> SortedOutputID; 
//It splits string by a delimiter
vector<string> split( string& str, const string& delim)
{
    vector<string> parts;
    size_t last = 0, current = 0;
    do
    {
        current = str.find(delim, last);
        if (current == string::npos) current = str.length();
        string temp = str.substr(last, current-last);
        if (!temp.empty()) parts.push_back(temp);
        last = current + delim.length();
    }
    while (current < str.length() && last < str.length());
    return parts;
}

// To check the Txn is valid or not
bool check_transation(transInfo *Txn, set<string> &CompletedTrans)
{
    int flag=1;
    for (auto i:Txn->parent)
    {
        if (CompletedTrans.find(i) == CompletedTrans.end())//Parent cur_Trans not found.
            flag=0;
    }
    return flag;
}
void Iter_solve(){
     for (auto i : unoMapTrans)
    {
        transSet.insert({(double)i.second->fee / (double)i.second->weight, i.second});
    }
    int blk_weight = 0, FeesTotal = 0;
    while (!transSet.empty() && blk_weight < maxWeight)
    {
        bool vis = false;
        for (auto itr = transSet.begin(); itr != transSet.end(); itr++)
        {
            transInfo *Trx_info_cur = (*itr).second;
            int curFee = Trx_info_cur->fee;
            int currentWeight = Trx_info_cur->weight;           
            int TotalWeight=blk_weight + currentWeight; 
            if (check_transation(Trx_info_cur, CompletedTrans) && (TotalWeight<= maxWeight))
            {
                CompletedTrans.insert(Trx_info_cur->id);
                SortedOutputID.push_back(Trx_info_cur->id);
                transSet.erase(itr);              //delete cur_Trans
                blk_weight += currentWeight;
                FeesTotal += curFee;
                vis = true;                       //mark as visited
                break;
            }
        }

        if (!vis) // INVALID Case
            break;
    }
       
    cout << "Total Weight: " << blk_weight << endl<<"Total Fees: " << FeesTotal << endl;
}
int main()
{    
    ifstream file("mempool.csv");
    vector<string> parts;
    string  line, sliced;
    getline(file, line);

    while (getline(file, line))
    {
        parts.clear();
        stringstream s(line);
        while (getline(s, sliced, ','))
        {
            parts.push_back(sliced); 
        }
        pair<string, transInfo *> cur_Trans_all ;
    auto cur_Trans = new transInfo();
    cur_Trans->id = parts[0];
    cur_Trans->fee = stoi(parts[1]);
    cur_Trans->weight = stoi(parts[2]);

    vector<string> parentcur_Transs;
    for (int i = 3; i <= (parts.size()-1);)
    {
        parentcur_Transs.push_back(parts[i++]);
    }
    cur_Trans->parent = parentcur_Transs;
    cur_Trans_all ={parts[0], cur_Trans};
        unoMapTrans[cur_Trans_all.first] = cur_Trans_all.second;
    }
    //read is complete
    file.close();
   
    Iter_solve();
   ofstream myfile("block.txt");

    for (auto id : SortedOutputID)
    myfile << id << endl;
    myfile.close();
    return 0;
}