//  root_to_txt.c
//  Created by Isaac Mooney on 1/22/21.

#include <iostream>
#include <fstream>
#include <vector>

//The purpose of this macro is to prepare a .txt file
//for conversion to the .yaml format required for
//submission to HepData.
//It takes in a root file (for a single figure in the paper) with:
    //N data sets
    //M panels. A 2-dimensional dependence of the observable, e.g. X-sec (p_{T}, R), is assumed - I don't know how easy this would be to extend to n-dim. (However, YAML-maker does not have the ability to read in a 2D dependence as far as I know, so it will be coded rather as NxM "data sets" for the observable).
    //symmetric statistical uncertainties (should be easily extensible to asymmetric uncertainties)
    //one set of systematic uncertainties for each data set (i.e. the net systematic). Should also be easily extensible to multiple kinds of systematic uncertainty.
//It outputs (to one .txt file per data set and panel) in the format preferred by YAML_maker, a project by Christine Nattrass' undergraduate student Tom Krobatsch  (https://github.com/tkrobatsch/YAML_Maker) (tutorial by Christine here: https://www.youtube.com/watch?v=_hz6EVPeuW4&feature=youtu.be)

//things to be set each time (possibly un-hard-coded later):
//N
//M
//name of root file - probably have it named according to the fig. number.
//name of hist for data points
//name of hist for stat errs (probably name of hist for data)
//name of hist for sym errs
//x-axis title, y-axis title(s) - just one y-axis title for now
//dataset labels (NxM of them)


//~~scratch for self~~:
//need to figure out what pre-formatting to do.
    //will probably require "name_" followed by a number from 0 to M-1, for each panel, and one "name" for each data set and a separate "name" for systematics.
//need to figure out how header and body are separated.
//need to figure out star's requirements for sig figs and how to code them up.
//note: only going to consider 1 auxiliary independent variable selection per file

using namespace std;

//pass: i < N, j < M, x-axis, y-axes, labels, errlabels, txtfile
void header (const int i, const int j, const string xtitle, vector<string> ytitles, vector<vector<string>> labels, vector<string> errlabels, ofstream& file) {
    file << xtitle << "\n";
    file << 2 << "\n"; //N_datasets / file = 1 by default
    file << ytitles[i] << "\n";
    file << labels[i][j] << "\n";
    file << "yes\n"; //data is binned
    file << "none\n"; //data has no x-stat err.
    file << "none\n"; //data has no x-syst err.
    file << errlabels.size() << "\n"; //number of uncertainties
    for (int k = 0; k < errlabels.size(); ++ k) {
        file << errlabels[k] << "\n";
        file << "symmetric\n"; //errors are symmetric
    }
    
    cout << "header written" << endl;
    return;
}

void body (TH1D* hist, vector<TH1D*> herrs, ofstream& file) {
    
    for (int i = 1; i <= hist->GetNbinsX(); ++ i) {//hstat->GetNbinsX(),hsyst->GetNbinsX() must give same #
        file << hist->GetXaxis()->GetBinLowEdge(i) << "-" << hist->GetXaxis()->GetBinUpEdge(i) << "\t";
        file << hist->GetBinContent(i) << "\t";
        for (int j = 0; j < herrs.size(); ++ j) {
            file << herrs[j]->GetBinError(i);
            if (j != (herrs.size()-1)) { file << "\t";}
            else {file << "\n";}
        }
    }
    cout << "body written" << endl;
    return;
}

//helper function to make sure I never forget to burn the comment line
void get_line (ifstream& file, string& burn, string& val) {
    getline(file, burn);
    getline(file, val);
    return;
}

void root_to_txt_inprogress() {
  
    cout << "A" << endl;
    string settingsFile = "settings.txt";
    ifstream fset;
    fset.open(settingsFile);
    if (fset.is_open()) {cout << "opened " << settingsFile << endl;}
    else {cerr << settingsFile << " could not be opened" <<endl; exit(1);}
    
    cout << "B" << endl;
    string val;
    string burn; //burn is for comments that we skip over - not strictly necessary, but helpful mentally
    
    get_line(fset,burn,val);
    int N = stoi(val);// N data sets
    get_line(fset,burn,val);
    int M = stoi(val); // M panels (e.g. variation over R or pT)
    get_line(fset,burn,val);
    string root_in = val;
    string extension = ".root";
    int len_extension = extension.length();
    cout << len_extension << endl;
    const string root_in_substr = root_in.substr(0,root_in.length() - len_extension); //removes ".root" (5 characters long)
    vector<string> datname;
    getline(fset,burn);
    for (int i = 0; i < N; ++ i) {
        getline(fset,val);//normal getline since we have N lines without comments
        datname.push_back(val);
    }
    //how many uncertainties?
    get_line(fset,burn,val);
    int Nerrs = stoi(val);
    cout << "test: " << Nerrs << endl;
    vector<vector<string>> errnames;
    getline(fset,burn);
    for (int i = 0; i < N; ++ i) {
        vector<string> errname_aux;
        for (int j = 0; j < Nerrs; ++ j) {
            getline(fset,val);
            errname_aux.push_back(val);
        }
        errnames.push_back(errname_aux);
    }
    
    cout << "C" << endl;
    
    vector<string> errlabels;
    getline(fset,burn);
    for (int i = 0; i < Nerrs; ++ i) {
        getline(fset,val);
        errlabels.push_back(val);
    }
    
    get_line(fset,burn,val);
    const string xaxis = val;
    
    getline(fset,burn);
    vector<string> yaxes;
    for (int i = 0; i < N; ++ i) {
        getline(fset,val);
        cout << "VAL? " << val << endl;
        yaxes.push_back(val);
        cout << "yak? " << yaxes[i] << endl;
    }
    
    cout << "D" << endl;
    
    vector<vector<string>> labels;
    vector<string> labels_aux;
    getline(fset,burn);
    for (int i = 0; i < N; ++ i) {
        vector<string> labels_aux;
        for (int j = 0; j < M; ++ j) {
            getline(fset,val);
            labels_aux.push_back(val);
        }
        labels.push_back(labels_aux);
    }

    //!!!!!! !!!!!!//
    
    cout << "E" << endl;
    
    TFile *fin = new TFile(root_in.c_str(),"READ");

    for (int i = 0; i < N; ++ i) {
         for (int j = 0; j < M; ++ j) {
             cout << "F" << " " << i << " " << j << endl;
             string fileName = root_in_substr+"_dataset"+to_string(i)+"_selection"+to_string(j)+"_temp.txt";
             ofstream fout;
             fout.open(fileName);
             if (fout.is_open()) {cout << "opened " << fileName << endl;}
             else {cerr << fileName << " could not be opened" <<endl; exit(1);}
             
             //only one distribution per file, for NxM files.
             header (i, j, xaxis, yaxes, labels, errlabels, fout);
             
             TH1D* hist = (TH1D*) fin->Get((datname[i]+to_string(j)).c_str());
             vector<TH1D*> herrs;
             for (int k = 0; k < Nerrs; ++ k) {
                 herrs.push_back((TH1D*) fin->Get((errnames[i][k]+to_string(j)).c_str()));
             }
             
             body(hist, herrs, fout);
             fout << "***";
             
             cout << "closing " << fileName << endl;
             fout.close();
         }
    }
    
    return;
}
