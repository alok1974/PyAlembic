ADD_LIBRARY(iexmodule ${LIB_TYPE}
    iexmodule.cpp
    )

TARGET_LINK_LIBRARIES(iexmodule
    ${PYTHON_LIBRARIES}
    ${ILMBASE_LIBRARIES}
    )

SET_TARGET_PROPERTIES(iexmodule
    PROPERTIES 
    PREFIX "" 
    SUFFIX ".pyd" 
    BUILD_WITH_INSTALL_RPATH ON
    )

