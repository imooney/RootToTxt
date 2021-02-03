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


//~~scratch for self~~:
//need to figure out what pre-formatting to do.
    //will probably require "name_" followed by a number from 0 to M-1, for each panel, and one "name" for each data set and a separate "name" for systematics.
//need to figure out star's requirements for sig figs and how to code them up.
//note: only going to consider 1 auxiliary independent variable selection per file

using namespace std;

//pass: i < N, j < M, x-axis, y-axes, labels, errlabels, txtfile
void header (const int i, const int j, const string xtitle, vector<string> ytitles, vector<vector<string>> labels, vector<string> errlabels, ofstream& file) {
    file << xtitle << "\n";
    file << 1 << "\n"; //N_datasets / output_file = 1 by default
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
    if (burn[0] != '#') {
        cerr << "Somehow an input line was treated as a comment! Please check the instructions in the settings.txt file, and try again." << endl; exit(1);
    }
    getline(file, val);
    if (val[0] == '#') {
        cerr << "Somehow a comment line was treated as an input! Please check the instructions in the settings.txt file, and try again." << endl; exit(1);
    }
    return;
}

void get_settings (int& N, int& M, string& root_in, string& xaxis, vector<string>& yaxes, vector<vector<string>>& labels, vector<string>& errlabels, vector<string>& datname, vector<vector<string>>& errnames) {

    string settingsFile = "../settings_Fig4b.txt";
    ifstream fset;
    fset.open(settingsFile);
    if (fset.is_open()) {cout << "opened " << settingsFile << endl;}
    else {cerr << settingsFile << " could not be opened" <<endl; exit(1);}
    
    string val;
    string burn; //burn is for comments that we skip over - not strictly necessary, but helpful mentally
    
    get_line(fset,burn,val);
    N = stoi(val);// N data sets
    get_line(fset,burn,val);
    M = stoi(val); // M panels (e.g. variation over R or pT)
    get_line(fset,burn,val);
    root_in = val;
    getline(fset,burn);
    for (int i = 0; i < N; ++ i) {
        getline(fset,val);//normal getline since we have N lines without comments
        datname.push_back(val);
    }
    //how many uncertainties?
    get_line(fset,burn,val);
    int Nerrs = stoi(val);
    getline(fset,burn);
    for (int i = 0; i < N; ++ i) {
        vector<string> errname_aux;
        for (int j = 0; j < Nerrs; ++ j) {
            getline(fset,val);
            errname_aux.push_back(val);
        }
        errnames.push_back(errname_aux);
    }
    
    getline(fset,burn);
    for (int i = 0; i < Nerrs; ++ i) {
        getline(fset,val);
        errlabels.push_back(val);
    }
    
    get_line(fset,burn,val);
    xaxis = val;
    
    getline(fset,burn);
    for (int i = 0; i < N; ++ i) {
        getline(fset,val);
        yaxes.push_back(val);
    }
    
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
    
    fset.close();
    cout << "closed settings.txt" << endl;
    
    return;
}

void root_to_txt() {
  
    //things to be set each time (possibly un-hard-coded later):
    int N; //number of datasets
    int M; //number of "panels" (i.e. auxiliary independent variables like pT)
    string root_in; //name of root file
    string xaxis; //title of x-axis
    vector<string> yaxes; //title of y-axis
    vector<vector<string>> labels; //dataset & "panel" labels
    vector<string> errlabels; //e.g. "stat", "syst", ...
    vector<string> datname; //histograms from which we pull values
    vector<vector<string>> errnames; //histograms from which we pull uncertainties
    
    get_settings(N, M, root_in, xaxis, yaxes, labels, errlabels, datname, errnames);
    
    string extension = ".root";
    int len_extension = extension.length();
    const string root_in_substr = root_in.substr(0,root_in.length() - len_extension); //removes ".root" (5 characters long)
    
    TFile *fin = new TFile(root_in.c_str(),"READ");

    for (int i = 0; i < N; ++ i) {
         for (int j = 0; j < M; ++ j) {
             string fileName = root_in_substr+"_dataset"+to_string(i)+"_selection"+to_string(j)+".txt";
             ofstream fout;
             fout.open(fileName);
             if (fout.is_open()) {cout << "opened " << fileName << endl;}
             else {cerr << fileName << " could not be opened" <<endl; exit(1);}
             
             //only one distribution per file, for NxM files.
             header (i, j, xaxis, yaxes, labels, errlabels, fout);
             
             TH1D* hist = (TH1D*) fin->Get((datname[i]+to_string(j)).c_str());
             vector<TH1D*> herrs;
             for (int k = 0; k < errlabels.size(); ++ k) {
                 herrs.push_back((TH1D*) fin->Get((errnames[i][k]+to_string(j)).c_str()));
             }
             
             body(hist, herrs, fout);
             fout << "***";
             
             fout.close();
             cout << "closed " << fileName << endl;
         }
    }
    
    cout << "done!" << endl;
    return;
}
