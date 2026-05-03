#include <iostream>
#include <string>
using namespace std;

const int maxn = 30;
const int start_x = 1;
const int start_ly = 1;



string get_ans(int st, int ly, int mx){
    if(ly > mx)
        return "";
    if(ly == mx)
        return to_string(st);
    string res = "";
    res += to_string(st) + '+';
    res += "\\frac{";
    res += get_ans(st * 2, ly + 1, mx);
    res += '}';
    res += "{" + get_ans(st * 2 + 1, ly + 1, mx) + "}";
    return res;
}
int main(){
    int n;
    cin >> n;
    cout << get_ans(start_x, start_ly, n) << "\n";


}
