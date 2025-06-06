#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <string>
#include <sstream>
unsigned numVariables = 0;
using namespace std;


//function to get the boolean term letters
string getVariables(string minterm)
{
    vector<string> vars = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P",
                           "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
    string result = "";
    int len = minterm.length();
    for (int i = 0; i < len; ++i)
    {
        if (minterm[i] == '-')
            continue;
        if (minterm[i] == '1')
            result += vars[i];
        else if (minterm[i] == '0')
            result += vars[i] + "'";
    }
    return result.empty() ? "1" : result;
}

//function to convert decimal to binary
string decToBin(int n, unsigned numBits)
{
    string binary = "";
    for (int i = numBits - 1; i >= 0; --i)
    {
        binary += ((n & (1 << i)) ? '1' : '0');
    }
    return binary;
}
//function to convert binary to decimal
int binToDec(const string &binary)
{
    int decimal = 0;
    for (int i = 0; i < binary.length(); ++i)
    {
        if (binary[i] == '1')
            decimal += (1 << (binary.length() - 1 - i));
    }
    return decimal;
}
//function to check if a minterm is valid
bool validMinterm(int n, int minterm)
{
    return minterm >= 0 && minterm < (1 << n);
}


//function to replace valid-gray code complements with '-'
string replaceComplements(const string &a, const string &b)
{
    string temp = "";
    for (int i = 0; i < a.length(); ++i)
    {
        if (a[i] != b[i])
            temp += '-';
        else
            temp += a[i];
    }
    return temp;
}


//function to check if two minterms differ by exactly one bit
bool isValidPair(const string &a, const string &b)
{
    int diffCount = 0;
    for (int i = 0; i < a.length(); ++i)
    {
        if (a[i] != b[i])
            diffCount++;
        if (diffCount > 1)
            return false;
    }
    return diffCount == 1;
}
//function to check if a string exists in a vector
bool in_vector(const vector<string> &vec, const string &str)
{
    return find(vec.begin(), vec.end(), str) != vec.end();
}

//function to combine minterms that differ by one bit
vector<string> combinedMinterms(const vector<string> &minterms)
{
    vector<string> newMinterms;
    vector<bool> used(minterms.size(), false);

    for (int i = 0; i < minterms.size(); ++i)
    {
        for (int j = i + 1; j < minterms.size(); ++j)
        {
            if (isValidPair(minterms[i], minterms[j]))
            {
                string combined = replaceComplements(minterms[i], minterms[j]);
                if (!in_vector(newMinterms, combined))
                {
                    newMinterms.push_back(combined);
                    used[i] = true;
                    used[j] = true;
                }
            }
        }
    }

    for (int i = 0; i < minterms.size(); ++i)
    {
        if (!used[i])
            newMinterms.push_back(minterms[i]);
    }

    return newMinterms;
}
int main()
{
    char check = 'y';
    printf("Welcome to the algorithm Implementation\n");
    while (check == 'y')
    {
        ios_base::sync_with_stdio(false);
        cin.tie(nullptr);
        cout.tie(nullptr);

        vector<string> minterms;

        printf("Enter the number of variables:\n");
        cin >> numVariables;

        printf("Enter the minterms(from 0 to %d) seperated by a comma:\n", (1 << numVariables) - 1);
        string input;
        cin >> input;

        printf("Enter the don't-care minterms(from 0 to %d) seperated by a comma (-1 there's not any):\n", (1 << numVariables) - 1);
        string dontCareInput;
        cin >> dontCareInput;

        if (!dontCareInput.empty() && dontCareInput != "-1")
        {
            stringstream ss(dontCareInput);
            string number;
            while (getline(ss, number, ','))
            {
                if (validMinterm(numVariables, stoi(number)) == false)
                {
                    printf("Invalid don't care minterm: %s. Please enter a valid minterm between 0 and %d.\n", number.c_str(), (1 << numVariables) - 1);
                    return 1;
                }
                int dontCareMinterm = stoi(number);
                minterms.push_back(decToBin(dontCareMinterm, numVariables));
            }
        }

        stringstream ss(input);
        string number;

        while (getline(ss, number, ','))
        {
            if (validMinterm(numVariables, stoi(number)) == false)
            {
                printf("Invalid minterm: %s. Please enter a valid minterm between 0 and %d.\n", number.c_str(), (1 << numVariables) - 1);
                return 1;
            }
            int minterm = stoi(number);
            minterms.push_back(decToBin(minterm, numVariables));
        }
        sort(minterms.begin(), minterms.end());
        minterms.erase(unique(minterms.begin(), minterms.end()), minterms.end());

        do
        {
            minterms = combinedMinterms(minterms); //combines until no more combinations can be made
            sort(minterms.begin(), minterms.end());
        } while (minterms != combinedMinterms(minterms));

        printf("The prime implicants in SOP form:\n");
        for (int i = 0; i < minterms.size(); ++i)
        {
            if (i > 0)
                cout << " + ";
            cout << getVariables(minterms[i]);
        }

        cout << endl;
        printf("Would you like to enter another expression? (y/n)\n");
        cin >> check;
        if (check != 'y' && check != 'Y')
        {
            break;
        }
    }
    //created by: "Mohanad Amr"
}
