include("Utils")
include("ExternalsUtils")

# REQUIRES SDL3
block()
    set(LIB_NAME "physfs")
    announce("Configuring \"${LIB_NAME}\" external library.")

    force_bool(PHYSFS_ARCHIVE_ZIP ON)         # Enable ZIP archive support
    force_bool(PHYSFS_ARCHIVE_7Z ON)          # Enable 7zip archive support
    force_bool(PHYSFS_ARCHIVE_GRP ON)         # Enable Build Engine GRP archive support
    force_bool(PHYSFS_ARCHIVE_WAD ON)         # Enable Doom WAD archive support
    force_bool(PHYSFS_ARCHIVE_CSM ON)         # Enable Chasm: The Rift CSM.BIN archive support
    force_bool(PHYSFS_ARCHIVE_HOG ON)         # Enable Descent I/II HOG archive support
    force_bool(PHYSFS_ARCHIVE_MVL ON)         # Enable Descent I/II MVL archive support
    force_bool(PHYSFS_ARCHIVE_QPAK ON)        # Enable Quake I/II QPAK archive support
    force_bool(PHYSFS_ARCHIVE_SLB ON)         # Enable I-War / Independence War SLB archive support
    force_bool(PHYSFS_ARCHIVE_ISO9660 ON)     # Enable ISO9660 archive support
    force_bool(PHYSFS_ARCHIVE_VDF ON)         # Enable Gothic I/II VDF archive support
    force_bool(PHYSFS_ARCHIVE_LECARCHIVES ON) # Enable LucasArts GOB/LAB/LFD archive support

    force_bool(PHYSFS_BUILD_STATIC ON)        # Build the static PhysFS library
    force_bool(PHYSFS_BUILD_SHARED OFF)        # Build the shared PhysFS library
    force_bool(PHYSFS_BUILD_TEST OFF)          # Build the stdio test program
    force_bool(PHYSFS_DISABLE_INSTALL ON)      # Disable installing PhysFS
    force_bool(PHYSFS_BUILD_DOCS OFF)          # Build doxygen-based documentation


    add_external_subdirectory(${LIB_NAME})
endblock()
