#include<vector>
#include<iostream>
#include<stdio.h>
#include<set>
#include<algorithm>

#define EPS 1e-9

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<pair<int,int>> vpi;
typedef pair<int, int> pii;

struct greater1{
  bool operator()(const pii& a,const pii& b) const{
      if(a.first == b.first)
      {
          return a.second > b.second;
      }
      return a.first < b.first;
  }
};

int main()
{
    int cases;
    scanf("%d", &cases);
    
    for(int i=0; i<cases; ++i){
        ll p, n, part, sum;
        sum = 0;
        vi a;
        vpi remain;
        vpi tafran;
        bool impossible=false;
        scanf("%d %d", &p, &n);
        part = p / n;
        sum = part * n;
        for(int j=0; j<n; ++j)
        {   
            int tmp;
            scanf("%d", &tmp);
            a.push_back(part);
            remain.push_back(pii(tmp-part, j));
            if(tmp-part < 0)
            {
                tafran.push_back(pii(abs(tmp-part), j));
            }
        }
        make_heap(remain.begin(), remain.end(), greater1());
        make_heap(tafran.begin(), tafran.end(), greater1());
        
        while(sum < p)
        {
            cout << "sum: " << sum << ", p:" << p << endl;
            pii r = remain.front();
            pop_heap(remain.begin(), remain.end(), greater1());
            remain.pop_back();
            int r_idx = r.second;
            int r_value = r.first;

            r_value -= 1;
            sum++;

            remain.push_back(pii(r_value, r_idx));
            push_heap(remain.begin(), remain.end(), greater1());
            a[r_idx] += 1;

            if(r_value < 0)
            {
                impossible=true;
            }
        }
        for (auto u: a)
            cout << u << ' ';
        while(tafran.size() && !impossible)
        {

            pii t = tafran.front();
            pop_heap(tafran.begin(), tafran.end(), greater1());
            tafran.pop_back();
            int t_idx = t.second;
            int t_value = t.first;

            pii r = remain.front();
            pop_heap(remain.begin(), remain.end(), greater1());
            remain.pop_back();
            int r_idx = r.second;
            int r_value = r.first;

            r_value -= 1;
            t_value -= 1;
            a[t_idx] -= 1;
            a[r_idx] += 1;


            if(r_value >= 0)
            {
                remain.push_back(pii(r_value, r_idx));
                push_heap(remain.begin(), remain.end(), greater1());
            }
            if(t_value != 0)
            {
                tafran.push_back(pii(t_value, t_idx));
                push_heap(tafran.begin(), tafran.end(), greater1());
            }
            // for (auto u: a)
            //     cout << u << ' ';
            // cout << endl;
            // for (auto u: tafran)
            //     cout << u.first << ' ';
            // cout << endl;
            // for (auto u: remain)
            //     cout << u.second << ' ';
            // cout << endl;
        }
        int diff = *max_element(a.begin(), a.end()) - *min_element(a.begin(), a.end());
        // printf("diff: %d\n", *min_element(a.begin(), a.end()));
        if(impossible || diff > part || diff < 0)
        {
            cout << "IMPOSSIBLE" << endl;
            cout << "diff: " << diff << endl;
            // continue;
        }
        for(int k=0; k<n-1; ++k)
        {
            cout << a[k] << " ";
        }
        cout << a[n-1] << endl;
    }
    cout << endl;
    return 0;
}