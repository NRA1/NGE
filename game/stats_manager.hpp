#ifndef NGE_STATS_MANAGER_HPP
#define NGE_STATS_MANAGER_HPP

#include <string>
#include <vector>

struct StatsEntry
{
    char Player[20];
    unsigned int Level;
    unsigned int Points;
};

class StatsManager
{
public:
    static void put(const std::string &path, const struct StatsEntry &stats);
    static std::vector<struct StatsEntry> getTopFive(const std::string &path);
};


#endif //NGE_STATS_MANAGER_HPP
