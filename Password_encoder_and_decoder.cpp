#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <random>
#include <regex>

using namespace std;

// Huffman Tree Node 
struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char ch, int freq) : ch(ch), freq(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq;
    }
};

// Function to build the Huffman Tree
Node* buildHuffmanTree(const unordered_map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    for (const auto& pair : freq) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        int sum = left->freq + right->freq;
        Node* newNode = new Node('\0', sum);
        newNode->left = left;
        newNode->right = right;
        minHeap.push(newNode);
    }

    return minHeap.top();
}

// Function to generate Huffman Codes
void generateHuffmanCodes(Node* root, const string& str, unordered_map<char, string>& huffmanCode) {
    if (!root) return;

    if (root->ch != '\0') {
        huffmanCode[root->ch] = str;
    }

    generateHuffmanCodes(root->left, str + "0", huffmanCode);
    generateHuffmanCodes(root->right, str + "1", huffmanCode);
}

// Function to encrypt the password using Huffman Codes
string encryptPassword(const string& password, const unordered_map<char, string>& huffmanCode) {
    string encryptedPassword;
    for (char ch : password) {
        encryptedPassword += huffmanCode.at(ch);
    }
    return encryptedPassword;
}

// Function to decrypt the encrypted password using Huffman Tree
string decryptPassword(const string& encryptedPassword, Node* root) {
    string decryptedPassword;
    Node* currNode = root;
    for (char bit : encryptedPassword) {
        if (bit == '0') {
            currNode = currNode->left;
        } else {
            currNode = currNode->right;
        }
        if (currNode->ch != '\0') {
            decryptedPassword += currNode->ch;
            currNode = root;
        }
    }
    return decryptedPassword;
}

// Function to delete the Huffman Tree nodes
void deleteHuffmanTree(Node* root) {
    if (!root) return;
    deleteHuffmanTree(root->left);
    deleteHuffmanTree(root->right);
    delete root;
}

// Function to check if the password meets length criteria
bool isPasswordValid(const string& password, int minLength, int maxLength) {
    int length = password.length();
    return (length >= minLength && length <= maxLength);
}

// Function to check password strength
bool isStrongPassword(const string& password) {
    if (password.length() < 8) {
        return false;
    }
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char ch : password) {
        if (isupper(ch)) hasUpper = true;
        else if (islower(ch)) hasLower = true;
        else if (isdigit(ch)) hasDigit = true;
        else hasSpecial = true;
    }
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

// Function to generate a random strong password
string generatePassword(int length) {
    const string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()";
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> distribution(0, characters.size() - 1);

    string password;
    for (int i = 0; i < length; ++i) {
        password += characters[distribution(generator)];
    }

    return password;
}

// Function to find minimum edits to make a string palindrome
int minEditsToPalindrome(const string& s) {
    int n = s.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        dp[i][i] = 1;
    }
    for (int length = 2; length <= n; length++) {
        for (int i = 0; i <= n - length; i++) {
            int j = i + length - 1;
            if (s[i] == s[j]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
        }
    }

    int lpsLength = dp[0][n - 1];
    int minEdits = n - lpsLength;

    return minEdits;
}

int main() {
    char choice;
    string password;
    Node* huffmanRoot = nullptr; // Pointer to the Huffman tree root

    // Menu-driven interface
    while (true) {
        cout << "Menu:" << endl;
        cout << "1. Encrypt Password" << endl;
        cout << "2. Decrypt Password" << endl;
        cout << "3. Show Huffman Codes" << endl;
        cout << "4. Check Password Validity" << endl;
        cout << "5. Check Password Strength" << endl;
        cout << "6. Generate Strong Password" << endl;
        cout << "7. Minimum Edits to Palindrome" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case '1': {
                if (huffmanRoot) {
                    deleteHuffmanTree(huffmanRoot);
                }
                cout << "Enter your password to encrypt: ";
                cin >> password;

                if (!isPasswordValid(password, 6, 20)) {
                    cout << "Password length should be between 6 and 20 characters." << endl;
                    break;
                }

                unordered_map<char, int> freq;
                for (char ch : password) {
                    freq[ch]++;
                }

                huffmanRoot = buildHuffmanTree(freq);

                unordered_map<char, string> huffmanCode;
                generateHuffmanCodes(huffmanRoot, "", huffmanCode);

                string encryptedPassword = encryptPassword(password, huffmanCode);
                cout << "Encrypted password: " << encryptedPassword << endl;

                break;
            }
            case '2': {
                if (!huffmanRoot) {
                    cout << "No password has been encrypted yet." << endl;
                    break;
                }
                cout << "Enter the encrypted password to decrypt: ";
                cin >> password;

                string decryptedPassword = decryptPassword(password, huffmanRoot);
                cout << "Decrypted password: " << decryptedPassword << endl;

                break;
            }
            case '3': {
                if (!huffmanRoot) {
                    cout << "No Huffman tree available." << endl;
                    break;
                }
                cout << "Huffman Codes:" << endl;
                unordered_map<char, string> huffmanCode;
                generateHuffmanCodes(huffmanRoot, "", huffmanCode);
                for (const auto& pair : huffmanCode) {
                    cout << "'" << pair.first << "': " << pair.second << endl;
                }
                break;
            }
            case '4': {
                cout << "Enter the password to check validity: ";
                cin >> password;
                if (isPasswordValid(password, 6, 20)) {
                    cout << "Password is valid." << endl;
                } else {
                    cout << "Password is invalid. Password length should be between 6 and 20 characters." << endl;
                }
                break;
            }
            case '5': {
                cout << "Enter a password to check its strength: ";
                cin >> password;

                if (isStrongPassword(password)) {
                    cout << "Password is strong." << endl;
                } else {
                    cout << "Password is weak. It should be at least 8 characters long, and include uppercase letters, lowercase letters, digits, and special characters." << endl;
                }
                break;
            }
            case '6': {
                int length;
                cout << "Enter the desired length for the password: ";
                cin >> length;

                string password = generatePassword(length);
                cout << "Generated password: " << password << endl;

                break;
            }
            case '7': {
                cout << "Enter the string to check minimum edits to palindrome: ";
                cin >> password;
                int edits = minEditsToPalindrome(password);
                cout << "Minimum edits needed to make the string a palindrome: " << edits << endl;
                break;
            }
            case '8': {
                if (huffmanRoot) {
                    deleteHuffmanTree(huffmanRoot);
                }
                cout << "Exiting the program." << endl;
                return 0;
            }
            default: {
                cout << "Invalid choice. Please enter a valid option." << endl;
            }
        }
    }

    return 0;
}