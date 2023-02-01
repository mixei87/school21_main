absolute_path=$(pwd)
sed -i '' -E "s!SET path_to_src.var TO '.*';!SET path_to_src.var TO '$absolute_path/data/';!" part1.sql

