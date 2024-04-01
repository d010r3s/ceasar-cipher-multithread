#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
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
            if (num >= 1 && num <= 26) flag = 1; 
            else cout << "Invalid number. Please enter a number between 1 and 26: \n";
        }
        
        //Начинаются изменения
        //до меня тут дошло, что если мы уже выбрали из 1 или 2, то смысла
        //в том, чтобы еще раз все писать нету. Но если надо, то могу вернуть.

        string text;
        string filename; //будущее название файла
        cout << "Enter the name of the file: \n"; //файл пишем вместе с расширением (file.txt)
        cin >> filename;  

        ifstream fileIn(filename); //открыли поток/файл
        if (!fileIn.is_open()) {
            cerr << "Error opening file!" << endl;
            return 1;
        }

        getline(fileIn, text); //прочитали что там в файле в text
        fileIn.close(); //закрыли поток

        vector<char> alphabet(26);
        for (int i = 0; i < key.length(); i++) {
            alphabet[num + i] = key[i];
        }

        vector<char> a;
        for (int k = 97; k < 123; k++) {
            if (find(key.begin(), key.end(), char(k)) == key.end()) {
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
                if (!(text[i] >= 'a' && text[i] <= 'z')){
                    encryptedText += text[i];
                }
                else{
                    encryptedText += alphabet[text[i] - 97];
                }
            }
            ofstream fileOut("encryptedText.txt"); //открыли другой поток и создали по сути новый файл
            if(!fileOut.is_open()) {
                cerr << "Error creating file!" << std::endl;
                return 1;
            }
            fileOut << encryptedText;
            fileOut.close();
            cout << "Text has been encrypted and saved to encryptedText.txt" << endl;
        }

        if (command == 2) {
            string decryptedText;
            for (int i = 0; i < text.length(); i++) {
                if (!(text[i] >= 'a' && text[i] <= 'z')){ //теперь можно использовать все знаки препинания, из-за гетлайна энтер нельзя использовать 
                    decryptedText += text[i];             //(понятия не имею как это исправить)
                }else{
                    for (int k = 0; k < alphabet.size(); k++){
                        if (text[i] == alphabet[k]){
                            decryptedText += char(k + 97);
                            break;
                        }
                    }
                }
            }
            
            ofstream fileOut("decryptedText");
            if(!fileOut.is_open()) {
                cerr << "Error creating file!" << std::endl;
                return 1;
            }
            fileOut << decryptedText;
            fileOut.close();
            cout << "Text has been decrypted and saved to decryptedText" << endl;
        }
    }

    return 0;
}


