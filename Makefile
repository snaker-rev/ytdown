CXX      := g++
CXXFLAGS := -std=c++20 -O2
TARGET   := ytdlp
SRC      := ytdlp.cpp

FTXUI_DIR   := ftxui
FTXUI_REPO  := https://github.com/ArthurSonzogni/FTXUI
FTXUI_TAG   := v5.0.0
FTXUI_BUILD := $(FTXUI_DIR)/build

INCLUDES := -I$(FTXUI_DIR)/include
LIBS     := -L$(FTXUI_BUILD) \
            -lftxui-component -lftxui-dom -lftxui-screen

.PHONY: all run clean

# If binary exists, just run it. Otherwise build then run.
all:
	@if [ -f "./$(TARGET)" ]; then \
		echo "[make] Binary already exists, running directly..."; \
		./$(TARGET); \
	else \
		$(MAKE) build && ./$(TARGET); \
	fi

build: $(FTXUI_BUILD)/libftxui-component.a $(SRC)
	@echo "[make] Compiling $(TARGET)..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(SRC) $(LIBS) -o $(TARGET)
	@echo "[make] Done → ./$(TARGET)"

# Clone FTXUI if missing
$(FTXUI_DIR)/.git:
	@echo "[make] Cloning FTXUI $(FTXUI_TAG)..."
	git clone --depth 1 --branch $(FTXUI_TAG) $(FTXUI_REPO) $(FTXUI_DIR)

# Build FTXUI static libs
$(FTXUI_BUILD)/libftxui-component.a: $(FTXUI_DIR)/.git
	@echo "[make] Building FTXUI..."
	cmake -S $(FTXUI_DIR) -B $(FTXUI_BUILD) \
		-DFTXUI_BUILD_EXAMPLES=OFF \
		-DFTXUI_BUILD_TESTS=OFF \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_SHARED_LIBS=OFF \
		> /dev/null
	cmake --build $(FTXUI_BUILD) --parallel $$(nproc)

run:
	@./$(TARGET)

clean:
	@echo "[make] Cleaning..."
	rm -f $(TARGET)
	rm -rf $(FTXUI_DIR) $(FTXUI_BUILD)