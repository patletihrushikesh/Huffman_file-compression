
#include <bits/stdc++.h>
using namespace std;

struct Node {
    unsigned char ch;
    uint64_t freq;
    Node *left, *right;

    Node(unsigned char c, uint64_t f)
        : ch(c), freq(f), left(nullptr), right(nullptr) {}

    Node(Node* l, Node* r)
        : ch(0), freq(l->freq + r->freq), left(l), right(r) {}

    bool leaf() const {
        return !left && !right;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

Node* buildTree(const unordered_map<unsigned char, uint64_t>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto [ch, f] : freq)
        pq.push(new Node(ch, f));

    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node(a, b));
    }

    return pq.empty() ? nullptr : pq.top();
}

void makeCodes(Node* root, string code,
               unordered_map<unsigned char, string>& codes) {
    if (!root) return;

    if (root->leaf()) {
        codes[root->ch] = code.empty() ? "0" : code;
        return;
    }

    makeCodes(root->left, code + "0", codes);
    makeCodes(root->right, code + "1", codes);
}

void saveTree(Node* root, vector<unsigned char>& data) {
    if (root->leaf()) {
        data.push_back(1);
        data.push_back(root->ch);
        return;
    }

    data.push_back(0);
    saveTree(root->left, data);
    saveTree(root->right, data);
}

Node* loadTree(const vector<unsigned char>& data, size_t& pos) {
    if (pos >= data.size()) return nullptr;

    unsigned char type = data[pos++];

    if (type == 1) {
        if (pos >= data.size()) return nullptr;
        return new Node(data[pos++], 0);
    }

    if (type != 0) return nullptr;

    Node* left = loadTree(data, pos);
    Node* right = loadTree(data, pos);

    if (!left || !right) {
        freeTree(left);
        freeTree(right);
        return nullptr;
    }

    return new Node(left, right);
}

bool compress(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);

    if (!in) {
        cout << "Cannot open input file.\n";
        return false;
    }

    vector<unsigned char> data(
        (istreambuf_iterator<char>(in)),
        istreambuf_iterator<char>()
    );

    in.close();

    if (data.empty()) {
        ofstream out(outputFile, ios::binary);
        unsigned char mode = 0;
        out.write((char*)&mode, 1);
        cout << "Empty file compressed.\n";
        return true;
    }

    unordered_map<unsigned char, uint64_t> freq;

    for (unsigned char c : data)
        freq[c]++;

    ofstream out(outputFile, ios::binary);

    if (!out) {
        cout << "Cannot create output file.\n";
        return false;
    }

    if (freq.size() == 1) {
        unsigned char mode = 1;
        unsigned char ch = data[0];
        uint64_t count = data.size();

        out.write((char*)&mode, 1);
        out.write((char*)&ch, 1);
        out.write((char*)&count, sizeof(count));

        cout << "Compression successful.\n";
        return true;
    }

    Node* root = buildTree(freq);

    unordered_map<unsigned char, string> codes;
    makeCodes(root, "", codes);

    vector<unsigned char> tree;
    saveTree(root, tree);

    unsigned char mode = 2;
    uint32_t treeSize = tree.size();

    out.write((char*)&mode, 1);
    out.write((char*)&treeSize, sizeof(treeSize));
    out.write((char*)tree.data(), tree.size());

    unsigned char buffer = 0;
    int bits = 0;
    vector<unsigned char> encoded;

    for (unsigned char c : data) {
        for (char bit : codes[c]) {
            buffer = (buffer << 1) | (bit - '0');
            bits++;

            if (bits == 8) {
                encoded.push_back(buffer);
                buffer = 0;
                bits = 0;
            }
        }
    }

    unsigned char validBits = bits;

    if (bits) {
        buffer <<= 8 - bits;
        encoded.push_back(buffer);
    } else {
        validBits = 8;
    }

    out.write((char*)&validBits, 1);
    out.write((char*)encoded.data(), encoded.size());

    out.close();
    freeTree(root);

    cout << "Compression successful.\n";
    return true;
}

bool decompress(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);

    if (!in) {
        cout << "Cannot open compressed file.\n";
        return false;
    }

    vector<unsigned char> data(
        (istreambuf_iterator<char>(in)),
        istreambuf_iterator<char>()
    );

    in.close();

    if (data.empty()) {
        cout << "Invalid compressed file.\n";
        return false;
    }

    size_t pos = 0;
    unsigned char mode = data[pos++];

    ofstream out(outputFile, ios::binary);

    if (!out) {
        cout << "Cannot create output file.\n";
        return false;
    }

    if (mode == 0) {
        cout << "Decompression successful.\n";
        return true;
    }

    if (mode == 1) {
        if (data.size() < 10) return false;

        unsigned char ch = data[pos++];
        uint64_t count;

        memcpy(&count, &data[pos], sizeof(count));
        pos += sizeof(count);

        for (uint64_t i = 0; i < count; i++)
            out.put((char)ch);

        cout << "Decompression successful.\n";
        return true;
    }

    if (mode != 2 || data.size() < pos + 4)
        return false;

    uint32_t treeSize;

    memcpy(&treeSize, &data[pos], sizeof(treeSize));
    pos += sizeof(treeSize);

    if (data.size() < pos + treeSize + 1)
        return false;

    vector<unsigned char> tree(
        data.begin() + pos,
        data.begin() + pos + treeSize
    );

    pos += treeSize;

    size_t treePos = 0;
    Node* root = loadTree(tree, treePos);

    if (!root || treePos != tree.size())
        return false;

    unsigned char validBits = data[pos++];

    if (validBits < 1 || validBits > 8) {
        freeTree(root);
        return false;
    }

    Node* current = root;

    for (size_t i = pos; i < data.size(); i++) {
        int limit = (i == data.size() - 1 && validBits != 8)
                        ? validBits : 8;

        for (int b = 7; b >= 8 - limit; b--) {
            int bit = (data[i] >> b) & 1;

            current = bit ? current->right : current->left;

            if (!current) {
                freeTree(root);
                return false;
            }

            if (current->leaf()) {
                out.put((char)current->ch);
                current = root;
            }
        }
    }

    out.close();
    freeTree(root);

    cout << "Decompression successful.\n";
    return true;
}

int main() {
    while (true) {
        cout << "\n1. Compress\n";
        cout << "2. Decompress\n";
        cout << "3. Exit\n";
        cout << "Choice: ";

        int choice;
        cin >> choice;

        if (choice == 1) {
            string input, output;

            cout << "Input file: ";
            cin >> input;

            cout << "Output file: ";
            cin >> output;

            compress(input, output);
        }
        else if (choice == 2) {
            string input, output;

            cout << "Compressed file: ";
            cin >> input;

            cout << "Output file: ";
            cin >> output;

            decompress(input, output);
        }
        else if (choice == 3) {
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
