#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPLogger.h"

#define TEST_FILES_DIR "logger_unit_test_files"

// This is a helper function which checks if two files are identical
static bool identicalFiles(const char* fname1, const char* fname2) {
	FILE *fp1, *fp2;
	fp1 = fopen(fname1, "r");
	fp2 = fopen(fname2, "r");
	char ch1 = EOF, ch2 = EOF;

	if (fp1 == NULL) {
		return false;
	} else if (fp2 == NULL) {
		fclose(fp1);
		return false;
	} else {
		ch1 = getc(fp1);
		ch2 = getc(fp2);

		while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
			ch1 = getc(fp1);
			ch2 = getc(fp2);
		}
		fclose(fp1);
		fclose(fp2);
	}
	if (ch1 == ch2) {
		return true;
	} else {
		return false;
	}
}

//Logger is not defined
static bool basicLoggerTest() {
	ASSERT_TRUE(spLoggerPrintError("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDEFINED);
	spLoggerDestroy();
	return true;
}

//Only Errors should be printed
static bool basicLoggerErrorTest() {
	const char* expectedFile = TEST_FILES_DIR"/""basicLoggerErrorTestExp.log";
	const char* testFile = TEST_FILES_DIR"/""basicLoggerErrorTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//All messages should be printed in debug level
static bool basicLoggerDebugTest() {
	const char* expectedFile = TEST_FILES_DIR"/""basicLoggerDebugTestExp.log";
	const char* testFile = TEST_FILES_DIR"/""basicLoggerDebugTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//All messages should be printed in warning level
static bool basicLoggerWarningTest() {
	const char* expectedFile = TEST_FILES_DIR"/""basicLoggerWarningTestExp.log";
	const char* testFile = TEST_FILES_DIR"/""basicLoggerWarningTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//All messages should be printed in info level
static bool basicLoggerInfoTest() {
	const char* expectedFile = TEST_FILES_DIR"/""basicLoggerInfoTestExp.log";
	const char* testFile = TEST_FILES_DIR"/""basicLoggerInfoTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

static bool invalidArgumentTest() {
	const char* testFile = TEST_FILES_DIR"/""invalidArgumentTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError(NULL,"sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_INVAlID_ARGUMENT);
	spLoggerDestroy();
	return true;
}

static bool freeStyleLoggerTest() {
	const char* testFile = TEST_FILES_DIR"/""freeStyleLoggerTest.log";
	const char* expectedFile = TEST_FILES_DIR"/""freeStyleLoggerTestExp.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg("MESSAGE") == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

static bool cannotOpenFileTest() {
    const char* testFile = TEST_FILES_DIR"/""cannotOpenFile.log";
    ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_CANNOT_OPEN_FILE);
    return true;
}

static bool doubleCreationTest() {
	const char* testFile = TEST_FILES_DIR"/""logger.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_DEFINED);
	return true;
}

int main() {
	RUN_TEST(basicLoggerTest);
	RUN_TEST(basicLoggerErrorTest);
	RUN_TEST(basicLoggerDebugTest);
	RUN_TEST(basicLoggerWarningTest);
	RUN_TEST(basicLoggerInfoTest);
	RUN_TEST(invalidArgumentTest);
	RUN_TEST(freeStyleLoggerTest);
	RUN_TEST(cannotOpenFileTest);
	RUN_TEST(doubleCreationTest);
	return 0;
}
