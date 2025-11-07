#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void width(int val){
    for(int i = 0 ;i  < val;i++)
        cout<<' ';
}

#define LINE width(2); cout<<string(120, '-');

struct Column {
    string name;  
    int width; 
};

Column bookColumns[] = {
    {"ID", 10},
    {"Title", 30},
    {"Author", 20},
    {"Total", 10},
    {"Issued", 10}
};

template<class T>

void printColumn(const T &text, const Column &col) {
    std::string s = text;  
    cout << text;
    int len = s.length();
    for (int i = len; i < col.width; i++)
        cout << ' ';
}//no need for char *text

class Library;

class Book
{
    string title;
    string author;
    string id;
    int totalCopies;
    int IssuedCount;
    static int idNum;

public:
    Book()
    {
        title = " ";
        author = " ";
        totalCopies = 0;
        IssuedCount = 0;
        idNum++;
        id = "B" + to_string(idNum);
    }

    Book(string title, string author, int totalCopies)
    {
        this->title = title;
        this->author = author;
        this->totalCopies = totalCopies;
        IssuedCount = 0;
        idNum++;
        id = "B" + to_string(idNum);
    }

    string getTitle() { return title; }
    string getAuthor() { return author; }
    string getId() { return id; }
    int getIssedCount() { return IssuedCount; }
    int copiesLeft() { return (totalCopies - IssuedCount); }
    void incBookCount() { totalCopies++; }
    void decBookCount() { totalCopies--; }
    int operator==(Book b) { return (title == b.title && author == b.author); }
    friend void displayBook(const Book &b);
    friend ostream& operator<<(ostream &os, const Book &b);
    friend class Library;
};

ostream& operator<<(ostream &os, const Book &b) {
    os << b.id << " | " << b.title << " | " << b.author << " | Total: " << b.totalCopies << " | Issued: " << b.IssuedCount << " |";
    return os;
}

void displayBook(const Book &b){
    cout<<b;
}

int intializeBookid(){
        fstream bookFile("books.txt",ios::in);
        string str;
        int num = 0;
        if(!bookFile){
            num = -1 ;
        }
        else{
            while (getline(bookFile, str))
            {
                string id;
                int pos1 = str.find('|');
                int pos2 = str.find('|', pos1 + 1);
                int pos3 = str.find('|', pos2 + 1);
                int pos4 = str.find('|', pos3 + 1);

                num = stoi(str.substr(1, pos1-1));
            }
        }
        bookFile.close();
        return num;
}

int Book ::idNum = intializeBookid();

class User
{
    string usrName;
    string pwd;
    string id;
    string role;

public:
    User()
    {
        usrName = " ";
        pwd = " ";
        role = " ";
        id = "U" + to_string(rand() % 10000);
    };

    User(string usrName, string pwd, string role)
    {
        this->usrName = usrName;
        this->pwd = pwd;
        this->role = role;
        id = "U" + to_string(rand() % 10000);
    }

    friend class Library;
    virtual void showMenu() = 0;
    string getUserName() { return usrName; }
    string getRole() { return role; }
    string getId() { return id; }
};

class Admin : public User
{
public:
    Admin() {};
    Admin(string username, string password, string role){};
    friend class Library;
    void showMenu();
};

class Member : public User
{
public:
    Member() {};
    Member(string username, string password, string role){};
    friend class Library;
    void showMenu();
};

