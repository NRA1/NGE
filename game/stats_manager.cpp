#include <fstream>
#include <cstring>
#include "../infrastructure/logger.hpp"
#include "stats_manager.hpp"
#include "../infrastructure/resource_loader.hpp"

void StatsManager::put(const std::string &path, const struct StatsEntry &stats)
{
    std::string full_path = ResourceLoader::fullPath(path).string();
    std::ofstream ofs(full_path + ".part", std::ios::binary);
    if (!ofs.is_open())
    {
        log() - Critical < "Failed to write statistics";
        return;
    }
    std::ifstream ifs(full_path, std::ios::binary);
    if (!ifs.is_open())
    {
        ofs.write((char *) &stats, sizeof(StatsEntry));
    }
    else
    {
        bool written = false;
        struct StatsEntry entry;
        while (ifs.read((char *) &entry, sizeof(StatsEntry)))
        {
            if (stats.Points > entry.Points && !written)
            {
                ofs.write((char *) &stats, sizeof(StatsEntry));
                written = true;
            }
            if (strcmp(entry.Player, stats.Player) != 0)
            {
                ofs.write((char *) &entry, sizeof(StatsEntry));
            }
            else if(!written)
            {
                ofs.write((char *) &entry, sizeof(StatsEntry));
                written = true;
            }
        }

        if(!written)
            ofs.write((char *) &stats, sizeof(StatsEntry));

        ifs.close();
    }

    ofs.close();
    remove(full_path.c_str());
    if (rename((full_path + ".part").c_str(), full_path.c_str()))
    {
        log() - Critical < "Failed to rename file " < full_path + ".part" < full_path;
        return;
    }
}

std::vector<struct StatsEntry> StatsManager::getTopFive(const std::string &path)
{
    std::vector<StatsEntry> entries;
    entries.reserve(5);

    std::ifstream ifs(ResourceLoader::fullPath(path), std::ios::binary);
    if(!ifs.is_open())
    {
        log() - Critical < "Failed to read statistics";
        return entries;
    }

    struct StatsEntry entry;
    for (int i = 0; i < 5; ++i)
    {
        if(!ifs.read((char*)&entry, sizeof(StatsEntry)))
            break;
        else
            entries.push_back(entry);
    }
    ifs.close();
    return entries;
}
