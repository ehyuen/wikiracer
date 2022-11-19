#include <iostream>     // for cout, cin
#include <fstream>      // for ifstream
#include <sstream>      // for stringstream
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"

using std::cout;            using std::endl;
using std::ifstream;        using std::stringstream;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;   using std::cin;

/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 *
 */

// get the number of common links between the current page and the end page
int countCommon(const unordered_set<string> linkset1, const unordered_set<string> linkset2){
    int size1 = linkset1.size();
    int size2 = linkset2.size();
    int num = 0;
    if (size1 < size2){
        for (const auto& elem: linkset1){
            if (linkset2.find(elem) != linkset2.end()){num++;}
            }
       return num;
    }else{
        for (const auto& elem: linkset2){
            if (linkset1.find(elem) != linkset1.end()){num++;}
            }
        return num;
    }
}

vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    // TODO: 1. Once your file reading is working, replace the below
    //          return statement with "return {};".
    //       2. Make sure that you've copied over your findWikiLinks method
    //          from Part A into wikiscraper.cpp.
    //       3. Finally, implement this function per Part B in the handout!
    //
    //                Best of luck!
    // creates WikiScraper object
        WikiScraper scraper;
        unordered_map<string, int> visited_pages; /*store number of common pages*/
        unordered_map<string, unordered_set<string>> visited_linksets; /*store link sets*/
        unordered_set<string> start_linkset = scraper.getLinkSet(start_page);
        unordered_set<string> end_linkset = scraper.getLinkSet(end_page);
        visited_pages[start_page] = countCommon(start_linkset, end_linkset);
        visited_linksets[start_page] = start_linkset;
        visited_linksets[end_page] = end_linkset;

        // Comparison function for priority_queue
        auto cmpFn = [&](const vector<string>& ladder1, const vector<string>& ladder2){
                      const string& page1 = ladder1.back();
                      const string& page2 = ladder2.back();
                      int num1 = visited_pages[page1];
                      int num2 = visited_pages[page2];
                      return (num1 < num2);
                      }/* declare lambda comparator function */;
          // creates a priority_queue names ladderQueue
        std::priority_queue<vector<string>, vector<vector<string>>,
            decltype(cmpFn)> ladderQueue(cmpFn);
        ladderQueue.push({start_page});

        while (! ladderQueue.empty()){
            vector<string> partial_ladder = ladderQueue.top();
//            cout << "pop out from queue: " << endl;
//            for (auto& i: partial_ladder){
//              std::cout << i << ' ';}
//            cout << "end pop" << endl;
            ladderQueue.pop();
            const string& current_page = partial_ladder.back();
            const unordered_set<string>& current_linkset = visited_linksets[current_page];
//            cout << "neighbors: " << endl;
//            for (const auto& elem: current_linkset) {
//                cout << elem << ' ' << endl;
//            }

            if (current_linkset.find(end_page) != current_linkset.end()){
                // if there is a link to end_page in the current page
                partial_ladder.push_back(end_page);
                return partial_ladder;
            }
            else{
                for (const string& neighbor_page: current_linkset) {
                    if (visited_pages.find(neighbor_page) == visited_pages.end()){
                        // push the new partial ladder to ladder queue and add the info of unvisited pages
                        vector<string> new_partial_ladder = partial_ladder;
                        new_partial_ladder.push_back(neighbor_page);
//                        cout << "push to queue: " << "last page: " << neighbor_page << endl;
                        unordered_set<string> neighbor_linkset = scraper.getLinkSet(neighbor_page);
                        visited_linksets[neighbor_page] = neighbor_linkset;
                        visited_pages[neighbor_page] = countCommon(neighbor_linkset, end_linkset);
                        ladderQueue.push(new_partial_ladder);
                    }
                }
            }
        }
        return {};
    }


int main() {
    /* Container to store the found ladders in */
    vector<vector<string>> outputLadders;

    cout << "Enter a file name: ";
    string filename;
    getline(cin, filename);

    // TODO: Create a filestream from the filename.
    //       For each pair {start_page, end_page} in the input file,
    //       retrieve the result of findWikiLadder(start_page, end_page)
    //       and append that vector to outputLadders.

    // Write code here
    std::ifstream stream (filename);
    string line;
    string start_page;
    string end_page;
    vector<string> wiki_ladder;
    int total_num = 0;
    int count = 0;
    while (getline(stream, line)){
        std::istringstream i(line);
        if (count == 0){
            i >> total_num;
//            cout << "num: " << total_num << endl;
        }else{
            std::istringstream i(line);
            i >> start_page >> end_page;
//            std::cout << start_page << " and " << end_page << endl;
            wiki_ladder = findWikiLadder(start_page, end_page);
            outputLadders.push_back(wiki_ladder);
        }
        ++count;
    }

    /*
     * Print out all ladders in outputLadders.
     * We've already implemented this for you!
     */
    for (auto& ladder : outputLadders) {
        if(ladder.empty()) {
            cout << "No ladder found!" << endl;
        } else {
            cout << "Ladder found:" << endl;
            cout << "\t" << "{";

            std::copy(ladder.begin(), ladder.end() - 1,
                      std::ostream_iterator<string>(cout, ", "));
            /*
             * The above is an alternate way to print to cout using the
             * STL algorithms library and iterators. This is equivalent to:
             *    for (size_t i = 0; i < ladder.size() - 1; ++i) {
             *        cout << ladder[i] << ", ";
             *    }
             */
            cout << ladder.back() << "}" << endl;
        }
    }
    return 0;
}




