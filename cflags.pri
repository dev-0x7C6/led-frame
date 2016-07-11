QMAKE_CFLAGS += -Werror -Wall -std=c++17
QMAKE_CXXFLAGS += -Werror -Wall -std=c++17
QMAKE_CFLAGS_RELEASE += -march=native -O2 -pipe -fomit-frame-pointer -ffast-math -std=c++17
QMAKE_CXXFLAGS_RELEASE += -march=native -O2 -pipe -fomit-frame-pointer -ffast-math -std=c++17
QMAKE_CFLAGS_DEBUG += -O0 -ggdb3 -g3 -fno-omit-frame-pointer -std=c++17
QMAKE_CXXFLAGS_DEBUG += -O0 -ggdb3 -g3 -fno-omit-frame-pointer -std=c++17
QMAKE_RESOURCE_FLAGS += -threshold 0 -compress 9
#Leak sanitizer
#QMAKE_CFLAGS_DEBUG += -fsanitize=leak
#QMAKE_CXXFLAGS_DEBUG += -fsanitize=leak
#LIBS += -llsan
#Undefined behavior sanitizer
#QMAKE_CFLAGS_DEBUG += -fsanitize=undefined
#QMAKE_CXXFLAGS_DEBUG += -fsanitize=undefined
#LIBS += -lubsan
#Address sanitizer
#QMAKE_CFLAGS_DEBUG += -fsanitize=address
#QMAKE_CXXFLAGS_DEBUG += -fsanitize=address
#LIBS += -lasan
#Thread sanitizer
#QMAKE_CFLAGS_DEBUG += -fsanitize=thread -fPIE -pie
#QMAKE_CXXFLAGS_DEBUG += -fsanitize=thread -fPIE -pie
#QMAKE_LFLAGS += -pie
#LIBS += -ltsan
