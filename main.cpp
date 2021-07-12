#include <iostream>
#include <algorithm>
#include <png++/png.hpp>
#include <gflags/gflags.h>
#include "code.hpp"

namespace CodeImage {
    DEFINE_string(f, "", "input png filename");
    DEFINE_int32(c, 1, "codel size");
}

int main (int argc, char *argv[]) {
    gflags::SetUsageMessage("./mpiet -f [input] -c [codelsize]");
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (CodeImage::FLAGS_f.length() > 0) {
        png::image<png::rgb_pixel> image(CodeImage::FLAGS_f);
        Code code(image, CodeImage::FLAGS_c);
        code.execute();
    } else {
        std::cerr << "input missing." << std::endl;
    }
    return 0;
}
