file(GLOB SRC
  *.cpp
  *.h
)

# 生成链接库
add_library (LeetCode ${SRC})