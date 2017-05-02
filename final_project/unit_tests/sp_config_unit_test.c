#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPConfig.h"

#define TEST_FILES_DIR "config_unit_test_files"
#define MAX_SIZE 1024

static bool cannotOpenFileTest() {
    SP_CONFIG_MSG msg;
    spConfigCreate(TEST_FILES_DIR"/""cannot_open_file.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_CANNOT_OPEN_FILE);
    return true;
}

static bool invalidIntegerTest() {
    SP_CONFIG_MSG msg;
    spConfigCreate(TEST_FILES_DIR"/""invalid_integer.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_INVALID_INTEGER);
    return true;
}

static bool invalidStringTest() {
    SP_CONFIG_MSG msg;
    spConfigCreate(TEST_FILES_DIR"/""invalid_string.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_INVALID_STRING);
    return true;
}

static bool missingDirTest() {
    SP_CONFIG_MSG msg;
    spConfigCreate(TEST_FILES_DIR"/""missing_dir.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_MISSING_DIR);
    return true;
}

static bool missingNumOfImagesTest() {
    SP_CONFIG_MSG msg;
    spConfigCreate(TEST_FILES_DIR"/""missing_num_of_images.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_MISSING_NUM_IMAGES);
    return true;
}

static bool missingPrefixTest() {
    SP_CONFIG_MSG msg;
    spConfigCreate(TEST_FILES_DIR"/""missing_prefix.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_MISSING_PREFIX);
    return true;
}

static bool missingSuffixTest() {
    SP_CONFIG_MSG msg;
    spConfigCreate(TEST_FILES_DIR"/""missing_suffix.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_MISSING_SUFFIX);
    return true;
}

static bool invalidConfigurationLine1Test() {
    SP_CONFIG_MSG msg;
    spConfigCreate(TEST_FILES_DIR"/""invalid_configuration_line1.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_INVALID_CONFIGURATION_LINE);
    return true;
}

static bool invalidConfigurationLine2Test() {
    SP_CONFIG_MSG msg;
    spConfigCreate(TEST_FILES_DIR"/""invalid_configuration_line2.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_INVALID_CONFIGURATION_LINE);
    return true;
}

static bool invalidArgumentTest() {
    SP_CONFIG_MSG msg;
    SPConfig config = NULL;
    spConfigGetNumOfFeatures(config, &msg);
    ASSERT_TRUE(msg == SP_CONFIG_INVALID_ARGUMENT);
    return true;
}

static bool outOfRangeTest() {
    SP_CONFIG_MSG msg;
    SPConfig config = spConfigCreate(TEST_FILES_DIR"/""good_config.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);
    char imagePath[MAX_SIZE];
    msg = spConfigGetImagePath(imagePath, config, 20);
    ASSERT_TRUE(msg == SP_CONFIG_INDEX_OUT_OF_RANGE);
    return true;
}

static bool imagePathTest() {
    char *expectedImagePath = "./images/img15.png";
    SP_CONFIG_MSG msg;
    SPConfig config = spConfigCreate(TEST_FILES_DIR"/""good_config.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);
    char imagePath[MAX_SIZE];
    msg = spConfigGetImagePath(imagePath, config, 15);
    ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);
    ASSERT_TRUE(strcmp(imagePath, expectedImagePath) == 0);
    return true;
}

static bool numOfImagesTest() {
    int expectedNumOfImages = 17;
    SP_CONFIG_MSG msg;
    SPConfig config = spConfigCreate(TEST_FILES_DIR"/""good_config.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);
    int numOfImages = spConfigGetNumOfImages(config, &msg);
    ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);
    ASSERT_TRUE(numOfImages == expectedNumOfImages);
    return true;
}

static bool goodConfigTest() {
    SP_CONFIG_MSG msg;
    spConfigCreate(TEST_FILES_DIR"/""good_config.config", &msg);
    ASSERT_TRUE(msg == SP_CONFIG_SUCCESS);
    return true;
}

int main() {
    RUN_TEST(cannotOpenFileTest);
    RUN_TEST(invalidIntegerTest);
    RUN_TEST(invalidStringTest);
    RUN_TEST(missingDirTest);
    RUN_TEST(missingNumOfImagesTest);
    RUN_TEST(missingPrefixTest);
    RUN_TEST(missingSuffixTest);
    RUN_TEST(invalidConfigurationLine1Test);
    RUN_TEST(invalidConfigurationLine2Test);
    RUN_TEST(invalidArgumentTest);
    RUN_TEST(outOfRangeTest);
    RUN_TEST(imagePathTest);
    RUN_TEST(numOfImagesTest);
    RUN_TEST(goodConfigTest);
    return 0;
}
