detectorSimulation
====

This is Geant4 simulation for muon g-2/EDM detector

# Usage
## Set enviroment for ROOT and Geant4
(at bepp)  
`source /gluster/share/share_root6_ssd01.sh`  

## Make build directory
`mkdir ../build`  
`cd ../build`  

## Compile
Edit CMakeLists.txt if needed and then do  
`cmake -DGeant4_DIR=/disk/ssd01/share_ssd01/geant4/geant4.10.03.p01/geant4.10.03.p01-install/lib64/Geant4-10.3.1 ../detectorSimulation`  
`make`  

## Run 
To run execution file, do the following command for example  
`./muong2edmsim macro/tmp_track_0.mac`  
Output file name will need to be editted in include/ApplicationManager.hh  
`  sprintf(filename,"/gluster/data/g2/tyosioka/data/mug2edm.%s.%s.root",buff,tok);`  

## Visualization
To enable OpenGL visualization, set  
`export G4VIS_USE=1`  
and do `make`
You can use /vis/viwer command in interactive mode of _muong2edmsim_  