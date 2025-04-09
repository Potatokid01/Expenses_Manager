void OutFile(const vector<Expense>& expenses) { //Xuất dữ liệu ra file expenses.txt
    ofstream outFile("expenses.txt");
    if (!outFile) {
        cout << "Khong the mo file de ghi!\n";
        return;
    }

    for (size_t i = 0; i < expenses.size(); i++) { //Duyệt qua từng chi tiêu trong danh sách expenses
        outFile << i+1 << ". " //STT
                << expenses[i].type << " | " //Loại chi tiêu
                << expenses[i].description << " | " //Nội dung chi tiêu
                << fixed << setprecision(0) << expenses[i].amount << " | " //Số tiền chi tiêu
                << expenses[i].date << "\n"; //Ngày chi tiêu
        }
    outFile.close();