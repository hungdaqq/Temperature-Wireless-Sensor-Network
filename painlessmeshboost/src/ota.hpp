#ifndef _PMBOOST_OTA_HPP_
#define _PMBOOST_OTA_HPP_

#include <boost/uuid/detail/md5.hpp>
using boost::uuids::detail::md5;

std::vector<std::string> split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

std::string toString(const md5::digest_type& digest) {
  const auto charDigest = reinterpret_cast<const char*>(&digest);
  std::string result;
  boost::algorithm::hex(charDigest, charDigest + sizeof(md5::digest_type),
                        std::back_inserter(result));
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

struct FileStat {
  bool newFile;
  std::string file;
  std::string md5;
  std::string hw;
  std::string role;
};

FileStat addFile(std::shared_ptr<std::map<std::string, std::string>> files,
                 boost::filesystem::path p, int dur) {
  // Check for time since change
  auto ftime =
      boost::posix_time::from_time_t(boost::filesystem::last_write_time(p));
  auto now = boost::posix_time::second_clock::universal_time();

  FileStat stat;
  stat.newFile = false;
  // We take some uncertainty in account when this was last run
  if ((now - ftime).total_milliseconds() < 2 * dur) {
    stat.file = p.filename().string();
    auto stem = p.stem().string();
    if (p.extension().string() != ".bin") return stat;
    auto fv = split(stem, '_');
    if (fv[0] != "firmware" || fv.size() < 2) return stat;
    stat.hw = fv[1];
    if (fv.size() == 3) stat.role = fv[2];
    std::ifstream input(p.string(), std::ios::binary);
    // copies all data into buffer
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input),
                                      {});
    md5 hash;
    md5::digest_type digest;
    hash.process_bytes(buffer.data(), buffer.size());
    hash.get_digest(digest);
    stat.md5 = toString(digest);
    if (files->count(stat.md5)) return stat;
    stat.newFile = true;
    auto data64 = painlessmesh::base64::encode(buffer.data(), buffer.size());
    files->operator[](stat.md5) = data64;
    return stat;
  }
  return stat;
}

#endif
