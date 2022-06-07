/***********************************************************************************
 * Title        : CS-103 Integrated Studio I Assessment 2: Vehicle Insurance System
 * File         : policies.cpp
 * Purpose      : Program file to manage Insurance Policies in the Vehicle Insurance System
 * Parameters   : struct User user
 * Returns      : None
 ************************************************************************************/

#include <iostream>
#include <string>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include "policies.h"

using namespace std;

// Structure definitions here
struct User
{
    string email;
    string password;
    string firstname;
    string lastname;
    string phone;
    int accessLevel;

    // Constructor to initialize members
    User(string em = "", string passwd = "", string fname = "", string lname = "", string ph = "", int access = 1)
    {

        email = em;
        password = passwd;
        firstname = fname;
        lastname = lname;
        phone = ph;
        accessLevel = access;
    }
};

struct Policy
{
    string policyNum;
    string username;
    string carMake;
    string carColor;
    string carRego;
    string dateInsured;
    string dateExpiry;
    char typeCover; // C-Comprehensive, F-Fire/Theft/Third Party, T-Third Party Only
    int carInsuredAmount;
    int excessAmount;
    float premiumTotalAmount; // C=6% of InsuredAmount, F=6% of InsuredAmount, T=10% of InsuredAmount --> for 12-months coverage
    float premiumPayAmount;   // premiumTotalAmount/payFrequency
    char payFrequency;        // W-Weekly, F-Fortnightly, M-Monthly

    // Constructor to initialize members
    Policy(string policyno = "", string uname = "", string cmake = "", string ccolor = "", string crego = "", string dstart = "", string dend = "", char tcover = 'C', int insured = 0, int excess = 0, float ptot = 0.00, float pamt = 0.00, char freq = 'F')
    {
        policyNum = policyno;
        username = uname;
        carMake = cmake;
        carColor = ccolor;
        carRego = crego;
        dateInsured = dstart;
        dateExpiry = dend;
        typeCover = tcover; // C-Comprehensive, F-Fire/Theft/Third Party, T-Third Party Only
        carInsuredAmount = insured;
        excessAmount = excess;
        premiumTotalAmount = ptot; // C=6% of InsuredAmount, F=8% of InsuredAmount, T=10% of InsuredAmount --> for 12-months coverage
        premiumPayAmount = pamt;   // premiumTotalAmount/payFrequency
        payFrequency = freq;       // W-Weekly, F-Fortnightly, M-Monthly
    }
};

// Function Prototypes here
int getLastPolicyNum();
void createPolicy(struct User user);
void savePolicy(struct Policy policy);
void showPolicy(struct Policy policy, string name);
vector<Policy> readPolicyFile(struct User user);
void viewPolicy(struct User user);
void showAdminPolicyMenu(struct User user);
void showUserPolicyMenu(struct User user);

void showPoliciesMenu(struct User user)
{
    // int choice = 0;
    vector<Policy> policy;

    if (user.accessLevel > 1)
    {
        showAdminPolicyMenu(user);
    }
    else
    {
        showUserPolicyMenu(user);
    }
}

void showAdminPolicyMenu(struct User user)
{
    int choice = 0;
    vector<Policy> policy;

    while (choice != 5)
    {
        cout << "\nManage Insurance Policies" << endl;
        cout << "============================================" << endl;
        cout << "[1] Create New Policy" << endl;
        cout << "[2] View Policy" << endl;
        cout << "[3] Edit Policy" << endl;
        cout << "[4] Delete Policy" << endl;
        cout << "[5] Exit" << endl;
        cout << "============================================" << endl;
        cout << "Choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            createPolicy(user);
            break;
        case 2:
            viewPolicy(user);
            break;
        case 3:
            cout << "[3] Edit Policy" << endl;
            break;
        case 4:
            cout << "[4] Delete Policy" << endl;
            break;
        default:
            break;
        }
    }
}

