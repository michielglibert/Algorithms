#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include "sortvector.h"
#include "chrono.h"
#include "Sorteermethode.h"
#include "csv.h"
#include "intstring.h"
using namespace std;

static int size = 17;


bool inputValid(int input) {
    return input > 0 && input < 16;
}

void clrScrn() {
    cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
}

template<class T>
void callMeet(Sorteermethode<T> *sort, int kortste, int langste, string name) {
    cout << "------------------------------" << name << "------------------------------" << endl;
    sort->meet(kortste, langste, cout);
}

template<class T>
int zoek(Sortvector<T> &v, T &getal, bool binair) {
    int l = 0, r = v.size(), m;
    while(l < r-1 && v[m] != getal) {
        if(binair) {
            m = l+(r-l)/2;
        } else {
            m = l+(((r-l)/(v[r-1]-v[l]))*(getal-v[l]));
        }

        if(getal < v[m]) {
            r = m;
        } else {
            l = m;
        }
    }

    return getal==v[l]?l:-1;
}



template<class T>
int doSort() {
    int input;

    cout << "-----Choose a sorting algorithm-----" << endl;
    cout << "1) Insertion Sort" << endl;
    cout << "2) Shell Sort" << endl;
    cout << "3) Selection Sort" << endl;
    cout << "4) Heap Sort" << endl;
    cout << "5) Merge Sort" << endl;
    cout << "6) Quick Sort" << endl;
    cout << "7) Dual Pivot QuickSort" << endl;
    cout << "8) Binair Zoeken" << endl;
    cout << "9) Interpolerend Zoeken" << endl;
    cout << "10) Count Sort" << endl;
    cout << "11) Radix Exchange Sort (Unavailable)" << endl;
    cout << "12) MSD Radix Sort" << endl;
    cout << "13) Ternaire Radix Sort (Unavailable)" << endl;
    cout << "14) LSD Radix Sort" << endl;
    cout << "15) Bucket Sort" << endl;

    cin >> input;

    if(inputValid(input)) {
        clrScrn();

        Sortvector<int> v(size);

        if(input < 8) {
            v.vul_random_zonder_dubbels();
        } else if (input > 9) {
            v.vul_random();
        } else {
            v.vul_range();
        }

        if(size <= 20) {
            cout << "-----Orginal array-----" << endl;
            cout << v << endl;
        }

        switch(input) {
            case 1:{
                InsertionSort<T> ins;
                ins(v);
                callMeet(&ins, 1, 10000, "Insertion Sort");
                break;
            }

            case 2:{
                ShellSort<T> sh;
                sh(v);
                callMeet(&sh, 1, 10000, "Shell Sort");
                break;
            }

            case 3:{
                SelectionSort<T> ss;
                ss(v);
                callMeet(&ss, 1, 10000, "Selection Sort");
                break;
            }

            case 4:{
                HeapSort<T> hs;
                cout << "Hoogte: " << v.getHoogte() << endl;
                hs(v);
                callMeet(&hs, 1, 1000000, "Heap Sort");
                break;
            }

            case 5:{
                MergeSort<T> ms;
                int top;
                cout << "Top down (1) or bottum up (2)?";
                cin >> top;
                while(top != 1 && top != 2) {
                    cout << "Wrong input: ";
                    cin >> top;
                }
                top==1?ms(v, true):ms(v, false);
                string name = "Merge Sort - ";
                callMeet(&ms, 1, 1000000, name + (top==1?"Top Down":"Bottom up"));
                break;
            }

            case 6:{
                QuickSort<T> qs;
                qs(v);
                //doesn't run well with  clion
                callMeet(&qs, 1, 1000, "Quick Sort");
                break;
            }

            case 7:{
                DualPivotQuickSort<T> dpqs;
                dpqs(v);
                callMeet(&dpqs, 1, 10000, "Dual Pivot QuickSort");
                break;
            }

            case 8: {
                T getal;
                cout << "Welk getal wil je zoeken: ";
                cin >> getal;
                int index = zoek(v, getal, true);
                if(index != -1) {
                    cout << "Getal gevonden op index " << index << endl;
                } else {
                    cout << "Getal niet gevonden" << endl;
                }
                break;
            }

            case 9: {
                T getal;
                cout << "Welk getal wil je zoeken: ";
                cin >> getal;
                int index = zoek(v, getal, false);
                if(index != -1) {
                    cout << "Getal gevonden op index " << index << endl;
                } else {
                    cout << "Getal niet gevonden" << endl;
                }
                break;
            }

            case 10:{
                //Not implemented
                CountSort<T> cs;
                cs(v);
                callMeet(&cs, 1, 1000000, "Dial Pivot QuickSort");
                break;
            }

            case 11:{
                //Not implemented
                RadixExchangeSort<T> res;
                res(v);
                callMeet(&res, 1, 1000000, "Radix Exchange Sort");
                break;
            }

            case 12:{
                //Not implemented
                MSDRadixSort<T> msd;
                msd(v);
                callMeet(&msd, 1, 1000000 , "MSD Radix Sort");
                break;
            }

            case 13:{
                //Not implemented
                TernaireRadixQuicksort<T> trq;
                trq(v);
                callMeet(&trq, 1, 1000000, "Ternaire Radix Sort");
                break;
            }

            case 14:{
                //Not implemented
                LSDRadixSort<T> lsd;
                lsd(v);
                callMeet(&lsd, 1, 1000000 , "LSD Radix Sort");
                break;
            }

            case 15:{
                //Not implemented
                BucketSort<T> bs;
                bs(v);
                callMeet(&bs, 1, 1000000, "Bucket Sort");
                break;
            }


            default:
                break;
        }

        if(size <= 20 &&input!=8&&input!=9) {
            cout << endl << "-----Sorted array-----" << endl;
            cout << v << endl;
        }
    } else if(input != -1) {
        cout << endl << "Wrong input" << endl;
    }

    return input;
}

void chooseSettings() {
    cout << "Give array size (default = 10): ";
    int size;
    if(cin.get() == '\n') {
        size = 10;
    }
    if(size < 2) {
        cout << "Wrong input, size set to 10" << endl;
    }
}

int main() {
    //chooseSettings();
    while(doSort<int>() != -1);

    return 0;
}

