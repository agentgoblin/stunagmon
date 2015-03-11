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

using namespace std;

#define MAX_NIC 3

int main(int argc, char* argv[])
{
   assert( sizeof(UInt8 ) == 1 );
   assert( sizeof(UInt16) == 2 );
   assert( sizeof(UInt32) == 4 );


   // Set initial server address to 0.0.0.0
   StunAddress4 stunServerAddr;
   stunServerAddr.addr=0;

   // UNKNOWN
   int srcPort=0;
   StunAddress4 sAddr[MAX_NIC];
   int retval[MAX_NIC];
   int numNic=0;

   // UNKNOWN
   for ( int i=0; i<MAX_NIC; i++ )
   {
      sAddr[i].addr=0;
      sAddr[i].port=0;
      retval[i]=0;
   }

   for ( int arg = 1; arg<argc; arg++ )
   {
      // Get interfaces for test PIECE OF SHIT
      if ( !strcmp( argv[arg] , "-i" ) )
      {
         arg++;
         if ( argc <= arg )
         {
            usage();
            exit(-1);
         }
         if ( numNic >= MAX_NIC )
         {
            cerr << "Can not have more than "<<  MAX_NIC <<" -i options" << endl;
            usage();
            exit(-1);
         }

         stunParseServerName(argv[arg], sAddr[numNic++]);
      }
      else if ( !strcmp( argv[arg] , "-p" ) )
      {
         arg++;
         if ( argc <= arg )
         {
            usage();
            exit(-1);
         }
         srcPort = strtol( argv[arg], NULL, 10);
      }
      else
      {
        char* ptr;
        int t =  strtol( argv[arg], &ptr, 10 );
        if ( *ptr == 0 )
        {
           // conversion worked
           testNum = t;
           cout << "running test number " << testNum  << endl;
        }
        else
        {
           bool ret = stunParseServerName( argv[arg], stunServerAddr);
           if ( ret != true )
           {
              cerr << argv[arg] << " is not a valid host name " << endl;
              usage();
              exit(-1);
           }
        }
      }
   }


}
