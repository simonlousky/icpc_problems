#include<vector>
#include<iostream>
#include<stdio.h>
#include<algorithm>
#include<numeric>

using namespace std;
#define CHESS_SIZE 8

typedef vector<int> vi;
typedef long long ll;
vi res_print(CHESS_SIZE);
int sln = 1;

void print_result()
{
    printf("%2d", sln++);
    cout << "     ";
    for (int i=0; i<res_print.size(); i++)
        cout << " " << res_print[i];
    cout << endl;
}

bool impossible(vi& res, int row)
{
    int current_column = res.front();
    for(int i=0; i<CHESS_SIZE; i++)
    {
        if(res_print[i] == 0)
            continue;
        int deltaRow = abs(row - res_print[i] + 1);
        int deltaCol = abs(current_column - i);
        // check slope
        if (deltaRow == deltaCol){
            // print_result();
            // cout << "current checking: " << res_print[i] << "," << i + 1 << endl;
            // cout << "proposed: " << row+1 <<"," << current_column +1 << endl;
            return true;
        }
    }

    return false;
}

void check(vi& res, vi& chess)
{
    if (chess.empty())
    {
        print_result();
        return;
    }
    for(int i=0; i<chess.size(); i++)
    {
        if(impossible(res, chess[i]))
            continue;
        int row = chess[i];
        int column = res.front();
        res_print[column] = row + 1;
        chess.erase(chess.begin() + i);
        res.erase(res.begin());
        check(res, chess);
        chess.insert(chess.begin() + i, row);
        res.insert(res.begin(), column);
        res_print[column] = 0;
    }
}

int main()
{
    ll t;    
    scanf("%lld", &t);
    while(t--)
    {

        vi chess(CHESS_SIZE);
        vi res(CHESS_SIZE);

        int r, c;
        
        iota(res.begin(), res.end(), 0);
        iota(chess.begin(), chess.end(), 0);
        fill(res_print.begin(), res_print.end(), 0);
        scanf("%d %d", &r, &c);
        c--;

        res_print[res[c]] = r;
        res.erase(res.begin() + c);
        chess.erase(chess.begin() + r - 1);

        cout << "SOLN       COLUMN" << endl;
        cout << " #      1 2 3 4 5 6 7 8" << endl << endl;
        check(res, chess);
        if(t)
            cout << endl;
        sln=1;
    }


    return 0;
}