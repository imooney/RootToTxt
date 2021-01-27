Isaac Mooney, WSU - Jan. 2021

This is a simple very simple macro to take some result histograms for a paper that reside in a .root file, and write the independent and dependent variable values to a table in a tab-separated .txt file. From there, one can then use YAML_maker, a project by Christine Nattrass' undergraduate student Tom Krobatsch (https://github.com/tkrobatsch/YAML_Maker) (tutorial by Christine here: https://www.youtube.com/watch?v=_hz6EVPeuW4&feature=youtu.be) to make YAML files from the .txt files which will be in the format required of that package. Then you should be ready to submit results to HepData!

TO DO:
* Write in this file the formatting requirements for the input .root files.
* Make a .txt file with the user-specified parameters for a given .root file, to be read by the program, so the macro does not need to be touched at all.
* Make the code compliant with the STAR experiment's rules for sig. fig. reporting (here: https://drupal.star.bnl.gov/STAR/pwg/common/policies/significant-digits-hepdata-table )
* At some point may go straight to yaml so we don't need an intermediary program, but don't want to reinvent the wheel.