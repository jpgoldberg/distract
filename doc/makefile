# makefile for producing the documentation
# note that note all of the macros needed for latexig drdoc are
# provided, so you won't be able to do this yourself

drdoc.dvi:	drdoc.tex drdoc.bbl
		latex drdoc

drdoc.bbl:	drdoc.bib
		bibtex drdoc

drdoc.ps:	drdoc.dvi
		dvips drdoc

drdoc.hp:	drdoc.dvi
		dvi2hp drdoc

drdoc.hpo:	drdoc.dvi
		dvi2hpts drdoc

drdoc.hpe:	drdoc.dvi
		dvi2hpts drdoc
