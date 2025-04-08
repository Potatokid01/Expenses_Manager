#include <bits/stdc++.h>

using namespace std;

const string EXPENSE_TYPES[] = {"An uong", "Di chuyen", "Nha cua", "Nhu yeu pham", "Dich vu"};

struct Expense {
    string type;
    string description;
    double amount;
    string date;
};

struct MonthlyExpenses {
    string monthYear; // Tháng/năm (mm/yyyy)
    vector<Expense> expenses; // Danh sách chi tiêu trong tháng
};

string getCurrentDate();
void clearInputBuffer();
void addExpense(vector<Expense>& expenses);
void displayExpenses(const vector<Expense>& expenses);
void deleteExpense(vector<Expense>& expenses);
void displayStatistics(const vector<Expense>& expenses);
void displayMainMenu();
void displayExpensesByMonth(const vector<Expense>& expenses);
bool cmpEBM(const pair<string, vector<Expense>>& a,const pair<string, vector<Expense>>& b);

int main(){
    vector<Expense> expenses;
    int choice;

    do {
        displayMainMenu();
        cin >> choice;
        if (cin.fail()) {
            cout << "Lua chon khong hop le! Vui long nhap lai.\n";
            cin.clear(); clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1:
                addExpense(expenses);
                break;
            case 2:
                displayExpenses(expenses);
                break;
            case 3:
                displayStatistics(expenses);
                break;
            case 4:
                deleteExpense(expenses);
                break;
            case 5:
                displayExpensesByMonth(expenses);
                break;
            case 6:
                cout << "Da luu va thoat chuong trinh!\n";
                return 0;
            default:
                cout << "Lua chon khong hop le! Vui long nhap lai.\n";
        }
    } while(choice != 6);

    return 0;
}

void displayMainMenu() { //Hiển thị menu chính
    cout << "\n=== QUAN LY CHI TIEU CA NHAN ===\n";
    cout << "1. Them chi tieu moi\n";
    cout << "2. Xem danh sach chi tieu\n";
    cout << "3. Xem thong ke chi tieu\n";
    cout << "4. Xoa chi tieu\n";
    cout << "5. Xem chi tieu theo thang\n";
    cout << "6. Luu va thoat chuong trinh\n";
    cout << "Lua chon: ";
}

string getCurrentDate() { //Lấy ngày hiện tại
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", ltm);
    return string(buffer);
}

void clearInputBuffer(){ //Xóa bộ đệm đầu vào
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); //Xoá các kí tự còn lại trong bộ đệm đầu vào
}

void addExpense(vector<Expense>& expenses) { //Thêm chi tiêu mới
    Expense newExpense;
    int typeChoice;

    cout << "\n--- THEM CHI TIEU MOI ---\n";
    cout << "Chon loai chi tieu:\n";
    for (int i = 0; i < 5; i++) cout << i + 1 << ". " << EXPENSE_TYPES[i] << endl;
    
    InputExpenseType:
        cout << "Lua chon: ";
        if (!(cin>>typeChoice) || typeChoice < 1 || typeChoice > 5) {
            cout << "Lua chon khong hop le! Vui long nhap lai.\n";
            cin.clear();
            clearInputBuffer();
        }
    clearInputBuffer();

    newExpense.type = EXPENSE_TYPES[typeChoice - 1];
    
    cout << "Noi dung chi tieu: ";
    getline(cin, newExpense.description);
    
    InputSpendingAmount: //Nhập số tiền chi tiêu
        cout << "So tien (VND): ";

        if (!(cin >> newExpense.amount) || newExpense.amount < 0) {
            cout << "Gia tri khong hop le! Vui long nhap lai.\n";
            cin.clear();
            clearInputBuffer();
            goto InputSpendingAmount;
        }
    clearInputBuffer();
    
    cout << "Ngay chi tieu. Neu de mac dinh nhan ENTER!" << " (mac dinh: " << getCurrentDate() << "): "; //Nhập ngày chi tiêu
    string dateInput;
    getline(cin,dateInput);
    newExpense.date = dateInput.empty() ? getCurrentDate() :  dateInput;// Nếu không nhập thì lấy ngày hiện tại

    expenses.push_back(newExpense); // Thêm chi tiêu vào danh sách expenses
    cout << "Da them chi tieu thanh cong!\n";
    newExpense = Expense(); // Khởi tạo lại biến mới để tránh lỗi khi nhập liệu tiếp theo
}

void displayExpenses(const vector<Expense>& expenses) { //Hiển thị danh sách chi tiêu theo thứ tự nhập vào
    if (expenses.empty()) {
        cout << "\nDanh sach chi tieu trong!\n";
        return;
    }
    
cout << "\n==================== DANH SACH CHI TIEU ====================\n";
cout << "+------+------------------+--------------------------------+---------------+------------+\n";
cout << "| STT  |      Loai        |            Noi dung            | So tien (VND) |    Ngay    |\n";
cout << "+------+------------------+--------------------------------+---------------+------------+\n";

for (size_t i = 0; i < expenses.size(); i++) { //Duyệt qua từng chi tiêu trong danh sách expenses
    cout << "| "
         << setw(4) << right << i + 1 << " | "
         << setw(16) << left << expenses[i].type << " | "
         << setw(30) << left << expenses[i].description << " | "
         << setw(13) << right << fixed << setprecision(0) << expenses[i].amount << " | "
         << setw(10) << left << expenses[i].date << " |\n";
}

cout << "+------+------------------+--------------------------------+---------------+------------+\n";
}

