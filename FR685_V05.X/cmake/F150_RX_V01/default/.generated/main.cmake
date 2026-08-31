include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(F150_RX_V01_default_library_list )

# Handle files with suffix (s|as|asm|AS|ASM|As|aS|Asm), for group default-XC8
if(F150_RX_V01_default_default_XC8_FILE_TYPE_assemble)
add_library(F150_RX_V01_default_default_XC8_assemble OBJECT ${F150_RX_V01_default_default_XC8_FILE_TYPE_assemble})
    F150_RX_V01_default_default_XC8_assemble_rule(F150_RX_V01_default_default_XC8_assemble)
    list(APPEND F150_RX_V01_default_library_list "$<TARGET_OBJECTS:F150_RX_V01_default_default_XC8_assemble>")

endif()

# Handle files with suffix S, for group default-XC8
if(F150_RX_V01_default_default_XC8_FILE_TYPE_assemblePreprocess)
add_library(F150_RX_V01_default_default_XC8_assemblePreprocess OBJECT ${F150_RX_V01_default_default_XC8_FILE_TYPE_assemblePreprocess})
    F150_RX_V01_default_default_XC8_assemblePreprocess_rule(F150_RX_V01_default_default_XC8_assemblePreprocess)
    list(APPEND F150_RX_V01_default_library_list "$<TARGET_OBJECTS:F150_RX_V01_default_default_XC8_assemblePreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC8
if(F150_RX_V01_default_default_XC8_FILE_TYPE_compile)
add_library(F150_RX_V01_default_default_XC8_compile OBJECT ${F150_RX_V01_default_default_XC8_FILE_TYPE_compile})
    F150_RX_V01_default_default_XC8_compile_rule(F150_RX_V01_default_default_XC8_compile)
    list(APPEND F150_RX_V01_default_library_list "$<TARGET_OBJECTS:F150_RX_V01_default_default_XC8_compile>")

endif()

# Handle files with suffix elf, for group default-XC8
if(F150_RX_V01_default_default_XC8_FILE_TYPE_objcopy_lss)
add_library(F150_RX_V01_default_default_XC8_objcopy_lss OBJECT ${F150_RX_V01_default_default_XC8_FILE_TYPE_objcopy_lss})
    F150_RX_V01_default_default_XC8_objcopy_lss_rule(F150_RX_V01_default_default_XC8_objcopy_lss)
    list(APPEND F150_RX_V01_default_library_list "$<TARGET_OBJECTS:F150_RX_V01_default_default_XC8_objcopy_lss>")

endif()


# Main target for this project
add_executable(F150_RX_V01_default_image_qauqaWjU ${F150_RX_V01_default_library_list})

set_target_properties(F150_RX_V01_default_image_qauqaWjU PROPERTIES
    OUTPUT_NAME "default"
    SUFFIX ".elf"
    ADDITIONAL_CLEAN_FILES "${output_extensions}"
    RUNTIME_OUTPUT_DIRECTORY "${F150_RX_V01_default_output_dir}")
target_link_libraries(F150_RX_V01_default_image_qauqaWjU PRIVATE ${F150_RX_V01_default_default_XC8_FILE_TYPE_link})
# Add the link options from the rule file.
F150_RX_V01_default_link_rule( F150_RX_V01_default_image_qauqaWjU)


#Add objcopy steps
F150_RX_V01_default_objcopy_lss_rule(F150_RX_V01_default_image_qauqaWjU)

