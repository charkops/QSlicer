#include "io.hpp"

#include "Point.hpp"

// For split
#include <boost/algorithm/string/split.hpp>

namespace qslicer {

  // NOTE (@charkops): This whole function needs fixing
  Triangle readFacet(std::ifstream &file, const std::string &line) {
    // Split line into members
    std::vector<std::string> members;
    boost::split(members, line, [](char c) {
      return c == ' ';
    });
    members.erase(std::remove_if(
        members.begin(),
        members.end(),
        [](const std::string &s){
          return s.empty();
        }
      ), members.end());
    if (members[0] == "facet") {
      Point normal (std::stof(members[2]), std::stof(members[3]), std::stof(members[4]));
      std::string l;
      std::getline(file, l);
      std::getline(file, l);
      boost::split(members, l, [](char c) {
        return c == ' ';
      });
      members.erase(std::remove_if(
        members.begin(),
        members.end(),
        [](const std::string &s){
          return s.empty();
        }
      ), members.end());
      Point p0 (std::stof(members[1]), std::stof(members[2]), std::stof(members[3]));
      std::getline(file, l);
      boost::split(members, l, [](char c) {
        return c == ' ';
      });
      members.erase(std::remove_if(
        members.begin(),
        members.end(),
        [](const std::string &s){
          return s.empty();
        }
      ), members.end());
      Point p1 (std::stof(members[1]), std::stof(members[2]), std::stof(members[3]));
      std::getline(file, l);
      boost::split(members, l, [](char c) {
        return c == ' ';
      });
      members.erase(std::remove_if(
        members.begin(),
        members.end(),
        [](const std::string &s){
          return s.empty();
        }
      ), members.end());
      Point p2 (std::stof(members[1]), std::stof(members[2]), std::stof(members[3]));

      return Triangle (p0, p1, p2, normal);
    }
  };

  std::vector<Triangle> fileToTriangles(const std::string &fileName) {
    // Open file as read
    std::ifstream file (fileName, std::ios::in);
    if (!file.good())
      throw std::runtime_error("Could not open/read file: " + fileName);

    std::vector<Triangle> triangles;
    std::string line;
    while(std::getline(file, line)) {
      std::vector<std::string> members;
      boost::split(members, line, [](char c){
        return c == ' ';
      });
      members.erase(std::remove_if(
        members.begin(),
        members.end(),
        [](const std::string &s){
          return s.empty();
        }
      ), members.end());
      if (members[0] == "facet")
        triangles.push_back(readFacet(file, line));
    }

    return triangles;
  };
} // namespace qslicer