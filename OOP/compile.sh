#!/usr/bin/sh

HEADER="g++  "

FLAGS="
"


LIBS="
-ldl
-lm
-lpthread
-lrt
-lstdc++
-ljasper
-ljpeg
-lpng
-ltiff
-lz
-lgtk-x11-2.0
"
for i in $LIBS; do
	echo $i
done



#g++ program1-4.cc -
#pthread -O3
#-msse -msse2 -rdynamic
#$(pkg-config --cflags opencv; for x in $(ls /usr/local/lib/libopencv*.a; ls /usr/local/lib/libopencv*.a);
#	do echo -n "$x\n"; done) 
#	/usr/local/share/opencv/3rdparty/lib/libopencv_lapack.a
#	/usr/local/share/opencv/3rdparty/lib/libzlib.a
#	-ldl -lm -lpthread -lrt -lstdc++ -ljasper -ljpeg -lpng -ltiff -lz -lgtk-x11-2.0
#	-lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpng12 -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lglib-2.0 -lgstbase-0.10 -lgstreamer-0.10 -lxml2 -lrt -lavcodec -lavformat -lavutil -lswscale -ldc1394 -lv4l1

