#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

//Reduces collisions
int HashString(const string& value) {
    int hashCode = 0;
    for (int i = 0; i < value.size(); ++i) {
        hashCode += (value[i] * pow(31, i)); //Better distribution
    }
    return (int)(hashCode & 0x7fffffff); //Ensure positive value
}

int main() {
    unordered_map<int, vector<string>> passwordTable; //Handles collisions using a vector
    ifstream passwordFile("10-million-password-list-top-10000.txt");

    if (!passwordFile) {
        cerr << "Error: Password file not found!" << endl;
        return 1;
    }

    string password;
    int collisionCount = 0; //Track collisions

    cout << "Reading passwords and hashing...\n";
    while (getline(passwordFile, password)) {
        int hashValue = HashString(password);
        
        //Check for collisions
        if (!passwordTable[hashValue].empty()) {
            ++collisionCount;
        }

        passwordTable[hashValue].push_back(password); //Store in vector to handle collisions
    }
    passwordFile.close();

    //Print the first 10 passwords and their hashes
    cout << "\nFirst 10 passwords and their hashes:\n";
    int count = 0;
    for (const auto& pair : passwordTable) {
        for (const string& pass : pair.second) {
            if (count < 10) {
                cout << "Password: " << pass << ", Hash: " << pair.first << endl;
                ++count;
            }
        }
        if (count >= 10) break;
    }

    cout << "\nHashing complete. Total collisions: " << collisionCount << endl;

    //Reverse lookup
    int userHash;
    while (true) {
        cout << "\nEnter hashed value (or -1 to exit): ";
        cin >> userHash;

        if (userHash == -1) break;

        if (passwordTable.find(userHash) != passwordTable.end()) {
            cout << "Possible original passwords:\n";
            for (const string& pass : passwordTable[userHash]) {
                cout << " - " << pass << endl;
            }
        } else {
            cout << "Password not found in database." << endl;
        }
    }

    cout << "Goodbye!" << endl;
    return 0;
}
