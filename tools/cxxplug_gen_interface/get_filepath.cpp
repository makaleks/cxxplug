#include "get_filepath.hpp"

#include <filesystem>
#include <sstream>
#include <fstream>
#include <climits>
#include <stdexcept>

using namespace std;

static const char *Suffix = "_interface";

static void remake_with_num (
    string      &destination,
    string_view directory_path,
    string_view interface_name,
    unsigned    number,
    string_view suffix,
    string_view extension
) {
    char num_buf[64] = {0};
    snprintf(num_buf, sizeof(num_buf), "%u", number);

    destination = directory_path;
    destination.append(interface_name);
    destination.append(num_buf);
    destination.append(suffix);
    destination.append(extension);
}

static bool is_good_filepath (
    const string_view &filepath_str,
    const Uuid dest_uuid, string_view method_pattern
) {
    const filesystem::path filepath(filepath_str);
    if (!filesystem::exists(filepath)) {
        return true;
    }
    else if (!filesystem::is_regular_file(filepath)) {
        return false;
    }
    else {
        stringstream accumulator;
        ifstream file(filepath);
        accumulator << file.rdbuf();

        const string content = accumulator.str();

        auto found_pos = content.find(method_pattern);
        if (string::npos == found_pos) {
            // Some alien content, manual remove required
            return false;
        }
        found_pos += method_pattern.length();

        found_pos = content.find("{", found_pos);
        if (string::npos == found_pos) {
            return false;
        }

        auto found_end_pos = content.find(
            "}", found_pos
        ); // assumed, that file is not too complex to make it not working
        if (string::npos == found_end_pos) {
            return false;
        }

        string_view uuid_mask = "Uuid(\"";
        found_pos = content.find(uuid_mask, found_pos);
        if (string::npos == found_pos) {
            return false;
        }
        found_pos += uuid_mask.length();

        found_end_pos = content.find("\"", found_pos);
        if (string::npos == found_end_pos) {
            return false;
        }

        try {
            string_view uuid_view(
                content.cbegin() + found_pos, content.cend() + found_end_pos
            );
            Uuid uuid(uuid_view.data());

            if (dest_uuid == uuid) {
                return true;
            }
        }
        catch (invalid_argument arg) {
            // Some alien content, manual remove required
            return false;
        }
        return false;
    }
}

static string get_filepath (
    Uuid        interface_uuid,
    string_view interface_name,
    string_view directory_or_file_path,
    string_view suffix,
    string_view extension,
    string_view id_method_pattern
) {
    if (
        !filesystem::exists(directory_or_file_path)
        || filesystem::is_regular_file(directory_or_file_path)
    ) {
        return directory_or_file_path.data();
    }
    else if (!filesystem::is_directory(directory_or_file_path)) {
        throw filesystem::filesystem_error(
            string("directory \"") + directory_or_file_path.data()
                    + "\" not found",
            error_code()
        );
    }

    string   result = string(directory_or_file_path)
                    + interface_name.data() + suffix.data() + extension.data();
    unsigned number_suffix = 0;

    while (!is_good_filepath(result, interface_uuid, id_method_pattern)) {
        number_suffix++;
        remake_with_num(
            result,
            directory_or_file_path, interface_name, number_suffix, suffix, extension
        );
    }

    return result;
}

std::string get_interface_filepath (
    const Parsed     &config,
    std::string_view directory_or_file_path
) {
    return get_filepath(
        config.interface_uuid,
        config.interface_name,
        directory_or_file_path,
        "_interface",
        ".hpp",
        "Uuid get_interface_id"
    );
}
