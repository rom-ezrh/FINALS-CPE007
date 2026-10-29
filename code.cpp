#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Appliance
{
    string appl_Name;
    double wattage;
    double hours;
    double energy;
};

struct Bills
{
    int month;
    double cost;
};

// Appliance Section
double kWh(Appliance a)
{
    return (a.wattage / 1000) * a.hours * 30;
}

double costCalc(Appliance b, double rate)
{
    return b.energy * rate;
}

double totalApl(double d, double e)
{
    return d += e;
}

double openAppliance(string aplFile)
{
    ifstream appl_File(aplFile);
    Appliance apl;

    if (!appl_File.is_open())
    {
        cerr << "Cannot open file" << endl;
        return -1;
    }

    double rate;
    double total_kWh = 0;
    double total = 0;

    cout << "What is the rate?: " << endl;
    cin >> rate;

    if (rate < 0)
    {
        cerr << "Error, enter a proper rate" << endl;
        return -1;
    }

    while (appl_File >> apl.appl_Name >> apl.wattage >> apl.hours)
    // finds the name first, then wattage, then hours
    {
        if (apl.wattage < 0)
        {
            cerr << "Error, wattage is a negative number" << endl;
            return -1;
        }

        if (apl.hours < 0)
        {
            cerr << "Error, hours is a negative number" << endl;
            return -1;
        }

        apl.energy = kWh(apl);
        double cost = costCalc(apl, rate);
        total = totalApl(total, cost);
        total_kWh += apl.energy;

        cout << apl.appl_Name << " | " << apl.wattage << " Watts | " << apl.hours << " Hours | " << apl.energy << " kWh | " << cost << " PHP | " << endl;
    }

    cout << "The Cost for this month's bill based on the given appliances is: " << total << " PHP" << endl;
    cout << "The total consumed energy is: " << total_kWh << " kWh" << endl;

    appl_File.close();
    return total;
}

// Bills Sections

double average(double f, int num)
{
    return f / num;
}

double openBills(string billFile)
{
    ifstream bill_File(billFile);
    Bills blf;
    double total = 0;
    int bCounter = 0;

    if (!bill_File.is_open())
    {
        cerr << "Error, cannot open file" << endl;
        return -1;
    }

    while (bill_File >> blf.month >> blf.cost)
    {
        cout << "Month " << blf.month << " " << blf.cost << " PHP" << endl;
        total += blf.cost;
        bCounter++;
    }

    if (bCounter == 0)
    {
        cerr << "Error, no bills found in file" << endl;
        return -1;
    }

    double final_avg = average(total, bCounter);
    return final_avg;
}

// estimate section

void estimate(double g, double h)
{
    double estim = (g + h) / 2;
    cout << "The estimated bill for this month will be " << estim << " PHP" << endl;
}

// rewriting files
void copyAppliancesBack(string fileName)
{
    ifstream tempFile("temp.txt");
    ofstream newFile(fileName);
    Appliance a;

    while (tempFile >> a.appl_Name >> a.wattage >> a.hours)
    {
        newFile << a.appl_Name << " " << a.wattage << " " << a.hours << endl;
    }

    tempFile.close();
    newFile.close();
}

void copyBillsBack(string fileName)
{
    ifstream tempFile("temp.txt");
    ofstream newFile(fileName);
    Bills b;

    while (tempFile >> b.month >> b.cost)
    {
        newFile << b.month << " " << b.cost << endl;
    }

    tempFile.close();
    newFile.close();
}

// APPLIANCES: Read
int viewAppliances(string fileName)
{
    ifstream file(fileName);
    Appliance a;
    int count = 0;

    while (file >> a.appl_Name >> a.wattage >> a.hours)
    {
        count++;
        a.energy = kWh(a);
        cout << count << ". " << a.appl_Name << " | " << a.wattage << " Watts | " << a.hours << " Hours | " << a.energy << " kWh" << endl;
    }

    file.close();

    if (count == 0)
    {
        cout << "No appliances saved yet." << endl;
    }

    return count;
}

