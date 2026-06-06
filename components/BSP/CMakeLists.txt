set(src_dirs
    camera)

set(include_dirs
    camera)

set(requires
    driver
	esp32-camera
    esp_timer)

idf_component_register(SRC_DIRS ${src_dirs} INCLUDE_DIRS ${include_dirs} REQUIRES ${requires})
component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)