#include<iostream>
using namespace std;
int main()
{
    int choice;
    do{
        cout << "\n========================================\n";
        cout << "Stock Exchange Simulator\n";
        cout << "1. Create Account\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin  >> choice;

        switch(choice)
        {
            case 1:
                cout << "\nCreate Account Feature Coming Soon...\n";
                break;

            case 2:
                cout << "\nLogin Feature Coming Soon...\n";
                break;

            case 3:
                cout << "\nThank You For Using Stock Exchange Simulator.\n";
                break;

            default:
                cout << "\nInvalid Choice. Please Try Again.\n";
        }
    }
    while(choice != 3);
}