class Library
{
public:
    /*Adding books*/
    static void addBook()
    {
        string title, author;
        int copies;
        
        cin.ignore();
        cout<<"Enter title: ";
        getline(cin,title);
        cout<<"Enter author: ";
        getline(cin,author);
        cout<<"Enter number of copies: ";
        cin>>copies;

        fstream bookFile("books.txt", ios::in);
        fstream temp("temp.txt", ios::out);
        if (!bookFile)
        {
            cout << "No existing file found."<<endl;
        }
        string str;
        bool found = false;
        while (getline(bookFile, str))
        {
            string id = "", a = "", t="", total="", issued="";
            int totalCopies = 0, issuedCount = 0;
            int field = 0;
            string tempStr = "";
            for(char ch : str){
                if(ch == '|'){
                    if(field == 0) id = tempStr;
                    else if(field == 1) t = tempStr;
                    else if(field == 2) a = tempStr;
                    else if(field == 3) {
                        total = tempStr;
                        totalCopies = stoi(total);
                    }
                    else if(field == 4){
                        issued = tempStr;
                        issuedCount = stoi(issued);
                    }
                    tempStr = "";
                    field++;
                }
                else{
                    tempStr+=ch;
                }
            }

            if (t == title && a == author)
            {
                totalCopies += copies;
                found = true;
                cout << "Book already exists , increased copies to " << totalCopies << endl;
            }
            temp << id << "|" << t << "|" << a << "|" << totalCopies << "|" << issuedCount << "|" << endl;
        }
        bookFile.close();
        if (!found)
        {
            Book b(title, author, copies);
            string bookid = b.getId(), booktitle = b.getTitle(), bookauthor = b.getAuthor();
            int bookissuedCount = b.getIssedCount();

            temp << bookid << "|" << booktitle << "|" << bookauthor << "|" << copies << "|" << bookissuedCount << "|" << endl;
            cout << "New book added successfully!"<<endl;
        }
        temp.close();
        remove("books.txt");
        rename("temp.txt", "books.txt");
    }

    /*view all books and their data*/
    static void viewAllBooks()
    {
        fstream bookFile("books.txt", ios::in);

        if (!bookFile)
        {
            cout << "No existing file found."<<endl;
        }

        string str;
        bool found = false;
        printColumn("Id", bookColumns[0]);
        printColumn("Title", bookColumns[1]);
        printColumn("Author", bookColumns[2]);
        printColumn("Total Copies", bookColumns[3]);
        printColumn("Issued", bookColumns[4]);
            cout << endl;
        while (getline(bookFile, str))
        {
            string id = "", a = "", t="", total="", issued="";
            int totalCopies = 0, issuedCount = 0;
            int field = 0;
            string tempStr = "";
            for(char ch : str){
                if(ch == '|'){
                    if(field == 0) id = tempStr;
                    else if(field == 1) t = tempStr;
                    else if(field == 2) a = tempStr;
                    else if(field == 3) {
                        total = tempStr;
                        totalCopies = stoi(total);
                    }
                    else if(field == 4){
                        issued = tempStr;
                        issuedCount = stoi(issued);
                    }
                    tempStr = "";
                    field++;
                }
                else{
                    tempStr+=ch;
                }
            }
            printColumn(id, bookColumns[0]);
            printColumn(t, bookColumns[1]);
            printColumn(a, bookColumns[2]);
            printColumn(total, bookColumns[3]);
            printColumn(issued, bookColumns[4]);
            cout << endl;
        }
        bookFile.close();
    }

    /*Issue Books*/
    static void issueBook()
    {
        string author,title;

        cin.ignore();
        cout<<"Enter title: ";
        getline(cin,title);
        cout<<"Enter author: ";
        getline(cin,author);

        ifstream bookFile("books.txt");
        ofstream temp("temp.txt");
        bool found = false;

        string str;
        while (getline(bookFile, str))
        {
            string id = "", a = "", t="", total="", issued="";
            int totalCopies = 0, issuedCount = 0;
            int field = 0;
            string tempStr = "";
            for(char ch : str){
                if(ch == '|'){
                    if(field == 0) id = tempStr;
                    else if(field == 1) t = tempStr;
                    else if(field == 2) a = tempStr;
                    else if(field == 3) {
                        total = tempStr;
                        totalCopies = stoi(total);
                    }
                    else if(field == 4){
                        issued = tempStr;
                        issuedCount = stoi(issued);
                    }
                    tempStr = "";
                    field++;
                }
                else{
                    tempStr+=ch;
                }
            }
            if (t == title && a == author)
            {
                found = true;
                if (totalCopies - issuedCount > 0)
                {
                    issuedCount++;
                    cout << "Book issued successfully!"<<endl;
                }
                else
                {
                    cout << "sorry no copies availabled."<<endl;
                }
            }
            temp << id << "|" << t << "|" << a << "|" << totalCopies << "|" << issuedCount << "|" << endl;
        }

        bookFile.close();
        temp.close();

        if (!found)
            cout << "Book not found."<<endl;

        remove("books.txt");
        rename("temp.txt", "books.txt");
    }

