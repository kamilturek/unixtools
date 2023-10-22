SRC_FILES := $(wildcard *.c)
EXECUTABLE := tail

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC_FILES)
	gcc -o $@ $(SRC_FILES) -lextras

clean:
	rm -f $(OBJ_FILES)

.PHONY: all clean install
