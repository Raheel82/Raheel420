#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cctype>

using namespace std;

const char DEPOSIT_OPTION = '1';
const char WITHDRAW_OPTION = '2';
const char BILL_PAYMENT_OPTION = '3';
const char SEND_MONEY_OPTION = '4';
#we can create a class
class Bank {
private:
    struct Transaction {
        char type;
        int amount;
    };

    struct Node {
        string name;
        string ID;
        string address;
        string contact;
        int cash;
        vector<Transaction> history;
        unique_ptr<Node> next;
    };

    unique_ptr<Node> head;
    int total;

public:
    Bank() : total(0) {}

    void clientMenu();
    void adminMenu();
    void perData();
    void show();
    void update();
    void search();
    void transactions();
    void del();

private:
    void deposit(Node* current, int& cash);
    void withdraw(Node* current, int& cash);
    void billPayment(Node* current, int& cash);
    void sendMoney(Node* current, int& cash);
    void handleTransaction(Node* current, char ch);
    char transactionMenu();
    void clearScreen();
};

void Bank::clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void Bank::perData() {
    auto newNode = make_unique<Node>();
    cout << "Enter data of person " << total + 1 << endl;
    cout << "Enter name: ";
    cin >> newNode->name;
    cout << "ID: ";
    cin >> newNode->ID;
    cout << "Address: ";
    cin.ignore();
    getline(cin, newNode->address);
    cout << "Contact: ";
    cin >> newNode->contact;
    cout << "Total Cash: ";
    cin >> newNode->cash;

    newNode->next = move(head);
    head = move(newNode);
    total++;
}

void Bank::show() {
    auto current = head.get();

    while (current != nullptr) {
        cout << "Data of person" << endl;
        cout << "Name: " << current->name << endl;
        cout << "ID: " << current->ID << endl;
        cout << "Address: " << current->address << endl;
        cout << "Contact: " << current->contact << endl;
        cout << "Cash: " << current->cash << endl;

        current = current->next.get();
    }
}

void Bank::update() {
    cout << "Enter ID of the person whose data you want to update" << endl;
    string id;
    cin >> id;

    Node* current = head.get();

    while (current != nullptr) {
        if (id == current->ID) {
            cout << "Previous data" << endl;
            cout << "Name: " << current->name << endl;
            cout << "ID: " << current->ID << endl;
            cout << "Address: " << current->address << endl;
            cout << "Contact: " << current->contact << endl;
            cout << "Cash: " << current->cash << endl;

            cout << "\nEnter new data" << endl;
            cout << "Enter name: ";
            cin >> current->name;
            cout << "ID: ";
            cin >> current->ID;
            cout << "Address: ";
            cin.ignore();
            getline(cin, current->address);
            cout << "Contact: ";
            cin >> current->contact;
            cout << "Total Cash: ";
            cin >> current->cash;

            break;
        }

        current = current->next.get();
    }

    if (current == nullptr) {
        cout << "No such record found" << endl;
    }
}

void Bank::search() {
    cout << "Enter ID of the person whose data you want to search" << endl;
    string id;
    cin >> id;

    Node* current = head.get();

    while (current != nullptr) {
        if (id == current->ID) {
            cout << "Name: " << current->name << endl;
            cout << "ID: " << current->ID << endl;
            cout << "Address: " << current->address << endl;
            cout << "Contact: " << current->contact << endl;
            cout << "Cash: " << current->cash << endl;
            break;
        }

        current = current->next.get();
    }

    if (current == nullptr) {
        cout << "No such record found" << endl;
    }
}

void Bank::deposit(Node* current, int& cash) {
    cout << "How much cash do you want to deposit?" << endl;
    cin >> cash;
    current->cash += cash;
    cout << "Your new cash is " << current->cash << endl;

    Transaction transaction;
    transaction.type = 'D';
    transaction.amount = cash;
    current->history.push_back(transaction);
}

void Bank::withdraw(Node* current, int& cash) {
    while (true) {
        cout << "How much cash do you want to withdraw?" << endl;
        cin >> cash;

        if (cash <= current->cash) {
            current->cash -= cash;
            cout << "Your new cash is " << current->cash << endl;

            Transaction transaction;
            transaction.type = 'W';
            transaction.amount = cash;
            current->history.push_back(transaction);

            break;
        } else {
            cout << "Insufficient funds. Please enter a valid withdrawal amount." << endl;
        }
    }
}

void Bank::billPayment(Node* current, int& cash) {
    cout << "Enter the amount for bill payment:" << endl;
    cin >> cash;

    if (cash <= current->cash) {
        current->cash -= cash;
        cout << "Bill payment successful! Your new cash is " << current->cash << endl;

        // Update the 'cash' variable to reflect the amount paid
        Transaction transaction;
        transaction.type = 'B';
        transaction.amount = cash;
        current->history.push_back(transaction);
    } else {
        cout << "Insufficient funds for bill payment." << endl;
    }
}

