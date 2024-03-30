#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    while (true) {
        cout << "To encrypt text, press 1. \n" <<
             "To decrypt text, press 2. \n" <<
             "To exit, press 3: \n";
        int command;
        cin >> command;

        if (command == 3) {
            break;
        }

        cout << "Enter the key word: \n";
        string key;
        cin >> key;
        key.erase(unique(key.begin(), key.end()), key.end());

        cout << "Enter the number (1-26): \n";
        int num;
        bool flag = 0;
        while (!flag) {
            cin >> num;
            if (num >= 1 && num <= 26) flag = 1; else cout << "Invalid number. Please enter a number between 1 and 26: \n";
        }

        string text;
        cin.ignore();
        if (command == 1) {
            cout << "Enter the text that needs to be encrypted: \n";
            getline(cin, text);
        } else if (command == 2) {
            cout << "Enter the text that needs to be decrypted: \n";
            getline(cin, text);
        }

        vector<char> alphabet(26);
        for (int i = 0; i < key.length(); i++) {
            alphabet[num + i] = key[i];
        }

        vector<char> a;
        for (int k = 97; k < 123; k++) {
            if (std::find(key.begin(), key.end(), char(k)) == key.end()) {
                a.push_back(char(k));
            }
        }

        int start = num + key.length();
        int n = 0;
        for (int j = start; j < 26; j++) {
            alphabet[j] = a[n];
            n++;
        }
        if (num != 0) {
            for (int l = 0; l < num; l++) {
                alphabet[l] = a[n];
                n++;
            }
        }

        if (command == 1) {
            string encryptedText;
            for (int i = 0; i < text.length(); i++) {
                if (text[i] == ' ') {
                    encryptedText += ' ';
                } else {
                    encryptedText += alphabet[text[i] - 97];
                }
            }
            cout << "Encrypted text: \n" << encryptedText << endl;
        }

        if (command == 2) {
            string decryptedText;
            for (int i = 0; i < text.length(); i++) {
                if (text[i] == ' ') {
                    decryptedText += ' ';
                } else {
                    for (int k = 0; k < alphabet.size(); k++) {
                        if (text[i] == alphabet[k]) {
                            decryptedText += char(k + 97);
                            break;
                        }
                    }
                }
            }
            cout << "Decrypted text: \n" << decryptedText << endl;
        }
    }

    return 0;
}