    /*Returning Books*/
    static void returnBook()
    {
        string title, author;

        cin.ignore();
        cout<<"Enter title: ";
        getline(cin,title);
        cout<<"Enter author: ";
        getline(cin,author);

        ifstream bookFile("books.txt");
        ofstream temp("temp.txt");
        bool found = false;

        string str;
        while (getline(bookFile, str))
        {
            string id = "", a = "", t="", total="", issued="";
            int totalCopies = 0, issuedCount = 0;
            int field = 0;
            string tempStr = "";
            for(char ch : str){
                if(ch == '|'){
                    if(field == 0) id = tempStr;
                    else if(field == 1) t = tempStr;
                    else if(field == 2) a = tempStr;
                    else if(field == 3) {
                        total = tempStr;
                        totalCopies = stoi(total);
                    }
                    else if(field == 4){
                        issued = tempStr;
                        issuedCount = stoi(issued);
                    }
                    tempStr = "";
                    field++;
                }
                else{
                    tempStr+=ch;
                }
            }

            if (t == title && a == author)
            {
                found = true;
                if (issuedCount > 0)
                {
                    issuedCount--;
                    cout << "Book returned successfully!"<<endl;
                }
                else
                {
                    cout << "All copies are already in library."<<endl;
                }
            }
            temp << id << "|" << t << "|" << a << "|" << totalCopies << "|" << issuedCount <<  "|" << endl;
        }

        bookFile.close();
        temp.close();

        if (!found)
            cout << "Book not found."<<endl;

        remove("books.txt");
        rename("temp.txt", "books.txt");
    }

    static void searchBook(string title, string author)
    {
        fstream bookFile("books.txt", ios::in);
        bool found = false;
        string str;
        while (getline(bookFile, str))
        {
            string id = "", a = "", t="", total="", issued="";
            int totalCopies = 0, issuedCount = 0;
            int field = 0;
            string tempStr = "";
            for(char ch : str){
                if(ch == '|'){
                    if(field == 0) id = tempStr;
                    else if(field == 1) t = tempStr;
                    else if(field == 2) a = tempStr;
                    else if(field == 3) {
                        total = tempStr;
                        totalCopies = stoi(total);
                    }
                    else if(field == 4){
                        issued = tempStr;
                        issuedCount = stoi(issued);
                    }
                    tempStr = "";
                    field++;
                }
                else{
                    tempStr+=ch;
                }
            }

            if (t == title && a == author)
            {
                if ((totalCopies - issuedCount) > 0)
                {
                    cout << "book found!" << endl;
                    cout << "copies left: " << totalCopies - issuedCount;
                    cout<<endl;
                    found = true;
                }

                else{
                    cout << "book is not available at the moment" << endl;
                    found = true;
                }
            }
        }

        bookFile.close();

        if (!found)
            cout << "Book not found."<<endl;
    }

    static void searchBook(string ID)
    {
        fstream bookFile("books.txt", ios::in);
        bool found = false;
        string str;
        while (getline(bookFile, str))
        {
            string id = "", a = "", t="", total="", issued="";
            int totalCopies = 0, issuedCount = 0;
            int field = 0;
            string tempStr = "";
            for(char ch : str){
                if(ch == '|'){
                    if(field == 0) id = tempStr;
                    else if(field == 1) t = tempStr;
                    else if(field == 2) a = tempStr;
                    else if(field == 3) {
                        total = tempStr;
                        totalCopies = stoi(total);
                    }
                    else if(field == 4){
                        issued = tempStr;
                        issuedCount = stoi(issued);
                    }
                    tempStr = "";
                    field++;
                }
                else{
                    tempStr+=ch;
                }
            }
            if (ID == id)
            {
                if ((totalCopies - issuedCount) > 0)
                {
                    cout << "book found!" << endl;
                    cout << "copies left: " << totalCopies - issuedCount;
                    cout<<endl;
                    found = true;
                }

                else{
                    cout << "book is not available at the moment" << endl;
                    found = true;
                }
            }
        }

        bookFile.close();

        if (!found)
            cout << "Book not found."<<endl;
    }

