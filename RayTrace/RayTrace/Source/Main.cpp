
#include <fstream>
#include <sstream>
#include <algorithm>


#define UNUSED(x) (x)


int tonemap(double v) {
    return std::min(
        std::max(int(std::pow(v, 1 / 2.2) * 255), 0), 255);
};

std::string tonemap(double r, double g, double b)
{
    std::stringstream ss;
    ss << tonemap(r) << " " << tonemap(g) << " " << tonemap(b);
    return ss.str();
}


int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    const int width = 1280;
    const int height = 720;

    std::ofstream ofs("result.ppm");

    ofs << "P3\n" << width << " " << height << "\n255\n";

    for (int i = 0; i < width * height; i++)
    {
        ofs << tonemap(0.608, 0.337, 0.384) << "\n";
    }

    return 0;
}

