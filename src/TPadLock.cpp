/*
 ============================================================================
 Name		: TPadLock.cpp
 Author	  : 
 Copyright   : Your copyright notice
 Description : Exe source file
 ============================================================================
 */

//  Include Files  

#include "TPadLock.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include <aknkeylock.h>
//  Constants

//  Global Variables

_LIT(KTextPanicMessage, "asynchronous error");
//  Local Functions

LOCAL_C void MainL()
	{
	//
	// add your program code here, example code below
	//
	RTimer timer;
	timer.CreateLocal();
	TRequestStatus timerStatus;
	RAknKeyLock keylock;
	while(true)
		{
		timer.Inactivity(timerStatus,120);
		User::WaitForRequest(timerStatus);
		if(KErrNone==keylock.Connect())
			{
			if(!keylock.IsKeyLockEnabled())
				keylock.EnableKeyLock();
			}
		keylock.Close();
		}
	}

LOCAL_C void DoStartL()
	{
	// Create active scheduler (to run active objects)
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	MainL();

	// Delete active scheduler
	CleanupStack::PopAndDestroy(scheduler);
	}

//  Global Functions

GLDEF_C TInt E32Main()
	{
	// Create cleanup stack
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();

	// Run application code inside TRAP harness, wait keypress when terminated
	TRAPD(mainError, DoStartL());
	if (mainError)
		User::Panic(KTextPanicMessage,mainError);

	delete cleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}

