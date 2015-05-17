PUML_FILES := $(wildcard *.puml)
PNG_FILES := $(PUML_FILES:.puml=.png)

all: $(PNG_FILES)

%.png: %.puml
	java -jar ./plantuml.jar $<

clean:
	rm -f *.png

