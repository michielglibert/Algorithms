#ifndef __SORTVECTOR
#define __SORTVECTOR
/**
 \class sortvector
 \brief is een klasse van sorteerbare vectoren
 Bovenop de vectoreigenschappen zijn er hulpfuncties om sorteervergelijkingen
 te vergemakkelijken.
*/
#include <iostream>

using std::istream;
using std::ostream;
using std::move;
using std::swap;

#include <iomanip>   // voor setw
#include <cstdlib>   // voor rand - opletten!!
#include <vector>
#include <math.h>

using std::vector;

template<class T>
class Sortvector : public vector<T> {
public:

    /// \fn Constructor: het argument geeft de grootte aan
    /// bij constructie zal de tabel opgevuld worden met
    /// n verschillende Ts in random volgorde
    /// (zie hulplidfuncties)
    Sortvector(int);

    Sortvector(const Sortvector<T> &v) = delete;

    Sortvector<T> &operator=(const Sortvector<T> &v) = delete;

    Sortvector<T> &operator=(Sortvector<T> &&v) = delete;

    Sortvector(Sortvector<T> &&v) = delete;

    /// \fn vul_range vul vector met de waarden T(0)...T(size()-1) in volgorde
    void vul_range();

    void draai_om();

    void vul_omgekeerd();

    void shuffle();

    void vul_random_zonder_dubbels();

    void vul_random();
    int geefaantalinversies() const;

    int geefaantalinversies_beter() const;


    void heapify();

    void heapify_rec(int i, int eindIndex);

    int getHoogte();

    int linkseKind(int);

    int rechtseKind(int);

    int getOuder(int);

    bool is_gesorteerd() const;

    /// \fn is_range controleert of *this eruit ziet als het resultaat van vul_range(), d.w.z.
    /// dat, voor alle i, (*this)[i]==T(i);
    bool is_range() const;

    friend ostream &operator<<(ostream &os, const Sortvector<T> &s) {
        s.schrijf(os);
        return os;
    }

private:
    void schrijf(ostream &os) const;

};

template<class T>
void Sortvector<T>::vul_range() {
    for (int i = 0; i < (*this).size(); i++) {
        (*this)[i] = (T) i;
    }
}

template<class T>
void Sortvector<T>::draai_om() {
    int j = (*this).size() - 1;
    for (int i = 0; i < (*this).size() / 2; i++) {
        swap((*this)[i], (*this)[j]);
        j--;
    }
}

template<class T>
void Sortvector<T>::vul_omgekeerd() {
    int j = (*this).size() - 1;
    for (int i = 0; i < (*this).size(); i++) {
        (*this)[i] = (T) j;
        j--;
    }
}

template<class T>
void Sortvector<T>::shuffle() {
    int n = (*this).size();
    for (int i = 0; i < n - 2; i++) {
        int j = i + rand() % (n - i);
        swap((*this)[i], (*this)[j]);
    }
}

template<class T>
void Sortvector<T>::vul_random_zonder_dubbels() {
    this->vul_range();
    this->shuffle();
}

template<class T>
void Sortvector<T>::vul_random() {
    for (int i = 0; i < this->size(); i++) {
        (*this)[i] = rand()%this->size();
    }
}

template<class T>
bool Sortvector<T>::is_gesorteerd() const {
    int i = 1;
    while (i < (*this).size()) {
        if ((*this)[i - 1] > (*this)[i]) {
            return false;
        }
        i++;
    }
    return true;
}

template<class T>
bool Sortvector<T>::is_range() const {
    int i = 0;
    while (i < (*this).size()) {
        if (i != (*this)[i]) {
            return false;
        }
        i++;
    }
    return true;
}

template<class T>
void Sortvector<T>::schrijf(ostream &os) const {
    for (auto &&t : *this) {
        os << t << " ";
    }
    os << "\n";
}


template<class T>
Sortvector<T>::Sortvector(int l) {//l is lengte
    this->reserve(l);
    for (int i = 0; i < l; i++) {
        this->emplace_back(i);
    }
    shuffle();
};

template<class T>
int Sortvector<T>::geefaantalinversies() const {
    int aantal = 0;
    for (int i = 0; i < this->size() - 1; i++) {
        for (int j = i; j < this->size(); j++) {
            if ((*this)[i] > (*this)[j]) {
                aantal++;
            }
        }
    }
    return aantal;
}

template<class T>
int Sortvector<T>::geefaantalinversies_beter() const {
    int aantal = 0;

    //via merge sort

    return aantal;
}

template<class T>
void Sortvector<T>::heapify() {
    //Heapify via samenvoegen O(n)
    for (int i = floor(this->size() / 2); i >= 0; i--)
        heapify_rec(i, this->size());
}

template<class T>
void Sortvector<T>::heapify_rec(int i, int eindIndex) {
    int indexLinkse = linkseKind(i);
    int indexRechtse = rechtseKind(i);

    int grootsteIndex = i;

    if (indexLinkse<eindIndex && (*this)[indexLinkse] > (*this)[grootsteIndex]) {
        grootsteIndex = indexLinkse;
    }

    if (indexRechtse<eindIndex && (*this)[indexRechtse] > (*this)[grootsteIndex]) {
        grootsteIndex = indexRechtse;
    }

    if (grootsteIndex != i) {
        swap((*this)[grootsteIndex], (*this)[i]);
        heapify_rec(grootsteIndex, eindIndex);
    }
}

template<class T>
int Sortvector<T>::getHoogte() {
    int h = 1;
    int teller = 2;

    while (this->size() >= teller) {
        h++;
        teller *= 2;
    }

    return h;
}

template<class T>
int Sortvector<T>::linkseKind(int i) {
    return 2 * i + 1;
}

template<class T>
int Sortvector<T>::rechtseKind(int i) {
    return 2 * i + 2;
}

template<class T>
int Sortvector<T>::getOuder(int i) {
    return (i - 1) / 2;
}

#endif