void showUserPolicyMenu(struct User user)
{
    int choice = 0;
    vector<Policy> policy;

    while (choice != 3)
    {
        cout << "\nManage Insurance Policy for: " << user.firstname << " " << user.lastname << endl;
        cout << "============================================" << endl;
        cout << "[1] Create New Policy" << endl;
        cout << "[2] View Policy" << endl;
        cout << "[3] Exit" << endl;
        cout << "============================================" << endl;
        cout << "Choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            createPolicy(user);
            break;
        case 2:
            viewPolicy(user);
            break;
        default:
            break;
        }
    }
}
void createPolicy(struct User user)
{
    int lastPolicyNum = getLastPolicyNum() + 1;
    char ans = 'n';

    // save here the lastPolicyNum to the policy.txt file -- replace existing data
    fstream policyFile("policyNum.txt", ios::out); // open file in write mode
    policyFile << lastPolicyNum << endl;
    policyFile.close();

    struct Policy policy;
    string name = user.firstname + " " + user.lastname;

    policy.policyNum = to_string(lastPolicyNum);
    policy.username = user.email;

    while (toupper(ans) != 'Y')
    {
        cout << endl;
        cout << "               Policy No.: " << lastPolicyNum << endl;
        cout << "      Creating policy for: " << name << endl;
        cout << "============================================" << endl;
        cout << "                 Car Make: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // This clears the input buffer - helps in getting the getline() function called to do its job (get input from user) instead of skipping it because of the newline character stuffed before.
        getline(cin, policy.carMake);
        cout << "                Car Color: ";
        getline(cin, policy.carColor);
        cout << "                 Car REGO: ";
        getline(cin, policy.carRego);
        cout << "               Start Date: ";
        getline(cin, policy.dateInsured);
        cout << "              Expiry Date: ";
        getline(cin, policy.dateExpiry);
        cout << "         Coverage (C/F/T): ";
        cin >> policy.typeCover;
        cout << "           Insured Amount: ";
        cin >> policy.carInsuredAmount;
        cout << "            Excess Amount: ";
        cin >> policy.excessAmount;
        cout << "Payment Frequency (W/F/M): ";
        cin >> policy.payFrequency;

        switch (toupper(policy.typeCover))
        {
        case 'C':
            policy.premiumTotalAmount = policy.carInsuredAmount * 0.06;
            break;
        case 'F':
            policy.premiumTotalAmount = policy.carInsuredAmount * 0.08;
            break;
        case 'T':
            policy.premiumTotalAmount = policy.carInsuredAmount * 0.10;
            break;

        default:
            break;
        }

        switch (toupper(policy.payFrequency))
        {
        case 'W':
            policy.premiumPayAmount = policy.premiumTotalAmount / 48;
            break;
        case 'F':
            policy.premiumPayAmount = policy.premiumTotalAmount / 24;
            break;
        case 'M':
            policy.premiumPayAmount = policy.premiumTotalAmount / 12;
            break;

        default:
            break;
        }
        showPolicy(policy, name);
        cout << "Save policy (y/n)? ";
        cin >> ans;
    }

    savePolicy(policy);
}

void showPolicy(struct Policy policy, string name)
{
    cout << endl;
    cout << "============================================" << endl;
    cout << "               Policy No.: " << policy.policyNum << endl;
    cout << "      Creating policy for: " << name << endl;
    cout << "============================================" << endl;
    cout << "                 Car Make: " << policy.carMake << endl;
    cout << "                Car Color: " << policy.carColor << endl;
    cout << "                 Car REGO: " << policy.carRego << endl;
    cout << "               Start Date: " << policy.dateInsured << endl;
    cout << "              Expiry Date: " << policy.dateExpiry << endl;
    cout << "         Coverage (C/F/T): " << policy.typeCover << endl;
    cout << "           Insured Amount: " << policy.carInsuredAmount << endl;
    cout << "            Excess Amount: " << policy.excessAmount << endl;
    cout << "Payment Frequency (W/F/M): " << policy.payFrequency << endl;
    cout << "============================================" << endl;
}

