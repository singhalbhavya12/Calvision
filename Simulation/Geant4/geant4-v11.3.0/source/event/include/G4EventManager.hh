//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// G4EventManager
//
// Class description:
//
// G4EventManager controls an event. This class must be a singleton
// and should be constructed by G4RunManager.

// Author: M.Asai, SLAC
// --------------------------------------------------------------------
#ifndef G4EventManager_hh
#define G4EventManager_hh 1

#include "G4StackManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4PrimaryTransformer.hh"
#include "G4Event.hh"
class G4UserEventAction;
class G4UserStackingAction;
class G4UserTrackingAction;
class G4UserSteppingAction;
class G4EvManMessenger;
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4VTrajectory.hh"
#include "G4TrackStatus.hh"
class G4SDManager;
class G4StateManager;
#include "globals.hh"
class G4VUserEventInformation;

class G4EventManager 
{
 public:
    static G4EventManager* GetEventManager();
      // This method returns the singleton pointer of G4EventManager.

    G4EventManager();
   ~G4EventManager();

    G4EventManager(const G4EventManager &right) = delete;
    G4EventManager& operator=(const G4EventManager& right) = delete;

    void ProcessOneEvent(G4Event* anEvent);
      // This method is the main entry to this class for simulating an event.

    void ProcessOneEvent(G4TrackVector* trackVector, G4Event* anEvent= nullptr);
      // This is an alternative entry for HEP experiments which create G4Track
      // objects by themselves directly without using G4VPrimaryGenerator or a
      // user-primary-generator action. Dummy G4Event object will be created if
      // "anEvent" is null for internal use, but this dummy object will be
      // deleted at the end of this method and will never be available for use
      // after the processing.
      // Note that in this case of null G4Event pointer, no output of the
      // simulated event is returned by this method; the user must implement
      // some mechanism of storing the output, e.g. in the UserEventAction
      // and/or in sensitive detectors.
      // If a valid G4Event object is given, this object will not be deleted
      // by this method, and output objects such as hits collections and
      // trajectories will be associated to the event object. If the event
      // object has valid primary vertices/particles, they will be added to
      // the given "trackvector" input.

    void StackTracks(G4TrackVector* trackVector, G4bool IDhasAlreadySet= false);
      // Helper function to stack a vector of tracks for processing in the
      // current event.

    inline const G4Event* GetConstCurrentEvent()
      { return currentEvent; }
    inline G4Event* GetNonconstCurrentEvent()
      { return currentEvent; }
      // These methods returns the pointers of const G4Event*
      // and G4Event*, respectively. Null will be returned when
      // an event is not processing.

    void AbortCurrentEvent();
      // This method aborts the processing of the current event. All stacked
      // tracks are deleted. The contents of G4Event object is not completed,
      // but trajectories, hits, and/or digits which are created before the
      // moment of abortion can be used.

    void SetUserAction(G4UserEventAction* userAction);
    void SetUserAction(G4UserStackingAction* userAction);
    void SetUserAction(G4UserTrackingAction* userAction);
    void SetUserAction(G4UserSteppingAction* userAction);
    inline G4UserEventAction* GetUserEventAction()
      { return userEventAction; }
    inline G4UserStackingAction* GetUserStackingAction()
      { return userStackingAction; }
    inline G4UserTrackingAction* GetUserTrackingAction()
      { return userTrackingAction; }
    inline G4UserSteppingAction* GetUserSteppingAction()
      { return userSteppingAction; }
      // Set and get methods for user action classes. User action classes
      // which should belong to the other managers will be sent to the 
      // corresponding managers.

    void KeepTheCurrentEvent();
      // If the current event exists, it is kept undeleted until
      // the end of the current run

    inline G4StackManager* GetStackManager() const
      { return trackContainer; }
    inline G4TrackingManager* GetTrackingManager() const
      { return trackManager; }

    inline G4int GetVerboseLevel()
      { return verboseLevel; }
    inline void SetVerboseLevel( G4int value )
      {
        verboseLevel = value;
        trackContainer->SetVerboseLevel( value );
        transformer->SetVerboseLevel( value );
      }
      // Set and get method of the verbose level

    void SetUserInformation(G4VUserEventInformation* anInfo);
    G4VUserEventInformation* GetUserInformation();
      // Set and get method of G4VUserEventInformation object associating with
      // the current event. Both methods are valid only for G4State_EventProc
      // application state.

    inline G4PrimaryTransformer* GetPrimaryTransformer() const
      { return transformer; }
    inline void SetPrimaryTransformer(G4PrimaryTransformer* tf)
      { transformer = tf; }
    inline void StoreRandomNumberStatusToG4Event(G4int vl)
      { storetRandomNumberStatusToG4Event = vl; }

    inline void UseSubEventParallelism(G4bool worker = false)
      {
        subEventPara = true; 
        subEventParaWorker = worker;
      }

    G4SubEvent* PopSubEvent(G4int ty);
      // If this method is invoked by the G4RunManager while an event is still 
      // in process. Null is returned if the sub-event does not have enough tracks.
      // This method is Mutex-protected.

    void TerminateSubEvent(const G4SubEvent* se,const G4Event* evt);
      // G4Event "evt" contains the results made by the worker thread.
      // The ownership of "evt" remains to the worker thread and it will be
      // deleted after this method. All necessary information in "evt" must
      // be copied into the corresponding master G4Event object.

    G4int StoreSubEvent(G4Event*, G4int&, G4SubEvent*);
      // This method is exclusively used by G4SubEventTrackStack class to 
      // store a new G4SubEevnt into the current G4Event, with Mutex lock
      // shared with above two methods.

  private:

    void DoProcessing(G4Event* anEvent,
           G4TrackVector* trackVector = nullptr, G4bool IDhasAlreadySet= false);
  
  private:

    static G4ThreadLocal G4EventManager* fpEventManager;

    G4Event* currentEvent = nullptr;

    G4StackManager* trackContainer = nullptr;
    G4TrackingManager* trackManager = nullptr;
    G4TrajectoryContainer* trajectoryContainer = nullptr;
    G4int trackIDCounter = 0;
    G4int verboseLevel = 0;
    G4SDManager* sdManager = nullptr;
    G4PrimaryTransformer* transformer = nullptr;
    G4bool tracking = false;
    G4bool abortRequested = false;
    G4bool subEventPara = false;
    G4bool subEventParaWorker = false;
    G4int evID_inSubEv = -1;

    G4EvManMessenger* theMessenger = nullptr;

    G4UserEventAction*    userEventAction = nullptr;
    G4UserStackingAction* userStackingAction = nullptr;
    G4UserTrackingAction* userTrackingAction = nullptr;
    G4UserSteppingAction* userSteppingAction = nullptr;

    G4int storetRandomNumberStatusToG4Event = 0;
    G4String randomNumberStatusToG4Event;

    G4StateManager* stateManager = nullptr;
};

#endif
