file(GLOB SRC
  *.cpp
  *.h
)

# 生成链接库
add_executable (WinndowsMain ${SRC})