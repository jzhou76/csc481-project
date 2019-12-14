TARGET = report

LATEX = pdflatex
BIBTEX = bibtex

TEXFILES = $(wildcard *.tex) $(wildcard tables/*.tex) $(wildcard algo/*.tex)
BIBFILES := $(wildcard *.bib) $(wildcard bib/*.bib)

$(TARGET): $(TARGET).pdf

$(TARGET).pdf: $(TEXFILES) $(BIBFILES)
	$(MAKE) clean
	$(LATEX) $(TARGET)
	# $(BIBTEX) $(TARGET)
	# $(LATEX) $(TARGET)
	# $(LATEX) $(TARGET)
	@/bin/echo ""
	@/bin/echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	@/bin/echo "               ++++ ANY UNDEFINED REFERENCES ++++"
	-@grep -i undef $(TARGET).log || echo "No undefined references."
	@/bin/echo "                 ++++ ANY EMPTY REFERENCES ++++"
	-@egrep -i -n -e 'cite{ *}' -e 'ref{ *}' $(TEXFILES) $(FIGFILES) || echo "No empty references."
	@/bin/echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
	@/bin/echo ""

clean:
	rm -rf *.aux *.bbl *.log *.blg *.toc *.out
clean-all: clean
	rm -rf $(TARGET).pdf

.PHONY: clean clean-all


