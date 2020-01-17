
noinst_LIBRARIES = libgtest.a libgtest_main.a

nodist_libgtest_a_SOURCES = $(GTEST_DIR)/src/gtest-all.cc

libgtest_a_CPPFLAGS = -I$(GTEST_DIR)/include -I$(GTEST_DIR)
libgtest_a_CXXFLAGS = -g -Wall -Wextra
libgtest_a_LDFLAGS = -pthread

nodist_libgtest_main_a_SOURCES = $(GTEST_DIR)/src/gtest_main.cc

libgtest_main_a_CPPFLAGS = -I$(GTEST_DIR)/include -I$(GTEST_DIR)
libgtest_main_a_CXXFLAGS = -g -Wall -Wextra
libgtest_main_a_LDFLAGS = -pthread
libgtest_main_a_LIBADD = libgtest.a

