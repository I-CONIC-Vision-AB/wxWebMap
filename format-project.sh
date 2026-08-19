#!/bin/bash

if [[ $1 = "--help" ]]; then
  printf "This script should be placed in the root folder of the project.
Arguments provided will be passed along to clang-format.
Run 'clang-format --help' to see the available options.

Example usage: ./format-project.sh --verbose --dry-run"
  exit 1
fi

cd `dirname "$0"`

dirs_to_format=(src test include)
extensions_to_format=(*.h *.hpp *.cpp *.cl)

for dir in ${dirs_to_format[@]}
  do
  echo "Formatting `readlink -e $PWD/$dir`"
  for extension in ${extensions_to_format[@]}
    do
      find `readlink -e ./$dir` -iname $extension | xargs --no-run-if-empty clang-format -i $1 $2 $3
    done
  done
