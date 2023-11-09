//
// Created by 58413 on 2023/10/16.
//

#ifndef FRIEREN_COMMON_INCLUDE_H
#define FRIEREN_COMMON_INCLUDE_H

#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <filesystem>
#include <memory>
#include <optional>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <typeinfo>
#include <type_traits>
#include <array>

#define JSON_DISABLE_ENUM_SERIALIZATION 1
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#endif //FRIEREN_COMMON_INCLUDE_H
