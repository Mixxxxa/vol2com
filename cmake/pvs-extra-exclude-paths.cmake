if(__CURRENT_FILE_VAR__)
  return()
endif()

if(NOT DEFINED QT_DIR)
    message(FATAL_ERROR "The QT_DIR variable is not set")
endif()

cmake_path(SET PVS_QT_DIR_TO_EXCLUDE NORMALIZE "${QT_DIR}/../../../include")

if(NOT EXISTS ${PVS_QT_DIR_TO_EXCLUDE})
    message(FATAL_ERROR "The Qt includes directroy is not exists: '${PVS_QT_DIR_TO_EXCLUDE}'")
endif()

configure_file("tools/extra-exclude-paths.pvsconfig.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/.PVS-Studio/extra-exclude-paths.pvsconfig")

set(__CURRENT_FILE_VAR__ TRUE)
