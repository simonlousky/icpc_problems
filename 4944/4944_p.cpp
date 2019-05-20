#include<vector>
#include<iostream>
#include<stdio.h>
#include<set>
#include<functional>
#include<algorithm>
#include<queue>
#include<tuple>

#define EPS 1e-9

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;
typedef vector<pair<int,int>> vpi;
typedef pair<int, int> pii;
typedef tuple<int, int, int> tiii;

struct cmp{
  bool operator()(const tiii& a,const tiii& b) const{
      // 0 - diff
      // 1 - remaining cash
      // 2 - idx
      if(get<0>(a) == get<0>(b) && get<1>(a) == get<1>(b))
      {
          return get<2>(a) > get<2>(b);
      }
      if(get<0>(a) == get<0>(b))
      {
          return get<1>(a) < get<1>(b);
      }
      return get<0>(a) > get<0>(b);
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
        priority_queue<tiii, vector<tiii>, cmp>rich;
        priority_queue<pii> tafran;
        bool impossible=false;
        scanf("%d %d", &p, &n);
        // initial duty of everybody
        part = p / n;
        sum = part * n;
        for(int j=0; j<n; ++j)
        {   
            int tmp;
            scanf("%d", &tmp);
            a.push_back(part);
            if(tmp-part >0)
            {
                rich.push(make_tuple(0, tmp-part, j));
            }
            if(tmp-part < 0)
            {
                tafran.push(pii(abs(tmp-part), j));
            }
        }
        
        // if part * n is less than the price make the richer pay
        while(sum < p)
        {
            // cout << "sum: " << sum << ", p:" << p << endl;
            tiii r = rich.top(); rich.pop();
            int r_diff = get<0>(r); int r_value = get<1>(r); int r_idx = get<2>(r);

            sum++;
            a[r_idx] += 1;
            r_value -= 1;

            if(r_value > 0)
            rich.push(make_tuple(r_diff+1, r_value, r_idx));

            if(sum < p && rich.empty())
            {
                impossible = true;
                break;
            }
        }
        // for (auto u: a)
        //     cout << u << ' ';

        // iterate until no tafran owes money
        while(tafran.size() && !impossible)
        {
            if(rich.empty())
            {
                impossible = true;
                break;
            }
            pii t = tafran.top(); tafran.pop();
            int t_value = t.first; int t_idx = t.second;

            tiii r = rich.top(); rich.pop();
            int r_diff = get<0>(r); int r_value = get<1>(r); int r_idx = get<2>(r);

            int additional = t_value / rich.size();
            while( )
            a[t_idx] -= 1;
            a[r_idx] += 1;
            r_diff = a[r_idx] - a[t_idx];
            r_value -= 1;
            t_value -= 1;

            if(r_value > 0)
            {
                rich.push(make_tuple(r_diff, r_value, r_idx));
            }
            if(t_value > 0)
            {
                tafran.push(pii(t_value, t_idx));
            }
        }
        int diff = *max_element(a.begin(), a.end()) - *min_element(a.begin(), a.end());
        // printf("diff: %d\n", *min_element(a.begin(), a.end()));
        if(impossible /*|| diff > part || diff < 0 */)
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
    cout << endl;
    return 0;
}