#ifndef __SORTEERMETHODE
#define __SORTEERMETHODE

#include "sortvector.h"
#include "chrono.h"
#include <iostream>
#include <iomanip>

using std::move;
using std::swap;
using std::endl;
using std::cout;
using std::setw;

#include <algorithm>   // voor sort()-methode uit STL
#include <cmath>
#include <vector>

#include "sortvector.h"
#include "chrono.h"
#include "csv.h"

/** class Sorteermethode
    \brief abstracte klasse van methodes die een vector sorteren
*/
template<typename T>
class Sorteermethode {
public:
/// \fn operator() sorteert de vector gegeven door het argument
    virtual void operator()(Sortvector<T> &v) const = 0;

/// \fn meet(int kortste, int langste, ostream& os) schrijft naar os een overzicht (met de nodige ornamenten)
/// met de snelheid van de opgegeven sorteermethode *this. Er wordt 1 lijn uitgedrukt voor elke mogelijke
/// grootte. Deze groottes zijn kleinste, 10*kleinste, 100*kleinste,
/// enzovoorts, tot aan grootste.
/// Op een lijn staat de snelheid van de methode toegepast op
/// (1) een random tabel
/// (2) een al gesorteerde tabel.
/// (3) een omgekeerd gesorteerde tabel.

/// Deze functie werkt alleen als T een toekenning van een int toelaat,
/// zodat bv.
///    T a=5;
/// geldig is.
    void meet(int kortste, int langste, ostream &os) {
        int i = 1, j = kortste;
        vector<double> aantalData, randomData, gesorteerdData, omgekeerdData;

        os << setw(15) << "lengte" << setw(15)
           << "random" << setw(15)
           << "gesorteerd" << setw(15)
           << "omgekeerd" << endl;
        while (j < langste) {
            j *= 10;
            aantalData.push_back(j);
            os << setw(15) << j << setw(15);

            //Chrono
            Chrono c;

            //Vector aanmaken om op te vullen
            Sortvector<T> v(j);
            v.vul_random_zonder_dubbels();
            c.start();
            this->operator()(v);
            c.stop();
            randomData.push_back(c.tijd());
            os << c.tijd() << setw(15);

            v.vul_range();
            c.start();
            this->operator()(v);
            c.stop();
            gesorteerdData.push_back(c.tijd());
            os << c.tijd() << setw(15);

            v.vul_omgekeerd();
            c.start();
            this->operator()(v);
            c.stop();
            omgekeerdData.push_back(c.tijd());
            os << c.tijd() << endl;

            i++;
        }

        CsvData csv("labo4", ',');
        csv.voegDataToe(aantalData);
        csv.voegDataToe(randomData);
        csv.voegDataToe(gesorteerdData);
        csv.voegDataToe(omgekeerdData);
    }
};

/** \class STLSort
   \brief STANDARD TEMPLATE LIBRARY SORT
*/
template<typename T>
class STLSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;
};

template<typename T>
void STLSort<T>::operator()(Sortvector<T> &v) const {
    sort(v.begin(), v.end());
}

/** \class InsertionSort
*/
template<typename T>
class InsertionSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;
};

template<typename T>
void InsertionSort<T>::operator()(Sortvector<T> &v) const {
    for (int i = 1; i < v.size(); i++) {
        int j = i - 1;
        T temp = move(v[i]);
        while (j >= 0 && v[j] > temp) {
            v[j + 1] = move(v[j]);
            j--;
        }
        v[j + 1] = move(temp);
    }
}

/** \class Shellsort
*/

template<typename T>
class ShellSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;
};

template<class T>
void ShellSort<T>::operator()(Sortvector<T> &v) const {
    int k = 2;
    while (k >= 1) {
        for (int i = k; i < v.size(); i++) {
            int j = i - k;
            T temp = v[i];
            while (j >= 0 && temp < v[j]) {
                v[j + k] = move(v[j]);
                j -= k;
            }
            v[j + k] = move(temp);
        }
        k--;
    }
}

/** \class SelectionSort
*/

template<typename T>
class SelectionSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;
};

template<class T>
void SelectionSort<T>::operator()(Sortvector<T> &v) const {
    for (int i = v.size() - 1; i > 0; i--) {
        int grootste = 0;
        for (int j = 0; j < i; j++) {
            if (v[grootste] < v[j]) {
                grootste = j;
            }
        }
        swap(v[i], v[grootste]);
    }

}


/** \class HeapSort
*/

template<typename T>
class HeapSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;
};

