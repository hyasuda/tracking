# $Id: GNUmakefile,v 1.2 2000/10/19 12:22:10 stanaka Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := muongm2edmsim
G4TARGET := $(name)
G4EXLIB := true

# comment out below if you want to change Geant4 work directory
#G4WORKDIR = .


ifndef G4INSTALL
	G4INSTALL = /gluster/share/geant4/geant4.9.6.p03/geant4.9.6.p03-install/share/Geant4-9.6.3/geant4make
endif
#
.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk


CPPFLAGS +=$(shell $(ROOTSYS)/bin/root-config --cflags)
LDFLAGS +=$(shell $(ROOTSYS)/bin/root-config --libs)

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*

