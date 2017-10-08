FILE(GLOB SRCS *.h *.cpp
    )
LIST(REMOVE_ITEM SRCS 
    ${CMAKE_CURRENT_LIST_DIR}/PyImathAutovectorize.h
    ${CMAKE_CURRENT_LIST_DIR}/PyImathAutovectorize.cpp
    ${CMAKE_CURRENT_LIST_DIR}/PyImathM44Array.h
    ${CMAKE_CURRENT_LIST_DIR}/PyImathM44Array.cpp
    )
ADD_LIBRARY(imathmodule ${LIB_TYPE}
    python_include.h
    ${SRCS}
    )

SET_TARGET_PROPERTIES(imathmodule
    PROPERTIES 
    PREFIX "" 
    SUFFIX ".pyd" 
    BUILD_WITH_INSTALL_RPATH ON
    )

TARGET_COMPILE_DEFINITIONS(imathmodule PUBLIC
    PYIMATH_EXPORTS=1
    )

TARGET_LINK_LIBRARIES(imathmodule
    ${ILMBASE_LIBRARIES}
    ${PYTHON_LIBRARIES}
    )

