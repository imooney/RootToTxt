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

//pass: k < N*M, x-axis, y-axes, labels, txtfile
void header (const int i, const int j, const string xtitle, vector<vector<string>> ytitles, vector<vector<string>> labels, ofstream& file) {
    file << xtitle << "\n";
    file << 2 << "\n"; //N_datasets / file = 1 by default
    file << ytitles[i][j] << "\n";
    file << labels[i][j] << "\n";
    file << "yes\n"; //data is binned
    file << "none\n"; //data has no x-stat err.
    file << "none\n"; //data has no x-syst err.
    file << "1\n"; //number of uncertainties
    file << "stat\n"; //uncertainty 1 label
    file << "symmetric\n"; //errors are symmetric
    file << "syst\n"; //uncertainty 2 label
    file << "symmetric\n"; //errors are symmetric
    
    cout << "header written" << endl;
    return;
}

void body (TH1D* hist, TH1D* hstat, TH1D* hsyst, ofstream& file) {
    
    for (int i = 1; i <= hist->GetNbinsX(); ++ i) {//hstat->GetNbinsX(),hsyst->GetNbinsX() must give same #
        file << hist->GetXaxis()->GetBinLowEdge(i) << "-" << hist->GetXaxis()->GetBinUpEdge(i) << "\t";
        file << hist->GetBinContent(i) << "\t";
        file << hstat->GetBinError(i) << "\t";
        file << hsyst->GetBinError(i) << "\n";
    }
    cout << "body written" << endl;
    return;
}

void root_to_txt() {
    
    // !!!!!! to be changed to suit your needs !!!!!! //
    const int N = 2; // N data sets
    const int M = 1; // M panels (e.g. variation over R or pT)
    const string root_in = "/Users/imoo/physics/AN/rootfiles/Fig2.root";
    const string root_in_substr = root_in.substr(0,root_in.length() - 5); //removes ".root".
    //const string txt_out = (root_in_substr+".txt").c_str();
    vector<string> datname;
    datname.push_back("nom_");
    datname.push_back("nom_g_");
    vector<string> statname = datname;//assumes that the data histograms also have the statistical errors
    vector<string> systname;
    systname.push_back("nom_");//w_systs_
    systname.push_back("nom_g_");//w_systs_g_
    
    const string xaxis = "$p_{T,jet} [GeV/c]$";
    
    vector<vector<string>> yaxes;
    for (int i = 0; i < N; ++ i) {
        vector<string> yaxes_aux;
        if (i == 0) {
        for (int j = 0; j < M; ++ j) {
            //default -- all data sets and selections have the same y-axis title:
            yaxes_aux.push_back("$1/N dN/dM [GeV/c^{2}]$");
        }
            yaxes.push_back(yaxes_aux);
        }
        if (i == 1) {
        for (int j = 0; j < M; ++ j) {
            //default -- all data sets and selections have the same y-axis title:
            yaxes_aux.push_back("$1/N dN/dM_{g} [GeV/c^{2}]$");
        }
            yaxes.push_back(yaxes_aux);
        }
        
    }
    
    vector<vector<string>> labels;
    //vector<string> labels_aux;
    //default to length 3. Not sure how to make this more general and less hard-coded, since the labels are unique to the particular paper and figure and will all be distinct from each other.
    //if (N*M < 3 || N*M > 3) {cerr << "Change the labels to suit this number of data sets / auxiliary independent variable selections!" << endl; exit(1);}
    //labels_aux.push_back("$20 < p_{T,jet} < 25 GeV/c$");
    //labels_aux.push_back("$25 < p_{T,jet} < 30 GeV/c$");
    //labels_aux.push_back("$30 < p_{T,jet} < 40 GeV/c$");
    for (int i = 0; i < N; ++ i) {
        vector<string> labels_aux;
        if (i == 0) {
    labels_aux.push_back("$M$ for $20 < p_{T} < 25 GeV/c");
    //labels_aux.push_back("$<M>$ for $R = 0.4$");
    //labels_aux.push_back("$<M>$ for $R = 0.6$");
    labels.push_back(labels_aux);
        }
        if (i == 1) {
    labels_aux.push_back("$M_{g}$ for $20 < p_{T} < 25 GeV/c$");
    //labels_aux.push_back("$<M_{g}>$ for $R = 0.4$");
    //labels_aux.push_back("$<M_{g}>$ for $R = 0.6$");
    labels.push_back(labels_aux);
        }
    }

    //!!!!!! !!!!!!//
    
    TFile *fin = new TFile(root_in.c_str(),"READ");

    for (int i = 0; i < N; ++ i) {
         for (int j = 0; j < M; ++ j) {
             string fileName = root_in_substr+"_dataset"+to_string(i)+"_selection"+to_string(j)+".txt";
             ofstream fout;
             fout.open(fileName);
             if (fout.is_open()) {cout << "opened " << fileName << endl;}
             else {cout << fileName << " could not be opened" <<endl;}
             
             //only one distribution per file, for NxM files.
             header (i, j, xaxis, yaxes, labels, fout);
             
             TH1D* hist = (TH1D*) fin->Get((datname[i]+to_string(j)).c_str());
             TH1D* hstat = (TH1D*) fin->Get((statname[i]+to_string(j)).c_str());//prob. a copy of "hist"
             TH1D* hsyst = (TH1D*) fin->Get((systname[i]+to_string(j)).c_str());
             
             body(hist, hstat, hsyst, fout);
             fout << "***";
             
             cout << "closing " << fileName << endl;
             fout.close();
         }
    }
    
    return;
}
