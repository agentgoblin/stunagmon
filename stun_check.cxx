#include <cassert>
#include <cstring>
#include <iostream>
#include <cstdlib>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <errno.h>


#include "udp.h"
#include "stun.h"
#include "stun_check.h"

using namespace std;

void usage()
{
    cerr << "Usage: check_stun [-v] [-H server[:port]] [-p sourcePort]" << endl;
}

int main(int argc, char* argv[])
{
    assert( sizeof(UInt8 ) == 1 );
    assert( sizeof(UInt16) == 2 );
    assert( sizeof(UInt32) == 4 );

    // Set initial server address to 0.0.0.0:3478
    const int stunDefaultPort = 3478;
    StunAddress4 stunServerAddr;
    stunServerAddr.addr = 0;
    stunServerAddr.port = stunDefaultPort;

    // Source port
    int srcPort=0;

    // Verbose mode
    bool verbose = false;


    // VVVVV DELETE THIS

    // Network interfaces
    // but resolve to 127.0.0.1:65535
    /*
    const int MAX_NIC = 3;
    StunAddress4 sAddr[MAX_NIC];
    int retval[MAX_NIC];
    int numNic=0;

    // Network interfaces addresses
    // fill zeros ips and ports
    for ( int i=0; i<MAX_NIC; i++ )
    {
        sAddr[i].addr=0;
        sAddr[i].port=0;
        retval[i]=0;
    }
    * */
    // ^^^^^ DELETE THIS

    /* Check for arguments */
    if ( argc == 1 )
    {
        usage();
        exit(prcUnknown);
    }

    /* Parse arguments */
    for ( int arg = 1; arg<argc; arg++ )
    {
        // verbose mode
        if ( !strcmp ( argv[arg], "-v" ) )
        {
            verbose = true;
        }
        // server name
        else if ( !strcmp( argv[arg], "-H") )
        {
            arg++;
            if ( argc <= arg)
            {
                usage();
                exit(prcUnknown);
            }
            bool ret = stunParseServerName( argv[arg], stunServerAddr);
            if ( ret != true )
            {
                usage();
                exit(prcUnknown);
            }
        }
        else if ( !strcmp( argv[arg] , "-p" ) )
        {
            arg++;
            if ( argc <= arg )
            {
                usage();
                exit(prcUnknown);
            }
            srcPort = strtol( argv[arg], NULL, 10);
        }
        else
        {
            usage();
            exit(prcUnknown);
        }
    }

    /*debug*/
    cout << "stunServerAddr  " << stunServerAddr << endl \
        << "stunServerAddr.addr  " << stunServerAddr.addr << endl \
        << "stunServerAddr.port  " << stunServerAddr.port << endl \
        << "verbose  " << verbose << endl \
        << "srcPort  " << srcPort << endl;



    exit(prcOK);
}
