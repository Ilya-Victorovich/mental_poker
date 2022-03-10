#include "iostream"
#include "vector"
#include <bitset>
#include "time.h"
#include "string"
#include "fstream"
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono> 
#include <filesystem>
#include <map>

using namespace std;

int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

bool isPrime(long long p)
{
    if (p <= 1)
        return false;
    long long b = (long long)pow(p, 0.5);
    for (long long i = 2; i <= b; ++i)
    {
        if ((p % i) == 0) return false;
    }
    return true;
}

long long getPrime(long long l, long long r) { // от(вкл) .. до ...
    long long x;
    try {
        if (l > r) {
            throw "left more then right";
        }
        do {
            x = rand() % (r - l) + l;
            //cout << x << endl;
        } while (isPrime(x) == false);
        return x;
    }
    catch (char const* a) {
        cout << a << endl;
        cout << "l = " << l << "\tr = " << r << endl;
        exit(1);
    }
}
//функция быстрого возведения числа в степень по модулю(слева-направо)
//НЕ РАБОТАЕТ С БОЛЬШИМИ ЧИСЛАМИ, возможно нужен перебор x в указанном порядке
long long fastPow2(long long a, long long x, long long p) { //основание, степень, модуль
    /*long long y = 1;
    while (x) {
        if (x % 2 == 1)
            y *= pow(a, x % 2);
        a = a * a % p;
        x /= 2;
        //cout << "a=" << a << "\tx=" << x << "\ty=" << y << "\tp=" << p << endl;
    }
    return (y % p);*/

    long long y = 1;
    bitset<31> b1(x);
    for (int i = b1.size() - 1; i >= 0; --i) {
        y = y * y % p;
        if (b1[i] == 1) {
            y = y * a % p;
        }
        //cout << "a=" << a << "\tx=" << x << "\ty=" << y << "\tp=" << p << endl;
    }
    return (y);
}
//функция быстрого возведения числа в степень по модулю(справа-налево)
long long fastPow(long long a, long long x, long long p) {
    long long y = 1, s = a;
    while (x) {
        if (x % 2 == 1) {
            y = y * s % p;
        }
        s = s * s % p;
        x /= 2;
        //cout << "x=" << x << "\ts=" << s << "\ty=" << y << endl;
    }
    return y;
}

/*Функция, реализующая обобщённый алгоритм Евклида.Функция должна позволять находить наибольший общий делитель и обе неизвестных из уравнения.*/
long long evclid(long long a, long long b, vector<long long>& U) {
    try {
        if (a < b || a < 0 || b < 0) {
            throw "Unsuitable data!";
        }

        //vector <long long> U{ a,1,0 };
        U.clear();
        U = { a,1,0 };
        vector <long long> V{ b,0,1 }, T(3);
        long long q;

        while (V[0] != 0) {
            q = U[0] / V[0];
            T = { U[0] % V[0] , U[1] - q * V[1] , U[2] - q * V[2] };
            U = V;
            V = T;
        }
        return 0;
    }
    catch (char const* a) {
        cout << a << endl;
        exit(1);
    }
}


/*Реализация алгоритма «Ментальный покер» для произвольного числа 
игроков и карт. Для примера использовать правила покера Техасский 
холдем. Каждому игроку раздать по 2 карты и выложить 5 карт на стол. 
Обязательно обоснование защищённости и честности предложенной вами
схемы. Приветствуется написание нормального графического интерфейса.*/
struct player {
    long long c, d;
};

vector <long long> vectRandMix(vector <long long> vect) {
    // obtain a time-based seed:
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine e(seed);

    shuffle(vect.begin(), vect.end(), e);

    /*cout << "shuffled elements:";
    for (long long& x : vect) cout  << x << " ";
    cout << endl;*/
    return vect;
}