void displayStatistics(const vector<Expense>& expenses) { //Hiển thị thống kê chi tiêu theo loại đã nhập (không theo tháng)
    if (expenses.empty()) {
        cout << "\nKhong co du lieu de thong ke!\n";
        return;
    }
    
    double totalAmount = 0;
    map<string, double> typeAmounts; //Khởi tạo map để lưu trữ số tiền chi tiêu theo từng loại
    for (int i = 0; i < sizeof(EXPENSE_TYPES) / sizeof(EXPENSE_TYPES[0]); i++) { //Số byte tổng của ds loại chi tiêu chia cho số byte của từng loại chi tiêu = số phần tử
        typeAmounts[EXPENSE_TYPES[i]] = 0;                                       //Khởi tạo giá trị cho từng loại chi tiêu
    }
    
    //
    for (size_t i = 0; i < sizeof(EXPENSE_TYPES) / sizeof(EXPENSE_TYPES[0]); i++) { //Tính tổng từng loại chi tiêu
        totalAmount += expenses[i].amount;
        typeAmounts[expenses[i].type] += expenses[i].amount;
    }
    
    cout << "\n--- THONG KE CHI TIEU ---\n";
    cout << "Tong tien: " << fixed << setprecision(0) << totalAmount << " VND\n\n"; //Hiển thị tổng số tiền chi tiêu
    
    for (int i = 0; i < 5; i++) { //Duyệt qua từng loại chi tiêu
        if (typeAmounts[EXPENSE_TYPES[i]] > 0) {
            double percent = (typeAmounts[EXPENSE_TYPES[i]] / totalAmount) * 100; //tinh toan phan tram so tien chi trong muc voi tong so tien chi
            cout << setw(20) << left << EXPENSE_TYPES[i] << ": " << fixed << setprecision(2) << percent << "%\n"; //Lam tron phan tram 2 chu so thap phan
        }
    }
};

void deleteExpense(vector<Expense>& expenses){ //Xóa chi tiêu theo STT đã nhập
    if (expenses.empty()) {
        cout << "\nKhong co du lieu de xoa!\n";
        return;
    }

    displayExpenses(expenses);

    int index;

    InputDeleteIndex:
        cout << "Nhap STT chi tieu muon xoa: ";
        if (!(cin >> index) || index < 0 || index > expenses.size()) { //Kiểm tra STT có hợp lệ không
            cout << "STT khong hop le! Vui long nhap lai. Hoac muon thoat nhan 0!\n";
            cin.clear();
            clearInputBuffer();
            goto InputDeleteIndex;
        }
    clearInputBuffer();

    if (index == 0) {
        cout << "Da huy xoa chi tieu!\n";
        return;
    }
    expenses.erase(expenses.begin() + (index - 1)); //Xoa chi tieu tai vi tri index - 1
    cout << "Da xoa chi tieu thanh cong!\n";
}

bool cmpEBM(const pair<string, vector<Expense>>& a, const pair<string, vector<Expense>>& b) { //So sánh hai cặp (tháng/năm, danh sách chi tiêu)
    int yearA = stoi(a.first.substr(3, 4)); // Lấy năm từ chuỗi "mm/yyyy" -> chuyển từ string sang int
    int yearB = stoi(b.first.substr(3, 4)); // Lấy năm từ chuỗi "mm/yyyy" -> chuyển từ string sang int
    if (yearA == yearB) {
        int monthA = stoi(a.first.substr(0, 2)); // Lấy tháng từ chuỗi "mm/yyyy" -> chuyển từ string sang int
        int monthB = stoi(b.first.substr(0, 2)); // Lấy tháng từ chuỗi "mm/yyyy" -> chuyển từ string sang int
        return monthA < monthB;
    }
    return yearA < yearB;
}

void displayExpensesByMonth(const vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "\nDanh sach chi tieu trong!\n";
        return;
    }

    // Sử dụng map để nhóm các chi tiêu theo tháng/năm
    map<string, vector<Expense>> groupedExpenses; 

    for (size_t i = 0; i < expenses.size(); i++) { //Duyệt qua từng chi tiêu trong danh sách expenses
        string monthYear = expenses[i].date.substr(3, 7); // Lấy "mm/yyyy"
        groupedExpenses[monthYear].push_back(expenses[i]);
    }

    // Chuyển map thành vector để sắp xếp theo thứ tự tăng dần của năm/tháng
    vector<pair<string, vector<Expense>>> sortedExpenses(groupedExpenses.begin(), groupedExpenses.end());
    sort(sortedExpenses.begin(), sortedExpenses.end(), cmpEBM); // Sắp xếp theo năm/tháng};

    // Hiển thị danh sách chi tiêu theo từng tháng
    for (size_t i = 0; i < sortedExpenses.size(); i++) {
        cout << "\n==================== CHI TIEU THANG " << sortedExpenses[i].first << " ====================\n";
        cout << "+------+------------------+--------------------------------+---------------+------------+\n";
        cout << "| STT  |      Loai        |            Noi dung            | So tien (VND) |    Ngay    |\n";
        cout << "+------+------------------+--------------------------------+---------------+------------+\n";

        const vector<Expense>& expensesInMonth = sortedExpenses[i].second; // Lấy danh sách chi tiêu trong tháng
        for (size_t i = 0; i < expensesInMonth.size(); i++) {
            const auto& expense = expensesInMonth[i];
            cout << "| "
                 << setw(4) << right << i + 1 << " | "
                 << setw(16) << left << expense.type << " | "
                 << setw(30) << left << expense.description << " | "
                 << setw(13) << right << fixed << setprecision(0) << expense.amount << " | "
                 << setw(10) << left << expense.date << " |\n";
        }

        cout << "+------+------------------+--------------------------------+---------------+------------+\n";
    }
}

