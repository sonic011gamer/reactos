# Split psdk to dxsdk

import os
import shutil
from typing import List

# FILE TO MOVE FROM MINGW INCLUDES (PSDK) TO DXSDK
FLIST_DXSDK = [
    "activecf.h",
    "amaudio.h",
    "amparse.h",
    "amstream.idl",
    "amva.h",
    "amvideo.idl",
    "atsmedia.h",
    "audevcod.h",
    "austream.idl",
    "aviriff.h",
    "axcore.idl",
    "axextend.idl",
    "axextendenums.h",
    "bdamedia.h",
    "bdatif.idl",
    "bdatypes.h",
    "CMakeLists.txt",
    "comlite.h",
    "d3d8caps.h",
    "d3d9caps.h",
    "d3dcaps.h",
    "d3dvec.inl",
    "d3dx9.h",
    "d3dx9anim.h",
    "d3dx9core.h",
    "d3dx9effect.h",
    "d3dx9math.h",
    "d3dx9math.inl",
    "d3dx9mesh.h",
    "d3dx9shader.h",
    "d3dx9shape.h",
    "d3dx9tex.h",
    "d3dx9xof.h",
    "ddrawex.h",
    "ddstream.idl",
    "devenum.idl",
    "dls1.h",
    "dls2.h",
    "dmdls.h",
    "dmerror.h",
    "dmksctrl.h",
    "dmo.h",
    "dmodshow.idl",
    "dmoreg.h",
    "dmort.h",
    "dmplugin.h",
    "dmusbuff.h",
    "dmusicc.h",
    "dmusicf.h",
    "dmusici.h",
    "dmusics.h",
    "dsconf.h",
    "dsdriver.h",
    "dsetup.h",
    "dshow.h",
    "dshowasf.idl",
    "dv.h",
    "dx7todx8.h",
    "dxdiag.h",
    "dxerr8.h",
    "dxerr9.h",
    "dxfile.h",
    "dxsdkver.h",
    "dyngraph.idl",
    "errors.h",
    "evcode.h",
    "il21dec.h",
    "Iwstdec.h",
    "ksguid.h",
    "ksuuids.h",
    "mediaerr.h",
    "mediaobj.idl",
    "medparam.idl",
    "mixerocx.idl",
    "mmstream.idl",
    "mpconfig.h",
    "mpeg2bits.h",
    "mpeg2error.h",
    "mpegtype.h",
    "pixplugin.h",
    "qedit.idl",
    "regbag.idl",
    "rmxfguid.h",
    "rmxftmpl.h",
    "tvratings.idl",
    "uuids.h",
    "vidcap.idl",
    "videoacc.idl",
    "vmrender.idl",
    "vpconfig.h",
    "vpnotify.h",
    "vptype.h",
]

# FILE TO MOVE FROM PSDK TO CRT

FLIST_CRT = [
    "comdef.h",
    "comdefsp.h",
    "comip.h",
    "comutil.h",
    "minmax.h",
    "pgobootrun.h",
    "rtcapi.h",
    "typeinfo.h",
    
]

def do_psdk_move(file_list: List[str], out_path: str):
    processed = []
    for root, dir, file in os.walk("psdk"):
        for x in file:
            if x in file_list:
                try:
                    shutil.move(os.path.join(root, x), os.path.join(root, "..", out_path, x))
                except FileNotFoundError:
                    print("not found", out_path, ":", x)
                processed.append(x)

    for x in file_list:
        if not x in processed:
            print("Unprocessed", out_path, ":", x)


def rm_h_idl():
    for root, dir, file in os.walk("psdk"):
        for x in file:
            if x[-4:] == ".idl":
                k = os.path.join(root, x.replace(".idl", ".h"))
                print("remove:", k)
                try:
                    os.remove(k)
                except FileNotFoundError:
                    pass   

do_psdk_move(FLIST_DXSDK, "dxsdk")
do_psdk_move(FLIST_CRT, "crt")
rm_h_idl()
