# cmake/file_list_remove.cmake

# A simple function to remove files from a given file list
#
# Params:
#   - list_name: the file list
#   - option: the options passed to cmake 'file' command, must be GLOB or GLOB_RECURSE
#   - ARGN: the files need to be removed
function(file_list_remove list_name option)
    file(${option} files_to_remove ${ARGN})
    list(REMOVE_ITEM ${list_name} ${files_to_remove})
    set(${list_name} ${${list_name}} PARENT_SCOPE)
endfunction()