    /*Functions for member*/
    static void addUser()
    {

        fstream userFile("users.txt", ios::app);
        string uname, pwd;
        bool added = false;
        int role;

        cin.ignore();
        cout << "Enter username: ";
        getline(cin,uname);
        cout << "Enter password: ";
        getline(cin,pwd);
        cout << "Enter role (Admin : 1/Member : 2): ";
        cin >> role;

        if (role == 1)
        {
            Admin a(uname, pwd, "Admin");
            userFile << a.getId() << "|" << uname << "|" << pwd << "|" << "Admin" << "|" << endl;
            cout << "Admin added successfully." << endl;
            added = true;
        }
        if (role == 2)
        {
            Member m(uname, pwd, "User");
            userFile << m.getId() << "|" << uname << "|" << pwd << "|" << "Member" << "|" << endl;
            cout << "Member added successfully." << endl;
            added = true;
        }
        if(!added)
        {
            cout << "pick either 1 for Admin or 2 for member" << endl;
        }
    }

    static void viewAllMembers()
    {
        ifstream bookFile("users.txt");
        if (!bookFile)
        {
            cout << "No members found."<<endl;
            return;
        }

        string id, name, pwd, role, str;

        width(10); cout << "ID"; width(20); cout<< "Username" ; width(15); cout << "Role" << endl;

        while (getline(bookFile, str))
        {
           string id="" ,name="",pwd="",role="";
            int field = 0;
            string tempStr = "";
            for(char ch : str){
                if(ch == '|'){
                    if(field == 0) id = tempStr;
                    else if(field == 1) name = tempStr;
                    else if(field == 2) pwd = tempStr;
                    else if(field == 3) role = tempStr;
                
                    tempStr = "";
                    field++;
                }
                else{
                    tempStr+=ch;
                }
            }

            width(10); cout << id; width(20); cout << name; width(15); cout << role << endl;
        }
        bookFile.close();
    }

    static void searchMember()
    {
        string uname;
        bool found = false;
        cin.ignore();
        cout << "Enter username to search: ";
        getline(cin,uname);

        ifstream userFile("users.txt");
        if (!userFile)
        {
            cout << "No members found."<<endl;
            return;
        }

        string str;
        LINE;
        width(10); cout << "ID"; width(20); cout << "Username"; width(15); cout << "Role" << endl;
        LINE;

        while (getline(userFile, str))
        {
            string id="" ,name="",pwd="",role="";
            int field = 0;
            string tempStr = "";
            for(char ch : str){
                if(ch == '|'){
                    if(field == 0) id = tempStr;
                    else if(field == 1) name = tempStr;
                    else if(field == 2) pwd = tempStr;
                    else if(field == 3) role = tempStr;
                
                    tempStr = "";
                    field++;
                }
                else{
                    tempStr+=ch;
                }
            }

            if (name == uname)
            {
                width(10); cout<< id; width(20); cout<<name; width(15); cout << role << endl;
                found = true;
                break;
            }
        }

        userFile.close();

        if (!found)
            cout << "Member not found."<<endl;
    }

    static void login()
    {
        string uname, pwd;
        cin.ignore();
        cout << "enter username: ";
        getline(cin,uname);
        cout << "password: ";
        getline(cin,pwd);

        bool found = false;

        fstream userfi("users.txt", ios::in);
        if (!userfi)
            cout << "No user database found" << endl;

        
            string str;
            while (getline(userfi, str))
            {
            string id="" ,name="",pswd="",role="";
            int field = 0;
            string tempStr = "";
            for(char ch : str){
                if(ch == '|'){
                    if(field == 0) id = tempStr;
                    else if(field == 1) name = tempStr;
                    else if(field == 2) pswd = tempStr;
                    else if(field == 3) role = tempStr;
                
                    tempStr = "";
                    field++;
                }
                else{
                    tempStr+=ch;
                }
            }
            if (uname == name && pswd == pwd)
                {
                    found = true;
                    cout << "login successful. Welcome , " << uname << endl;
                    if (role == "Admin")
                    {
                        Admin a;
                        a.showMenu();
                    }
                    else
                    {
                        Member m;
                        m.showMenu();
                    }
                    break;
                }
        }
        if(!found)
        cout<<"member does not exist"<<endl;
    }
};

