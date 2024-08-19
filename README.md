# BounceLab

A configurable and interactive graphical simulator for various shapes in 2D physical space.  

<div align="center"><img src="demo.gif" alt="animated"/></div>  

## Table of Contents  

* [Description](#bouncelab)  
* [Terminology](#features)  
* [License](#license)  
* [Disclaimer](#disclaimer)  

## Features  

* Configurable Shapes  
  * Shapes are randomly mixed by default, but can be overridden to circles, rectangles, or triangles  
    * Triangles are currently broken  
    * More shapes are coming soon  
  * Shape density and size are configurable within reasonable limits  
  * Support for up to 300 shapes in the simulation at one time  
    * Optimizations coming soon for better performance with high shape counts  

* Configurable Physics  
  * Configurable world friction  
  * Configurable gravity  
  * Configurable world collision elasticity  

* Keyboard Controls  
  * Using the arrow keys will move all shapes in the direction of the pressed keys  
  * The shapes will still be affected by all physical factors such as collision elasticity, shape size, and shape density, all of which may affect and/or limit control over the shapes  

* Configurable Colors  
  * Colors are random by default, but can be overridden for shapes and the background  
  
* Mouse Controls Coming _Eventually_

## Other Info

This program was implemented using QT and Box2D libraries in C++. It can be compiled using qmake or a program such as QT Creator.  

## License  

This project is licensed under the GNU General Public License v3.0 (GPL-3.0). See the [LICENSE](LICENSE.txt) file for details.  

## Disclaimer  

This repository is provided as-is and is intended for informational and reference purposes only. The author assumes no responsibility for any errors or omissions in the content or for any consequences that may arise from the use of the information provided. Always exercise caution and seek professional advice if necessary.  
