#include <bits/stdc++.h>

using namespace std;

int main(){
    string a = "02";
    int month = stoi(a); // Chuyển đổi chuỗi thành số nguyên
    if (month == 2){
        cout << "Thang 2" << endl;
    } else {
        cout << "Khong phai thang 2" << endl;
    }
    cout << a << endl;
    cout << month + 8 << endl;
    return 0;
}