// APPLIANCES: Create
void createAppliance(string fileName)
{
    Appliance a;

    cout << "Appliance name (no spaces, use _ instead): ";
    cin >> a.appl_Name;
    cout << "Wattage: ";
    cin >> a.wattage;
    cout << "Hours used per day: ";
    cin >> a.hours;

    if (a.wattage < 0 || a.hours < 0)
    {
        cerr << "Error, wattage and hours cannot be negative" << endl;
        return;
    }

    ofstream file(fileName, ios::app);
    file << a.appl_Name << " " << a.wattage << " " << a.hours << endl;
    file.close();

    cout << "Appliance added." << endl;
}

// APPLIANCES: Update
void updateAppliance(string fileName)
{
    int total = viewAppliances(fileName);

    if (total == 0)
    {
        return;
    }

    int choice;
    cout << "Enter the number of the appliance to update: ";
    cin >> choice;

    if (choice < 1 || choice > total)
    {
        cerr << "Error, invalid number" << endl;
        return;
    }

    Appliance newApl;

    cout << "New name (no spaces): ";
    cin >> newApl.appl_Name;
    cout << "New wattage: ";
    cin >> newApl.wattage;
    cout << "New hours: ";
    cin >> newApl.hours;

    if (newApl.wattage < 0 || newApl.hours < 0)
    {
        cerr << "Error, wattage and hours cannot be negative. Nothing was saved." << endl;
        return;
    }

    ifstream oldFile(fileName);
    ofstream tempFile("temp.txt");
    Appliance a;
    int count = 0;

    while (oldFile >> a.appl_Name >> a.wattage >> a.hours)
    {
        count++;

        if (count == choice)
        {
            // UPDATES VALUES
            tempFile << newApl.appl_Name << " " << newApl.wattage << " " << newApl.hours << endl;
        }
        else
        {
            // KEEPS THE SAME
            tempFile << a.appl_Name << " " << a.wattage << " " << a.hours << endl;
        }
    }

    oldFile.close();
    tempFile.close();

    copyAppliancesBack(fileName);

    cout << "Appliance updated." << endl;
}

// APPLIANCES: Delete
void deleteAppliance(string fileName)
{
    int total = viewAppliances(fileName);

    if (total == 0)
    {
        return;
    }

    int choice;
    cout << "Enter the number of the appliance to delete: ";
    cin >> choice;

    if (choice < 1 || choice > total)
    {
        cerr << "Error, invalid number" << endl;
        return;
    }

    ifstream oldFile(fileName);
    ofstream tempFile("temp.txt");
    Appliance a;
    int count = 0;

    while (oldFile >> a.appl_Name >> a.wattage >> a.hours)
    {
        count++;

        if (count != choice)
        {
            tempFile << a.appl_Name << " " << a.wattage << " " << a.hours << endl; // skips entry as if to delete
        }
    }

    oldFile.close();
    tempFile.close();

    copyAppliancesBack(fileName);

    cout << "Appliance deleted." << endl;
}

// BILLS: Read
int viewBills(string fileName)
{
    ifstream file(fileName);
    Bills b;
    int count = 0;

    while (file >> b.month >> b.cost)
    {
        count++;
        cout << count << ". Month " << b.month << " | " << b.cost << " PHP" << endl;
    }

    file.close();

    if (count == 0)
    {
        cout << "No bills saved yet." << endl;
    }

    return count;
}

// BILLS: Create
void createBill(string fileName)
{
    Bills b;

    cout << "Month (1-12): ";
    cin >> b.month;
    cout << "Bill amount (PHP): ";
    cin >> b.cost;

    if (b.month < 1 || b.month > 12 || b.cost < 0)
    {
        cerr << "Error, month must be 1-12 and the amount cannot be negative" << endl;
        return;
    }

    ofstream file(fileName, ios::app);
    file << b.month << " " << b.cost << endl;
    file.close();

    cout << "Bill added." << endl;
}

