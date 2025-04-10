#include <bits/stdc++.h>

using namespace std;

const string EXPENSE_TYPES[] = {"An uong", "Di chuyen", "Nha cua", "Nhu yeu pham", "Dich vu"};

//Khai báo cấu trúc Expense để lưu trữ thông tin chi tiêu
struct Expense { 
    string type;
    string description;
    double amount;
    string date;
};

//Khai báo các hàm
string getCurrentDate(); //Hàm lấy ngày hiện tại
bool cmpEBM(const pair<string, vector<Expense>>& a,const pair<string, vector<Expense>>& b); //Ham so sánh hai cặp (tháng/năm, danh sách chi tiêu) dùng compare trong hàm sort
void clearInputBuffer(); //Hàm xóa bộ đệm đầu vào
void addExpense(vector<Expense>& expenses); //Hàm thêm chi tiêu mới
void displayExpenses(const vector<Expense>& expenses); //Hàm hiển thị danh sách chi tiêu theo thứ tự nhập vào
void deleteExpense(vector<Expense>& expenses); //Hàm xóa chi tiêu theo STT đã nhập
void displayStatistics(const vector<Expense>& expenses); //Hàm hiển thị thống kê chi tiêu theo loại đã nhập (không theo tháng)
void displayMainMenu(); //Hàm hiển thị menu chính
void displayExpensesByMonth(const vector<Expense>& expenses); //Hàm hiển thị danh sách chi tiêu theo tháng
void ImportFromCSV(vector<Expense>& expenses); //Hàm nhập dữ liệu từ file CSV
void OutFile(const vector<Expense>& expenses); // Hàm xuất dữ liệu ra file
void OutFileCSV(const vector<Expense>& expenses); //Hàm xuất dữ liệu ra file .csv
string askForFilePath(const string& defaultFilePath); //Hàm yêu cầu người dùng nhập đường dẫn file

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
                deleteExpense(expenses);
                break;
            case 3:
                displayExpenses(expenses);
                break;
            case 4:
                displayExpensesByMonth(expenses);
                break;
            case 5:
                displayStatistics(expenses);
                break;
            case 6:
                ImportFromCSV(expenses); // Nhập dữ liệu từ file CSV
                break;
            case 7:
                cout << "Chuan bi luu vao file.\n";
                OutFile(expenses); // Xuất dữ liệu ra file expenses.txt
                OutFileCSV(expenses); // Xuất dữ liệu ra file CSV
                cout << "Thoat chuong trinh!\n";
                return 0;
            default:
                cout << "Lua chon khong hop le! Vui long nhap lai.\n";
        }
    } while(choice != 7);

    return 0;
}

