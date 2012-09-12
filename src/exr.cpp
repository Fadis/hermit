#include <iostream>
#include <boost/gil/gil_all.hpp>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>
/*
template <typename View>
void apply(const View& view) {
  Imf::RgbaInputFile file ( "sample.exr" );
  Imath::Box2i dw = file.dataWindow();
  int width = dw.max.x - dw.min.x + 1;
  int height = dw.max.y - dw.min.y + 1;
  std::cout << width << "x" << height << std::endl;
  Imf::Array2D<Imf::Rgba> pixels;
  pixels.resizeErase (height, width);
  file.setFrameBuffer( &pixels[0][0] - dw.min.x - dw.min.y * width, 1, width );
  file.readPixels (dw.min.y, dw.max.y);
  
  std::vector<pixel<bits8,layout<typename color_space_type<View>::type> > > row(view.width());
  JSAMPLE* row_address=(JSAMPLE*)&row.front();
  for(int y=0;y<view.height();++y) {
      io_error_if(jpeg_read_scanlines(&_cinfo,(JSAMPARRAY)&row_address,1)!=1,
                  "jpeg_reader::apply(): fail to read JPEG file");
      std::copy(row.begin(),row.end(),view.row_begin(y));
  }
}
*/

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

