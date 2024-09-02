# Compiler flags
CCFLAGS = -W -Wall -Werror -g

# Use pkg-config to retrieve the necessary flags
PKG_CFLAGS = `pkg-config --cflags glib-2.0 gtk+-3.0`
PKG_LIBS = `pkg-config --libs glib-2.0 gtk+-3.0`

# Directories (use relative paths)
PIXEL_DIR = ./zpixel
IMAGE_DIR = ./image
ARBRE_DIR = ./arbre_zpixels

# Source files
SOURCES = main.c $(PIXEL_DIR)/zpixel.c $(IMAGE_DIR)/image.c $(ARBRE_DIR)/arbre_zpixels.c

# Object files
OBJECTS = main.o $(PIXEL_DIR)/zpixel.o $(IMAGE_DIR)/image.o $(ARBRE_DIR)/arbre_zpixels.o

# Include directories
INCLUDES = -I$(PIXEL_DIR) -I$(IMAGE_DIR) -I$(ARBRE_DIR)

# Target to build the program
prog: $(OBJECTS)
	gcc -g $(OBJECTS) -o prog $(PKG_LIBS) -lm

# Rule to compile source files into object files
%.o: %.c
	gcc -c $(CCFLAGS) $< -o $@ $(PKG_CFLAGS) $(INCLUDES)

# Clean up
rm:
	rm -f $(OBJECTS) prog
