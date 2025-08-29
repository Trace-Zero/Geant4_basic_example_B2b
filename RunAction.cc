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
/// \file B2/B2b/src/RunAction.cc
/// \brief Implementation of the B2::RunAction class

#include "RunAction.hh"

#include "G4RunManager.hh"
//safiye
#include "TFile.h"
#include "TTree.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TH2I.h"
#include "TObject.h"
namespace B2
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(100);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  //safiye  ROOT File + Tree
  fFile = TFile::Open("b2b_hits.root","RECREATE");
  fTree = new TTree("hits","B2b tracker hits");

  fTree->Branch("event",   &b_event,   "event/I");
  fTree->Branch("trackID", &b_trackID, "trackID/I");
  fTree->Branch("chamber", &b_chamber, "chamber/I");
  fTree->Branch("gap",     &b_gap,     "gap/I");
  fTree->Branch("edep",    &b_edep,    "edep/D");
  fTree->Branch("x",       &b_x,       "x/D");
  fTree->Branch("y",       &b_y,       "y/D");
  fTree->Branch("z",       &b_z,       "z/D");
  // Creating ntuple
  //
  analysisManager->CreateNtuple("B2", "Edep and TrackL");
  analysisManager->CreateNtupleDColumn("Eabs");
  analysisManager->CreateNtupleDColumn("Egap");
  analysisManager->CreateNtupleDColumn("Labs");
  analysisManager->CreateNtupleDColumn("Lgap");
  analysisManager->FinishNtuple();
}



// ---- label branches with human-friendly titles (units) ----
auto setTitle = [this](const char* br, const char* title){
  if (auto* b = fTree->GetBranch(br)) b->SetTitle(title);
};
setTitle("event",   "Event ID");
setTitle("trackID", "Track ID");
setTitle("chamber", "Chamber index");
setTitle("gap",     "Gap index");
setTitle("edep",    "Deposited energy [MeV]");
setTitle("x",       "X position [mm]");
setTitle("y",       "Y position [mm]");
setTitle("z",       "Z position [mm]");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*) {
//safiye
  if (fFile) {
    fFile->cd();
    if (fTree) fTree->Write();
    fFile->Close();
    delete fFile;
  }
  fFile = nullptr;
  fTree = nullptr;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//safiye
void RunAction::FillHit(int ev, int trk, int chamb, int gap,
                        double edep, double x, double y, double z) {
  if (!fTree) return;
  b_event   = ev;
  b_trackID = trk;
  b_chamber = chamb;
  b_gap     = gap;
  b_edep    = edep;
  b_x = x; b_y = y; b_z = z;
  fTree->Fill();
}

}  // namespace B2
