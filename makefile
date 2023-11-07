TARGET_EXEC := cpp_interview
CXXFLAGS := -Wall -std=c++17 -g
BUILD_DIR := ./build

.PHONY: clean test run patch

# A list of source files used
SRCS := interview/main.cpp test/Tests.cpp
# Every folder in will need to be passed to GCC so that it can find header files
INC_DIRS := interview/ test/

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS := $(INC_FLAGS) -MMD -MP

# Build it all
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

clean:
	rm -r $(BUILD_DIR)

test:
	#$(BUILD_DIR)/$(TARGET_EXEC) test
	$(BUILD_DIR)/$(TARGET_EXEC) ./data/data2.csv > ./data/result2c.csv
	awk -f ./data/extract_agg_stat.awk ./data/data2.csv > ./data/result2a.csv
	diff ./data/result2c.csv ./data/result2a.csv

run:
	$(BUILD_DIR)/$(TARGET_EXEC) ./data/data.csv

patch:
	git add .
	git diff --binary origin/main > exyn_interview_solution.patch

-include $(DEPS)
