
#include "gen.h"
#define ERROR_MSG(a) ERRerror("ConnectRefToArrayMember", 1, ERR_ORIG, a)


int ConnectRefToArray_ConnectRefToArrayMods_ConnectRefToArrayCore::connect( OMevent_mask event_mask, 
					     int seq_num)
{
   // ArrayOfIDs (group_array read req notify)
   // SelectedIndex (OMXint read write req notify)
   // RefToConnect (group read write)

   int      ReturnValue;
   int      ResetConnection;
   int      ArraySize;
   int      ArrayMember;

   OMobj_id RefToConnectID;
   OMobj_id ConnectionID;
   OMobj_id TargetID;

   /***********************/
   /* Function's Body     */
   /***********************/

   //
   // This module takes an array of group references (ArrayOfIDs), an index
   // (SelectedIndex) and a group reference (RefToConnect).
   //
   // There are two reasons why it will be called:
   //
   //   ArrayOfIDs has changed size.  If this is the case, make sure
   //   it didn't change size because the member RefToConnect is currently
   //   connected to was deleted.  If it did, connect RefToConnect to
   //   ArrayOfIDs[0].  Member 0 is considered the default connection.
   //
   //   SelectedIndex changed.  If this is the case, just connect RefToConnect
   //   to ArrayOfIDs[SelectedIndex].

      // RefToConnect is a reference to the object that we want to connect,
      // so first get the actual object, then get the object it is connected
      // to.

   ResetConnection = 0;

   ReturnValue = OMget_obj_ref( RefToConnect.obj_id(),
                                &RefToConnectID, 0 );
   if( ReturnValue != OM_STAT_SUCCESS )
   {
      ERROR_MSG( "Can't get value of RefToConnect" );
      return OM_STAT_FAILURE;
   }

   ReturnValue = OMget_obj_ref( RefToConnectID, &ConnectionID, 0 );
   if( ReturnValue == OM_STAT_ERROR )
   {
      ERROR_MSG( "Can't get value of value of RefToConnect" );
      return OM_STAT_FAILURE;
   }
   else if( ReturnValue == OM_STAT_UNDEF )
   {
      memcpy( &ConnectionID, &OMnull_obj, sizeof(OMobj_id) );
   }

   if( ArrayOfIDs.changed(seq_num) )
   {
      if( OMis_null_obj(ConnectionID) )
      {
	 SelectedIndex = 0;
	 ResetConnection = 0;
      }
      else
      {
	 // array size has changed, but connection is still valid, so
         // figure out which array member represents the connection and
         // reset SelectedIndex to that index (and don't mess with the
         // connection).
	 ArraySize = ArrayOfIDs.ret_array_size();
	 for( ArrayMember = 0; ArrayMember < ArraySize; ArrayMember++ )
	 {
            ReturnValue = OMget_obj_ref( ArrayOfIDs[ArrayMember].obj_id(),
                                         &TargetID, 0 );
            if( ReturnValue == OM_STAT_ERROR )
	    {
               ERROR_MSG( "Can't get value of ArrayOfIDs[index]" );
               return OM_STAT_FAILURE;
	    }

	    if( OMequal_objs(ConnectionID, TargetID) )
	    {
	       SelectedIndex = ArrayMember;
	       ResetConnection = 0;
	       break;
	    }
	 }
      }
   }

   if( SelectedIndex.changed(seq_num) )
   {
      ResetConnection = 1;
   }

   if( ResetConnection )
   {
      if( (int)SelectedIndex >= 0 )
      {
         ReturnValue = OMget_obj_ref( ArrayOfIDs[(int)SelectedIndex].obj_id(),
				      &TargetID, 0 );
	 if( ReturnValue == OM_STAT_ERROR )
         {
	    ERROR_MSG( "Can't get value of ArrayOfIDs[index]" );
	    return OM_STAT_FAILURE;
	 }

	 ReturnValue = OMset_obj_ref( RefToConnectID, TargetID, 0 );
	 if( ReturnValue != OM_STAT_SUCCESS )
	 {
	    ERROR_MSG( "Can't connect RefToConnect to ArrayOfIDs[index]" );
	    return OM_STAT_FAILURE;
	 }
      }
      else
      {
         if( OMis_null_obj(ConnectionID) != 1 )
	 {
	    ReturnValue = OMdel_obj_ref( RefToConnectID, ConnectionID, 0 );
	    if( ReturnValue != OM_STAT_SUCCESS )
	    {
	       ERROR_MSG( "Can't delete connection RefToConnect to "
		          "ArrayOfIDs[index]" );
	       return OM_STAT_FAILURE;
	    }
	 }
      }
   }

   return OM_STAT_SUCCESS;
}
