
list(APPEND SOURCE
    precomp.h
    bank.c     
    bitblt.c   
    bltmm.c    
    brush.c    
    color.c    
    ddraw.c    
    ddraw3d.c  
    debug.c    
    dma.c      
    enable.c   
    escape.c   
    fillpath.c 
    heap.c     
    icd.c      
    iline.c    
    lines.c    
    lpb.c      
    mcd.c      
    mcdclip.c  
    mcdline.c  
    mcdpoint.c 
    mcdprim.c  
    mcdrend.c  
    mcdtex.c   
    mcdtri.c   
    mcdutil.c  
    misc.c     
    multi.c    
    nodma.c    
    paint.c    
    palette.c  
    pointer.c  
    strips.c   
    stroke.c   
    strtch3d.c 
    text3d.c   
    textout.c  
    thunk.c    
    virge.rc   
    d3d.c      
    d3ddx6.c   
    d3dline.c  
    d3dpoint.c 
    d3dstate.c 
    d3dtri.c   
    d3dtess.c  
    d3dutil.c)

add_library(s3virge MODULE
    ${SOURCE}
    virge.rc)

set_module_type(s3virge kerneldll ENTRYPOINT DrvEnableDriver 12)
add_pch(s3virge precomp.h SOURCE)
target_link_libraries(s3virge libcntpr)
add_importlibs(s3virge win32k)
add_dependencies(s3virge psdk)
add_cd_file(TARGET s3virge DESTINATION reactos/system32 FOR all)