template<class T>
void HeapSort<T>::operator()(Sortvector<T> &v) const {
    //Omzetten naar heap
    v.heapify();

    //Telkens het grootste element verwijderen
    for (int i = v.size() - 1; i >= 0; i--) {
        swap(v[0], v[i]);

        //Opnieuw aan heapvoorwaarde voldoen
        v.heapify_rec(0, i);
    }
}


/** \class MergeSort
*/
template<class T>
class MergeSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;

    void operator()(Sortvector<T> &v, bool topDown) const;

    void topDownMergeSort(Sortvector<T> &v, int l, int r, vector<T> &h) const;

    void bottomUpMergeSort(Sortvector<T> &v, vector<T> &h) const;

    void merge(Sortvector<T> &v, int l, int m, int r, vector<T> &h) const;
};

template<class T>
void MergeSort<T>::operator()(Sortvector<T> &v) const {
    vector<T> h(v.size() / 2);
    topDownMergeSort(v, 0, v.size(), h);
}

template<class T>
void MergeSort<T>::operator()(Sortvector<T> &v, bool topDown) const {
    vector<T> h(v.size() / 2);
    if (topDown)
        topDownMergeSort(v, 0, v.size(), h);
    else
        bottomUpMergeSort(v, h);
}

template<class T>
void MergeSort<T>::topDownMergeSort(Sortvector<T> &v, int l, int r, vector<T> &h) const {
    if (l < r - 1) {
        int m = l + (r - l) / 2;
        topDownMergeSort(v, l, m, h);
        topDownMergeSort(v, m, r, h);
        merge(v, l, m, r, h);
    }
}

template<class T>
void MergeSort<T>::bottomUpMergeSort(Sortvector<T> &v, vector<T> &h) const {

    for (int width = 1; width < v.size(); width *= 2) {
        for (int i = 0; i < v.size(); i = i + 2 * width) {
            int l = i;
            int m = i + width;
            int r = i + 2 * width <= v.size() ? i + 2 * width : v.size();

            merge(v, l, m, r, h);
        }
    }
}

template<class T>
void MergeSort<T>::merge(Sortvector<T> &v, int l, int m, int r, vector<T> &h) const {
    int i = 0, j = m, k = l;
    //Hulptabel kopieren
    while (i < m - l) {
        h[i] = v[k];
        i++;
        k++;
    }

    //Terug op initiële waardes plaatsen
    k = l;
    i = 0;

    while (i < m - l && j < r) {
        if (h[i] < v[j]) {
            v[k] = move(h[i]);
            i++;
        } else {
            v[k] = move(v[j]);
            j++;
        }
        k++;
    }

    while (i < m - l) {
        v[k] = move(h[i]);
        k++;
        i++;
    }

    while (j < r) {
        v[k] = move(v[j]);
        k++;
        j++;
    }
}

/** \class QuickSort
*/

template<typename T>
class QuickSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;

    void quickSort(Sortvector<T> &v, int l, int r) const;
};

template<class T>
void QuickSort<T>::operator()(Sortvector<T> &v) const {
    quickSort(v, 0, v.size());
}

template<class T>
void QuickSort<T>::quickSort(Sortvector<T> &v, int l, int r) const {
    while (l < r - 1) {
        T pivot = v[l + (r - l) / 2];
        int i = l, j = r - 1;
        while (v[j] > pivot) {
            j--;
        }
        while (v[i] < pivot) {
            i++;
        }
        while (i < j) {
            swap(v[i], v[j]);

            while (v[i] <= pivot) {
                i++;
            }

            while (v[j] > pivot) {
                j--;
            }
        }

        //cout << "l " << l << " -- r " << r << endl;
        //cout << "i " << i << " -- j " << j << endl;
        //enkel kleinste recursen zodat stack O(lgn) wordt ipv O(n)
        if (j - l < r - j) {
            quickSort(v, l, j + 1);
            l = i;
        } else {
            quickSort(v, j + 1, r);
            r = j;
        }
    }
}

/** \class Dual pivot QuickSort
*/

template<typename T>
class DualPivotQuickSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;

    void dualPivotQuickSort(Sortvector<T> &v, int l, int r) const;
};

template<class T>
void DualPivotQuickSort<T>::operator()(Sortvector<T> &v) const {
    dualPivotQuickSort(v, 0, v.size() - 1);
}