long long mentalPoker() {
    const int n = 2;
    player* players = new player[n];
    const int packSize = 52;
    //long long K[packSize];
    vector <long long> K(packSize);
    long long p = getPrime(100, 1000);
    vector <long long> U;

    
    ofstream fout("log.txt", ios::out | ios::trunc);
    if (!fout)
    {
        cout << "Unable to open file" << endl;
        return 0;
    }


    for (int i = 0; i < n; ++i) {
        do {
            players[i].c = rand() % (p - 1);
            evclid(p - 1, players[i].c, U);
            players[i].d = U[2];
        } while (!(U[0] == 1 && players[i].d >= 0 && players[i].c * players[i].d % (p - 1) == 1));
    }
    //cout << "cA=" << players[0].c << "\tdA=" << players[0].d << "\tcB=" << players[1].c << "\tdB=" << players[1].d << endl;
    fout << "p=" << p << "\tcA=" << players[0].c << "\tdA=" << players[0].d << "\tcB=" << players[1].c << "\tdB=" << players[1].d << endl;
    //fout << "cA*dA%(p-1)=" << players[0].c * players[0].d % (p - 1) << endl;
    //fout << "cB*dB%(p-1)=" << players[1].c * players[1].d % (p - 1) << endl << endl;

    //Form pack
    K[0] = (rand() % (p - 3) + 2);
    for (int i = 1; i < K.size(); ++i) {
        bool b = 0;
        do {
        gt:
            K[i] = (rand() % (p - 3) + 2);
            for (int j = 0; j < i; ++j) {
                if (K[j] == K[i]) {
                    goto gt;
                }
                b = 1;
            }
        } while (b == 0);
    }

    map <long long, string> dictPack;
    fout << "Pack: ";
    for (int i = 0, suit = 1, count = 2; i < K.size(); ++i, ++count) {
        if (count == 15) {
            count = 2;
            suit++;
        }
        string cardName = "";
        if (count >= 2 && count <= 10) {
            cardName += to_string(count);
            cardName += " ";
        }
        else if (count == 11) {
            cardName += "Jack ";
        }
        else if (count == 12) {
            cardName += "Queen ";
        }
        else if (count == 13) {
            cardName += "King ";
        }
        else if (count == 14) {
            cardName += "Ace ";
        }

        if (suit == 1) {
            cardName += "clubs";
        }
        else if (suit == 2) {
            cardName += "diamonds";
        }
        else if (suit == 3) {
            cardName += "hearts";
        }
        else if (suit == 4) {
            cardName += "spades";
        }

        dictPack.insert(make_pair(K[i], cardName));
        cout << K[i] << "\t-\t" << cardName << endl;
        //cout << "K[" << i << "]=" << K[i] << endl;
        fout << K[i] << " ";
    } fout << endl << endl;

 
    //for vector random mix
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine e(seed);


    /*Шаг 1. Алиса вычисляет числа и высылает u1, u2, u3 Бобу, предварительно перемешав их случайным образом.*/
    fout << "Alice encryptes pack: ";
    for (int i = 0; i < K.size(); ++i) {
        K[i] = fastPow(K[i], players[0].c, p);
        fout << K[i] << " ";
    } fout << endl;

    K = vectRandMix(K);
    fout << "Alice shuffles pack: ";
    for (int i = 0; i < K.size(); ++i) {
        fout << K[i] << " ";
    } fout << endl;

    /*Шаг 2. Боб получает три числа, выбирает случайно одно из них,
    например u2 , и отправляет его Алисе по линии связи. Это и
    будет карта, которая достанется ей в процессе раздачи. Алиса,
    получив это сообщение, может вычислить*/
    int r = rand() % K.size();
    fout << "Bob picks card " << K[r] << " to give it Alice" << endl;
    long long uH = fastPow(K[r], players[0].d, p);
    K.erase(K.begin() + r);
    //cout << "Alice has " << uH << endl;
    fout << "Alice decryptes card and has: " << uH << endl << endl;
    cout << "Alice has: " << dictPack[uH] << endl;

    /*Шаг 3. Боб продолжает свои действия.Он вычисляет для оставшихся двух чисел*/
    fout << "Bob encryptes pack: ";
    for (int i = 0; i < K.size(); ++i) {
        K[i] = fastPow(K[i], players[1].c, p);
        fout << K[i] << " ";
    } fout << endl;

    K = vectRandMix(K);
    fout << "Bob shuffles pack: ";
    for (int i = 0; i < K.size(); ++i) {
        fout << K[i] << " ";
    } fout << endl;
    /*переставляет эти числа и отправляет Алисе.*/

    /*Шаг 4. Алиса выбирает случайно одно из полученных чисел*/
    r = rand() % K.size();
    fout << "Alice picks card " << K[r] << " to give it Bob" << endl;
    long long card = fastPow(K[r], players[0].d, p);
    fout << "Alice decryptes card: " << card << endl;
    /*и отправляет это число обратно к Бобу*/

    /*Боб вычисляет число*/
    long long z = fastPow(card, players[1].d, p);
    K.erase(K.begin() + r);
    //cout << "Bob has " << z << endl;
    fout << "Bob decryptes card and has: " << z << endl << endl;
    cout << "Bob has: " << dictPack[z] << endl;

    //more 2 cards
    r = rand() % K.size();
    fout << "Bob picks card " << K[r] << " to give it Alice" << endl;
    card = fastPow(K[r], players[1].d, p);
    fout << "Bob decryptes card: " << card << endl;
    card = fastPow(card, players[0].d, p);
    fout << "Alice decryptes card and has: " << card << endl << endl;
    cout << "Alice has: " << dictPack[card] << endl;
    K.erase(K.begin() + r);

    r = rand() % K.size();
    fout << "Alice picks card " << K[r] << " to give it Bob" << endl;
    card = fastPow(K[r], players[0].d, p);
    fout << "Alice decryptes card: " << card << endl;
    card = fastPow(card, players[1].d, p);
    fout << "Bob decryptes card and has: " << card << endl << endl;
    cout << "Bob has: " << dictPack[card] << endl;
    K.erase(K.begin() + r);

    //cards in buy-in
    fout << "Cards in buy-in: ";
    cout << "Cards in buy-in: ";
    for (int i = 0; i < 5; ++i) {
        card = fastPow(K[i], players[0].d, p);
        card = fastPow(card, players[1].d, p);
        fout << card << " ";
        cout << dictPack[card] << ", ";
    } fout << endl; cout << endl;

    fout.close();
    delete[] players;
    return 0;
}
int main() {
    setlocale(0, "Rus");
    srand(time(NULL));

    mentalPoker();
	return 0;
}