void displayMainMenu() { //Hiển thị menu chính
    cout << "\n=== QUAN LY CHI TIEU CA NHAN ===\n";
    cout << "1. Them chi tieu moi\n";
    cout << "2. Xoa chi tieu\n";
    cout << "3. Xem danh sach chi tieu\n";
    cout << "4. Xem chi tieu theo thang (danh cho file .csv chua sap xep)\n";
    cout << "5. Xem thong ke chi tieu\n";
    cout << "6. Nhap file du lieu tu file csv\n";
    cout << "7. Luu du lieu va thoat chuong trinh\n";
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
            cin.clear(); clearInputBuffer();
            goto InputExpenseType;
        }
    clearInputBuffer();

    newExpense.type = EXPENSE_TYPES[typeChoice - 1];
    
    InputSpendingDescription: //Nhập nội dung chi tiêu
        cout << "Noi dung chi tieu (toi da 30 ky tu): ";
        getline(cin, newExpense.description);
        if (newExpense.description.length() > 30) {
            cout << "Noi dung chi tieu khong hop le. Vui long nhap lai!\n";
            goto InputSpendingDescription;
        }
    
    InputSpendingAmount: //Nhập số tiền chi tiêu
        cout << "So tien (VND): ";

        if (!(cin >> newExpense.amount) || newExpense.amount < 0) {
            cout << "Gia tri khong hop le! Vui long nhap lai.\n";
            cin.clear(); clearInputBuffer();
            goto InputSpendingAmount;
        }
    clearInputBuffer();
    
    InputDate: //Nhập ngày chi tiêu
        cout << "Ngay chi tieu. Neu de mac dinh nhan ENTER!" << " (mac dinh: " << getCurrentDate() << "): "; //Nhập ngày chi tiêu
        string dateInput;
        getline(cin, dateInput);
        if (dateInput.empty()) {
            newExpense.date = getCurrentDate(); //Nếu không nhập thì lấy ngày hiện tại
        } else if (dateInput.length() != 10) {
            cout << "Dinh dang ngay khong hop le. Vui long nhap lai!\n";
            goto InputDate;
        } else newExpense.date = dateInput; // Nếu nhập đúng thì lấy ngày chi tiêu đã nhập

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
    for (int i = 0; i < expenses.size(); i++) { //Số byte tổng của ds loại chi tiêu chia cho số byte của từng loại chi tiêu = số phần tử
        typeAmounts[EXPENSE_TYPES[i]] = 0;                                       //Khởi tạo giá trị cho từng loại chi tiêu
    }
    
    for (size_t i = 0; i < expenses.size(); i++) { //Tính tổng từng loại chi tiêu
        totalAmount += expenses[i].amount;
        typeAmounts[expenses[i].type] += expenses[i].amount;
    }
    
    cout << "\n--- THONG KE CHI TIEU ---\n";
    cout << "Tong tien: " << fixed << setprecision(0) << totalAmount << " VND\n\n"; //Hiển thị tổng số tiền chi tiêu
    
    for (size_t i = 0; i < 5; i++) { //Duyệt qua từng loại chi tiêu
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

bool cmpEBM(const pair<string, vector<Expense>>& a, const pair<string, vector<Expense>>& b) { //So sánh hai cặp (tháng/năm, danh sách chi tiêu) dùng compare trong hàm sort
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
            cout << "| "
                 << setw(4) << right << i + 1 << " | "
                 << setw(16) << left << expensesInMonth[i].type << " | "
                 << setw(30) << left << expensesInMonth[i].description << " | "
                 << setw(13) << right << fixed << setprecision(0) << expensesInMonth[i].amount << " | "
                 << setw(10) << left << expensesInMonth[i].date << " |\n";
        }

        cout << "+------+------------------+--------------------------------+---------------+------------+\n";
    }
}

string askForFilePath(const string& defaultFilePath) { //Hàm yêu cầu người dùng nhập đường dẫn file
    string file_Path;
    const string invalidChars = "\\/:*?\"<>|"; //Các ký tự không hợp lệ trong đường dẫn file
    //Kiểm tra xem đường dẫn có chứa ký tự không hợp lệ không
    InputFilePath:
        cout << "Nhap ten file (khong co dau cach va duoi file): ;
        cin >> file_Path; //Nhập đường dẫn file
        if (file_Path.empty()) {
            file_Path = "expenses" + defaultFilePath; // Nếu không nhập thì trả về đường dẫn mặc định
            return file_Path;
        } else { 
            for (int i = 0; i < file_Path.length(); i++) { //Kiểm tra xem đường dẫn có chứa ký tự không hợp lệ không
                if (invalidChars.find(file_Path[i]) != string::npos) {
                    cout << "Duong dan chua ky tu khong hop le! Vui long nhap lai.\n";
                    cin.clear(); clearInputBuffer();
                    goto InputFilePath; //Nếu có kí tự đặc biệt thì yêu cầu nhập lại
                }
            }
        }
        return file_Path + defaultFilePath; // Trả về đường dẫn hợp lệ với phần mở rộng
}

