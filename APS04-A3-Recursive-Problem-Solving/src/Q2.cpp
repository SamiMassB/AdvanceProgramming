#include <bits/stdc++.h>
using namespace std;

const int maxn = 100 + 12;
const int INV_RES = -1;
void get_data(int &n, int Ti[maxn], int age[maxn]){
    cin >> n;
    for(int i = 0; i <n ; i++)
        cin >> age[i];
    for(int i = 0; i < n; i++)
        cin >> Ti[i];

}
int find_lower_right(int loc, int n, int age[maxn]){
    int res = INV_RES, start;  
    start = loc;
    loc += 1;
    if(loc == n)
        return INV_RES;
    
    while(loc < n && age[loc] != 0){
        if(age[start] > age[loc] && (age[loc] > age[res] || res == INV_RES))
            res = loc;
        loc += 1;
    }
    if(res == INV_RES)
        return INV_RES;
    return res;
}
int find_lower_left(int loc, int n, int age[maxn]){
    int res = INV_RES, start;  
    start = loc;    
    loc -= 1;
    if(loc == -1)
        return INV_RES;

    while(loc >= 0 && age[loc] != 0){
        if(age[start] > age[loc] && (age[loc] > age[res] || res == INV_RES))
            res = loc;
        loc -= 1;

    }

    if(res == INV_RES)
        return INV_RES;
    return res;
}
void solve(int loc, int n, int Ti[maxn], int age[maxn], int sum[maxn]){
    int left_low, right_low;
    left_low =  find_lower_left(loc, n, age);
    

    right_low = find_lower_right(loc, n, age);  
    age[loc] = 0;    
    if(left_low == INV_RES && right_low == INV_RES){
        sum[loc] += Ti[loc];
        return;
    }
    else if(left_low == INV_RES){
        sum[right_low] += Ti[loc];
        solve(right_low, n, Ti, age, sum);
        return;
    }
    else if(right_low == INV_RES){
        sum[left_low] += Ti[loc];
        solve(left_low, n, Ti, age, sum);
        return;


    }
    else if(right_low == left_low){
        sum[left_low] += Ti[loc];
        solve(left_low, n, Ti, age, sum);
        return;


    }
    sum[left_low] += Ti[loc] / 2;
    sum[right_low] += Ti[loc] / 2;
    solve(right_low, n, Ti, age, sum);
    solve(left_low, n, Ti, age, sum);
}
void prepare(int n, int &loc, int age[maxn], int sum[maxn]){
    loc = 0;
    for(int i = 0; i < n; i++)
        if(age[loc] < age[i])
            loc = i;
    for(int i = 0; i < n; i++)
        sum[i] = 0;
    

}
void print_result(int n, int sum[maxn]){
    for(int i = 0; i < n; i++)
        cout << sum[i] << ' ';
    cout << endl;
}
int main(){
    int n, loc, Ti[maxn], sum[maxn], age[maxn];
    get_data(n, Ti, age);
    prepare(n, loc, age, sum);
    solve(loc, n, Ti, age, sum);
    print_result(n, sum);
}
