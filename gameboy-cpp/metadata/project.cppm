export module metadata:project;

import std;

import :common;
import :project.metadata;

namespace meta
{
    export struct project_metadata
    {
        std::string author;
        std::string name;
        std::string version;
        std::string description;
        std::string url;
        std::string license;
        std::string hash;
    };

    export [[nodiscard]] project_metadata get_project_metadata()
    {
        return
        {
            project_author,
            project_name,
            project_version,
            project_description,
            project_url,
            project_license,
            git_hash
        };
    }
}
