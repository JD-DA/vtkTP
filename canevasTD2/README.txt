Pour compiler ce caneva:

installer les paquetages suivants:
apt-get install libvtk9.1 libvtk9-java libvtk9-dev libvtk9-qt-dev 


(pas sur que libvtk9-java soit utile)

puis: 
	mkdir BUILD
	cd BUILD
	cmake ..

puis: 
	make 
ou bien:
	make check

Pour executer:
make test

ou bien:
./vtktp


Pour Apple sous Catalina:

Télécharger les sources de vtk 9 et compiler ces sources:
https://vtk.org/download/

depuis le répertoire des sources:

mkdir build; cd build

cmake ..
make
sudo make install


ou (testé l’an passé seulement)

----------

brew install cmake
brew install qt5
wget https://www.vtk.org/files/release/7.1/VTK-7.1.1.zip
unzip VTK-7.1.1.zip
cd VTK-7.1.1
mkdir Build
cd Build
 cmake -DVTK_QT_VERSION:STRING=5      -DQT_QMAKE_EXECUTABLE:PATH=/usr/local/Cellar/qt/5.15.2/bin/qmake     -DVTK_Group_Qt:BOOL=ON      -DCMAKE_PREFIX_PATH:PATH=/usr/local/Cellar/qt/5.15.2/lib/cmake      -DBUILD_SHARED_LIBS:BOOL=ON  ..
make -j4

il y a des erreurs de compilation donc par exemple ajouter : #include <QPainterPath> dans :/usr/local/Cellar/qt/5.15.2/lib/QtGui.framework/Headers/qfont.h

sudo make install







