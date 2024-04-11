#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <thread>
#include <mutex>
#include <condition_variable>

#include <ctime>

using namespace std;
mutex mtx;
condition_variable cv1, cv2, cv3;
int current_thread = 1; // Variable to keep track of the current thread



//Encryption/Decryption
void workWithText(int act, const string& key, int num, const string& text, int flag) {
    unique_lock<mutex> lock(mtx);

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
    // The thread waits for its turn
    while (current_thread != flag) {
        if (flag == 2)
            cv2.wait(lock);
        else if (flag == 3)
            cv3.wait(lock);
    }
    
    cout << finalText;
    
    // Switch to the next stream
    if (current_thread == 1) {
        current_thread = 2;
        cv2.notify_one();
    }
    else if (current_thread == 2) {
        current_thread = 3;
        cv3.notify_one();
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
        
        cout << (command == 1 ? "Encrypted text: " : "Decrypted text: ") << endl;
        
        //For each stream - 1/3 of the text
        int partLength = text.length() / 3;

        part1 = text.substr(0, partLength);
        part2 = text.substr(partLength, partLength);
        part3 = text.substr(2 * partLength);

        int flag1 = 1, flag2 = 2, flag3 = 3;
        thread t1(workWithText, command, key, num, part1, flag1);
        thread t2(workWithText, command, key, num, part2, flag2);
        thread t3(workWithText, command, key, num, part3, flag3);
        
        //Start with the first thread
        cv1.notify_one();
        
        t1.join();
        t2.join();
        t3.join();
    }

    return 0;
}
