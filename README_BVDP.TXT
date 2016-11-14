To download the images for testing, execute:
  ./getData.sh 


---------------------------------------------------
activer visu
#define VIS 1
-> passer en paramètre au lancement

ajout de la ligne de commande fournie

il manque le dossier dans lequel les images debug sont rangées
bvandepo@garossos:~/zodiac/matlab/fisheye/Scaramuzza_OCamCalib_v3.0_mac_os3/autoCornerFinder$ mkdir pictureVis

numerotation différente pour l'appli C (8x8) et matlab (7x7)

j'ai fait un projet qt pour pouvoir débugger,

depuis matlab j'obtiens 
callString='./FindCorners.exe -w 8 -h 8 -m 64 pictures.txt'
callBack   = system(callString)

./FindCorners.exe -w 8 -h 8 -m 64 pictures.txt: Signal 126

d'après http://tldp.org/LDP/abs/html/exitcodes.html, 
126	Command invoked cannot execute	/dev/null	Permission problem or command is not an executable





~/zodiac/matlab/fisheye/Scaramuzza_OCamCalib_v3.0_mac_os3/autoCornerFinder$ ldd FindCorners.exe
	linux-vdso.so.1 =>  (0x00007ffc6ebe5000)
	libopencv_core.so.2.4 => /usr/local/lib/libopencv_core.so.2.4 (0x00007f34c5d68000)
	libopencv_highgui.so.2.4 => /usr/local/lib/libopencv_highgui.so.2.4 (0x00007f34c5b15000)
	libopencv_imgproc.so.2.4 => /usr/local/lib/libopencv_imgproc.so.2.4 (0x00007f34c5648000)
	libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f34c5348000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f34c504c000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f34c4e36000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f34c4a78000)
	libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f34c4861000)
	libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f34c4644000)
	librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f34c443c000)
	libjpeg.so.8 => /usr/lib/x86_64-linux-gnu/libjpeg.so.8 (0x00007f34c41ec000)
	libpng16.so.16 => /usr/local/lib/libpng16.so.16 (0x00007f34c3f94000)
	libtiff.so.4 => /usr/lib/x86_64-linux-gnu/libtiff.so.4 (0x00007f34c3d30000)
	libjasper.so.1 => /usr/lib/x86_64-linux-gnu/libjasper.so.1 (0x00007f34c3ad8000)
	libIlmImf.so.6 => /usr/lib/libIlmImf.so.6 (0x00007f34c3817000)


system('ldd FindCorners.exe')
	linux-vdso.so.1 =>  (0x00007ffee0697000)
	libopencv_core.so.2.4 => /usr/local/MATLAB/R2014a/bin/glnxa64/libopencv_core.so.2.4 (0x00007f9abc897000)
	libopencv_highgui.so.2.4 => /usr/local/MATLAB/R2014a/bin/glnxa64/libopencv_highgui.so.2.4 (0x00007f9abc56c000)
	libopencv_imgproc.so.2.4 => /usr/local/MATLAB/R2014a/bin/glnxa64/libopencv_imgproc.so.2.4 (0x00007f9abc0c4000)
	libstdc++.so.6 => /usr/local/MATLAB/R2014a/sys/os/glnxa64/libstdc++.so.6 (0x00007f9abbdbd000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f9abbac1000)
	libgcc_s.so.1 => /usr/local/MATLAB/R2014a/sys/os/glnxa64/libgcc_s.so.1 (0x00007f9abb8ac000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f9abb4ee000)
	libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f9abb2d7000)
	libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f9abb0d3000)
	libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f9abaeb6000)
	


 system('export LD_LIBRARY_PATH=/usr/local/lib')



 export LD_LIBRARY_PATH=/usr/local/MATLAB/R2014a/sys/os/glnxa64:/usr/local/MATLAB/R2014a/bin/glnxa64:/usr/local/MATLAB/R2014a/extern/lib/glnxa64:/usr/local/MATLAB/R2014a/runtime/glnxa64:/usr/local/MATLAB/R2014a/sys/java/jre/glnxa64/jre/lib/amd64/native_threads:/usr/local/MATLAB/R2014a/sys/java/jre/glnxa64/jre/lib/amd64/server



setenv(name,value)


setenv(LD_LIBRARY_PATH,/usr/local/lib)




Demander à matlab de ne pas utiliser les librairies opencv2.4.5 
  system('env |grep LD')
  setenv('LD_LIBRARY_PATH','/usr/local/lib')
  system('env |grep LD')
cette ligne ne fonctionne pas car la variable d'environnement est perdue
  system('export LD_LIBRARY_PATH=/usr/local/lib')



