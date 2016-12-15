# Copyright 2016 Freeman Zhang <zhanggyb@gmail.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set(XDG_SHELL_V6_XML ${WAYLAND_PROTOCOLS_SOURCE_DIR}/unstable/xdg-shell/xdg-shell-unstable-v6.xml)
set(XDG_SHELL_V6_CLIENT_HEADER "xdg-shell-unstable-v6-client-protocol.h")
set(XDG_SHELL_V6_SERVER_HEADER "xdg-shell-unstable-v6-server-protocol.h")
set(XDG_SHELL_V6_SOURCE "xdg-shell-unstable-v6-protocol.c")

#add_custom_target(xdg-shell-unstable-v6
#        COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_V6_XML} ${XDG_SHELL_V6_CLIENT_HEADER}
#        COMMAND ${WAYLAND_SCANNER} server-header ${XDG_SHELL_V6_XML} ${XDG_SHELL_V6_SERVER_HEADER}
#        COMMAND ${WAYLAND_SCANNER} code ${XDG_SHELL_V6_XML} ${XDG_SHELL_V6_SOURCE}
#        DEPENDS ${XDG_SHELL_V6_XML}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#        COMMENT "Generating xdg-shell unstable v6..."
#        VERBATIM)

# FIXME: I cannot make add_custom_target or add_custom_command work
execute_process(COMMAND
        ${WAYLAND_SCANNER} client-header ${XDG_SHELL_V6_XML} ${XDG_SHELL_V6_CLIENT_HEADER}
        COMMAND ${WAYLAND_SCANNER} server-header ${XDG_SHELL_V6_XML} ${XDG_SHELL_V6_SERVER_HEADER}
        COMMAND ${WAYLAND_SCANNER} code ${XDG_SHELL_V6_XML} ${XDG_SHELL_V6_SOURCE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

#add_custom_target(xdg-shell-unstable-v6 ALL DEPENDS ${XDG_SHELL_V6_XML})
#
#add_custom_command(TARGET xdg-shell-unstable-v6
#        PRE_BUILD
#        COMMAND ${WAYLAND_SCANNER} client-header ${XDG_SHELL_V6_XML} ${XDG_SHELL_V6_CLIENT_HEADER}
#        COMMAND ${WAYLAND_SCANNER} server-header ${XDG_SHELL_V6_XML} ${XDG_SHELL_V6_SERVER_HEADER}
#        COMMAND ${WAYLAND_SCANNER} code ${XDG_SHELL_V6_XML} ${XDG_SHELL_V6_SOURCE}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#        COMMENT "Generating xdg-shell unstable v6..."
#        VERBATIM)

add_library(xdg-shell-unstable-v6-client
        ${XDG_SHELL_V6_CLIENT_HEADER}
        ${XDG_SHELL_V6_SOURCE})

#add_dependencies(xdg-shell-unstable-v6-client xdg-shell-unstable-v6)