// BILLS: Update
void updateBill(string fileName)
{
    int total = viewBills(fileName);

    if (total == 0)
    {
        return;
    }

    int choice;
    cout << "Enter the number of the bill to update: ";
    cin >> choice;

    if (choice < 1 || choice > total)
    {
        cerr << "Error, invalid number" << endl;
        return;
    }

    Bills newBill;

    cout << "New month (1-12): ";
    cin >> newBill.month;
    cout << "New amount (PHP): ";
    cin >> newBill.cost;

    if (newBill.month < 1 || newBill.month > 12 || newBill.cost < 0)
    {
        cerr << "Error, month must be 1-12 and the amount cannot be negative. Nothing was saved." << endl;
        return;
    }

    ifstream oldFile(fileName);
    ofstream tempFile("temp.txt");
    Bills b;
    int count = 0;

    while (oldFile >> b.month >> b.cost)
    {
        count++;

        if (count == choice)
        {
            tempFile << newBill.month << " " << newBill.cost << endl;
        }
        else
        {
            tempFile << b.month << " " << b.cost << endl;
        }
    }

    oldFile.close();
    tempFile.close();

    copyBillsBack(fileName);

    cout << "Bill updated." << endl;
}

// BILLS: Delete
void deleteBill(string fileName)
{
    int total = viewBills(fileName);

    if (total == 0)
    {
        return;
    }

    int choice;
    cout << "Enter the number of the bill to delete: ";
    cin >> choice;

    if (choice < 1 || choice > total)
    {
        cerr << "Error, invalid number" << endl;
        return;
    }

    ifstream oldFile(fileName);
    ofstream tempFile("temp.txt");
    Bills b;
    int count = 0;

    while (oldFile >> b.month >> b.cost)
    {
        count++;

        if (count != choice)
        {
            tempFile << b.month << " " << b.cost << endl;
        }
    }

    oldFile.close();
    tempFile.close();

    copyBillsBack(fileName);

    cout << "Bill deleted." << endl;
}

void applianceMenu()
{
    int choice;

    do
    {
        cout << "--- APPLIANCES ---" << endl;
        cout << "1. Add appliance" << endl;
        cout << "2. View appliances" << endl;
        cout << "3. Update appliance" << endl;
        cout << "4. Delete appliance" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
            createAppliance("appliances.txt");
        else if (choice == 2)
            viewAppliances("appliances.txt");
        else if (choice == 3)
            updateAppliance("appliances.txt");
        else if (choice == 4)
            deleteAppliance("appliances.txt");
        else if (choice != 0)
            cout << "Invalid choice" << endl;

    } while (choice != 0);
}

void billsMenu()
{
    int choice;

    do
    {
        cout << "--- BILLS ---" << endl;
        cout << "1. Add bill" << endl;
        cout << "2. View bills" << endl;
        cout << "3. Update bill" << endl;
        cout << "4. Delete bill" << endl;
        cout << "0. Back" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
            createBill("bills.txt");
        else if (choice == 2)
            viewBills("bills.txt");
        else if (choice == 3)
            updateBill("bills.txt");
        else if (choice == 4)
            deleteBill("bills.txt");
        else if (choice != 0)
            cout << "Invalid choice" << endl;

    } while (choice != 0);
}

void runEstimate()
{
    double final_total = openAppliance("appliances.txt");

    if (final_total == -1)
    {
        return;
    }

    double bill_average = openBills("bills.txt");

    if (bill_average == -1)
    {
        return;
    }

    cout << "The possible cost for this month based on previous bills is " << bill_average << " PHP" << endl;

    estimate(final_total, bill_average);
}

int main()
{
    cout << "ELECTRIC BILL CALCULATOR" << endl;
    cout << "PLEASE HAVE THE NECESSARY FILES WITHIN" << endl;
    cout << "USE PROPER FILE NAME //appliances.txt// and //bills.txt//" << endl;

    int choice;

    do
    {
        cout << "===== MAIN MENU =====" << endl;
        cout << "1. Manage appliances" << endl;
        cout << "2. Manage bills" << endl;
        cout << "3. Calculate estimated bill" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1)
            applianceMenu();
        else if (choice == 2)
            billsMenu();
        else if (choice == 3)
            runEstimate();
        else if (choice != 0)
            cout << "Invalid choice" << endl;

    } while (choice != 0);

    return 0;
}
