#include<iostream>
#include<string>
#include<fstream>
#include <sstream>
using namespace std;

// Hardcoded values
const int MAX_USERS = 10;
const int MAX_CARS = 10;
const int MAX_HISTORY = 10;


// User data arrays
string usernames[MAX_USERS];
string passwords[MAX_USERS];
double balances[MAX_USERS];
int rentedCarIndex[MAX_USERS];
double rentalAmount[MAX_USERS];
string rentalHistory[MAX_USERS][MAX_HISTORY];
int rentalDays[MAX_USERS][MAX_HISTORY];
int rentalCount[MAX_USERS];
int contactNumbers[MAX_USERS];
string rentalDates[MAX_USERS][MAX_HISTORY];
string returnDates[MAX_USERS][MAX_HISTORY];

// Car data arrays
string carModels[MAX_CARS] = {"Toyota Grande","Toyota Fortuner","Honda Civic","Honda City","BMW i7","BMW M5","Audi r7","Audi A4"};
double carRentPerDay[MAX_CARS] = {200.0, 280.0, 320.0,300.0,450.0,570.0,600.0,670.0};
bool carAvailability[MAX_CARS] = {true, true, true,true,true,true, true, true};

int userCount = 0;
int carCount = 8;

void registerUser();
int loginUser();
void displayPolicies();
void rentCar(int loggedInUserIndex);
void returnCar(int loggedInUserIndex);
void showPortfolio(int loggedInUserIndex);
void displayMenu();
void saveDataToFile();
string calculateReturnDate(string rentDate, int rentalDays);
int main() {
    int choice;
    int loggedInUserIndex = -1;
    

    // Initialize rental tracking arrays
    for (int i = 0; i < MAX_USERS; i++)
	{
        rentedCarIndex[i] = -1;
        rentalAmount[i] = 0.0;
        rentalCount[i] = 0;
    for (int j = 0; j < MAX_HISTORY; j++) {
        rentalHistory[i][j] = "None";
        rentalDays[i][j] = 0;}
    }
    while (true) {
        if (loggedInUserIndex == -1) {
            // Main Menu
            cout << "\n=== Main Menu ===\n";
            cout << "1. Register\n";
            cout << "2. Login\n";
            cout << "3. Exit\n";
            cout << "Enter your choice (1-3): ";
            cin >> choice;

            switch (choice) {
                case 1:
                {
					registerUser();
                    break;
				}
                case 2:
                {
				    loggedInUserIndex = loginUser();
                    if (loggedInUserIndex != -1) {
                        cout << "\nLogin successful!\n";
                    } else {
                        cout << "\nLogin failed. Please check your username or password.\n";
                    }
                    break;
                }
                case 3:
                {
					cout << "\nExiting the program. Goodbye!\n";
                    return 0;
                }
                default:
                {
				    cout << "\nInvalid choice. Please enter a valid option (1-3).\n";
				}
            }
        } else {
            // User Menu
            displayMenu();
            cout << "Enter your choice (1-4): ";
            cin >> choice;

            switch (choice) {
                case 1:
                    rentCar(loggedInUserIndex);
                    break;
                case 2:
                    returnCar(loggedInUserIndex);
                    break;
                case 3:
                    showPortfolio(loggedInUserIndex);
                    break;
                case 4:
                    cout << "\nLogging out...\n";
                    loggedInUserIndex = -1;
                    break;
                default:
                    cout << "\nInvalid choice. Please enter a valid option (1-4).\n";
            }
        }
    }
    return 0;
}

void registerUser() {
    if (userCount >= MAX_USERS) 
	{
        cout << "\nUser limit reached. Cannot register more users.\n";
        return ;
    }
    string username, password;
    double balance;
    int contacts;
    cout << "\n=== Register User ===\n";
    cout << "Enter a new username: ";
    cin >> username;
    cout << "Enter contact number: ";
    cin >> contacts;
    cout << "Enter a new password: ";
    cin >> password;
    cout<<"Enter balance: $";
    cin>>balance;

    // Check if username already exists
    for (int i = 0; i < userCount; i++) {
        if (usernames[i] == username) {
            cout << "\nUsername already exists. Please choose a different username.\n";
            return;
        }
    }
    saveDataToFile();  
    usernames[userCount] = username;
    passwords[userCount] = password;
    balances[userCount] = balance;
    contactNumbers[userCount] = contacts;
    userCount++;
    cout << "\nRegistration successful!\n";
    saveDataToFile();
}

int loginUser() {
    string username, password;
    cout << "\n=== Login ===\n";
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    for (int i = 0; i < userCount; i++) {
        if (usernames[i] == username && passwords[i] == password) {
            return i;
        }
    }
    return -1;
}

