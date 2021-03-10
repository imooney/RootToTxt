"""
This is a adaptation of the program described in the comment below - thanks to the author for saving me some work. I both generalized and specified it for use with a small project of mine called "RootToTxt" which is just a macro to convert data in a root file to a txt file for eventual conversion to yaml to submit as HepData.
Specification: the txt file I input has some variable number of comment lines before the data, so instead of looking at the second line of the file, this program searches for the first bin (assumes the data is binned) by looking for the string "*-*" where * is any number. It skips over any comment lines, say separating datasets if there are multiple.
Generalizations:
    *There can now be any number of uncertainty columns. We look for the smallest one to determine the number of decimal places of the data value, and format each uncertainty to its proper number of sig. figs.
    *The code had a problem with empty bins (the log function used for sig figs gave an infinity), and with scientific notation in the input file (it checked to see if any zeroes should be appended to the output number string, but after converting a small number to a string it would be in scientific notation and append the zero to the exponent making e.g. 9e-05 into 9e-050). The code now allows empty bins and scientific notation.
-Isaac Mooney, WSU, Feb. 2021
"""

"""
This is a little program to edit .txt files to match the PDG format for error
Version 2.1 (this version includes more exceptions)
Author: Joseph Dominicus Lap
"""

from matplotlib import pyplot as plt
import math
import decimal
import numpy as np
import sys
from decimal import *
import re

#assumes for now that the files are in the same directory as the script - change to suit your needs
in_dir='/path/to/dir/in/'
out_dir='/path/to/dir/out/'

#change to the input and output file names respectively
file_name='in.txt'
new_file_name = 'out.txt'

file=in_dir+file_name
newfile=out_dir+new_file_name

def read_file(file, newfile):
    '''
    input - file to read, name of newfile to write
    '''
    file=open(file, 'r')
    newfile=open(newfile, 'w')
    line=file.readline()
    while line:                     #for each new line in file
        newline=parse_line(line)
        newfile.write(str(newline))         #write the return of parse_line to new file
        line=file.readline()
        
def parse_line(line):
        if re.match('.*[0-9]\t.*[0-9]', line):
                subline=np.fromstring(line, dtype=float, sep='\t')
                array_no_bin=subline[2:len(subline)]
                bin = line.split('\t')[0]+'\t'+line.split('\t')[1]
                er_var_dig_arr=[]
                for i in range(1,len(array_no_bin)): #starts at 1 bc we skip the value, only compare error sizes
                    er_var_dig_arr.append(ERR_Format(array_no_bin[i]))
                errs = [x[0] for x in er_var_dig_arr] #[er1,er2,er3,...]
                dig=0
                var=0
                newline=[bin]
                var = er_var_dig_arr[np.argmin(errs)][1]
                dig = er_var_dig_arr[np.argmin(errs)][2] #second value in list corresponding to min. error size of all types of error
                rounded_number=[my_round(array_no_bin[0],var,dig)]
                newline = newline+rounded_number+errs
                newline.append('\n')
                newline = str('\t'.join(newline)) #collapses array into single line separated by tabs
        if re.match('.*[0-9]\t.*[0-9]', line)==None:
            newline=line
            
        return(newline)

#A script to round correctly. Python default uses IEEE754 standard which rounds to even numbers.
def my_round(n, var,dig):
    #This gives us our precision
    ndigits=var-dig
    part = Decimal(n) * 10 ** ndigits
    delta = part - int(part)
    # always round "away from 0"
    if delta >= 0.5 or -0.5 < delta <= 0:
        part = math.ceil(part)
    else:
        part = math.floor(part)
    output=str(Decimal(part) / (10 ** ndigits))
    #Here we make sure we don't get rid of trailing zeroes
    if(dig>=2 and len(output)!=dig):
        print('This should not occur. Something is wrong')
        sys.exit()
    elif(dig<2 and len(output)!=var-dig+2 and part != 0):
        output+='0'
    return output

#A little script to follow PDG guidelines on error
def ERR_Format(err):
    #How many digits is our error?
    if (err == 0):
        digits = 0
    else:
        digits=int(np.floor(np.log10(Decimal(err)))+1)
    #What are the first 3 digits of our error?
    #We've removed the annoying edge case of  having a '.' as a digit
    if (digits<3):
        rounding=int((10**(3-digits))*Decimal(err))
    else:
        rounding=int(err[:3])
    #Here we check the first 3 digits and decide
    if (rounding <355):
        #2 significant figures
        var=2
    elif(rounding<949):
        # 1 significant figure
        var=1
    else:
        # round the error up, but keep 2 sigfig on the value
        var=1
    #Here we round the error to the specified amount
    #%g takes off trailing 0's
    realerr=my_round(err,var,digits)
    return realerr,var,digits

read_file(file, newfile)
