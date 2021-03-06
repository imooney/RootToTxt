Isaac Mooney, WSU - Jan. 2021

This is a very simple macro to take some result histograms, for a publication, that reside in a .root file, and write the independent and dependent variable values to a table in a tab-separated .txt file. From there, one can then use YAML_maker, a project by Christine Nattrass' undergraduate student Tom Krobatsch (https://github.com/tkrobatsch/YAML_Maker) (tutorial by Christine here: https://www.youtube.com/watch?v=_hz6EVPeuW4&feature=youtu.be) to make YAML files from the .txt files which will be in the format required of that package. Then you should be ready to submit results to HepData!

There is now also a python script (copied and edited very slightly from Joseph Dominicus Lap's "formatter" script) called sigfigs.py which will render the data in your output .txt file compliant (in a new file) with the STAR experiment's rules for sig. fig. reporting (here: https://drupal.star.bnl.gov/STAR/pwg/common/policies/significant-digits-hepdata-table ).
To use it: after conversion of the root file to a txt file, say "Fig1_unformatted.txt" simply change "file_name" to "Fig1_unformatted.txt", and "new_file_name" to say "Fig1.txt". And make sure the input and output directory are set correctly. They default to the same directory as the script. Then just "python sigfigs.py".


Formatting requirements:
Just one input root file, with some number of histograms of data. Different "datasets" in the same file (e.g. different centralities or even different observables) can have different names, but different selections/panels (e.g. pT, eta, or other auxiliary independent variables) should be indexed with "_#" where # starts at 0 and increments by 1. E.g. "rho_0", "rho_1", "rho_2" for the underlying event density for three selections of rapidity. There do not need to be any statistical or systematic errors -- if there are any, they can either be the same histograms as those which give the data, or different histograms, as long as the indexing convention is followed. So, e.g. there could be one set of statistical errors to be pulled from the same histograms as the data: "rho_0", "rho_1", "rho_2", via hist->GetBinError(i), and two sources of systematic errors: "det_systs_0", "det_systs_1", "det_systs_2", and "proc_systs_0", "proc_systs_1", "proc_systs_2" (also via hist->GetBinError(i) - n.b., not with hist->GetBinContent(i)!). It's assumed that all histograms for a plot have the same number of bins (and indeed that the data is binned). That's it!

How to run:
1) Update settings.txt to your specifications (see instructions in that file). If you want one settings file for each figure you have, just update "root_to_txt.c"'s "settingsFile" to be "/path/to/your/settings/file/settings_figN.txt".
2) Run the macro with: "root root_to_txt.c" - this produces an output .txt in the same location as the input root file.
3) Format the .txt file(s) to STAR's sig. figs. requirement (see above) by changing in sigfigs.py: "in_dir" and "out_dir" (as necessary), and "file_name" and "new_file_name".
4) Run the python script with: python sigfigs.py.
5) Once you have all of your .txt files, you can download and make YAML_maker (see above), and run: "find /path/to/directory/containing/txt/files/* -exec ~/path/to/executible/yaml_data 0 {} \;"
6) You now have a bunch of properly formatted yaml files in that directory! Just make the submission.yaml, zip it all, and upload it to HepData!



TO DO:
* Might want to do a kind of batch submission to sigfigs.py so that if there are a lot of results, don't need to go one-by-one...
* At some point may go straight to yaml so we don't need an intermediary program, but don't want to reinvent the wheel.
* Generalize to asymmetric errors? Would need to incorporate TGraphAsymErrors into the functionality...