void Admin :: showMenu(){   
        int ch;

        do{
            LINE
            width(50);cout<<"ADMIN MENU"<<endl;
            LINE   
            cout << "1. Add Book"<<endl;
            cout << "2. View All Books"<<endl;
            cout << "3. Add Member"<<endl;
            cout << "4. View All Members"<<endl;
            cout << "0. Logout"<<endl;
            cout << "Enter Choice: ";
            cin >> ch;
            switch(ch){
                case 1 : 
                    Library::addBook();
                    break;
                
                case 2 :
                    Library::viewAllBooks();
                    break;
                case 3 : 
                    Library::addUser();
                    break;
                case 4 : 
                    Library::viewAllMembers();
                    break;
                case 0 :
                    cout<<"Logging out"<<endl;
                    break;
                default : 
                    cout<<"Please enter a number between 0 and 4"<<endl;
                    break;
            }
        }while(ch!=0);
    }

void Member :: showMenu(){
    int ch;
        string title, author , ID;
        do{
            LINE        
            width(50);cout<<"MEMBER MENU"<<endl;
            LINE
            cout << "1. View Books"<<endl;
            cout << "2. Borrow Book"<<endl;
            cout << "3. Return Book"<<endl;
            cout << "4. Search Book using author and title"<<endl;
            cout << "5. Search Book using id"<<endl;
            cout << "0. Logout"<<endl;
            cout << "Enter Choice: ";
            cin>>ch;

            switch(ch){
                case 1 : 
                    Library::viewAllBooks();
                    break;
                
                case 2 :
                    Library::issueBook();
                    break;
                case 3 : 
                    Library::returnBook();
                    break;
                case 4 : 
                    cin.ignore();
                    cout<<"Enter title: ";
                    getline(cin,title);
                    cout<<"Enter author: ";
                    getline(cin,author);
                    Library::searchBook(title,author);
                    break;
                case 5 :
                    cin.ignore();
                    cout<<"Enter id: ";
                    getline(cin,ID);
                    Library::searchBook(ID);
                    break;
                case 0 :
                    cout<<"Logging out"<<endl;
                    break;
                default : 
                    cout<<"Please enter a number between 0 and 5"<<endl;
                    break;
            }
        }while(ch!=0);
    }

void initializeLibraryData()
{
    fstream bookFile("books.txt", ios::in);
    bool empty = true;

    if (bookFile)
    {
        char ch;
        if (bookFile >> ch)  
            empty = false;   
    }

    bookFile.close();
    if (empty)
    {
        bookFile.close();
        ofstream newBookFile("books.txt");
        newBookFile << "B1|1984|George Orwell|5|1|" << endl;
        newBookFile << "B2|The Catcher in the Rye|J.D. Salinger|4|2|" << endl;
        newBookFile << "B3|To Kill a Mockingbird|Harper Lee|6|0|" << endl;
        newBookFile << "B4|The Hobbit|J.R.R. Tolkien|7|3|" << endl;
        newBookFile.close();
        intializeBookid();
    }
    else
    {
        bookFile.close();
    }

    fstream userFile("users.txt", ios::in);

    empty = true;

    if (userFile)
    {
        char ch;
        if (userFile >> ch)
            empty = false;
    }

    userFile.close();

    if (empty)
    {
        userFile.close();
        ofstream newUserFile("users.txt");
        newUserFile << "U1001|admin|admin123|Admin|" << endl;
        newUserFile << "U1002|member|member123|Member|" << endl;
        newUserFile.close();
    }
    else
    {
        userFile.close();
    }
}

int main()
{
    srand(47);
    initializeLibraryData();
    int ch;

    do{
        LINE;
        width(50);cout<<"LIBRARY MANAGEMENT SYSTEM"<<endl;
        LINE;
        cout<<"1.Login"<<endl;
        cout<<"0.exit"<<endl;
        cout<<"Enter Choice: ";
        cin>>ch;
        switch(ch){
            case 1 : 
                Library::login();
                break;
            case 0 : 
                cout<<"Exiting the program."<<endl;
                break;
            default: 
                cout<<"Please choose either 0 or 1"<<endl;
                break;
        }
    }while(ch!=0);  

}
