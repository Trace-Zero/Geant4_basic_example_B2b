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
//
/// \file B2/B2b/src/EventAction.cc
/// \brief Implementation of the B2::EventAction class

#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4TrajectoryContainer.hh"
#include "globals.hh"


//safiye
#include "G4SDManager.hh"        // to get collection ID
#include "G4HCofThisEvent.hh"    // to access hits of this event
#include "TrackerHit.hh"         // B2::TrackerHit and B2::TrackerHitsCollection

namespace B2
{

//safiye
EventAction::EventAction(RunAction* runAction)
 : G4UserEventAction(),
   fRunAction(runAction),
   fTotalEdep(0.0),
   fTotalTrackLength(0.0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event) {
  // Reset value
  fTotalEdep = 0.0;
  fTotalTrackLength = 0.0;


 if (event) {
    fEventID = event->GetEventID();
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  if (!event) return;

  // get number of stored trajectories
  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  std::size_t n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // periodic printing
  G4int eventID = event->GetEventID();
  if (eventID < 100 || eventID % 100 == 0) {
    G4cout << ">>> Event: " << eventID << G4endl;
    if (trajectoryContainer) {
      G4cout << "    " << n_trajectories
             << " trajectories stored in this event" << G4endl;
    }

    // Safe access to hits collection
    G4HCofThisEvent* hce_print = event->GetHCofThisEvent();
    if (hce_print) {
      G4VHitsCollection* hc0 = hce_print->GetHC(0);
      if (hc0) {
        G4cout << "    " << hc0->GetSize()
               << " hits stored in this event" << G4endl;
      }
    }
}
  //safiye  ---- ROOT filling block ---------
  if (!fRunAction) return; // RunAction manages ROOT file

  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if (!hce) return;

  auto* sdman = G4SDManager::GetSDMpointer();
  // Use the same collection name defined in TrackerSD
  G4int hcID = sdman->GetCollectionID("TrackerHitsCollection");
  // If not found, adjust the name
  if (hcID < 0) return;

  auto* hits = static_cast<B2::TrackerHitsCollection*>( hce->GetHC(hcID) );
  if (!hits) return;

  // Loop over hits and fill ROOT tree
  for (size_t i = 0; i < hits->GetSize(); ++i) {
    auto* h = (*hits)[i];
    auto p = h->GetPos();
    fRunAction->FillHit(eventID, h->GetTrackID(),   // Track ID
                        h->GetChamberNb(), // Chamber number
                        h->GetGapNb(),     // Gap number
                        h->GetEdep(),      // Energy deposit
                        p.x(), p.y(), p.z() // Position
    );
  }
}

}  // namespace B2
