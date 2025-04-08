#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <map>
#include <limits>

using namespace std;

struct Expense {
    string type;
    string content;
    double amount;
    string date;
};

const string EXPENSE_TYPES[] = {"An uong", "Di chuyen", "Nha cua", "Xe co", "Nhu yeu pham", "Dich vu"};

string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y", ltm);
    return string(buffer);
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void addExpense(vector<Expense>& expenses) {
    Expense newExpense;
    int typeChoice;

    cout << "\n--- THEM CHI TIEU MOI ---\n";
    cout << "Chon loai chi tieu:\n";
    for (int i = 0; i < 6; i++) {
        cout << i + 1 << ". " << EXPENSE_TYPES[i] << endl;
    }
    
    while (true) {
        cout << "Lua chon: ";
        cin >> typeChoice;
        if (cin.fail() || typeChoice < 1 || typeChoice > 6) {
            cout << "Lua chon khong hop le! Vui long nhap lai.\n";
            clearInputBuffer();
        } else {
            clearInputBuffer();
            break;
        }
    }
    newExpense.type = EXPENSE_TYPES[typeChoice - 1];
    
    cout << "Noi dung chi tieu: ";
    getline(cin, newExpense.content);
    
    while (true) {
        cout << "So tien (VND): ";
        cin >> newExpense.amount;
        if (cin.fail() || newExpense.amount < 0) {
            cout << "Gia tri khong hop le! Vui long nhap lai.\n";
            clearInputBuffer();
        } else {
            clearInputBuffer();
            break;
        }
    }
    
    cout << "Ngay (dd/mm/yyyy) [Mac dinh: " << getCurrentDate() << "]: ";
    string dateInput;
    getline(cin, dateInput);
    newExpense.date = dateInput.empty() ? getCurrentDate() : dateInput;
    
    expenses.push_back(newExpense);
    cout << "Da them chi tieu thanh cong!\n";
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
         << setw(30) << left << expenses[i].content << " | "
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
    
    // Use a traditional for loop instead of range-based for loop
    for (size_t i = 0; i < expenses.size(); i++) {
        totalAmount += expenses[i].amount;
        typeAmounts[expenses[i].type] += expenses[i].amount;
    }
    
    cout << "\n--- THONG KE CHI TIEU ---\n";
    cout << "Tong tien: " << fixed << setprecision(0) << totalAmount << " VND\n\n";
    
    for (int i = 0; i < 6; i++) {
        if (typeAmounts[EXPENSE_TYPES[i]] > 0) {
            double percent = (typeAmounts[EXPENSE_TYPES[i]] / totalAmount) * 100;
            cout << setw(15) << left << EXPENSE_TYPES[i] << ": " << fixed << setprecision(1) << percent << "%\n";
        }
    }
}

void updateExpense(vector<Expense>& expenses) {
    if (expenses.empty()) { //kiem tra vector co rong hay khong
        cout << "\nKhong co du lieu de cap nhat!\n";
        return;
    }

    displayExpenses(expenses); //

    int index;
    cout << "Nhap STT chi tieu muon cap nhat: ";
    cin >> index;
    clearInputBuffer();

    if (index < 1 || index > expenses.size()) {
        cout << "STT khong hop le!\n";
        return;
    }

    Expense& exp = expenses[index - 1];

    cout << "\n--- CAP NHAT CHI TIEU ---\n";
    cout << "Loai cu: " << exp.type << "\n";
    cout << "Chon loai moi (hoac nhap 0 de giu nguyen):\n";
    for (int i = 0; i < 6; i++) {
        cout << i + 1 << ". " << EXPENSE_TYPES[i] << endl;
    }

    int typeChoice;
    cin >> typeChoice;
    clearInputBuffer();
    if (typeChoice >= 1 && typeChoice <= 6) {
        exp.type = EXPENSE_TYPES[typeChoice - 1];
    }

    cout << "Noi dung cu: " << exp.content << "\n";
    cout << "Nhap noi dung moi (bo trong de giu nguyen): ";
    string newContent;
    getline(cin, newContent);
    if (!newContent.empty()) {
        exp.content = newContent;
    }

    cout << "So tien cu: " << exp.amount << "\n";
    cout << "Nhap so tien moi (nhap -1 de giu nguyen): ";
    double newAmount;
    cin >> newAmount;
    clearInputBuffer();
    if (newAmount >= 0) {
        exp.amount = newAmount;
    }

    cout << "Ngay cu: " << exp.date << "\n";
    cout << "Nhap ngay moi (dd/mm/yyyy) (bo trong de giu nguyen): ";
    string newDate;
    getline(cin, newDate);
    if (!newDate.empty()) {
        exp.date = newDate;
    }

    cout << "Cap nhat thanh cong!\n";
}


void deleteExpense(vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "\nKhong co du lieu de xoa!\n";
        return;
    }

    displayExpenses(expenses);

    int index;
    cout << "Nhap STT chi tieu muon xoa: ";
    cin >> index;
    clearInputBuffer();

    if (index < 1 || index > expenses.size()) {
        cout << "STT khong hop le!\n";
        return;
    }

    expenses.erase(expenses.begin() + (index - 1));
    cout << "Da xoa chi tieu thanh cong!\n";
}


void displayMainMenu() {
    cout << "\n=== QUAN LY CHI TIEU CA NHAN ===\n";
    cout << "1. Them chi tieu moi\n";
    cout << "2. Xem danh sach chi tieu\n";
    cout << "3. Xem thong ke chi tieu\n";
 	cout << "4. Cap nhat chi tieu\n";         
    cout << "5. Xoa chi tieu\n";              
    cout << "6. Thoat chuong trinh\n";        
    cout << "Lua chon: ";
}

int main() {
    vector<Expense> expenses;
    int choice;
    
    do {
        displayMainMenu();
        cin >> choice;
        if (cin.fail()) {
            cout << "Lua chon khong hop le!\n";
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        
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
		        updateExpense(expenses); 
		        break;
	    	case 5:
		        deleteExpense(expenses); 
		        break;
	    	case 6:
		        cout << "Cam on da su dung chuong trinh!\n";
		        break;
            default:
                cout << "Lua chon khong hop le. Vui long chon lai!\n";
        }
    } while (choice != 6);
    
    return 0;
}
