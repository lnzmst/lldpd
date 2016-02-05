file( GLOB SRCS *.cc )

add_executable( lldpd ${SRCS} )
target_compile_options( lldpd PRIVATE ${NET_SNMP_CFLAGS} )
target_link_libraries( lldpd ${NET_SNMP_LDFLAGS} )

install ( TARGETS lldpd DESTINATION ${CMAKE_SOURCE_DIR}/usr/bin )
install ( FILES lldpd.conf DESTINATION ${CMAKE_SOURCE_DIR}/usr/etc/snmp )
