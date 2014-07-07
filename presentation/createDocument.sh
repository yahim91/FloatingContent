#!/bin/bash
#pdflatex -interaction nonstopmode -file-line-error document
rubber --pdf slides
#pdflatex document
#bibtex document
#pdflatex document
#pdflatex document
#dvipdfm document.dvi
