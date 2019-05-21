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
#define N_MAX 20

/*
 * Input graphs should be represented as an adjacency list
 * where the nodes are named 0,1,...,|V|-1.
 * If g is unweighted (type vvi), g[u] contains the neighbors of u
 * If g is weighted (type vvii), g[u] contains pairs (v,w) such that u->v has weight w
 */

/********** topological sort **********/

// input: a graph (g)
// output: is g a DAG (return value), a topological ordering of g (order)
// comment: order is valid only if g is a DAG
bool topological_sort(const vvi& g, vi& order) {
    // compute indegree of all nodes
    vi indegree (g.size(), 0);
    for (int v=0; v<g.size(); v++) for (int u : g[v]) indegree[u]++;
    // order sources first
    order = vector<int>();
    for (int v=0; v<g.size(); v++) if (indegree[v] == 0) order.push_back(v);
    // go over the ordered nodes and remove outgoing edges,
    // add new sources to the ordering
    for (int i=0; i<order.size(); i++) for (int u : g[order[i]]) {
        indegree[u]--;
        if (indegree[u]==0) order.push_back(u);
    }
    return order.size()==g.size();
}


int main()
{
    char entry[N_MAX];
    vvi g('Z' - 'A' + 1);
    vi order;
    vector<string> index;
    while (scanf("%s", &entry))
    {
        // printf("%s\n", entry);
        string tmp(entry);
        if (tmp == "#"){
            break;
        }
        index.push_back(tmp);
    }
    // cout << "exited loop" << endl;
    for(int i=0; i<N_MAX; i++)
    {
        // cout << "index size: " << index.size() << endl;
        for(int j=1; j<index.size(); j++) // start at j=1 and check j-1 with j
        {
            if(index[j-1].length() <= i || index[j].length() <= i)
            {
                continue;
            }
            if(i > 0 && index[j-1][i-1] != index[j][i-1])
            {
                continue;
            }

            if(index[j-1][i] != index[j][i])
            {
                int idx_1 = int(index[j-1][i]) - 'A';
                int idx_2 = int(index[j][i]) - 'A';
                // add edge X->Z if not the same letter (use asci value)
                g[idx_1].push_back(idx_2);
            }
        }
    }
    topological_sort(g, order);
    char tmp;
    int k=0;
    for(int i=0; i<order.size(); i++)
    {
        if(g[order[i]].size())
        {
            k = i;
            break;
        }
    }
    for(int i=k; i<order.size();i++)
    {
        tmp = (char)(order[i] + 'A');
        cout << tmp;
    }

    return 0;
}