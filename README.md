detectorSimulation
====

This is Geant4 simulation for muon g-2/EDM detector

# git usage
## git version
If your git version is older than 1.7.10, it is recommended to use a newer version.  
At bepp, you can use a new version of git (2.13.2) by  
`export PATH=/gluster/share/git/git-2.13.2/bin:$PATH`  

## Clone git repository to local
To start git work, the first thing to do is to clone git repository to your local directory.  
`git clone https://gitlab.in2p3.fr/yamanaka/detectorSimulation.git`  
Then, *detectorSimulation* will be created in the current directory.  
`cd detectorSimulation`  

## Edit a file and commit
You can edit existing files or add new files.  
`emacs README.md`  
`emacs NewFile.txt`  
and write something.

To commit these changes to the repository, you need to add them first  
`git add README.md`  
`git add NewFile.txt` 

Then, commit with some comments  
`git commit -m "edit README and add a new file"`  
At this stage, these changes are still limited in your local directory.  

Finally, you need to do  
`git push -u origin master`  
origin is the name of the remoter server. You don't need to change this usually.  
master is the default branch. You can make another branch which will be explained later.  

## Check the status
You can check the status of commit anytime by  
`git status`  

## Obtain other updates
If someone is also working on the same repository as the same time, your changes might conflict with them.  
You can obtain them to your local by  
`git pull origin master`  
which will obtain changes in master-branch at origin-server.  

## Create a branch
If you don't want to change the default branch (master) directly, you can make another branch to work.
You can see the existing branches and where you are by  
`git branch`  
If you want to make a new branch, do  
`git checkout -b mybranch`  
You can move to an existing branch by  
`git checkout mybranch`  
When editing files in another branch, you can do same for `git add` and `git commit`  
When push changes to the new branch not to the default branch, do  
`git push -u origin mybranch`  

## Merge a branch


---

# detectorSimulation usage
## Set enviroment for ROOT and Geant4
(at bepp)  
    source /gluster/share/share.sh  
    source /gluster/share/geant4/geant4.9.6.p03/geant4.9.6.p03-install/bin/geant4.sh  
    source /gluster/share/geant4/geant4.9.6.p03/geant4.9.6.p03-install/share/Geant4-9.6.3/geant4make/geant4make.sh  

## Compile
Edit GNUMakefile if needed and then do  
`make`  

## Run 
To run execution file, do the following command for example  
`muong2edmsim macro/tmp_track_0.mac`  
Output file name will need to be editted in include/ApplicationManager.hh  
`  sprintf(filename,"/gluster/data/g2/tyosioka/data/mug2edm.%s.%s.root",buff,tok);`  

