#include <iostream>
#include <string> 
#include <vector>
#include <algorithm>
using namespace std;


void get_all_possible(int loc, int n, int x, int l, int r, vector <pair<string, int>> foods, vector <pair<string, int>> cur_foods, long double sum, vector <pair<string, int>>&ans, long double &lsum){
    if(loc == n){
        sum = sum * 11 / 10;
        if(sum < l)
            return;
        if(sum * ( x) / 100 > r)
            return; 
        if(sum > lsum){
            ans.clear();
            ans = cur_foods;
            lsum = sum;
        }
        return;

    }
    get_all_possible(loc + 1, n, x, l, r, foods, cur_foods, sum, ans, lsum);
    cur_foods.push_back(foods[loc]);
    get_all_possible(loc + 1, n, x, l, r, foods, cur_foods, sum + foods[loc].second, ans, lsum);
    cur_foods.pop_back();



}
int main(){
    int x, l, r, n, food_price;
    vector <pair<string, int>> foods, cur_foods, ans;
    string food_name;
    cin >> n >> x >> l >> r;
    for(int i = 0; i < n; i++){
        cin >> food_name >> food_price;
        
        foods.push_back(make_pair(food_name, food_price));
    }
    long double foods_sum = 0;
    get_all_possible(0, n, x, l, r, foods, cur_foods, 0, ans, foods_sum);
    for(auto i: ans)   
        cout << i.first << endl;


}
