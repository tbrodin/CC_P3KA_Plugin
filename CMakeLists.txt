cmake_minimum_required( VERSION 3.0 )

# Add an option to CMake to control whether we build this plugin or not
option( PLUGIN_LOAD_P3KA "Check to install example plugin" OFF )

if (PLUGIN_LOAD_P3KA )
	# Name the plugin
    project( LoadP3KAPlugin )
    
    # load any subdirectories (see qAdditionalIO for an example)
    # add_subdirectory( LIB1 )
    
    include( ../CloudCompare/plugins/CMakePluginTpl.cmake )
    
    # set dependencies to necessary libraries (see qPCV for an example)
    # target_link_libraries( ${PROJECT_NAME} LIB1 )
    # include_directories( ${LIB1_INCLUDE_DIR} )
endif()
