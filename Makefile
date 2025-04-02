# 设置 C++ 编译选项
CXXFLAGS += -std=c++1z -Wno-deprecated-declarations -finput-charset=UTF-8 -fexec-charset=UTF-8 \
            $(shell /opt/wxWidgets-3.2.6/build-cocoa-debug/wx-config --cxxflags) \
            -I/opt/homebrew/opt/xz/include \
            -I/opt/homebrew/opt/zstd/include \
            -I/opt/homebrew/opt/jbigkit/include \
            -I/opt/homebrew/opt/pcre2/include \
            -I/opt/liblerc/include \
            -I/opt/libtiff/include \
            -I/opt/homebrew/opt/jpeg-turbo/include \
            -I/opt/webp/include \
            -I/opt/openssl/include

# 设置链接器选项
LDFLAGS += $(shell /opt/wxWidgets-3.2.6/build-cocoa-debug/wx-config --libs) \
           -L/opt/homebrew/opt/xz/lib \
           -L/opt/homebrew/opt/zstd/lib \
           -L/opt/homebrew/opt/pcre2/lib \
           -L/opt/homebrew/opt/jbigkit/lib \
           -L/opt/liblerc/lib \
           -L/opt/libtiff/lib \
           -L/opt/homebrew/opt/jpeg-turbo/lib \
           -L/opt/openssl/lib \
           -L/opt/webp/lib

# 设置框架（仅在 macOS 下需要）
ifeq ($(shell uname), Darwin)
    LDFLAGS += -framework Cocoa \
               -framework QuartzCore \
               -framework AudioToolbox \
               -framework IOKit \
               -framework Security \
               -framework OpenGL \
               -framework Carbon \
               -Wno-deprecated-declarations
endif

# 设置库文件
LIBS += /opt/homebrew/opt/xz/lib/liblzma.a \
        /opt/homebrew/opt/zstd/lib/libzstd.a \
        /opt/liblerc/lib/libLerc.a \
        /opt/homebrew/opt/pcre2/lib/libpcre2-32.a \
        /opt/homebrew/opt/jpeg-turbo/lib/libjpeg.a \
        /opt/homebrew/opt/jbigkit/lib/libjbig.a \
        /opt/openssl/lib/libssl.a \
        /opt/openssl/lib/libcrypto.a \
        /opt/libtiff/lib/libtiff.a \
        /opt/webp/lib/libwebp.a \
        /opt/webp/lib/libsharpyuv.a

# 源文件和目标文件
#SRC = CustomButton.cpp ImageButton.cpp pic/imageon.cpp pic/imageoff.cpp pic/imageon1.cpp pic/imageoff1.cpp pic/yybj.cpp pic/yybj2.cpp pic/bubj.cpp pic/bubj2.cpp zdelv.cpp
# 源文件和目标文件
SRC_CPP = CustomButton.cpp ImageButton.cpp MyFrame.cpp MyFrame2.cpp \
          MyProcess.cpp MyTaskBarIcon.cpp AutoCloseDialog.cpp ThemeListener.cpp CustomGauge.cpp \
          pic/imageon.cpp pic/imageoff.cpp \
          pic/imageon1.cpp pic/imageoff1.cpp \
          pic/yybj.cpp pic/yybj2.cpp \
          pic/bubj.cpp pic/bubj2.cpp pic/icon.cpp

SRC_MM = zdelv.mm Command.mm

#OBJ = $(SRC:.cpp=.o)
OBJ = $(SRC_CPP:.cpp=.o) $(SRC_MM:.mm=.o)
TARGET = zdelv

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LDFLAGS) $(LIBS) -o $(TARGET)
	@echo "Build successful: $(TARGET)"

# 编译 C++ 文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 编译 Objective-C++ 文件
%.o: %.mm
	$(CXX) $(CXXFLAGS) -ObjC++ -c $< -o $@

# 清理目标
clean:
	rm -f $(OBJ) $(TARGET)
