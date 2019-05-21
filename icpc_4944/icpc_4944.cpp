#include<vector>
#include<iostream>
#include<stdio.h>
#include<set>
#include<algorithm>
#include<tuple>
#include<queue>

#define EPS 1e-9

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<pair<int,int>> vpi;
typedef pair<int, int> pii;

bool cmp(const pii& a,const pii& b){
    if(a.first == b.first)
    {
        return a.second < b.second;
    }
    return a.first > b.first;
}

int main()
{
    int cases;
    scanf("%d", &cases);
    for(int i=0; i<cases; ++i){
        int n, p, sum;
        sum = 0;
        vi a;
        vpi remain;
        bool impossible=false;
        scanf("%d %d", &p, &n);
        for(int j=0; j<n; ++j)
        {   
            int tmp;
            scanf("%d", &tmp);
            sum += tmp;
            a.push_back(0);
            remain.push_back(pii(tmp, j));
            // cout << "n: " << n << " p: " << p << endl;
        }
        if(sum < p)
        {
            impossible = true;
        }
        sort(remain.begin(), remain.end(), cmp);
        while(p > 0 && !impossible)
        {
            int needed_beat = p / remain.size(); int rmd = p % remain.size();
            int next_beat = remain.back().first; int idx = remain.back().second;
            if(needed_beat < next_beat)
            {
                next_beat = needed_beat;
            }
            if(needed_beat == 0)
            {
                next_beat = 1;
            }

            // cout << "beat: " << next_beat << endl;
            for(auto& payer : remain)
            {
                p -= next_beat;
                pii new_value = pii(payer.first - next_beat, payer.second);
                payer = new_value;
                a[payer.second] += next_beat;

                if(p == 0)
                {
                    break;
                }
            }
            // remove all empty handed
            for(auto j = remain.rbegin(); remain.back().first == 0; ++j)
            {
                remain.pop_back();
            }
        }

        if(impossible)
        {
            cout << "IMPOSSIBLE" << endl;
            // cout << "diff: " << diff << endl;
            continue;
        }
        for(int k=0; k<n-1; ++k)
        {
            cout << a[k] << " ";
        }
        cout << a[n-1] << endl;
    }
    // cout << endl;
    return 0;
}