#include <iostream>

#include <format>

#include <filesystem>
namespace fs = std::filesystem;

#include <chrono>
namespace ch = std::chrono;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

struct Config
{
    fs::path input_dir;
    fs::path output_dir;
};

Config parse_arguments(int argc, char** argv)
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("input,i", po::value<fs::path>(), "input directory")
        ("output,o", po::value<fs::path>(), "output directory")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        std::exit(0);
    }

    Config config;

    if(!vm.count("input"))
    {
        throw std::invalid_argument("missing input");
    }

    if(!vm.count("output"))
    {
        throw std::invalid_argument("missing output");
    }

    config.input_dir = vm["input"].as<fs::path>();
    config.output_dir = vm["output"].as<fs::path>();

    if(!fs::is_directory(config.input_dir))
    {
        throw std::invalid_argument("invalid input");
    }

    if(!fs::is_directory(config.output_dir))
    {
        throw std::invalid_argument("invalid output");
    }

    return config;
}

void run(const Config & config)
{
    const ch::time_zone* tz = ch::current_zone();

    for(const fs::path & in : fs::recursive_directory_iterator(config.input_dir))
    {
        const fs::path rel = fs::relative(in, config.input_dir);
        // fs::path out = config.output_dir / rel;
        // out.remove_filename();
        // fs::create_directories(out);
        if(fs::is_regular_file(in))
        {
            fs::file_time_type file_time = fs::last_write_time(in);
            ch::sys_time sys_time = ch::clock_cast<ch::system_clock>(file_time);
            ch::local_time local_time = tz->to_local(sys_time);

            std::string date = std::format("{:%Y.%m.%d-%H.%M.%OS}_", local_time);

            fs::path out = config.output_dir / date;
            out += in.filename();

            fs::copy_file(in, out);
        }
    }
}

int main(int argc, char** argv)
{
    try
    {
        Config config = parse_arguments(argc, argv);

        std::cout << "input=" << config.input_dir << std::endl;
        std::cout << "output=" << config.output_dir << std::endl;

        run(config);
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}