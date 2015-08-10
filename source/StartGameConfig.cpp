#include "StartGameConfig.h"

// regions name: farm, winter, underwater, steppe, asia, australia
// Explanation is needed here, start animals will be added only if their region is in START_REGIONS

const std::string START_REGIONS[2] = {"farm", "winter"};

const std::string START_FARM[3] = {"cat", "sheep", "dog"};
const std::string START_WINTER[3] = {"fox", "penguin", "polarbear"};
const std::string START_UNDERWATER[3] = {"dolphin", "octopus", "fish_1"};
const std::string START_STEPPE[3] = {"elephant", "girafee", "gnu"};
const std::string START_ASIA[3] = {"yak", "tiger", "snake"};
const std::string START_AUSTRALIA[3] = {"crocodile", "kangaroo", "kiwi"};

const int START_ANIMALS_COUNT = 5;
const int FEED_INTERVAL_SECONDS = 3600;
const int HUNGER_BARRIER_TIME_SECONDS = 86400;
const int HUNGER_HAPPINES_DECREASE_PERCENT = 10;
const int START_MONEY = 10000;
const int START_EXP = 100;

const int HUMAN_CHARS_COUNT = 7;