void savePolicy(struct Policy policy)
{
    cout << "Policy No.: " << policy.policyNum;
    fstream policyFile("policy.csv", ios::app); // open file in append mode
    policyFile << policy.policyNum << "," << policy.username << "," << policy.carMake << "," << policy.carColor << "," << policy.carRego << "," << policy.dateInsured << "," << policy.dateExpiry << "," << policy.typeCover << "," << policy.carInsuredAmount << "," << policy.excessAmount << "," << policy.premiumTotalAmount << "," << policy.premiumPayAmount << "," << policy.payFrequency << endl;
    policyFile.close();
}

void viewPolicy(struct User user)
{
    vector<Policy> policy;

    policy = readPolicyFile(user);
    string msg = user.accessLevel > 1 ? "Viewing all policies" : "Viewing Policy for: " + user.firstname + " " + user.lastname;

    cout << endl;
    cout << msg << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << setw(5)
         << "Policy"
         << setw(11)
         << "Email"
         << setw(25)
         << "CarMake"
         << setw(15)
         << "Color"
         << setw(12)
         << "REGO"
         << setw(12)
         << "Covered"
         << setw(6)
         << "Type"
         << setw(8)
         << "Start"
         << setw(10)
         << "Expiry"
         << setw(12)
         << "T.Premium"
         << setw(8)
         << "Excess"
         << setw(8)
         << "Regular"
         << setw(9)
         << "PayFreq"
         << endl;
    cout << "---------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    for (int i = 0; i < (int)policy.size(); i++)
    {
        cout << setw(5) << policy[i].policyNum
             << setw(16) << policy[i].username
             << setw(25) << policy[i].carMake
             << setw(15) << policy[i].carColor
             << setw(10) << policy[i].carRego
             << setw(10) << policy[i].carInsuredAmount
             << setw(4) << char(toupper(policy[i].typeCover))
             << setw(11) << policy[i].dateInsured
             << setw(11) << policy[i].dateExpiry
             << setw(6) << policy[i].premiumTotalAmount
             << setw(10) << policy[i].excessAmount
             << setw(8) << policy[i].premiumPayAmount
             << setw(7) << char(toupper(policy[i].payFrequency))
             << endl;
    }
    cout << "---------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
}

vector<Policy> readPolicyFile(struct User user)
{
    vector<Policy> tmpPolicy;
    struct Policy policy;
    string txtLine;

    const char *charTypeCover;
    const char *charPayFreq;

    fstream policyFile("policy.csv", ios::in);

    while (getline(policyFile, txtLine))
    {
        istringstream linestream(txtLine); // to split the row into columns/properties

        string item;

        getline(linestream, item, ',');
        policy.policyNum = item;

        getline(linestream, item, ',');
        policy.username = item;

        getline(linestream, item, ',');
        policy.carMake = item;

        getline(linestream, item, ',');
        policy.carColor = item;

        getline(linestream, item, ',');
        policy.carRego = item;

        getline(linestream, item, ',');
        policy.dateInsured = item;

        getline(linestream, item, ',');
        policy.dateExpiry = item;

        getline(linestream, item, ',');
        charTypeCover = item.c_str();
        policy.typeCover = charTypeCover[0];

        getline(linestream, item, ',');
        policy.carInsuredAmount = stoi(item);

        getline(linestream, item, ',');
        policy.excessAmount = stoi(item);

        getline(linestream, item, ',');
        policy.premiumTotalAmount = stoi(item);

        getline(linestream, item, ',');
        policy.premiumPayAmount = stoi(item);

        getline(linestream, item, ',');
        charPayFreq = item.c_str();
        policy.payFrequency = charPayFreq[0];

        if ((user.accessLevel == 1 && policy.username == user.email) || user.accessLevel > 1)
        {
            tmpPolicy.push_back(policy); // save only when it meets the condition
        }
    }
    policyFile.close();

    return tmpPolicy;
}

int getLastPolicyNum()
{
    string txtLine;
    int policyNo = 0;

    fstream policyNumFile("policyNum.txt", ios::in);

    while (getline(policyNumFile, txtLine))
    {
        istringstream linestream(txtLine); // to split the row into columns/properties
        string item;
        getline(linestream, item, '\n');
        policyNo = stoi(item);
    }
    policyNumFile.close();

    return policyNo;
}