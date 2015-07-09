#menu-cierre
echo "Instalación de menu-cierre"
echo "Copiando iconos"
sudo mkdir /usr/share/menu-cierre
sudo cp close.png /usr/share/menu-cierre/close.png
echo "Compilando menu-cierre"
g++ menu-cierre.cpp -o menu-cierre `pkg-config --cflags --libs gtk+-3.0`
echo "Moviendo binario de menu-cierre a /usr/bin"
sudo mv menu-cierre /usr/bin/menu-cierre
echo "menu-cierre ha sido instalado correctamente. Ejecute 'menu-cierre' para abrir el programa"
