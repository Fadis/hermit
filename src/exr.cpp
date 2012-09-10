#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>

int main() {
  Imf::RgbaInputFile file ( "sample.exr" );
  Imath::Box2i dw = file.dataWindow();
  int width = dw.max.x - dw.min.x + 1;
  int height = dw.max.y - dw.min.y + 1;
  std::cout << width << "x" << height << std::endl;
  Imf::Array2D<Imf::Rgba> pixels;
  pixels.resizeErase (height, width);
  file.setFrameBuffer( &pixels[0][0] - dw.min.x - dw.min.y * width, 1, width );
  file.readPixels (dw.min.y, dw.max.y);
}

