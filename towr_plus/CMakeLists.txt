cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(towr_plus VERSION 1.3.2)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(ifopt 2.0.1 REQUIRED)


###########
## Build ##
###########
IF(NOT CMAKE_BUILD_TYPE MATCHES Release)
  message(STATUS "CMAKE_BUILD_TYPE not set to Release -> impacts performance")
endif()

include_directories("${PROJECT_SOURCE_DIR}")
add_subdirectory(myYaml)

# so dynamic library libtowr.so retains link to ifopt_core.so
set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

# The motion-planning problem formulated through variables and constraints
add_library(${PROJECT_NAME} SHARED
  # sample formulation usage
  src/nlp_formulation.cc
  src/parameters.cc
  # variables
  src/nodes_variables.cc
  src/nodes_variables_all.cc
  src/nodes_variables_phase_based.cc
  src/phase_durations.cc
  # models
  src/robot_model.cc
  src/dynamic_model.cc
  src/single_rigid_body_dynamics.cc
  # constraints
  src/time_discretization_constraint.cc
  src/base_motion_constraint.cc
  src/terrain_constraint.cc
  src/swing_constraint.cc
  src/force_constraint.cc
  src/total_duration_constraint.cc
  src/dynamic_constraint.cc
  src/range_of_motion_constraint.cc
  src/spline_acc_constraint.cc
  src/linear_constraint.cc
  # costs
  src/node_cost.cc
  src/soft_constraint.cc
  # initialization
  src/gait_generator.cc
  src/monoped_gait_generator.cc
  src/biped_gait_generator.cc
  src/quadruped_gait_generator.cc
  # terrain
  src/height_map_examples.cc
  src/height_map.cc
  # helpers
  src/state.cc
  src/polynomial.cc
  src/spline.cc
  src/phase_spline.cc
  src/node_spline.cc
  src/nodes_observer.cc
  src/spline_holder.cc
  src/euler_converter.cc
  src/phase_durations_observer.cc
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    ifopt::ifopt_core
    my_yaml
)
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


add_executable(${PROJECT_NAME}-example
  test/hopper_example.cc
)
target_link_libraries(${PROJECT_NAME}-example
  PRIVATE
    ${PROJECT_NAME}
    ifopt::ifopt_ipopt
)

add_executable(atlas-example test/atlas_example)
target_link_libraries(atlas-example PRIVATE ${PROJECT_NAME}
    ifopt::ifopt_ipopt)

############
# Install ##
############
#include(GNUInstallDirs) # for correct libraries locations across platforms
#set(config_package_location "share/${PROJECT_NAME}/cmake") # for .cmake find-scripts installs
#install(
  #TARGETS ${PROJECT_NAME} ${PROJECT_NAME}-example
  #EXPORT ${PROJECT_NAME}-targets
  #ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  #LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  #RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}/towr
#)
#install(
  #DIRECTORY include/towr/
  #DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/towr
#)
#install(
  #EXPORT ${PROJECT_NAME}-targets
  #FILE ${PROJECT_NAME}-targets.cmake
  #namespace towr_plus::
  #DESTINATION ${config_package_location}
#)
#set(version_file "${CMAKE_CURRENT_BINARY_DIR}/towr-config-version.cmake")
#include(CMakePackageConfigHelpers)
#write_basic_package_version_file(
  #${version_file}
  #VERSION ${towr_VERSION}
  #COMPATIBILITY SameMajorVersion
#)
#install(
  #FILES
    #"${CMAKE_CURRENT_SOURCE_DIR}/cmake/towr-config.cmake"   # self-written
    #${version_file}                                         # generated
  #DESTINATION ${config_package_location}
#)
#install(FILES package.xml DESTINATION share/${PROJECT_NAME})
