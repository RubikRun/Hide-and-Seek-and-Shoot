#include <fstream>
#include <string>
#include <map>

#include <stdexcept>

namespace
{

namespace ConfigUtils
{

/* Config type.
   A config is essentially a bunch of strings with attached values,
   so this is represented with a map of strings to strings */
typedef std::map<std::string, std::string> Config;

// Character that will be used as a separator in config files
char const CONFIG_FILE_SEPARATOR = '=';

/**
 * Reads configuration from a config file.
 * 
 * @param[in] filename
 *  Name of the file from which the configuration will be read
 * 
 * @return Config object representing the read configuration
 */
Config ReadConfig(std::string const& filename)
{
    // Open config file
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Error: Cannot open config file \"" + filename + "\".");
    }

    // Config object to which the configuration will be read
    Config config;

    // Read config file file line by line
    std::string line;
    while (file >> line)
    {
        // Split line into two parts by the separator
        int sepInd = line.find(CONFIG_FILE_SEPARATOR);
        std::string first = line.substr(0, sepInd);
        std::string second = line.substr(sepInd + 1);

        // Map first part of the line to the second one
        config[first] = second;
    }

    // Close config file
    file.close();
    if (file.is_open())
    {
        throw std::runtime_error("Error: Cannot close config file \"" + filename + "\".");
    }

    return std::move(config);
}

} // namespace ConfigUtils

} // namespace