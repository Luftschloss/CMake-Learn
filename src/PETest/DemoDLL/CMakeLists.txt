file(GLOB SRC
  *.cpp
  *.h
)

add_library(DemoDLL  SHARED  ${SRC})