#include <cstddef>
#include <exception>
#include <fstream>
#include <print>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <vector>
#include <string>
#include <ranges>
using pixel = std::tuple<int,int,int>;
using pixel_vector = std::vector<pixel>;
using image = std::vector<pixel_vector>;

constexpr std::string_view delimiter_pixel(" ");
constexpr std::string_view delimiter_pixel_int(",");

constexpr pixel red = std::make_tuple(255,0,0);
constexpr pixel green = std::make_tuple(0,255,0);
constexpr pixel blue = std::make_tuple(0,0,255);

auto get_pixel(const std::string& pixel_str) -> pixel
{
    try
    {
        auto pixel_ints = std::ranges::views::split(pixel_str, delimiter_pixel_int);
        std::vector<int> rgb;
        for (const auto& pix_int : pixel_ints)
        {
            rgb.push_back(std::stoi(std::string(pix_int.begin(), pix_int.end())));
        }

        if (rgb.size() != 3) {throw std::invalid_argument("Incorrect input of pixel");}
        
        return std::make_tuple(rgb[0], rgb[1], rgb[2]);
    }
    catch (...) {throw std::logic_error("Error on pixel serialization");}
}

auto get_pixel_vector(const std::string& pixel_vec_str) -> pixel_vector
{
    try
    {
        pixel_vector pix_vec;

        auto pixels = std::ranges::views::split(pixel_vec_str, delimiter_pixel);
        for (const auto& pixel : pixels)
        {
            std::string pixel_str = std::string(pixel.begin(), pixel.end());
            pix_vec.push_back(get_pixel(pixel_str));
        }

        return pix_vec;
    }
    catch (...) {throw;}
}

auto load_image(const std::string& filename) -> image
{
    try
    {
        std::ifstream file(filename);
        if (!file.is_open()) {throw std::invalid_argument("File cant be opened.");}

        int lcount = std::count(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>(), '\n') + 1;

        if (lcount != 16) {throw std::logic_error("Invalid number of lines.");}

        file.seekg(0, std::ios::beg);

        image img;
        std::string pixel_vec_str;
        for (size_t i = 0; i < 16; i++)
        {
            std::getline(file, pixel_vec_str);
            if (pixel_vec_str == "") {throw std::invalid_argument("Error empty line found.");}
            img.push_back(get_pixel_vector(pixel_vec_str));
        }

        return img;
    }
    catch (...) {throw;}
}

auto main(int argc, char* argv[]) -> int
{
    if (argc != 4) 
    {
        std::println("Error on launch. \nUse format:  " \
            "./main <input_file_name> <color: red/green/blue> <output_file_name>\n" \
            "And please include .txt in file name");
        return 1;
    }

    auto check_input = [argv]() -> void {
        std::string input = std::string(argv[1]);
        std::string output = std::string(argv[3]);
        
        if (!input.contains(".txt") || !output.contains(".txt"))
        {
            std::println("Please include .txt file extension.");
            exit(1);
        }

        std::string color = std::string(argv[2]);
        if (color != "red" && color != "green" && color != "blue")
        {
            std::println("Invalid color, pick only red/green/blue.");
            exit(1);
        }
    };

    check_input();

    try
    {
        image img = load_image(std::format("./local_files/{}", argv[1]));
        pixel fav;
        std::string color = std::string(argv[2]);
        if (color == "red") {fav = red;}
        if (color == "green") {fav = green;}
        if (color == "blue") {fav = blue;}

        for (size_t y = 0; y < img.size(); ++y) 
        {
            for (size_t x = 0; x < img[y].size(); ++x) 
            {
                if (img[y][x] == fav) 
                {
                    if (y > 0) img[y - 1][x] = fav;
                    if (x > 0) img[y][x - 1] = fav;

                    if (y == 0 && x == 0) { continue;}
                }
            }
        }

        std::ofstream file_o(std::format("./build/{}", argv[3]));
        std::string line;
        for (const auto& pxl_vec : img)
        {
            for (const auto& pxl : pxl_vec)
            {
                auto [r, g, b] = pxl;
                line += std::format("{},{},{} ", r, g, b);
            }
            file_o << line << "\n";
            line = "";
        }
        file_o.close();

        std::println("Color schema was successfuly written in ./local_files/{}", argv[3]);
    }
    catch (std::exception& exc) {std::println("{}", exc.what()); return 1;}
    return 0;
}