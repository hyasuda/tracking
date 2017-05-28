detectorSimulation
====

This is Geant4 simulation for muon g-2/EDM detector

# Usage
## Set enviroment for ROOT and Geant4
(at bepp)  
`source /gluster/share/share.sh`  
`source /gluster/share/geant4/geant4.9.6.p03/geant4.9.6.p03-install/bin/geant4.sh`  
`source /gluster/share/geant4/geant4.9.6.p03/geant4.9.6.p03-install/share/Geant4-9.6.3/geant4make/geant4make.sh`  

## Compile
Edit GNUMakefile if needed and then do  
`make`  

## Run 
To run execution file, do the following command for example  
`muong2edmsim macro/tmp_track_0.mac`  
Output file name will need to be editted in include/ApplicationManager.hh  
`  sprintf(filename,"/gluster/data/g2/tyosioka/data/mug2edm.%s.%s.root",buff,tok);`  

