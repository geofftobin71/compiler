CC = gcc
CFLAGS = -MMD
# CFLAGS = -MMD -std=c++11
INCLUDES = src

DEBUG ?= 0

ifeq ($(DEBUG), 1)
	BUILD = debug
	CFLAGS += -ggdb
else
	BUILD = release
	CFLAGS += -O2 -DNDEBUG
endif

TARGET = gt

SOURCES = $(notdir $(wildcard src/*.c))
OBJECTS = $(addprefix $(BUILD)/,$(subst .c,.o,$(SOURCES)))

all: $(TARGET)

$(TARGET): $(BUILD) $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@ $(LDLIBS)

CFLAGS += $(addprefix -I ,$(INCLUDES))

$(BUILD)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	test -d $@ || mkdir -p $@

DEPENDS = $(subst .o,.d,$(OBJECTS))

ifneq "$(MAKECMDGOALS)" "clean"
	-include $(DEPENDS)
endif

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS) $(DEPENDS)
