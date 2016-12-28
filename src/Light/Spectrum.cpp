#include "Light/Spectrum.h"

namespace Kaguya
{

Spectrum::Spectrum()
{
    color = COLOR_WHITE;
    intensity = 1;
}
Spectrum::Spectrum(Float its, ColorRGBA spc)
{
    intensity = its;
    color = spc;
}
Spectrum::Spectrum(ColorRGBA spc)
{
    color = spc;
    intensity = 1;
}
Spectrum::~Spectrum()
{
}
ColorRGBA Spectrum::operator*(const ColorRGBA &color2) const
{
    return color * color2 * intensity;
}
ColorRGBA operator*(const ColorRGBA &color, const Spectrum &spec)
{
    return spec * color;
}
void Spectrum::printInfo() const
{
    std::cout << "Intensity:\t" << intensity << std::endl;
    std::cout << "Color:";
    color.printInfo();
}

}
