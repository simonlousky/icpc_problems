#include<iostream>
#include<vector>
#include<stdio.h>
#include<math.h>
#include<queue>
#include<stack>
#include<vector>
#include<set>

#define EPS 1E-9
#define TOP 10

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

/* ========= Suffix Array implementation ========= */
#include <algorithm>
#include <string>
#include <iostream>

using namespace std;
typedef vector<int> vi;

string SA_str; // the input string, up to 100K characters
int n; // the length of input string
vi RA, tempRA; // rank array and temporary rank array
vi SA, tempSA; // suffix array and temporary suffix array
vi c; // for counting/radix sort
vi Phi,PLCP,LCP;

void countingSort(int k) { // O(n)
    int n = SA_str.length()-1;
    int i, sum, maxi = max(300, n); // up to 255 ASCII chars or length of n
    c.assign(maxi,0);
    for (i = 0; i < n; i++) // count the frequency of each integer rank
        c[i + k < n ? RA[i + k] : 0]++;
    for (i = sum = 0; i < maxi; i++) {
        int t = c[i]; c[i] = sum; sum += t; }
    for (i = 0; i < n; i++) // shuffle the suffix array if necessary
        tempSA[c[SA[i]+k < n ? RA[SA[i]+k] : 0]++] = SA[i];
    for (i = 0; i < n; i++) // update the suffix array SA
        SA[i] = tempSA[i];
}

void constructSA() { // this version can go up to 100000 characters
    int n = SA_str.length()-1;
    RA.assign(n,0);
    tempRA.assign(n,0);
    SA.assign(n,0);
    tempSA.assign(n,0);
    int i, k, r;
    for (i = 0; i < n; i++) RA[i] = SA_str[i]; // initial rankings
    for (i = 0; i < n; i++) SA[i] = i; // initial SA: {0, 1, 2, ..., n-1}
    for (k = 1; k < n; k <<= 1) { // repeat sorting process log n times
        countingSort(k); // actually radix sort: sort based on the second item
        countingSort(0); // then (stable) sort based on the first item
        tempRA[SA[0]] = r = 0; // re-ranking; start from rank r = 0
        for (i = 1; i < n; i++) // compare adjacent suffixes
        tempRA[SA[i]] = // if same pair => same rank r; otherwise, increase r
        (RA[SA[i]] == RA[SA[i-1]] && RA[SA[i]+k] == RA[SA[i-1]+k]) ? r : ++r;
        for (i = 0; i < n; i++) // update the rank array RA
        RA[i] = tempRA[i];
        if (RA[SA[n-1]] == n-1) break; // nice optimization trick
    }
}

/* ======= End of Suffix Array implementation ======= */

struct str_compare {
    bool operator() (const pair<string, int>& a, const pair<string, int>& b) const {
        if(a.first.length() != b.first.length())
        {
            return a.first.length() < b.first.length();
        }
        if(a.first != b.first){
            return a.first < b.first;
        }
        return a.second < b.second;
    }
};

bool prefix(const string& s, const string& sub){
    for(int i=0; i<sub.length(); i++){
        if(s[i] != sub[i]){
            return false;
        }
    }
    return true;
}

/* binary search lower and upper bound */
int lower(string& s, string& sub, vi& SA, vi& origin_end_ptr){
    int n = s.length();
    int a = 0;
    int b = n-1;
    int m;

    while(a <= b) {
        if(a == b)
            return a;

        m = (a + b) / 2;
        string sub_s = s.substr(SA[m], origin_end_ptr[SA[m]]-SA[m]);
        // cout << "tmp word: " << sub_s << endl;
        if(sub_s < sub){
            a = m + 1;
        }else if(sub_s > sub && !prefix(sub_s, sub)){
            b = m - 1;
        }else{
            b = m;
        }
            
    }
    return m;

}

int upper(string& s, string& sub, vi& SA, vi& origin_end_ptr){
    int n = s.length();
    int a = 0;
    int b = n-1;
    int m;

    while(a <= b) {
        if(a == b)
            return a-1;

        m = (a + b) / 2;
        string sub_s = s.substr(SA[m], origin_end_ptr[SA[m]]-SA[m]);
        // cout << "tmp word: " << sub_s << endl;
        if(sub_s < sub || prefix(sub_s, sub)){
            a = m + 1;
        }else {
            b = m;
        }
            
    }
    return m-1;
}
/* =================================================================== */

int main() {
    int N;
    int cursor = 0;
    string s;
    cin >> N;
    vi lut, origin_ptr, origin_end_ptr;
    for(int i=0; i<N; i++){
        cin >> s;
        SA_str.append(s + "$");
        for(int j=0; j<s.length()+1; j++){
            lut.push_back(i+1);
            origin_ptr.push_back(cursor);
            origin_end_ptr.push_back(cursor + s.length());
        }
        cursor+=s.length()+1;
    }
    n = SA_str.length();
    SA_str += '$'; // add terminating character
    constructSA();

    int Q;
    string qi;
    scanf("%d", &Q);

    for(int i=0; i<Q; i++){
        set<pair<string,int>, str_compare> words;
        int words_idx[N+1] = {0};
        cin >> qi;
        int l = lower(SA_str, qi, SA, origin_end_ptr);
        int r = upper(SA_str, qi, SA, origin_end_ptr);
        int k=l;

        if(prefix(SA_str.substr(SA[l]), qi)){

            while(k <= r){
            // while(prefix(SA_str.substr(SA[k]), qi)){
                if(words_idx[lut[SA[k]]]){
                    k++;
                    continue;
                }

                words_idx[lut[SA[k]]]++;
                string sub = SA_str.substr(origin_ptr[SA[k]], origin_end_ptr[SA[k]]-origin_ptr[SA[k]]);
                // cout << "inserting " << sub << endl;
                words.insert(make_pair(sub, lut[SA[k]]));
                if (words.size() > TOP){
                    words.erase(*words.rbegin());
                }
                k++;
            }
        }

        k = 1;
        if(words.size() > 0){
            auto it = words.begin();
            cout << it->second;
            while(++it != words.end() && k != TOP){
                cout << " " << it->second;
                k++;
            }
            cout << endl;
        }else{
            cout << -1 << endl;
        }

    }

    return 0;
}

/* ===================== */