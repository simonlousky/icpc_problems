#include<vector>
#include<iostream>
#include<stdio.h>
#include<set>
#include<algorithm>
#include<math.h>
#include<tuple>
#include<queue>
#include<stack>

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int,int> ii;
typedef vector<ii> vii;
typedef vector<vii> vvii;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef set<int> si;
typedef vector<si> vsi;

const int INF = 1e9;
#define EPS 1e-9
#define N_MAX 101

/*
 * Input graphs should be represented as an adjacency list
 * where the nodes are named 0,1,...,|V|-1.
 * If g is unweighted (type vvi), g[u] contains the neighbors of u
 * If g is weighted (type vvii), g[u] contains pairs (v,w) such that u->v has weight w
 */

/********** Dijkstra **********/

// input: digraph (g), source (s)
// output: distances from s (dist)
void Dijkstra(const vector<vii>& g, int s, vi& dist) {
    dist = vi(g.size(), INF);
    dist[s] = 0;
    priority_queue<ii, vii, greater<ii>> q;
    q.push({0, s});
    while (!q.empty()) {
        ii front = q.top(); q.pop();
        int d = front.first, u = front.second;
        if (d > dist[u]) continue; // We may have found a shorter way to get to u after inserting it to q.
        // In that case, we want to ignore the previous insertion to q.
        for (ii next : g[u]) {
            int v = next.first, w = next.second;
            if (dist[u]+w < dist[v]) {
                dist[v] = dist[u]+w;
                q.push({dist[v], v});
            }
        }
    }
}

int main()
{
    int cases;
    bool bad = false;
    scanf("%d", &cases);
    for(int i=0; i<cases; ++i){
        ll N,E,T,M;
        vvii g(N_MAX);
        vi dist(N_MAX);

        scanf("%lld %lld %lld %lld", &N, &E, &T, &M);
        for (int i=0; i<M; i++){
            ll a,b,t;
            scanf("%lld %lld %lld", &a, &b, &t);
            // build graph with inverted directions
            g[b].push_back(ii(a, t));
        }
        // find distances from E
        Dijkstra(g, E, dist);
        int mouses = 0;
        for(int i=0; i<N_MAX; i++)
        {
            if(dist[i] <= T)
            {
                mouses++;
            }
        }
        cout << mouses << endl;
        if(i+1<cases)
            cout << endl;
    }

    return 0;
}