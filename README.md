Isaac Mooney, WSU - Jan. 2021

This is a very simple macro to take some result histograms, for a publication, that reside in a .root file, and write the independent and dependent variable values to a table in a tab-separated .txt file. From there, one can then use YAML_maker, a project by Christine Nattrass' undergraduate student Tom Krobatsch (https://github.com/tkrobatsch/YAML_Maker) (tutorial by Christine here: https://www.youtube.com/watch?v=_hz6EVPeuW4&feature=youtu.be) to make YAML files from the .txt files which will be in the format required of that package. Then you should be ready to submit results to HepData!

Formatting requirements:
Just one input root file, with some number of histograms of data. Different "datasets" in the same file (e.g. different centralities or even different observables) can have different names, but different selections/panels (e.g. pT, eta, or other auxiliary independent variables) should be indexed with "_#" where # starts at 0 and increments by 1. E.g. "rho_0", "rho_1", "rho_2" for the underlying event density for three selections of rapidity. There do not need to be any statistical or systematic errors -- if there are any, they can either be the same histograms as those which give the data, or different histograms, as long as the indexing convention is followed. So, e.g. there could be one set of statistical errors to be pulled from the same histograms as the data: "rho_0", "rho_1", "rho_2", via hist->GetBinError(i), and two sources of systematic errors: "det_systs_0", "det_systs_1", "det_systs_2", and "proc_systs_0", "proc_systs_1", "proc_systs_2" (also via hist->GetBinError(i) - n.b., not with hist->GetBinContent(i)!). It's assumed that all histograms for a plot have the same number of bins (and indeed that the data is binned). That's it!

TO DO:
* Make the code compliant with the STAR experiment's rules for sig. fig. reporting (here: https://drupal.star.bnl.gov/STAR/pwg/common/policies/significant-digits-hepdata-table )
* At some point may go straight to yaml so we don't need an intermediary program, but don't want to reinvent the wheel.