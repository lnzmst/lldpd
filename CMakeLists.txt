cmake_minimum_required( VERSION 3.0 )

project( lldpd )


find_program( NET_SNMP_CONFIG NAMES "net-snmp-config" ENV PATH )

if(NOT NET_SNMP_CONFIG)
  message( FATAL_ERROR "\n[1;31mnet-snmp-config not found ![0m" )
endif(NOT NET_SNMP_CONFIG)
message( STATUS "NET_SNMP_CONFIG=${NET_SNMP_CONFIG}" )

execute_process( COMMAND ${NET_SNMP_CONFIG} --base-cflags OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NET_SNMP_CFLAGS )
separate_arguments( NET_SNMP_CFLAGS UNIX_COMMAND "${NET_SNMP_CFLAGS}" ) # to avoid surrounding quotes
message( STATUS "NET_SNMP_CFLAGS=${NET_SNMP_CFLAGS}" )

execute_process( COMMAND ${NET_SNMP_CONFIG} --agent-libs OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NET_SNMP_LDFLAGS )
message( STATUS "NET_SNMP_LDFLAGS=${NET_SNMP_LDFLAGS}" )


add_subdirectory( src )
