detectorSimulation
====

This is Geant4 simulation for muon g-2/EDM detector

# detectorSimulation usage
## Set enviroment for ROOT and Geant4
(at bepp)  
`source /gluster/share/share_root6_ssd01.sh`  
or set paths to the following software  
* gcc(6.3.0)
* python(2.7.13)
* cmake(3.8.2)
* ROOT(6.08/06)
* Geant(4.10.03.p01)

## Make build directory
`mkdir ../build`  
`cd ../build`  

## Compile
Edit CMakeLists.txt if needed and then do  
(at bepp)  
`cmake -DGeant4_DIR=/disk/ssd01/share_ssd01/geant4/geant4.10.03.p01/geant4.10.03.p01-install/lib64/Geant4-10.3.1 ../detectorSimulation`  
`make`  

## Run 
To run execution file, do the following command for example  
`./muong2edmsim macro/tmp_track_0.mac`  
Output file name can be editted in include/ApplicationManager.hh  
`  sprintf(filename,"data/mug2edm_%s_%s.root",buff,tok);`  

## Visualization
To enable OpenGL visualization, set  
`export G4VIS_USE=1`  
and do `make`  
You can use /vis/viwer command in interactive mode by running  
`./muong2edmsim`  
