#include <string>
#include <set>
#include <vector>

#include "diskchanges.h"
#include "globals.h"
#include "tools.h"
#include "telegram.h"

namespace diskchanges {

	std::set<char> GetDiskList() {
        std::set<char> output;
        std::string s = tools::info::cmdOutput("wmic logicaldisk get caption");
        for (int i = 7; i < s.size(); i++) {
            if (s[i] >= 65 && s[i] <= 90) {
                output.insert(s[i]);
            }
        }
        return output;
	}

	void Diskchanges() {
        std::set<char> disklist = GetDiskList();

        if (disklist != DISK_LIST) {
            std::vector<char> diff;
            std::set_symmetric_difference(disklist.begin(), disklist.end(), DISK_LIST.begin(), DISK_LIST.end(), std::back_inserter(diff));

            std::string diff_s;
            for (auto& e : diff) {
                diff_s += e;
                diff_s += " ";
            }

            if (disklist.size() > DISK_LIST.size()) {
                telegram::SendText(ID + " New disks: " + diff_s);
            }
            else {
                telegram::SendText(ID + " Removed disks: " + diff_s);
            }

            DISK_LIST = disklist;
        }
	}
}