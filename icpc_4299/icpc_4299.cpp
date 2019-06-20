#include<iostream>
#include<vector>

using namespace std;
#define MAX_M 10005
#define MAX_DISTANCE 1000
typedef vector<int> vi;
typedef vector<vi> vvi;

/* Floyd-Warshal */
void floyd(const vvi& g, vvi& d){
    for(int k=0; k<g.size(); k++){
        for(int u=0; u<g.size(); u++){
            for(int v=0; v<g.size(); v++){
                d[u][v] = min(d[u][v], d[u][k]+d[k][v]);
            }
        }
    }
}

int main(){
    int T;
    scanf("%d", &T);
    for (int t = 0; t < T; t++){
        int N, R;
        vvi d, g;
        scanf("%d %d", &N, &R);
        for(int i=0; i<N; i++){
            string connections;
            cin >> connections;

            d.push_back(vi());
            g.push_back(vi());
            for(int j=0; j<N; j++){
                int w = connections[j] == 'N' ? MAX_DISTANCE : 1;
                
                if(i == j){
                    g[i].push_back(0);
                    d[i].push_back(0);
                }else{
                    g[i].push_back(w);
                    d[i].push_back(w);
                }
            }
        }
        floyd(d, d);

        for (auto a : d){
            for (auto b: a){
                cout << b << " ";
            }
            cout << endl;
        }
    }

    return 0;
}