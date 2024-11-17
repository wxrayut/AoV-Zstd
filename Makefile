CC = gcc
CFLAGS = -fPIC -Wall -Werror

SRC_DIR = ./src
BUILD_DIR = ./build

SRC_FILES = $(SRC_DIR)/args.c \
            $(SRC_DIR)/io.c \
            $(SRC_DIR)/main.c \
            $(SRC_DIR)/message.c \
            $(SRC_DIR)/utils.c \
            $(SRC_DIR)/version.c \
            $(SRC_DIR)/zstandard.c

OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

EXEC = AoV_Zstd

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $^ -lzstd
	rm -rf $(BUILD_DIR)/*.o

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Test case
decompress_with_dir_option:
	./$(EXEC) --decompress --dir ./tests/106_XiaoQiao/skill -o ./output -V

compress_with_dir_option:
	./$(EXEC) --compress --dir ./output -V

decompress_with_file_option:
	./$(EXEC) --decompress --file ./tests/106_XiaoQiao/imprint/10620_imprint.xml -o ./10620_imprint_decompressed.xml -V

compress_with_file_option:
	./$(EXEC) --compress --file ./10620_imprint_decompressed.xml -o ./10620_imprint_compressed.xml -V

test: decompress_with_dir_option compress_with_dir_option decompress_with_file_option compress_with_file_option

clean:
	rm -rf $(BUILD_DIR)/*.o $(EXEC)
	rm -rf $(BUILD_DIR)

.PHONY: all clean
