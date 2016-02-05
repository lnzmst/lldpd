#include <signal.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

//#include "lldpV2MIB.h"

static bool keep_running = true;

RETSIGTYPE shutdown_daemon( int )
{
    keep_running = false;
}

int main( int, char ** )
{
  bool is_agentx_subagent = true; // true: SNMP subagent; false: SNMP master agent
  bool run_in_background = false; // true: run in the background
  bool with_syslog = false;       // flag for using syslog

  // print log errors to syslog or stderr
  if (with_syslog)
    snmp_enable_calllog();
  else
    snmp_enable_stderrlog();

  // we're an agentx subagent?
  if (is_agentx_subagent) {
    // make us an agentx client.
    netsnmp_ds_set_boolean( NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1 );

    // AGENTX_ADDRESS is initialized to default port for AgentX.
    netsnmp_ds_set_string( NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_X_SOCKET, "localhost:1705" );

    syslog( LOG_INFO, "lldp-daemon starting as AgentX Subagent.");
  }

  /* run in background, if requested */
  if (run_in_background and netsnmp_daemonize( 1, not with_syslog ))
      exit( 1 );

  /* initialize tcpip, if necessary */
  SOCK_STARTUP;

  // Initializes the embedded agent. This function must be called before the init_snmp() call.
  // The name is used to specify what configuration file to read when init_snmp() is called later.
  init_agent( "lldpd" );

  // Initialize mib code here
  //init_lldpV2MIB();  

  // Initialize vacm/usm access control
  if (not is_agentx_subagent) {
    //init_vacm_vars();
    //init_usmUser();
  }

  // Initializes the SNMP library, which causes the agent to read the application's configuration file.
  // The configuration file can be used to configure access control, for instance. See the snmp_config(4)
  // and snmpd.conf(4) man pages for more information about configuration files.
  init_snmp( "lldpd" );

  // If we're going to be a snmp master agent, initiate the ports
  if (not is_agentx_subagent)
    init_master_agent();  // open the port to listen on (defaults to udp:161)

  // In case we recevie a request to stop (kill -TERM or kill -INT)
  signal( SIGTERM, shutdown_daemon );
  signal( SIGINT, shutdown_daemon );

  snmp_log( LOG_INFO, "lldp daemon is up and running.\n" );

  while (keep_running) { // your main loop here ...
    /* if you use select(), see snmp_select_info() in snmp_api(3) */
    /*     --- OR ---  */
    agent_check_and_process( 1 ); // 0 == don't block
  }

  snmp_log( LOG_INFO, "Shutdown lldpd\n" );

  snmp_shutdown( "lldpd" );

  SOCK_CLEANUP;

  return 0;
}
