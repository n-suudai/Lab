
#include <fstream>


#define UNUSED(x) (x)


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
        ofs << "255 0 255\n";
    }

    return 0;
}

