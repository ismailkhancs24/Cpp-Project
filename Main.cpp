#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<cstdlib>

using namespace std;

class User {
	public:
	    virtual void displayMenu() = 0;
	    virtual ~User() {}
};

class MenuItem {
	private:
	    int id;
	    string name;
	    double price;
	
	public:
	    MenuItem() : id(0), name(" "), price(0.0) {}
	    MenuItem(int i, string n, double p) : id(i), name(n), price(p) {}
	
	    int getID() const { return id; }
	    string getName() const { return name; }
	    double getPrice() const { return price; }

    void display() const {
        cout << "Product ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Price: RS " << price << endl;
    }
};

class FileHandler {
	public:
	    static void saveMenuItems(const MenuItem& item) {
	        ofstream out("Menu.txt", ios::app);
	        out << item.getID() << endl;
	        out << item.getName() << endl;
	        out << item.getPrice() << endl;
	        out.close();
	    }
	
	    static vector<MenuItem> loadMenuItems() {
	        vector<MenuItem> menu;
	        ifstream in("Menu.txt");
	        int id;
	        string name;
	        double price;
	        while (in >> id) {
	            in.ignore();
	            getline(in, name);
	            in >> price;
	            in.ignore();
	            menu.emplace_back(id, name, price);
	        }
	        in.close();
	        return menu;
	    }
	
	    static void saveOrder(const vector<MenuItem>& order) {
	        ofstream out("Orders.txt", ios::app);
	        out << "--- New Order ---\n";
	        double total = 0.0;
	        for (size_t i = 0; i < order.size(); ++i) {
	            out << order[i].getName() << " Rs. " << order[i].getPrice() << endl;
	            total += order[i].getPrice();
	        }
	        out << "Total: Rs. " << total << "\n\n";
	        out.close();
	    }
};

class Admin : public User {
	public:
	    void addMenuItem() {
	        system("cls");
	        int id;
	        string name;
	        double price;
	        cout << "Enter Item ID: ";
	        cin >> id;
	        cin.ignore();
	        cout << "Enter Item Name: ";
	        getline(cin, name);
	        cout << "Enter Item Price: ";
	        cin >> price;
	        MenuItem item(id, name, price);
	        FileHandler::saveMenuItems(item);
	    }
	
	    void displayMenu() override {
	        system("cls");
	        cout << "========================" << endl;
	        cout << "========  Menu  ========" << endl;
	        cout << "========================" << endl;
	        vector<MenuItem> items = FileHandler::loadMenuItems();
	        for (size_t i = 0; i < items.size(); ++i) {
	            items[i].display();
	            cout << endl;
	        }
	    }
};

class Customer : public User {
	public:
	    void displayMenu() override {
	        system("cls");
	        cout << "========================" << endl;
	        cout << "========  Menu  ========" << endl;
	        cout << "========================" << endl;
	        vector<MenuItem> items = FileHandler::loadMenuItems();
	        for (size_t i = 0; i < items.size(); ++i) {
	            items[i].display();
	            cout << endl;
	        }
	    }
	
	    void placeOrder() {
	    vector<MenuItem> menu = FileHandler::loadMenuItems();
	    vector<pair<MenuItem, int>> order;
	    char choice;
	    double totalBill = 0.0;
	
	    do {
	        int id;
	        cout << "Enter Item ID to order: ";
	        cin >> id;
	
	        bool found = false;
	        for (size_t i = 0; i < menu.size(); ++i) {
	            if (menu[i].getID() == id) {
	                int qty;
	                cout << "Enter quantity: ";
	                cin >> qty;
	                if (qty <= 0) qty = 1;
	
	                order.push_back({menu[i], qty});
	                totalBill += menu[i].getPrice() * qty;
	                found = true;
	                break;
	            }
	        }
	
	        if (!found)
	            cout << "Invalid ID.\n";
	
	        cout << "Add another item? (y/n): ";
	        cin >> choice;
	
	    } while (choice == 'y' || choice == 'Y');
	
	    ofstream out("Orders.txt", ios::app);
	    out << "--- New Order ---\n";
	    for (const auto& item : order) {
	        out << item.first.getName() << " x" << item.second
	            << " = Rs. " << item.first.getPrice() * item.second << endl;
	    }
	    out << "Total: Rs. " << totalBill << "\n\n";
	    out.close();
	
	    cout << "\nOrder placed successfully!\n";
	    cout << "Your total bill is: Rs. " << totalBill << endl << endl;
	}
	
};

int main() {
    int role;

    do {
        cout << "=================================" << endl;
        cout << "   Cafeteria Management System   " << endl;
        cout << "=================================" << endl;

        cout << "1. Admin" << "\n2. Customer" << "\n3. Exit" << "\nEnter your choice: ";
        cin >> role;
        system("cls");

        switch (role) {
        case 1: {
            Admin admin;
            int adminChoice;
            do {
                cout << "=================" << endl;
                cout << "====  Admin  ====" << endl;
                cout << "=================" << endl;
                cout << "1. View Menu" << "\n2. Add Menu Items" << "\n3. Back" << "\nEnter your choice: ";
                cin >> adminChoice;
                system("cls");

                switch (adminChoice) {
                case 1:
                    admin.displayMenu();
                    break;
                case 2:
                    admin.addMenuItem();
                    break;
                case 3:
                    cout << "Returning...\n";
                    break;
                default:
                    cout << "Invalid option.\n";
                }
            } while (adminChoice != 3);
            break;
        }

        case 2: {
            Customer customer;
            int customerChoice;
            do {
                cout << "1. View Menu" << "\n2. Place Order" << "\n3. Back" << "\nEnter your choice: ";
                cin >> customerChoice;

                switch (customerChoice) {
                case 1:
                    customer.displayMenu();
                    break;
                case 2:
                    customer.placeOrder();
                    break;
                case 3:
                    cout << "Returning...\n";
                    break;
                default:
                    cout << "Invalid option.\n";
                }
            } while (customerChoice != 3);
            break;
        }

        case 3:
            cout << "Goodbye!\n";
            break;

        default:
            cout << "Invalid input.\n";
        }
    } while (role != 3);

    return 0;
}
