# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindCURL
# --------
#
# Find curl
#
# Find the native CURL headers and libraries.
#
# ::
#
#   CURL_INCLUDE_DIRS   - where to find curl/curl.h, etc.
#   CURL_LIBRARIES      - List of libraries when using curl.
#   CURL_FOUND          - True if curl found.
#   CURL_VERSION_STRING - the version of curl found (since CMake 2.8.8)

# Look for the header file.
find_path(CURL_INCLUDE_DIR NAMES curl/curl.h HINTS ${PROJECT_3RD_LOC}/include)
mark_as_advanced(CURL_INCLUDE_DIR) 

# Look for the library (sorted from most current/relevant entry to least).
find_library(CURL_LIBRARY_DIR NAMES
    curl
  # Windows MSVC prebuilts:
    curllib
    libcurl_imp
    curllib_static
  # Windows older "Win32 - MSVC" prebuilts (libcurl.lib, e.g. libcurl-7.15.5-win32-msvc.zip):
    libcurl
    HINTS ${PROJECT_3RD_LOC}/lib
)
mark_as_advanced(CURL_LIBRARY_DIR)

if(CURL_INCLUDE_DIR)
  foreach(_curl_version_header curlver.h curl.h)
    if(EXISTS "${CURL_INCLUDE_DIR}/curl/${_curl_version_header}")
      file(STRINGS "${CURL_INCLUDE_DIR}/curl/${_curl_version_header}" curl_version_str REGEX "^#define[\t ]+LIBCURL_VERSION[\t ]+\".*\"")

      string(REGEX REPLACE "^#define[\t ]+LIBCURL_VERSION[\t ]+\"([^\"]*)\".*" "\\1" CURL_VERSION_STRING "${curl_version_str}")
      unset(curl_version_str)
      break()
    endif()
  endforeach()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CURL
                                  REQUIRED_VARS CURL_LIBRARY_DIR CURL_INCLUDE_DIR
                                  VERSION_VAR CURL_VERSION_STRING)

if(CURL_FOUND)
  message(STATUS "Found CURL include dir - ${Green}${CURL_INCLUDE_DIR}${ColourReset}")
  message(STATUS "Found CURL library dir - ${Green}${CURL_LIBRARY_DIR}${ColourReset}")
else()
  message(WARNING ${Red}"CURL not found"${ColourReset})
endif()