void rentCar(int loggedInUserIndex) {
	int agreement;
    displayPolicies();
    cin >> agreement;

    if (agreement != 1)
	{
        cout << "\nYou must agree to the policies to rent a car.\n";
        return;
    }
    cout << "\n=== Rent a Car ===\n";
    cout << "Available Cars:\n";
    for (int i = 0; i < carCount; ++i)
	{
        if (carAvailability[i]) 
		{
            cout << i + 1 << ". Model: " << carModels[i]<< ", Rent/Day: $" << carRentPerDay[i] << "\n";
        }
    }

    int carIndex, days;
    cout << "Enter the car number to rent: ";
    cin >> carIndex;
    cout << "Enter the number of days to rent: ";
    cin >> days;
	string rentDate;
    cout << "Enter rental date (dd-mm-yyyy): ";
    cin >> rentDate;
    string returnDate = calculateReturnDate(rentDate, days);
    
    carIndex--;
    if (carIndex < 0 || carIndex >= carCount || !carAvailability[carIndex]) {
        cout << "\nInvalid car selection. Please choose an available car.\n";
        return;
    }

    double rentAmount = days * carRentPerDay[carIndex];
    if (balances[loggedInUserIndex] < rentAmount) {
        cout << "\nInsufficient balance to rent this car.\n";
        cout << "Would you like to add amount?\n";
        cout<<"1.Yes"<<endl;
        cout<<"2.No"<<endl;
        int choice1;
        cout<<"choice (1 or 2): ";
        cin>>choice1;
        if (choice1 == 1) {
            double addAmount;
            cout << "Enter amount to add: $";
            cin >> addAmount;
            balances[loggedInUserIndex] += addAmount;}
        return;
    }
    balances[loggedInUserIndex] -= rentAmount;
    carAvailability[carIndex] = false;
    rentedCarIndex[loggedInUserIndex] = carIndex;
    rentalAmount[loggedInUserIndex] = rentAmount;
    rentalDates[loggedInUserIndex][rentalCount[loggedInUserIndex]] = rentDate;
    returnDates[loggedInUserIndex][rentalCount[loggedInUserIndex]] = returnDate;

    if (rentalCount[loggedInUserIndex] < MAX_HISTORY) {
    rentalHistory[loggedInUserIndex][rentalCount[loggedInUserIndex]] = carModels[carIndex];
    rentalDays[loggedInUserIndex][rentalCount[loggedInUserIndex]] = days;
    rentalCount[loggedInUserIndex]++;
} else {
    cout << "Rental history is full. Cannot log more rentals.\n";
}
    

    cout << "\nCar rented successfully! Rent amount: $" << rentAmount << "\n";
    saveDataToFile();
}

void returnCar(int loggedInUserIndex) {
    int carIndex = rentedCarIndex[loggedInUserIndex];
    if (carIndex == -1) {
        cout << "\nNo car to return.\n";
        return;
    }   
    carAvailability[carIndex] = true;
    rentedCarIndex[loggedInUserIndex] = -1;
    rentalAmount[loggedInUserIndex] = 0.0;
    cout << "\nCar returned successfully! Thank you for using our service." << "\n";
    saveDataToFile();
}

void showPortfolio(int loggedInUserIndex) {
    cout << "\n=== User Portfolio ===\n";
    cout << "Username: " << usernames[loggedInUserIndex] << "\n";
    cout << "Balance: $" << balances[loggedInUserIndex] << "\n";
    if (rentedCarIndex[loggedInUserIndex] != -1) {
        cout << "Currently Rented Car: " 
             << carModels[rentedCarIndex[loggedInUserIndex]] << "\n";
        cout << "Rental Amount: $" 
             << rentalAmount[loggedInUserIndex] << "\n";
    } else {
        cout << "Currently Rented Car: None\n";
    }

    // Rental history
    cout << "\nRental History:\n";
    if (rentalCount[loggedInUserIndex] == 0) {
        cout << "No rentals yet.\n";
    } else {
        for (int i = rentalCount[loggedInUserIndex] - 1; i >= 0; i--) {
        cout << rentalCount[loggedInUserIndex] - i << ". " << rentalHistory[loggedInUserIndex][i] << " for " << rentalDays[loggedInUserIndex][i] << " day(s)\n"
		<< " | Rented on: " << rentalDates[loggedInUserIndex][i] << " | Return by: " << returnDates[loggedInUserIndex][i] << "\n";
        }       
       }
    }

void displayMenu() {
    cout << "\n=== User Menu ===\n";
    cout << "1. Rent a Car\n";
    cout << "2. Return a Car\n";
    cout << "3. View User Portfolio\n";
    cout << "4. Logout\n";
}
void displayPolicies() {
    cout << "\n=== Rental Policies ===\n";
    cout << "1. You must have sufficient balance to rent a car.\n";
    cout << "2. You can rent only one car at a time.\n";
    cout << "3. Delay in returning the car will lead to balance deduction.\n";
    cout << "4. Misuse or damage of cars may lead to account suspension.\n";
    cout << "Do you agree to the above policies?\n";
    cout << "1. Yes\n";
    cout << "2. No\n";
    cout << "Enter your choice (Press 1 to agree): ";
}
string calculateReturnDate(string rentDate, int rentalDays) {
    int day, month, year;
    char dash;

    stringstream ss(rentDate);
    ss >> day >> dash >> month >> dash >> year;

    int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    day += rentalDays;
    while (day > daysInMonth[month - 1]) {
        day -= daysInMonth[month - 1];
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }

    stringstream result;
    result << (day < 10 ? "0" : "") << day << "-"<< (month < 10 ? "0" : "") << month << "-"<< year;

    return result.str();
}
void saveDataToFile() {
    ofstream file("car_rental_data.txt");
    if (!file) {
        cout << "Error opening file for saving data.\n";
        return;
    }

    file << userCount << "\n";

    for (int i = 0; i < userCount; i++) {
        file <<"Username: "<< usernames[i] << "\n"
             <<"Contact No.: "<< contactNumbers[i] << "\n"
             <<"Password: "<< passwords[i] << "\n"
             << "Current Balance: "<< balances[i] << "\n "
             << "Rented Car Index: "<<rentedCarIndex[i] << "\n "
             << "Rental Amount: "<<rentalAmount[i] << "\n "
             << "Rental Count: "<<rentalCount[i] << "\n";

        for (int j = 0; j < rentalCount[i]; j++)
		{
          file << rentalHistory[i][j] << " "<< rentalDays[i][j] << " "<< "Rented on: "<<rentalDates[i][j] << " "<< "Return by: "<<returnDates[i][j] << "\n";
        }
        file << "\n";
    }

    file.close();
}

