#!/bin/bash

lcov --directory . -b "$(realpath build/)" --capture --initial -o coverage.base &&
lcov --rc lcov_branch_coverage=1 --directory . -b "$(realpath build/)" --capture -o coverage.capture &&
lcov --directory . -b "$(realpath build/)" --add-tracefile coverage.base --add-tracefile coverage.capture -o coverage.info &&
lcov --directory . -b "$(realpath build/)" --remove coverage.info '*/unit-tests/*' '/opt/*' -o coverage.info &&
echo "Generated 'coverage.info'." &&
genhtml coverage.info -o coverage

rm -f coverage.base coverage.capture