void ImportFromCSV(vector<Expense>& expenses) {
    cout << "NHAP TEN FILE MA BAN MUON LAY DU LIEU:\n";
    const string filePath = askForFilePath(".csv"); // Yêu cầu người dùng nhập tên file mong muốn
    ifstream inFile(filePath);
    
    if (!inFile) {
        cout << "\nKhong the mo file CSV! Hay dam bao file " << filePath << " co trong thu muc!\n";
        return;
    }
    
    string line;
    // Bỏ qua dòng header
    getline(inFile, line);
    
    // Đọc từng dòng trong file CSV
    while (getline(inFile, line)) {
        Expense newExpense;
        stringstream ss(line);
        string token;
        
        getline(ss, token, ','); // Bỏ qua STT
        getline(ss, newExpense.type, ','); // Đọc loại chi tiêu
        
        // Đọc nội dung chi tiêu (có thể có dấu phẩy bên trong)
        getline(ss, token, '"');  // Đọc đến dấu ngoặc kép đầu tiên
        getline(ss, newExpense.description, '"');  // Đọc nội dung giữa cặp dấu ngoặc kép
        getline(ss, token, ','); // Loại bỏ dấu phẩy sau dấu ngoặc kép
        getline(ss, token, ','); // Đọc số tiền
        newExpense.amount = stod(token);  // Chuyển đổi từ string sang double
        getline(ss, newExpense.date); // Đọc ngày chi tiêu
        expenses.push_back(newExpense); // Thêm chi tiêu vào danh sách
    }
    
    inFile.close();
    cout << "Da nhap du lieu tu file " << filePath << " thanh cong!\n";
    cout << "So chi tieu da nhap: " << expenses.size() << "\n";
}

void OutFile(const vector<Expense>& expenses) { //Xuất dữ liệu ra file expenses.txt
    const string filePath = askForFilePath(".txt"); //Yêu cầu người dùng nhập tên file mong muốn
    ofstream outFile(filePath); //Mở file expenses.txt để ghi dữ liệu vào cuối file (ios::app)
    if (!outFile) {
        cerr << "Khong the mo file de ghi! Kiem tra thu muc hoac quyen truy cap: " << filePath << "\n";
        return;
    }
    outFile << "\t==================== DANH SACH CHI TIEU ====================\n";
    outFile << "| STT  |      Loai        |            Noi dung            | So tien (VND) |    Ngay    |\n";
    for (size_t i = 0; i < expenses.size(); i++) { //Duyệt qua từng chi tiêu trong danh sách expenses
        outFile << setw(3) << left << i+1 << " | " //STT
                << setw(13) << left << expenses[i].type << " | " //Loại chi tiêu
                << setw(32) << left << expenses[i].description << " | " //Nội dung chi tiêu
                << setw(13) << fixed << setprecision(0) << expenses[i].amount << " | " //Số tiền chi tiêu
                << expenses[i].date << "\n"; //Ngày chi tiêu
        }
    outFile.close();
}

void OutFileCSV(const vector<Expense>& expenses) {
    if (expenses.empty()) {
        cout << "\nDanh sach chi tieu trong! Khong co du lieu de xuat ra file CSV.\n";
        return;
    }

    const string filePath = askForFilePath(".csv"); // Yêu cầu người dùng nhập tên file mong muốn
    ofstream outFile(filePath);
    
    if (!outFile) {
        cout << "Khong the mo file de ghi! Kiem tra thu muc hoac quyen truy cap: " << filePath << "\n";
        return;
    }
    
    // Thêm header cho file CSV
    outFile << "STT,Loai,Noi dung,So tien (VND),Ngay\n";
    
    // Ghi dữ liệu chi tiêu vào file CSV
    for (size_t i = 0; i < expenses.size(); i++) {
        outFile << i + 1 << ","
                << expenses[i].type << ","
                << "\"" << expenses[i].description << "\"" << ","  // Đặt nội dung trong dấu ngoặc kép để tránh lỗi nếu có dấu phẩy
                << fixed << setprecision(0) << expenses[i].amount << ","
                << expenses[i].date << "\n";
    }
    
    outFile.close();
    cout << "Da xuat du lieu vao file .csv " << filePath << " thanh cong!\n";
}

