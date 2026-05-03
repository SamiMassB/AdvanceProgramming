#include <bits/stdc++.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;
const int maxn = 20 + 12;
int check_val(int n, int m, int i, int j){

    if(i < 0 || i >= n)
        return 0;
    if(j < 0 || j >= m)
        return 0;
    return 1;
}
int dfs(int i, int j, int n, int m, char cur_char, int marked[maxn][maxn], char jadval[maxn][maxn]){
    marked[i][j] = 1;
    int dx[4] = {0, 0, -1, 1};
    int dy[4] = {1, -1, 0, 0};
    int sum = 0;
    for(int c = 0; c < 4; c++){
        int nx, ny;
        nx = i + dx[c];
        ny = j + dy[c];
        if(!check_val(n, m, nx, ny))
            continue;
        if(marked[nx][ny])
            continue;    
        if(jadval[i][j] == cur_char && (jadval[nx][ny] != cur_char && jadval[nx][ny] != '%'))
            continue;
        if(jadval[i][j] == '~' && jadval[nx][ny] != '~' && jadval[nx][ny] != '%')
            continue;
      
        if(jadval[i][j] == '%' && jadval[nx][ny] != '~' && jadval[nx][ny] != '%' && jadval[nx][ny] != cur_char)
            continue;
        
        sum += dfs(nx, ny, n, m, cur_char, marked, jadval);
    }
    return sum + (jadval[i][j] == cur_char);
}

int main(){
    int n, m, ans[10], marked[maxn][maxn];
    char jadval[maxn][maxn];
    cin >> n >> m;
    for(int i = 0; i < 10; i++)
        ans[i] = 0;

    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            cin >> jadval[i][j];
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++){
            if(jadval[i][j] < '0' || jadval[i][j] > '9')
                continue;
            int cur_ans = dfs(i, j, n, m, jadval[i][j], marked, jadval);
            ans[jadval[i][j] - '0'] = max(ans[jadval[i][j] - '0'], cur_ans);
            for(int i = 0; i < n; i++)
                for(int j = 0; j < m; j++)
                    if(jadval[i][j] < '0' || jadval[i][j] > '9')
                        marked[i][j] = 0;
                    }
    for(int i = 0; i < 10; i++)
        cout << ans[i] << "\n";

}
