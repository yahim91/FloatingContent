#!/bin/bash
#pdflatex -interaction nonstopmode -file-line-error document
bibtex document
rubber --pdf document
#pdflatex document
#bibtex document
#pdflatex document
#pdflatex document
#dvipdfm document.dvi
