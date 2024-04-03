#include <iostream>
#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
mutex mtx;

//Encryption/Decryption
void workWithText(int act, const string& key, int num, const string& text) {
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    string alphabet0 = alphabet;

    string ukey = "";
    for (int i = 0; i < key.length(); i++) {
        char ch = key[i];
        if (find(ukey.begin(), ukey.end(), ch) == ukey.end()) {
            ukey += ch;
        }
    }

    for (int i = 0; i < key.length(); i++) {
        char ch = key[i];
        alphabet0.erase(remove(alphabet0.begin(), alphabet0.end(), ch), alphabet0.end());
    }

    alphabet0 = ukey + alphabet0;
    alphabet0 = alphabet0.substr(26 - num) + alphabet0.substr(0, 26 - num);
    string finalText;

    //Encryption
    if (act == 1) {
        for (int i = 0; i < text.length(); i++) {
            char ch = text[i];
            if (ch >= 97 && ch <= 123) {
                size_t j = alphabet.find(ch);
                finalText += alphabet0[j];
            } else {
                finalText += ch;
            }
        }
    }

    //Decryption
    if (act == 2) {
        for (int i = 0; i < text.length(); i++) {
            char ch = text[i];
            if (ch >= 97 && ch <= 123) {
                size_t j = alphabet0.find(ch);
                finalText += alphabet[j];
            } else {
                finalText += ch;
            }
        }
    }

    //Correct stream output
    {
        lock_guard<std::mutex> lock(mtx);
        cout << finalText;
    }
}


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
            if (num >= 1 && num <= 26) flag = 1;
            else cout << "Invalid number. Please enter a number between 1 and 26: \n";
        }

        cout << "Enter the text. Press enter twice to finish:\n";
        cin.ignore();

        //Saving everything entered into the text
        string text, part1, part2, part3;
        string line;
        while (getline(cin, line)) {
            if (line.empty()) {
                break;
            }
            text += line + '\n';
        }

        //For each stream - 1/3 of the text
        int partLength = text.length() / 3;

        part1 = text.substr(0, partLength);
        part2 = text.substr(partLength, partLength);
        part3 = text.substr(2 * partLength);

        cout << (command == 1 ? "Encrypted text: " : "Decrypted text: ") << endl;

        thread t1(workWithText, command, key, num, part1);
        thread t2(workWithText, command, key, num, part2);
        thread t3(workWithText, command, key, num, part3);

        t1.join();
        t2.join();
        t3.join();
    }

    return 0;
}
