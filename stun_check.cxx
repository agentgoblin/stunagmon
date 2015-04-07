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
    if ( verbose )
    {
        cout << "stunServerAddr  " << stunServerAddr << endl \
            << "stunServerAddr.addr  " << stunServerAddr.addr << endl \
            << "stunServerAddr.port  " << stunServerAddr.port << endl \
            << "verbose  " << verbose << endl \
            << "srcPort  " << srcPort << endl;
    }

    /* Test server */
    int fd1,fd2;
    StunAddress4 mappedAddr;
    StunAddress4 srcAddr;
    srcAddr.addr = 0;

    bool ok = stunOpenSocketPair(stunServerAddr,
                                    &mappedAddr,
                                    &fd1,
                                    &fd2,
                                    srcPort,
                                    &srcAddr,
                                    verbose);
    if ( ok )
    {
        closesocket(fd1);
        closesocket(fd2);
        cout << "Got port pair at " << mappedAddr.port << endl;
        exit(prcOK);
    }
    else
    {
        /* cout because Nagios cannot into cerr*/
        cout << "Opened a stun socket pair FAILED" << endl;
        exit(prcCritical);
    }

    exit(prcOK);
}
