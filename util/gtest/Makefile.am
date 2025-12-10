
noinst_LIBRARIES = libgtest.a libgtest_main.a

# Copy gtest sources locally to avoid permission issues with dependency tracking
gtest-all.cc: $(GTEST_DIR)/src/gtest-all.cc
	cp $(GTEST_DIR)/src/gtest-all.cc .

gtest_main.cc: $(GTEST_DIR)/src/gtest_main.cc
	cp $(GTEST_DIR)/src/gtest_main.cc .

libgtest_a_SOURCES = gtest-all.cc
libgtest_a_CPPFLAGS = -I$(GTEST_DIR)/include -I$(GTEST_DIR)
libgtest_a_CXXFLAGS = -g -Wall -Wextra
libgtest_a_LDFLAGS = -pthread

libgtest_main_a_SOURCES = gtest_main.cc
libgtest_main_a_CPPFLAGS = -I$(GTEST_DIR)/include -I$(GTEST_DIR)
libgtest_main_a_CXXFLAGS = -g -Wall -Wextra
libgtest_main_a_LDFLAGS = -pthread
libgtest_main_a_LIBADD = libgtest.a

# Clean up copied files
CLEANFILES = gtest-all.cc gtest_main.cc

