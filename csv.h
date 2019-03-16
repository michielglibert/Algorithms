#ifndef __CSV_H__
#define __CSV_H__
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::cerr;
using std::cin;
using std::cout;
using std::endl;


#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cassert>
#include <cmath>
#include <vector>

class CsvData{
public:
//scheidingsteken: teken in vlottendekommagetallen
//noot: voor een Nederlandstalige excel, scheidingsteken ',' opgeven
    CsvData(const std::string& _bestandsnaam, char _scheidingsteken='.'):
            bestandsnaam(_bestandsnaam),scheidingsteken(_scheidingsteken){};
    template<class T>//T kan int zijn, unsigned int, float, enz
    void voegDataToe(const std::vector<T>& nieuwedata){
        std::vector<double> nuvector(nieuwedata.size());
        for (int i=0; i<nieuwedata.size(); i++ ){
            nuvector[i]=static_cast<double>(nieuwedata[i]);//dataconversie
        }
        data.emplace_back(std::move(nuvector));
    }

//speciaal geval: geen dataconversie nodig
    void voegDataToe(const std::vector<double>& nieuwedata){
        data.push_back(nieuwedata);
    }
    char* formatteer(char* buf, double x){
        char formstring[12];
        sprintf(formstring,"%%%ie",kolombreedte);
        sprintf(buf,formstring,x);
        for (int i=0; i<kolombreedte; i++ ){
            if (buf[i]=='.')
                buf[i]=scheidingsteken;
        }
        return buf;
    }

    ~CsvData(){
        char besnaam[256];
        strcpy(besnaam,bestandsnaam.c_str());
        if (!strstr(besnaam,".csv")){//geen extensie? toevoegen!
            strcat(besnaam,".csv");
        }
        std::ofstream uit(besnaam);
        assert(uit);
        unsigned int maxlengte=0;//lengte langste kolom
        for (int i=0; i<data.size(); i++ ){
            if (maxlengte < data[i].size())
                maxlengte=data[i].size();
        }
        char* uitbuf =new char[kolombreedte+1];
        int maxbuflen=0;
        for (int i=0; i<maxlengte; i++ ){//maxlengte=aantal rijen
            for (int j=0; j<data.size() ;j++ ){
                if (data[j].size() > i)//blanco als te kort
                    uit<<formatteer(uitbuf,data[j][i]);
                if (strlen(uitbuf)>maxbuflen)
                    maxbuflen=strlen(uitbuf);
                uit<< (j==data.size()-1?
                       '\n':
                       ';');
            }
        }
//     std::cerr <<" maxbuflen "<<maxbuflen <<" kolombreedte "<<kolombreedte <<"\n";
        delete uitbuf;
    }
protected:
    std::vector<std::vector<double> > data;
    char scheidingsteken;
    std::string bestandsnaam;
    static const int kolombreedte;
};
const int CsvData::kolombreedte=12;

#endif