template<class T>
void DualPivotQuickSort<T>::dualPivotQuickSort(Sortvector<T> &v, int l, int r) const {
    if (l < r) {
        if (v[l] > v[r]) {
            swap(v[l], v[r]);
        }

        int k = l + 1, m = l + 1, g = r - 1;

        while (m <= g) {
            //cout << "m" << m << " g" << g << endl;
            if (v[m] < v[l]) {
                swap(v[m], v[k]);
                k++;
            } else if (v[m] > v[r]) {
                while (v[g] > v[r])
                    g--;
                swap(v[m], v[g]);
                g--;
                if (v[m] < v[l]) {
                    swap(v[m], v[k]);
                    k++;
                }
            }
            m++;
        }
        //cout << "m" << m << " g" << g << endl;
        //cout << "l" << l << " r" << r << endl;
        k--;
        g++;

        swap(v[k], v[l]);
        swap(v[m], v[r]);

        dualPivotQuickSort(v, l, m - 1);
        dualPivotQuickSort(v, m + 1, g - 1);
        dualPivotQuickSort(v, g + 1, r);
    }
}

/** \class Count Sort
*/

template<typename T>
class CountSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;
};

template<class T>
void CountSort<T>::operator()(Sortvector<T> &v) const {
    vector<int> freq(v.size(), 0), output(v.size(), 0);

    for (int i = 0; i < v.size(); i++) {
        freq[v[i]]++;
    }

    for (int i = 1; i < v.size(); i++) {
        freq[i] += freq[i - 1];
    }

    for (int i = v.size() - 1; i >= 0; i--) {
        output[freq[v[i]]] = v[i];
        freq[v[i]]--;
    }

    for (int i = 0; i < v.size(); i++) {
        v[i] = move(output[i]);
    }
}


/** \class Radix Exchange Sort
*/

template<typename T>
class RadixExchangeSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;
};

template<class T>
void RadixExchangeSort<T>::operator()(Sortvector<T> &v) const {
    cout << "This sorting method has not been implemented yet";
}

/** \class MSD Radix Sort
*/

template<typename T>
class MSDRadixSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;

    void radixCountSort(Sortvector<T> &v, int exp) const;
};

template<class T>
void MSDRadixSort<T>::operator()(Sortvector<T> &v) const {
    //Maximum zoeken
    T max = v[0];
    for (int i = 1; i < v.size(); i++) {
        if (max < v[i]) {
            max = v[i];
        }
    }

    //Get number of digits
    int digits = 1;
    int count = 0;
    while (digits < max) {
        digits *= 10;
        count++;
    }

    //Run count sort for each digit
    for (count; count > 0; count--)
        digits /= 10;
    radixCountSort(v, digits);
}

template<class T>
void MSDRadixSort<T>::radixCountSort(Sortvector<T> &v, int exp) const {
    vector<int> freq(v.size(), 0), output(v.size(), 0);

    for (int i = 0; i < v.size(); i++) {
        freq[v[i] / exp]++;
    }

    for (int i = 1; i < v.size(); i++) {
        freq[i] += freq[i - 1];
    }

    for (int i = v.size() - 1; i >= 0; i--) {
        output[freq[v[i] / exp]] = v[i];
        freq[v[i] / exp]--;
    }

    for (int i = 0; i < v.size(); i++) {
        v[i] = move(output[i]);
    }
}


/** \class MSD Radix Sort
*/

template<typename T>
class TernaireRadixQuicksort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;
};

template<class T>
void TernaireRadixQuicksort<T>::operator()(Sortvector<T> &v) const {
    cout << "This sorting method has not been implemented yet";
}

/** \class LSD Radix Sort
*/

template<typename T>
class LSDRadixSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;

    void radixCountSort(Sortvector<T> &v, int exp) const;
};

template<class T>
void LSDRadixSort<T>::operator()(Sortvector<T> &v) const {
    //Maximum zoeken
    T max = v[0];
    for (int i = 1; i < v.size(); i++) {
        if (max < v[i]) {
            max = v[i];
        }
    }

    //Run count sort for each digit
    for (int exp = 1; max / exp > 0; exp *= 10)
        radixCountSort(v, exp);
}

template<class T>
void LSDRadixSort<T>::radixCountSort(Sortvector<T> &v, int exp) const {
    vector<int> freq(v.size(), 0), output(v.size(), 0);

    for (int i = 0; i < v.size(); i++) {
        freq[(v[i] / exp) % 10]++;
    }

    for (int i = 1; i < v.size(); i++) {
        freq[i] += freq[i - 1];
    }

    for (int i = v.size() - 1; i >= 0; i--) {
        output[freq[(v[i] / exp) % 10]] = v[i];
        freq[(v[i] / exp) % 10]--;
    }

    for (int i = 0; i < v.size(); i++) {
        v[i] = move(output[i]);
    }
}

/** \class Bucket Sort
*/

template<typename T>
class BucketSort : public Sorteermethode<T> {
public:
    void operator()(Sortvector<T> &v) const;
};

template<class T>
void BucketSort<T>::operator()(Sortvector<T> &v) const {

}

#endif