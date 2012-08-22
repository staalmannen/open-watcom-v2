# WSTUB Builder Control file
# ==========================

set PROJDIR=<CWD>

[ INCLUDE <OWROOT>/build/master.ctl ]
[ LOG <LOGFNAME>.<LOGEXT> ]

cdsay .

[ BLOCK <1> build rel2 ]
#=======================
    [ INCLUDE prereq.ctl ]

[ BLOCK <1> rel2 ]
#=================
    cdsay <PROJDIR>

[ BLOCK <1> rel2 cprel2 ]
#========================
  [ IFDEF (all build os_dos os_win cpu_i86 cpu_386 "") <2*> ]
    <CPCMD> wstub.exe   <OWRELROOT>/binw/wstub.exe
    <CPCMD> wstubq.exe  <OWRELROOT>/binw/wstubq.exe
    <CPCMD> wstub.c     <OWRELROOT>/src/wstub.c

[ BLOCK <1> clean ]
#==================
    pmake -d build <2> <3> <4> <5> <6> <7> <8> <9> -h clean

[ BLOCK . . ]
#============

cdsay <PROJDIR>