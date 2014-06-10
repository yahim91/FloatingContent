#!/bin/bash

latex document
bibtex document
latex document
latex document
dvipdfm document.dvi
