#include <bits/stdc++.h>

using namespace std;

const string EXPENSE_TYPES[] = {"An uong", "Di chuyen", "Nha cua", "Xe co", "Nhu yeu pham", "Dich vu"};

struct Expense {
    string type;
    string description;
    double amount;
    string date;
};

string getCurrentDate();
void clearInputBuffer();
void addExpense(vector<Expense>& expenses);
void displayExpenses(const vector<Expense>& expenses);
void deleteExpense(vector<Expense>& expenses);
void displayStatistics(const vector<Expense>& expenses);
void displayMainMenu();

struct MonthlyExpenses {
    string monthYear; // Tháng/năm (mm/yyyy)
    vector<Expense> expenses; // Danh sách chi tiêu trong tháng
};

void displayExpensesByMonth(const vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "\nDanh sach chi tieu trong!\n";
        return;
    }

    // Sử dụng map để nhóm các chi tiêu theo tháng/năm
    map<string, vector<Expense>> groupedExpenses;

    for (const auto& expense : expenses) {
        string monthYear = expense.date.substr(3, 7); // Lấy "mm/yyyy"
        groupedExpenses[monthYear].push_back(expense);
    }

    // Chuyển map thành vector để sắp xếp theo thứ tự tăng dần của năm/tháng
    vector<pair<string, vector<Expense>>> sortedExpenses(groupedExpenses.begin(), groupedExpenses.end());
    sort(sortedExpenses.begin(), sortedExpenses.end(), [](const pair<string, vector<Expense>>& a, const pair<string, vector<Expense>>& b) {
        int yearA = stoi(a.first.substr(3, 4));
        int yearB = stoi(b.first.substr(3, 4));
        if (yearA == yearB) {
            int monthA = stoi(a.first.substr(0, 2));
            int monthB = stoi(b.first.substr(0, 2));
            return monthA < monthB;
        }
        return yearA < yearB;
    });

    // Hiển thị danh sách chi tiêu theo từng tháng
    for (const auto& group : sortedExpenses) {
        cout << "\n==================== CHI TIEU THANG " << group.first << " ====================\n";
        cout << "+------+------------------+--------------------------------+---------------+------------+\n";
        cout << "| STT  |      Loai        |            Noi dung            | So tien (VND) |    Ngay    |\n";
        cout << "+------+------------------+--------------------------------+---------------+------------+\n";

        const auto& expensesInMonth = group.second;
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

void displayMainMenu() {
    cout << "\n=== QUAN LY CHI TIEU CA NHAN ===\n";
    cout << "1. Them chi tieu moi\n";
    cout << "2. Xem danh sach chi tieu\n";
    cout << "3. Xem thong ke chi tieu\n";
    cout << "4. Xoa chi tieu\n";
    cout << "5. Xem chi tieu theo thang\n";
    cout << "6. Luu va thoat chuong trinh\n";
    cout << "Lua chon: ";
}

string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", ltm);
    return string(buffer);
}

void clearInputBuffer(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void addExpense(vector<Expense>& expenses) {
    Expense newExpense;
    int typeChoice;

    cout << "\n--- THEM CHI TIEU MOI ---\n";
    cout << "Chon loai chi tieu:\n";
    for (int i = 0; i < 6; i++) cout << i + 1 << ". " << EXPENSE_TYPES[i] << endl;
    
    InputExpenseType:
        cout << "Lua chon: ";
        if (!(cin>>typeChoice) || typeChoice < 1 || typeChoice > 6) {
            cout << "Lua chon khong hop le! Vui long nhap lai.\n";
            cin.clear();
            clearInputBuffer();
        }
    clearInputBuffer();

    newExpense.type = EXPENSE_TYPES[typeChoice - 1];
    
    cout << "Noi dung chi tieu: ";
    getline(cin, newExpense.description);
    
    InputSpendingAmount:
        cout << "So tien (VND): ";

        if (!(cin >> newExpense.amount) || newExpense.amount < 0) {
            cout << "Gia tri khong hop le! Vui long nhap lai.\n";
            cin.clear();
            clearInputBuffer();
            goto InputSpendingAmount;
        }
    clearInputBuffer();
    
    cout << "Ngay chi tieu: " << " (mac dinh): "<< endl;
    getline(cin, newExpense.date);
    // newExpense.date = getCurrentDate();
    clearInputBuffer();
    
    expenses.push_back(newExpense);
    cout << "Da them chi tieu thanh cong!\n";
    newExpense = Expense(); // Khởi tạo lại biến mới để tránh lỗi khi nhập liệu tiếp theo
}

void displayExpenses(const vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "\nDanh sach chi tieu trong!\n";
        return;
    }
    
cout << "\n==================== DANH SACH CHI TIEU ====================\n";
cout << "+------+------------------+--------------------------------+---------------+------------+\n";
cout << "| STT  |      Loai        |            Noi dung            | So tien (VND) |    Ngay    |\n";
cout << "+------+------------------+--------------------------------+---------------+------------+\n";

for (size_t i = 0; i < expenses.size(); i++) {
    cout << "| "
         << setw(4) << right << i + 1 << " | "
         << setw(16) << left << expenses[i].type << " | "
         << setw(30) << left << expenses[i].description << " | "
         << setw(13) << right << fixed << setprecision(0) << expenses[i].amount << " | "
         << setw(10) << left << expenses[i].date << " |\n";
}

cout << "+------+------------------+--------------------------------+---------------+------------+\n";
}

void displayStatistics(const vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "\nKhong co du lieu de thong ke!\n";
        return;
    }
    
    double totalAmount = 0;
    map<string, double> typeAmounts;
    for (int i = 0; i < 6; i++) {
        typeAmounts[EXPENSE_TYPES[i]] = 0;
    }
    
    //
    for (size_t i = 0; i < expenses.size(); i++) {
        totalAmount += expenses[i].amount;
        typeAmounts[expenses[i].type] += expenses[i].amount;
    }
    
    cout << "\n--- THONG KE CHI TIEU ---\n";
    cout << "Tong tien: " << fixed << setprecision(0) << totalAmount << " VND\n\n";
    
    for (int i = 0; i < 6; i++) {
        if (typeAmounts[EXPENSE_TYPES[i]] > 0) {
            double percent = (typeAmounts[EXPENSE_TYPES[i]] / totalAmount) * 100; //tinh toan phan tram so tien chi trong muc voi tong so tien chi
            cout << setw(20) << left << EXPENSE_TYPES[i] << ": " << fixed << setprecision(2) << percent << "%\n"; //Lam tron phan tram 2 chu so thap phan
        }
    }
};

void deleteExpense(vector<Expense>& expenses){
    if (expenses.empty()) {
        cout << "\nKhong co du lieu de xoa!\n";
        return;
    }

    displayExpenses(expenses);

    int index;

    InputDeleteIndex:
        cout << "Nhap STT chi tieu muon xoa: ";
        if (!(cin >> index) || index < 0 || index > expenses.size()) {
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