void Bank::sendMoney(Node* current, int& cash) {
    cout << "Enter the ID of the recipient account:" << endl;
    string recipientId;
    cin >> recipientId;

    Node* recipient = head.get();
    while (recipient != nullptr) {
        if (recipientId == recipient->ID) {
            cout << "Enter the amount to send:" << endl;
            cin >> cash;
            if (cash > current->cash) {
                cout << "Insufficient funds for the transfer" << endl;
            } else {
                current->cash -= cash;
                recipient->cash += cash;
                cout << "Money sent successfully!" << endl;

                Transaction transactionSender;
                transactionSender.type = 'S';
                transactionSender.amount = cash;
                current->history.push_back(transactionSender);

                Transaction transactionRecipient;
                transactionRecipient.type = 'R';
                transactionRecipient.amount = cash;
                recipient->history.push_back(transactionRecipient);
            }
            break;
        }
        recipient = recipient->next.get();
    }
    if (recipient == nullptr) {
        cout << "Recipient account not found" << endl;
    }
}

void Bank::handleTransaction(Node* current, char ch) {
    int cash;

    switch (ch) {
        case DEPOSIT_OPTION:
            deposit(current, cash);
            break;
        case WITHDRAW_OPTION:
            withdraw(current, cash);
            break;
        case BILL_PAYMENT_OPTION:
            billPayment(current, cash);
            break;
        case SEND_MONEY_OPTION:
            sendMoney(current, cash);
            break;
        default:
            cout << "Invalid input" << endl;
            break;
    }
}

char Bank::transactionMenu() {
    char ch;

    do {
        cout << "Press 1 for deposit" << endl;
        cout << "Press 2 for withdraw" << endl;
        cout << "Press 3 for bill payment" << endl;
        cout << "Press 4 to send money to another account" << endl;
        cin >> ch;

        if (ch >= '1' && ch <= '4') {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return ch;
        } else {
            cout << "Invalid input. Please enter a valid option." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (true);
}

void Bank::transactions() {
    int cash;
    char ch;

    cout << "Enter ID of the person for the transaction" << endl;
    string id;
    cin >> id;

    Node* current = head.get();

    while (current != nullptr) {
        if (id == current->ID) {
            cout << "Name: " << current->name << endl;
            cout << "Address: " << current->address << endl;
            cout << "Contact: " << current->contact << endl;
            cout << "\nExisting Cash " << current->cash << endl;
            ch = transactionMenu();

            handleTransaction(current, ch);

            break;
        }

        current = current->next.get();
    }

    if (current == nullptr) {
        cout << "No such record found" << endl;
    }
}

void Bank::del() {
    char ch;

    cout << "Press 1 to remove a specific record" << endl;
    cout << "Press 2 to remove the entire record" << endl;
    cin >> ch;
    cin.ignore();

    switch (ch) {
        case '1': {
            cout << "Enter ID of the person whose data you want to remove" << endl;
            string id;
            cin >> id;

            Node* current = head.get();
            Node* prev = nullptr;

            while (current != nullptr) {
                if (id == current->ID) {
                    if (prev == nullptr) {
                        head = move(current->next);
                    } else {
                        prev->next = move(current->next);
                    }

                    total--;
                    cout << "Your required data is deleted" << endl;
                    break;
                }

                prev = current;
                current = current->next.get();
            }

            if (current == nullptr) {
                cout << "No such record found" << endl;
            }
            break;
        }
        case '2':
            while (head != nullptr) {
                head = move(head->next);
            }
            total = 0;
            cout << "All records are deleted" << endl;
            break;
        default:
            cout << "Invalid Input" << endl;
            break;
    }
}

void Bank::clientMenu() {
    char ch;

    do {
        cout << "Enter 1 to view account information" << endl;
        cout << "Enter 2 for transactions" << endl;
        cout << "Enter 3 to exit" << endl;
        cin >> ch;

        switch (ch) {
            case '1':
                show();
                break;
            case '2':
                transactions();
                break;
            case '3':
                return;
            default:
                cout << "Invalid input. Please enter a valid option." << endl;
        }
    } while (true);
}

void Bank::adminMenu() {
    char ch;

    do {
        cout << "Enter 1 to add a new record" << endl;
        cout << "Enter 2 to update a record" << endl;
        cout << "Enter 3 to search for a record" << endl;
        cout << "Enter 4 to delete records" << endl;
        cout << "Enter 5 to view all records" << endl;
        cout << "Enter 6 to exit" << endl;
        cin >> ch;

        switch (ch) {
            case '1':
                perData();
                break;
            case '2':
                update();
                break;
            case '3':
                search();
                break;
            case '4':
                del();
                break;
            case '5':
                show();
                break;
            case '6':
                return;
            default:
                cout << "Invalid input. Please enter a valid option." << endl;
        }
    } while (true);
}

int main() {
    Bank b;
    char userType;

    do {
        cout << "Enter 'C' for Client or 'A' for Admin. Enter 'E' to exit: ";
        cin >> userType;

        switch (toupper(userType)) {
            case 'C':
                b.clientMenu();
                break;
            case 'A':
                b.adminMenu();
                break;
            case 'E':
                return 0;
            default:
                cout << "Invalid user type. Enter 'E' to exit or try again." << endl;
        }
    } while (true);

    return 0;
}
