#
# Project:  SFC 2016
# Author:   Petr Kubat, xkubat11
# 
# Usage:
#         make -           compiles the project
#         make clean -     removes all temporary files
#         make clean-pdf - removes the pdf file
#         make clean-all - removes all files made by make
#

NAME=sfc
FILES=$(NAME).aux $(NAME).dvi $(NAME).log $(NAME).ps $(NAME).nav $(NAME).out $(NAME).snm $(NAME).toc

all:
	pdflatex $(NAME).tex
	pdflatex $(NAME).tex

clean:
	rm -f $(FILES)

clean-pdf:
	rm -f $(NAME).pdf

clean-all: clean clean-pdf
	
