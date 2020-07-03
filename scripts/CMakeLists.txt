list(
    APPEND PROJECT_SCRIPTS
    cxxplug_gen.cmake
)

foreach(project_script ${PROJECT_SCRIPTS})
    file(
        COPY ${project_script}
        DESTINATION "${MY_TARGET_OUTPUT_DIRECTORY}/scripts/"
    )
endforeach()
