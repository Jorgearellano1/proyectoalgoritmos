#include<bits/stdc++.h>
using namespace std;

class CuckooHashing {
private:
    static const int ver = 2;
    int **hashtable;
    int *pos;
    int size;
    int total_attempts;


public:
    CuckooHashing(int initial_size = 5) {
        size = initial_size;
        hashtable = new int*[ver];
        pos = new int[ver];
        for (int i = 0; i < ver; ++i)
            hashtable[i] = new int[size];
        initTable();
    }

    ~CuckooHashing() {
        for (int i = 0; i < ver; ++i)
            delete[] hashtable[i];
        delete[] hashtable;
        delete[] pos;
    }

    void initTable() {
        for (int j = 0; j < size; j++)
            for (int i = 0; i < ver; i++)
                hashtable[i][j] = INT_MIN;
    }

    int hash(int function, int key) {
        switch (function) {
            case 1: return key % size;
            case 2: return (key / size) % size;
        }
        return 0;
    }

    void place(int key, int tableID, int cnt, int n, int &total_attempts) {
        if (total_attempts > 3 * n) {
            cout << "Máximo de intentos alcanzado. Iniciando rehash." << endl;
            rehash();
            total_attempts = 0;
        }

        if (cnt == n) {
            cout << key << " no posicionado. Incrementando contador de intentos." << endl;
            total_attempts++;
            return;
        }

        for (int i = 0; i < ver; i++) {
            pos[i] = hash(i + 1, key);
            if (hashtable[i][pos[i]] == key)
                return;
        }

        if (hashtable[tableID][pos[tableID]] != INT_MIN) {
            int dis = hashtable[tableID][pos[tableID]];
            hashtable[tableID][pos[tableID]] = key;
            place(dis, (tableID + 1) % ver, cnt + 1, n, total_attempts);
        } else {
            hashtable[tableID][pos[tableID]] = key;
        }
    }

    void rehash() {
        int **old_table = hashtable;
        int old_size = size;

        size *= 2;
        hashtable = new int*[ver];
        for (int i = 0; i < ver; ++i)
            hashtable[i] = new int[size];
        initTable();

        for (int i = 0; i < ver; ++i)
            for (int j = 0; j < old_size; ++j)
                if (old_table[i][j] != INT_MIN)
                    place(old_table[i][j], 0, 0, size, total_attempts);

        for (int i = 0; i < ver; ++i)
            delete[] old_table[i];
        delete[] old_table;
    }

    void printTable() {
        cout << "Tablas hash finales:" << endl;
        for (int i = 0; i < ver; i++, cout << endl)
            for (int j = 0; j < size; j++)
                (hashtable[i][j] == INT_MIN) ? cout << "- " : cout << hashtable[i][j] << " ";
    }

    void cuckoo(int keys[], int n) {
        initTable();
        int total_attempts = 0;
        for (int i = 0; i < n; i++)
            place(keys[i], 0, 0, n, total_attempts);
        printTable();
    }
};

int main() {
    CuckooHashing cuckooHash;

    int n;
    cout << "Ingrese el número de elementos: ";
    cin >> n;

    int keys[n];
    cout << "Ingrese los elementos: ";
    for (int i = 0; i < n; i++) {
        cin >> keys[i];
    }

    cuckooHash.cuckoo(keys, n);

    return 0;
}
