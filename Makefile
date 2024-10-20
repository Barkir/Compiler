BIN=bin
SRC=src
INCLUDE=include

SOURCES=$(notdir $(foreach dir,$(SRC),$(wildcard $(dir)/*.cpp)))
TARGET=$(addprefix $(BIN)/,$(shell basename `pwd`))
OBJECTS=$(addprefix $(BIN)/,$(SOURCES:.cpp=.o))

CFLAGS= -I -Wall \
    -pedantic -pedantic-errors -g3 -lm $(addprefix -I,$(INCLUDE))
LDFLAGS= -lm

vpath %.cpp $(SRC)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK.o) $^ $(OUTPUT_OPTION) $(LDFLAGS)

$(BIN)/%.o: %.cpp
	$(COMPILE.c) $< $(OUTPUT_OPTION)

clean:
	$(RM) $(OBJECTS) $(TARGET)



.PHONY: all clean
